/**
 * @file main.cpp
 * @brief City Designer - Main Entry Point
 * 
 * This is a feature-based city generation and visualization tool.
 * All 5 creative features are implemented:
 * 1. Building Window Lights (night time)
 * 2. Day/Night Cycle (sky colors, time progression)
 * 3. Traffic System (vehicle animation)
 * 4. Click-to-Place Buildings (interactive placement)
 * 5. Save/Load System (JSON serialization)
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STB Image Implementation (must be defined once)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Core Systems
#include "core/application.h"
#include "core/city_config.h"

// Generation Systems
#include "generation/city_generator.h"

// Rendering Systems
#include "rendering/camera.h"
#include "rendering/city_renderer.h"
#include "rendering/texture_manager.h"
#include "rendering/shaders/shader_manager.h"

// Feature Systems (The 5 Creative Features)
#include "features/building_lights/building_lighting_system.h"
#include "features/day_night_cycle/day_night_cycle.h"
#include "features/traffic_system/traffic_generator.h"
#include "features/building_placement/building_placement_system.h"
#include "features/save_load/city_serializer.h"

// Utility Systems
#include "utils/input_handler.h"

/**
 * @brief Main application entry point
 * 
 * Initializes all systems, creates the 5 feature systems,
 * and runs the main render loop.
 */
