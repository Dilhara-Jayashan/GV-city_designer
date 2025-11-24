#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "core/application.h"
#include "core/city_config.h"
#include "utils/algorithms.h"
#include "utils/input_handler.h"
#include "generation/city_generator.h"
#include "rendering/texture_manager.h"
#include "rendering/shaders/shader_manager.h"
#include "rendering/camera.h"
#include "rendering/city_renderer.h"

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
    
    // Initialize application (GLFW + OpenGL context)
    Application app(SCREEN_WIDTH, SCREEN_HEIGHT, "City Designer - Interactive Mode");
    if (!app.isValid()) {
        return -1;
    }
    
    // Create camera
    Camera camera(glm::vec3(0.0f, 2.0f, 6.0f), -90.0f, 0.0f);
    camera.setMovementSpeed(5.0f);
    
    // Setup callbacks (framebuffer resize + mouse)
    app.setupCallbacks(&camera);
    
    // Create renderer
    CityRenderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);

    // ----- Shader Compilation (Using ShaderManager) -----
    ShaderManager shaderManager;
    if (!shaderManager.compileShaders()) {
        std::cout << "Failed to compile shaders\n";
        return -1;
    }

    // Enable point size for better visibility
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(2.0f);
    
    // Enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);
    
    // ----- Load Textures (Using TextureManager) -----
    TextureManager textureManager;
    textureManager.loadAllTextures();
    
    GLuint brickTexture = textureManager.getTexture("brick");
    GLuint concreteTexture = textureManager.getTexture("concrete");
    GLuint glassTexture = textureManager.getTexture("glass");
    GLuint roadTexture = textureManager.getTexture("road");
    GLuint grassTexture = textureManager.getTexture("grass");
    GLuint fountainTexture = textureManager.getTexture("fountain");

    // Connect input handler to city generator
    inputHandler.setCityGenerator(&cityGenerator);

    std::cout << "\n✅ OpenGL initialized successfully!\n";
    std::cout << "Press 'G' to generate a city, or adjust parameters first.\n\n";

    // Track view mode changes
    bool lastView3D = cityConfig.view3D;
    
    // ----- Render Loop -----
    while (!app.shouldClose())
    {
        // Process user input
        inputHandler.processInput(app.getWindow());
        
        // FPP Camera movement (WASD + Shift for sprint)
        camera.processKeyboard(app.getWindow(), 0.016f); // Assuming ~60 FPS
        
        // Check if view mode changed (need to regenerate building buffers)
        bool viewModeChanged = (cityConfig.view3D != lastView3D);
        if (viewModeChanged) {
            lastView3D = cityConfig.view3D;
        }
        
        // If city was generated OR view mode changed, update rendering data
        if (inputHandler.generationRequested() || viewModeChanged) {
            inputHandler.clearGenerationRequest();
            
            if (cityGenerator.hasCity()) {
                const CityData& city = cityGenerator.getCityData();
                renderer.updateCity(city, cityConfig.view3D);
            }
        }
        
        // Dark background (like a city at dusk)
        glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderManager.use();
        
        // Setup view and projection matrices based on view mode
        glm::mat4 view, projection;
        
        if (cityConfig.view3D) {
            // 3D perspective view
            projection = glm::perspective(glm::radians(45.0f), 
                (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
            view = camera.getViewMatrix();
        } else {
            // 2D orthographic view
            projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 10.0f);
            view = glm::mat4(1.0f);  // Identity matrix
        }
        
        shaderManager.setView(glm::value_ptr(view));
        shaderManager.setProjection(glm::value_ptr(projection));

        // Render the city if generated
        if (cityGenerator.hasCity() && renderer.isReady()) {
            const CityData& city = cityGenerator.getCityData();
            renderer.render(city, cityConfig.view3D, shaderManager,
                          brickTexture, concreteTexture, glassTexture,
                          roadTexture, grassTexture, fountainTexture);
        }

        app.update();
    }
    
    // Cleanup - All resources automatically cleaned up by destructors:
    // - Application handles GLFW termination
    // - CityRenderer handles VAO/VBO cleanup
    // - TextureManager handles texture cleanup
    // - ShaderManager handles shader cleanup
    // - Camera has no GPU resources

    return 0;
}
