#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "algorithms.h"

// Window resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Vertex Shader Source
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
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

// Function to convert algorithm points to OpenGL vertices
std::vector<float> pointsToVertices(const std::vector<Point>& points, int screenWidth, int screenHeight) {
    std::vector<float> vertices;
    for (const auto& point : points) {
        // Convert pixel coordinates to normalized device coordinates (-1 to 1)
        float x = (point.x / (screenWidth / 2.0f)) - 1.0f;
        float y = 1.0f - (point.y / (screenHeight / 2.0f));
        vertices.push_back(x);
        vertices.push_back(y);
    }
    return vertices;
}

int main()
{
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "City Designer", NULL, NULL);
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

    // Get color uniform location
    int colorLocation = glGetUniformLocation(shaderProgram, "color");

    // ----- Generate Algorithm Data -----
    std::cout << "\n=== TESTING ALGORITHMS ===\n";
    
    // Test Bresenham's Line Algorithm - Draw road grid
    std::cout << "1. Bresenham's Line Algorithm:\n";
    std::vector<Point> line1 = bresenhamLine(100, 100, 700, 100);  // Horizontal road
    std::vector<Point> line2 = bresenhamLine(100, 100, 100, 500);  // Vertical road
    std::vector<Point> line3 = bresenhamLine(100, 500, 700, 100);  // Diagonal road
    std::cout << "   - Generated " << line1.size() << " points for horizontal road\n";
    std::cout << "   - Generated " << line2.size() << " points for vertical road\n";
    std::cout << "   - Generated " << line3.size() << " points for diagonal road\n";
    
    // Test Midpoint Circle Algorithm - Draw parks/fountains
    std::cout << "\n2. Midpoint Circle Algorithm:\n";
    std::vector<Point> circle1 = midpointCircle(200, 300, 80);  // Large park
    std::vector<Point> circle2 = midpointCircle(500, 300, 50);  // Medium fountain
    std::vector<Point> circle3 = midpointCircle(600, 450, 30);  // Small roundabout
    std::cout << "   - Generated " << circle1.size() << " points for large park (r=80)\n";
    std::cout << "   - Generated " << circle2.size() << " points for fountain (r=50)\n";
    std::cout << "   - Generated " << circle3.size() << " points for roundabout (r=30)\n";
    
    // Convert all points to vertices
    auto line1Verts = pointsToVertices(line1, 800, 600);
    auto line2Verts = pointsToVertices(line2, 800, 600);
    auto line3Verts = pointsToVertices(line3, 800, 600);
    auto circle1Verts = pointsToVertices(circle1, 800, 600);
    auto circle2Verts = pointsToVertices(circle2, 800, 600);
    auto circle3Verts = pointsToVertices(circle3, 800, 600);

    // Create VAO and VBO for each shape
    unsigned int VAOs[6], VBOs[6];
    glGenVertexArrays(6, VAOs);
    glGenBuffers(6, VBOs);

    // Setup line 1
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, line1Verts.size() * sizeof(float), line1Verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Setup line 2
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, line2Verts.size() * sizeof(float), line2Verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Setup line 3
    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, line3Verts.size() * sizeof(float), line3Verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Setup circle 1
    glBindVertexArray(VAOs[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, circle1Verts.size() * sizeof(float), circle1Verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Setup circle 2
    glBindVertexArray(VAOs[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[4]);
    glBufferData(GL_ARRAY_BUFFER, circle2Verts.size() * sizeof(float), circle2Verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Setup circle 3
    glBindVertexArray(VAOs[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[5]);
    glBufferData(GL_ARRAY_BUFFER, circle3Verts.size() * sizeof(float), circle3Verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    std::cout << "\n=== VISUALIZATION ===\n";
    std::cout << "Yellow lines: Roads (Bresenham's Algorithm)\n";
    std::cout << "Green circles: Parks and fountains (Midpoint Circle Algorithm)\n";
    std::cout << "Press ESC or close window to exit\n\n";

    // ----- Render Loop -----
    while (!glfwWindowShouldClose(window))
    {
        // Dark background (like a city at dusk)
        glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Draw roads using Bresenham's algorithm (Yellow/Orange color)
        glUniform3f(colorLocation, 1.0f, 0.8f, 0.2f);  // Yellow-orange for roads
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_POINTS, 0, line1Verts.size() / 2);
        
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_POINTS, 0, line2Verts.size() / 2);
        
        glBindVertexArray(VAOs[2]);
        glDrawArrays(GL_POINTS, 0, line3Verts.size() / 2);

        // Draw parks/fountains using Midpoint Circle algorithm (Green color)
        glUniform3f(colorLocation, 0.2f, 0.8f, 0.3f);  // Green for parks
        glBindVertexArray(VAOs[3]);
        glDrawArrays(GL_POINTS, 0, circle1Verts.size() / 2);
        
        glUniform3f(colorLocation, 0.3f, 0.7f, 1.0f);  // Blue for fountain
        glBindVertexArray(VAOs[4]);
        glDrawArrays(GL_POINTS, 0, circle2Verts.size() / 2);
        
        glUniform3f(colorLocation, 0.9f, 0.9f, 0.2f);  // Yellow for roundabout
        glBindVertexArray(VAOs[5]);
        glDrawArrays(GL_POINTS, 0, circle3Verts.size() / 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(6, VAOs);
    glDeleteBuffers(6, VBOs);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