int main()
{
    // ===== CONFIGURATION =====
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    
    CityConfig cityConfig;
    InputHandler inputHandler(cityConfig);
    CityGenerator cityGenerator(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // ===== DISPLAY WELCOME MESSAGE =====
    std::cout << "\n";
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    🏙️  CITY DESIGNER 🏙️                   ║\n";
    std::cout << "║            Interactive 3D City Generation Tool            ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║  5 Creative Features:                                     ║\n";
    std::cout << "║  1️⃣  Building Window Lights (day/night)                   ║\n";
    std::cout << "║  2️⃣  Day/Night Cycle (sky transitions)                    ║\n";
    std::cout << "║  3️⃣  Traffic System (animated vehicles)                   ║\n";
    std::cout << "║  4️⃣  Click-to-Place Buildings (interactive)               ║\n";
    std::cout << "║  5️⃣  Save/Load System (JSON persistence)                  ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n";
    InputHandler::displayControls();
    cityConfig.printConfig();
    
    // ===== INITIALIZE OPENGL =====
    Application app(SCREEN_WIDTH, SCREEN_HEIGHT, "City Designer - Feature-Based Architecture");
    if (!app.isValid()) {
        return -1;
    }
    
    Camera camera(glm::vec3(0.0f, 2.0f, 6.0f), -90.0f, 0.0f);
    camera.setMovementSpeed(5.0f);
    app.setupCallbacks(&camera);
    
    // Set initial cursor mode
    glfwSetInputMode(app.getWindow(), GLFW_CURSOR,
                     cityConfig.view3D ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    
    CityRenderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // ===== FEATURE SYSTEMS INITIALIZATION =====
    BuildingLightingSystem buildingLights;          // Feature 1: Window Lights
    DayNightCycle dayNightCycle(14.0f, true);      // Feature 2: Day/Night Cycle
    TrafficGenerator trafficSystem;                 // Feature 3: Traffic
    BuildingPlacementSystem buildingPlacement;      // Feature 4: Click-to-Place
    // Feature 5 (Save/Load) is used via CitySerializer static methods
    
    // ===== SHADERS & TEXTURES =====
    ShaderManager shaderManager;
    if (!shaderManager.compileShaders()) {
        std::cout << "Failed to compile shaders\n";
        return -1;
    }
    
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(2.0f);
    glEnable(GL_DEPTH_TEST);
    
    TextureManager textureManager;
    textureManager.loadAllTextures();
    
    inputHandler.setCityGenerator(&cityGenerator);
    
    std::cout << "\n✅ All systems initialized!\n";
    std::cout << "Press 'G' to generate a city.\n";
    std::cout << "Press 'H' for keyboard controls.\n\n";
    
    // ===== RENDER LOOP =====
    bool lastView3D = cityConfig.view3D;
    float lastTime = glfwGetTime();
    float lastTimeOfDay = dayNightCycle.getTimeOfDay();
    
    while (!app.shouldClose())
    {
        // Calculate delta time
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // FEATURE 2: Update Day/Night Cycle
        dayNightCycle.update(deltaTime);
        cityConfig.timeOfDay = dayNightCycle.getTimeOfDay();
        
        // Process input
        inputHandler.processInput(app.getWindow());
        inputHandler.processMouseInput(app.getWindow(), SCREEN_WIDTH, SCREEN_HEIGHT);
        
        // Check if time was manually changed (via 'M' key)
        if (cityConfig.timeOfDay != lastTimeOfDay && cityConfig.timeOfDay != dayNightCycle.getTimeOfDay()) {
            // Manual time change detected - sync to dayNightCycle
            dayNightCycle.setTimeOfDay(cityConfig.timeOfDay);
        }
        lastTimeOfDay = cityConfig.timeOfDay;
        camera.processKeyboard(app.getWindow(), deltaTime);
        
        // Handle view mode changes
        bool viewModeChanged = (cityConfig.view3D != lastView3D);
        if (viewModeChanged) {
            lastView3D = cityConfig.view3D;
            glfwSetInputMode(app.getWindow(), GLFW_CURSOR,
                           cityConfig.view3D ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }
        
        // FEATURE 5: Handle load request
        if (inputHandler.loadCityRequested()) {
            inputHandler.clearLoadRequest();
            if (CitySerializer::loadCity(cityGenerator.getCityData(), "city_save")) {
                const CityData& city = cityGenerator.getCityData();
                renderer.updateCity(city, cityConfig.view3D);
                
                if (cityConfig.showTraffic && cityConfig.numCars > 0) {
                    trafficSystem.generateTraffic(city.roads, cityConfig.numCars,
                                                 city.parks, city.fountain,
                                                 SCREEN_WIDTH, SCREEN_HEIGHT);
                    renderer.updateTraffic(trafficSystem.getTrafficData(), cityConfig.view3D);
                }
            }
        }
        
        // Handle generation request
        if (inputHandler.generationRequested() || viewModeChanged) {
            inputHandler.clearGenerationRequest();
            
            if (cityGenerator.hasCity()) {
                const CityData& city = cityGenerator.getCityData();
                renderer.updateCity(city, cityConfig.view3D);
                
                // FEATURE 3: Generate traffic
                if (cityConfig.showTraffic && cityConfig.numCars > 0) {
                    trafficSystem.generateTraffic(city.roads, cityConfig.numCars,
                                                 city.parks, city.fountain,
                                                 SCREEN_WIDTH, SCREEN_HEIGHT);
                    renderer.updateTraffic(trafficSystem.getTrafficData(), cityConfig.view3D);
                }
                
                // Show keyboard controls after generation
                if (!viewModeChanged) {  // Only show on actual generation, not view mode change
                    InputHandler::displayControls();
                }
            }
        }
        
        // FEATURE 3: Update traffic
        if (trafficSystem.hasTraffic() && cityGenerator.hasCity()) {
            const CityData& city = cityGenerator.getCityData();
            trafficSystem.updateTraffic(deltaTime, city.roads);
            renderer.updateTraffic(trafficSystem.getTrafficData(), cityConfig.view3D);
        }
        
        // FEATURE 4: Handle building placement
        if (inputHandler.buildingPlacementPending() && cityGenerator.hasCity()) {
            double mouseX, mouseY;
            inputHandler.getBuildingPlacementPos(mouseX, mouseY);
            inputHandler.clearBuildingPlacement();
            
            CityData& city = cityGenerator.getCityData();
            if (buildingPlacement.tryPlaceBuilding((float)mouseX, (float)mouseY,
                                                   city.buildings, city.roads,
                                                   city.parks, city.fountain,
                                                   cityConfig, SCREEN_WIDTH, SCREEN_HEIGHT)) {
                renderer.updateCity(city, cityConfig.view3D);
            }
        }
        
        // FEATURE 2: Set sky color
        glm::vec3 skyColor = cityConfig.view3D ?
            dayNightCycle.getSkyColor() : dayNightCycle.getSkyColor2D();
        
        glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Setup shaders
        shaderManager.use();
        shaderManager.setTimeOfDay(cityConfig.timeOfDay);
        
        // Setup camera matrices
        glm::mat4 view, projection;
        if (cityConfig.view3D) {
            projection = glm::perspective(glm::radians(45.0f),
                (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
            view = camera.getViewMatrix();
        } else {
            projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 10.0f);
            view = glm::mat4(1.0f);
        }
        
        shaderManager.setView(glm::value_ptr(view));
        shaderManager.setProjection(glm::value_ptr(projection));
        
        // Render city
        if (cityGenerator.hasCity() && renderer.isReady()) {
            const CityData& city = cityGenerator.getCityData();
            renderer.render(city, cityConfig, cityConfig.view3D, shaderManager,
                          textureManager.getTexture("brick"),
                          textureManager.getTexture("concrete"),
                          textureManager.getTexture("glass"),
                          textureManager.getTexture("road"),
                          textureManager.getTexture("grass"),
                          textureManager.getTexture("fountain"));
            
            if (trafficSystem.hasTraffic()) {
                renderer.renderTraffic(trafficSystem.getTrafficData(), cityConfig,
                                      cityConfig.view3D, shaderManager);
            }
        }
        
        app.update();
    }
    
    // Cleanup handled by destructors
    return 0;
}
