#include "utils/input_handler.h"
#include "generation/city_generator.h"
#include "features/save_load/city_serializer.h"
#include <iostream>
#include <cstring>

InputHandler::InputHandler(CityConfig& cfg) 
    : config(cfg), cityGen(nullptr), genRequested(false), 
      mouseButtonPressed(false), lastMouseX(0), lastMouseY(0), 
      buildingPlacementRequested(false), loadRequested(false) {
    // Initialize key states
    std::memset(keysPressed, 0, sizeof(keysPressed));
}

bool InputHandler::isKeyJustPressed(GLFWwindow* window, int key) {
    int state = glfwGetKey(window, key);
    bool wasPressed = keysPressed[key];
    keysPressed[key] = (state == GLFW_PRESS);
    
    // Return true only on the first press, not while held
    return (state == GLFW_PRESS && !wasPressed);
}

void InputHandler::processInput(GLFWwindow* window) {
    // ESC to close
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    // H - Display help
    if (isKeyJustPressed(window, GLFW_KEY_H)) {
        displayControls();
    }
    
    // P - Print current configuration
    if (isKeyJustPressed(window, GLFW_KEY_P)) {
        config.printConfig();
    }
    
    // === BUILDING CONTROLS ===
    // 1/2 - Increase/Decrease number of buildings
    if (isKeyJustPressed(window, GLFW_KEY_1)) {
        config.numBuildings = std::max(1, config.numBuildings - 2);
        std::cout << "Buildings: " << config.numBuildings << "\n";
    }
    if (isKeyJustPressed(window, GLFW_KEY_2)) {
        config.numBuildings = std::min(100, config.numBuildings + 2);
        std::cout << "Buildings: " << config.numBuildings << "\n";
    }
    
    // 3/4 - Increase/Decrease layout size
    if (isKeyJustPressed(window, GLFW_KEY_3)) {
        config.layoutSize = std::max(5, config.layoutSize - 1);
        config.updateStandardBuildingSize();  // Auto-adjust building size
        std::cout << "Layout Size: " << config.layoutSize << "x" << config.layoutSize << "\n";
        if (config.useStandardSize) {
            std::cout << "  Building Size adjusted to: " << static_cast<int>(config.standardWidth) 
                      << "x" << static_cast<int>(config.standardDepth) << " px\n";
        }
    }
    if (isKeyJustPressed(window, GLFW_KEY_4)) {
        config.layoutSize = std::min(20, config.layoutSize + 1);
        config.updateStandardBuildingSize();  // Auto-adjust building size
        std::cout << "Layout Size: " << config.layoutSize << "x" << config.layoutSize << "\n";
        if (config.useStandardSize) {
            std::cout << "  Building Size adjusted to: " << static_cast<int>(config.standardWidth) 
                      << "x" << static_cast<int>(config.standardDepth) << " px\n";
        }
    }
    
    // B - Toggle standard/random building size
    if (isKeyJustPressed(window, GLFW_KEY_B)) {
        config.useStandardSize = !config.useStandardSize;
        std::cout << "Building Size: " << (config.useStandardSize ? "Standard" : "Random") << "\n";
        if (config.useStandardSize) {
            std::cout << "  (Width/Depth: " << config.standardWidth << "x" << config.standardDepth << " px)\n";
        }
    }
    
    // === ROAD PATTERN CONTROLS ===
    // R - Cycle through road patterns
    if (isKeyJustPressed(window, GLFW_KEY_R)) {
        int current = static_cast<int>(config.roadPattern);
        current = (current + 1) % 3;  // 3 patterns
        config.roadPattern = static_cast<RoadPattern>(current);
        std::cout << "Road Pattern: " << config.getRoadPatternString() << "\n";
    }
    
    // 5/6 - Increase/Decrease road width
    if (isKeyJustPressed(window, GLFW_KEY_5)) {
        config.roadWidth = std::max(2, config.roadWidth - 2);
        std::cout << "Road Width: " << config.roadWidth << " pixels (Press G to regenerate)\n";
    }
    if (isKeyJustPressed(window, GLFW_KEY_6)) {
        config.roadWidth = std::min(20, config.roadWidth + 2);
        std::cout << "Road Width: " << config.roadWidth << " pixels (Press G to regenerate)\n";
    }
    
    // === SKYLINE CONTROLS ===
    // L - Cycle through skyline types
    if (isKeyJustPressed(window, GLFW_KEY_L)) {
        int current = static_cast<int>(config.skylineType);
        current = (current + 1) % 4;  // 4 skyline types
        config.skylineType = static_cast<SkylineType>(current);
        std::cout << "Skyline Type: " << config.getSkylineTypeString() << "\n";
    }
    
    // === TEXTURE CONTROLS ===
    // T - Cycle through texture themes
    if (isKeyJustPressed(window, GLFW_KEY_T)) {
        int current = static_cast<int>(config.textureTheme);
        current = (current + 1) % 4;  // 4 texture themes
        config.textureTheme = static_cast<TextureTheme>(current);
        std::cout << "Texture Theme: " << config.getTextureThemeString() << "\n";
    }
    
    // === PARK/FOUNTAIN CONTROLS ===
    // 7/8 - Increase/Decrease park radius
    if (isKeyJustPressed(window, GLFW_KEY_7)) {
        config.parkRadius = std::max(10, config.parkRadius - 5);
        std::cout << "Park Radius: " << config.parkRadius << "\n";
    }
    if (isKeyJustPressed(window, GLFW_KEY_8)) {
        config.parkRadius = std::min(100, config.parkRadius + 5);
        std::cout << "Park Radius: " << config.parkRadius << "\n";
    }
    
    // 9/0 - Increase/Decrease number of parks
    if (isKeyJustPressed(window, GLFW_KEY_9)) {
        config.numParks = std::max(0, config.numParks - 1);
        std::cout << "Number of Parks: " << config.numParks << "\n";
    }
    if (isKeyJustPressed(window, GLFW_KEY_0)) {
        config.numParks = std::min(10, config.numParks + 1);
        std::cout << "Number of Parks: " << config.numParks << "\n";
    }
    
    // F - Fountain radius toggle
    if (isKeyJustPressed(window, GLFW_KEY_F)) {
        config.fountainRadius = (config.fountainRadius == 25) ? 40 : 25;
        std::cout << "Fountain Radius: " << config.fountainRadius << "\n";
    }
    
    // === TIME OF DAY CONTROLS ===
    // N - Toggle day/night cycle auto-progression
    if (isKeyJustPressed(window, GLFW_KEY_N)) {
        config.autoTimeProgress = !config.autoTimeProgress;
        std::cout << "Auto Time: " << (config.autoTimeProgress ? "ON" : "OFF") << "\n";
    }
    
    // M - Manual time control (skip forward 2 hours)
    if (isKeyJustPressed(window, GLFW_KEY_M)) {
        config.timeOfDay += 2.0f;
        if (config.timeOfDay >= 24.0f) config.timeOfDay -= 24.0f;
        std::cout << "Time: " << static_cast<int>(config.timeOfDay) << ":00\n";
    }
    
    // === VIEW MODE ===
    // V - Toggle 2D/3D view
    if (isKeyJustPressed(window, GLFW_KEY_V)) {
        config.view3D = !config.view3D;
        std::cout << "View Mode: " << (config.view3D ? "3D" : "2D") << "\n";
    }
    
    // G - Generate new city with current settings
    if (isKeyJustPressed(window, GLFW_KEY_G)) {
        genRequested = true;
        if (cityGen) {
            // Show keyboard controls BEFORE generation
            displayControls();
            // Now generate the city
            cityGen->generateCity(config);
        }
    }
    
    // Z - Save current city
    if (isKeyJustPressed(window, GLFW_KEY_Z)) {
        if (cityGen && cityGen->hasCity()) {
            CitySerializer::saveCity(cityGen->getCityData(), "city_save");
        } else {
            std::cout << "⚠️  No city to save! Generate a city first (press G).\n";
        }
    }
    
    // X - Load saved city
    if (isKeyJustPressed(window, GLFW_KEY_X)) {
        loadRequested = true;
    }
}

