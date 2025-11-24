#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "algorithms.h"
#include "city_config.h"
#include "input_handler.h"
#include "city_generator.h"

// Window resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Vertex Shader Source (supports both 2D and 3D)
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 view;
uniform mat4 projection;
uniform bool is2D;
void main() {
    if (is2D) {
        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    } else {
        gl_Position = projection * view * vec4(aPos, 1.0);
    }
}
)";

// Fragment Shader Source
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 color;
void main() {
    FragColor = vec4(color, 1.0);
}
)";

// Function to convert algorithm points to OpenGL vertices (2D with z=0)
std::vector<float> pointsToVertices(const std::vector<Point>& points, int screenWidth, int screenHeight) {
    std::vector<float> vertices;
    for (const auto& point : points) {
        // Convert pixel coordinates to normalized device coordinates (-1 to 1)
        float x = (point.x / (screenWidth / 2.0f)) - 1.0f;
        float y = 1.0f - (point.y / (screenHeight / 2.0f));
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);  // Z coordinate for 2D elements
    }
    return vertices;
}

// Function to generate 3D cube vertices for a building
// Returns 36 vertices (6 faces * 2 triangles * 3 vertices each) in format (x, y, z)
std::vector<float> buildingToVertices(const Building& building, int screenWidth, int screenHeight) {
    std::vector<float> vertices;
    
    // Convert pixel coordinates to normalized device coordinates
    float centerX = (building.x / (screenWidth / 2.0f)) - 1.0f;
    float centerY = 1.0f - (building.y / (screenHeight / 2.0f));
    float halfWidth = building.width / (screenWidth / 2.0f);
    float halfDepth = building.depth / (screenHeight / 2.0f);
    float heightNorm = building.height / 300.0f;  // Normalize height for viewing
    
    // Calculate the 8 corners of the cube
    float x0 = centerX - halfWidth;
    float x1 = centerX + halfWidth;
    float y0 = centerY - halfDepth;
    float y1 = centerY + halfDepth;
    float z0 = 0.0f;           // Ground level
    float z1 = heightNorm;     // Top of building
    
    // Front face (2 triangles)
    vertices.insert(vertices.end(), {
        x0, y0, z0,  x1, y0, z0,  x1, y0, z1,
        x0, y0, z0,  x1, y0, z1,  x0, y0, z1
    });
    
    // Back face
    vertices.insert(vertices.end(), {
        x1, y1, z0,  x0, y1, z0,  x0, y1, z1,
        x1, y1, z0,  x0, y1, z1,  x1, y1, z1
    });
    
    // Left face
    vertices.insert(vertices.end(), {
        x0, y1, z0,  x0, y0, z0,  x0, y0, z1,
        x0, y1, z0,  x0, y0, z1,  x0, y1, z1
    });
    
    // Right face
    vertices.insert(vertices.end(), {
        x1, y0, z0,  x1, y1, z0,  x1, y1, z1,
        x1, y0, z0,  x1, y1, z1,  x1, y0, z1
    });
    
    // Bottom face
    vertices.insert(vertices.end(), {
        x0, y0, z0,  x0, y1, z0,  x1, y1, z0,
        x0, y0, z0,  x1, y1, z0,  x1, y0, z0
    });
    
    // Top face
    vertices.insert(vertices.end(), {
        x0, y0, z1,  x1, y0, z1,  x1, y1, z1,
        x0, y0, z1,  x1, y1, z1,  x0, y1, z1
    });
    
    return vertices;
}

int main()
{
    // Window dimensions
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    
    // Create city configuration with default values
    CityConfig cityConfig;
    InputHandler inputHandler(cityConfig);
    
    // Create city generator
    CityGenerator cityGenerator(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Display welcome message and controls
    std::cout << "\n";
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    🏙️  CITY DESIGNER 🏙️                   ║\n";
    std::cout << "║            Interactive 3D City Generation Tool            ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n";
    InputHandler::displayControls();
    cityConfig.printConfig();
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "City Designer - Interactive Mode", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ----- Shader Compilation -----
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Enable point size for better visibility
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(2.0f);
    
    // Enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);

    // Get uniform locations
    int colorLocation = glGetUniformLocation(shaderProgram, "color");
    int viewLocation = glGetUniformLocation(shaderProgram, "view");
    int projectionLocation = glGetUniformLocation(shaderProgram, "projection");
    
    // Simple identity matrices for now (we'll add proper camera later)
    float identityMatrix[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, identityMatrix);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, identityMatrix);

    // Connect input handler to city generator
    inputHandler.setCityGenerator(&cityGenerator);

    std::cout << "\n✅ OpenGL initialized successfully!\n";
    std::cout << "Press 'G' to generate a city, or adjust parameters first.\n\n";

    // Variables for dynamic rendering
    std::vector<unsigned int> VAOs;
    std::vector<unsigned int> VBOs;
    std::vector<int> vertexCounts;
    
    // ----- Render Loop -----
    while (!glfwWindowShouldClose(window))
    {
        // Process user input
        inputHandler.processInput(window);
        
        // If city was generated, update rendering data
        if (inputHandler.generationRequested()) {
            inputHandler.clearGenerationRequest();
            
            // Clean up old buffers
            if (!VAOs.empty()) {
                glDeleteVertexArrays(VAOs.size(), VAOs.data());
                glDeleteBuffers(VBOs.size(), VBOs.data());
                VAOs.clear();
                VBOs.clear();
                vertexCounts.clear();
            }
            
            if (cityGenerator.hasCity()) {
                const CityData& city = cityGenerator.getCityData();
                
                // Create buffers for roads
                for (const auto& road : city.roads) {
                    auto vertices = pointsToVertices(road.points, SCREEN_WIDTH, SCREEN_HEIGHT);
                    
                    unsigned int VAO, VBO;
                    glGenVertexArrays(1, &VAO);
                    glGenBuffers(1, &VBO);
                    
                    glBindVertexArray(VAO);
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), 
                                vertices.data(), GL_STATIC_DRAW);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                    glEnableVertexAttribArray(0);
                    
                    VAOs.push_back(VAO);
                    VBOs.push_back(VBO);
                    vertexCounts.push_back(vertices.size() / 3);
                }
                
                int roadCount = city.roads.size();
                
                // Create buffers for parks
                for (const auto& park : city.parks) {
                    auto vertices = pointsToVertices(park, SCREEN_WIDTH, SCREEN_HEIGHT);
                    
                    unsigned int VAO, VBO;
                    glGenVertexArrays(1, &VAO);
                    glGenBuffers(1, &VBO);
                    
                    glBindVertexArray(VAO);
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), 
                                vertices.data(), GL_STATIC_DRAW);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                    glEnableVertexAttribArray(0);
                    
                    VAOs.push_back(VAO);
                    VBOs.push_back(VBO);
                    vertexCounts.push_back(vertices.size() / 3);
                }
                
                // Create buffers for buildings (3D cubes)
                for (const auto& building : city.buildings) {
                    auto vertices = buildingToVertices(building, SCREEN_WIDTH, SCREEN_HEIGHT);
                    
                    unsigned int VAO, VBO;
                    glGenVertexArrays(1, &VAO);
                    glGenBuffers(1, &VBO);
                    
                    glBindVertexArray(VAO);
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), 
                                vertices.data(), GL_STATIC_DRAW);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                    glEnableVertexAttribArray(0);
                    
                    VAOs.push_back(VAO);
                    VBOs.push_back(VBO);
                    vertexCounts.push_back(vertices.size() / 3);
                }
            }
        }
        
        // Dark background (like a city at dusk)
        glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Render the city if generated
        if (cityGenerator.hasCity() && !VAOs.empty()) {
            const CityData& city = cityGenerator.getCityData();
            size_t roadCount = city.roads.size();
            size_t parkCount = city.parks.size();
            size_t buildingStart = roadCount + parkCount;
            
            // Set is2D uniform for 2D elements
            int is2DLocation = glGetUniformLocation(shaderProgram, "is2D");
            glUniform1i(is2DLocation, 1);  // Enable 2D mode
            
            // Draw roads (yellow/orange)
            glUniform3f(colorLocation, 1.0f, 0.8f, 0.2f);
            for (size_t i = 0; i < roadCount && i < VAOs.size(); i++) {
                glBindVertexArray(VAOs[i]);
                glDrawArrays(GL_POINTS, 0, vertexCounts[i]);
            }
            
            // Draw parks (green)
            glUniform3f(colorLocation, 0.2f, 0.8f, 0.3f);
            for (size_t i = roadCount; i < buildingStart && i < VAOs.size(); i++) {
                glBindVertexArray(VAOs[i]);
                glDrawArrays(GL_POINTS, 0, vertexCounts[i]);
            }
            
            // Draw buildings (3D with color based on type)
            glUniform1i(is2DLocation, 0);  // Disable 2D mode for buildings
            for (size_t i = buildingStart; i < VAOs.size(); i++) {
                size_t buildingIndex = i - buildingStart;
                if (buildingIndex < city.buildings.size()) {
                    const Building& building = city.buildings[buildingIndex];
                    
                    // Set color based on building type
                    switch (building.type) {
                        case BuildingType::LOW_RISE:
                            glUniform3f(colorLocation, 0.7f, 0.4f, 0.3f);  // Brick red
                            break;
                        case BuildingType::MID_RISE:
                            glUniform3f(colorLocation, 0.5f, 0.5f, 0.5f);  // Gray
                            break;
                        case BuildingType::HIGH_RISE:
                            glUniform3f(colorLocation, 0.6f, 0.7f, 0.8f);  // Glass blue
                            break;
                    }
                    
                    glBindVertexArray(VAOs[i]);
                    glDrawArrays(GL_TRIANGLES, 0, vertexCounts[i]);
                }
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup
    if (!VAOs.empty()) {
        glDeleteVertexArrays(VAOs.size(), VAOs.data());
        glDeleteBuffers(VBOs.size(), VBOs.data());
    }

    // Cleanup
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