void InputHandler::displayControls() {
    std::cout << "\n";
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║              CITY DESIGNER - KEYBOARD CONTROLS            ║\n";
    std::cout << "╠═══════════════════════════════════════════════════════════╣\n";
    std::cout << "║  BUILDING CONTROLS:                                       ║\n";
    std::cout << "║    1/2  : Decrease/Increase number of buildings           ║\n";
    std::cout << "║    3/4  : Decrease/Increase layout size                   ║\n";
    std::cout << "║    B    : Toggle standard/random building size            ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║  ROAD CONTROLS:                                           ║\n";
    std::cout << "║    R    : Cycle road pattern (Grid/Radial/Random)        ║\n";
    std::cout << "║    5/6  : Decrease/Increase road width                    ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║  SKYLINE CONTROLS:                                        ║\n";
    std::cout << "║    L    : Cycle skyline type                              ║\n";
    std::cout << "║           (Low-Rise/Mid-Rise/Skyscraper/Mixed)            ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║  TEXTURE CONTROLS:                                        ║\n";
    std::cout << "║    T    : Cycle texture theme                             ║\n";
    std::cout << "║           (Modern/Classic/Industrial/Futuristic)          ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║  PARK/FOUNTAIN CONTROLS:                                  ║\n";
    std::cout << "║    7/8  : Decrease/Increase park radius                   ║\n";
    std::cout << "║    9/0  : Decrease/Increase number of parks               ║\n";
    std::cout << "║    F    : Toggle fountain size (small/large)              ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║  TIME OF DAY:                                             ║\n";
    std::cout << "║    N    : Toggle auto day/night cycle                     ║\n";
    std::cout << "║    M    : Manual time advance (+2 hours)                  ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║  VIEW & GENERATION:                                       ║\n";
    std::cout << "║    V    : Toggle 2D/3D view mode                          ║\n";
    std::cout << "║    G    : Generate new city with current settings         ║\n";
    std::cout << "║    Z    : Save current city to file                       ║\n";
    std::cout << "║    X    : Load saved city from file                       ║\n";
    std::cout << "║    P    : Print current configuration                     ║\n";
    std::cout << "║    H    : Display this help menu                          ║\n";
    std::cout << "║    ESC  : Exit application                                ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║  3D NAVIGATION (3D mode only):                            ║\n";
    std::cout << "║    W/A/S/D : Move forward/left/backward/right             ║\n";
    std::cout << "║    SHIFT   : Sprint (faster movement)                     ║\n";
    std::cout << "║    MOUSE   : Look around (camera rotation)                ║\n";
    std::cout << "║ Make sure put mouse in middle of the screen when move to  ║\n";
    std::cout << "║ 3d view (if you can not find check 360 around you)        ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║  2D BUILDING PLACEMENT:                                   ║\n";
    std::cout << "║    LEFT CLICK: Place building at cursor (2D mode only)    ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
}

// Process mouse input for building placement
void InputHandler::processMouseInput(GLFWwindow* window, int screenWidth, int screenHeight) {
    // Only process mouse clicks in 2D mode
    if (config.view3D) {
        return;
    }
    
    // Check left mouse button
    int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    
    // Detect mouse button press (not held)
    if (mouseState == GLFW_PRESS && !mouseButtonPressed) {
        // Get cursor position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        // Store position and mark placement as requested
        lastMouseX = xpos;
        lastMouseY = ypos;
        buildingPlacementRequested = true;
        
        std::cout << "🏢 Building placement requested at (" << (int)xpos << ", " << (int)ypos << ")\n";
    }
    
    // Update button state
    mouseButtonPressed = (mouseState == GLFW_PRESS);
}
