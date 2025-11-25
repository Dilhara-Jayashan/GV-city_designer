/**
 * @file input_handler.h
 * @brief User Input Processing System
 * 
 * Handles all keyboard and mouse input for the application:
 * - City generation controls (G key)
 * - Parameter adjustment (number keys, R, S, T, etc.)
 * - View mode switching (V for 2D/3D)
 * - Save/Load operations (C, L keys)
 * - Mouse clicks for building placement (2D mode only)
 * 
 * The input handler maintains key states to prevent repeat
 * actions when keys are held down. It updates the CityConfig
 * directly and signals generation requests via flags.
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <GLFW/glfw3.h>
#include "core/city_config.h"

// Forward declaration to avoid circular dependency
class CityGenerator;

/**
 * @class InputHandler
 * @brief Processes user input and updates application state
 * 
 * This class serves as the bridge between user actions and
 * the application logic. It handles:
 * 
 * Keyboard Input:
 * - G: Generate new city
 * - V: Toggle 2D/3D view
 * - R: Cycle road patterns (Grid → Radial → Random)
 * - S: Cycle skyline types (Low → Mid → High → Mixed)
 * - T: Cycle time of day / Toggle auto-progress
 * - 1-4: Adjust building count
 * - 5-6: Adjust layout size
 * - 7-0: Adjust park parameters
 * - C: Save city to JSON
 * - L: Load city from JSON
 * - H: Show/hide help
 * - ESC: Exit application
 * 
 * Mouse Input (2D mode only):
 * - Left Click: Place building at cursor position
 * 
 * Key State Tracking:
 * The handler maintains a key state array to detect when
 * a key transitions from released to pressed, preventing
 * repeated actions when keys are held down.
 */
class InputHandler {
private:
    CityConfig& config;                         ///< Reference to city configuration
    bool keysPressed[GLFW_KEY_LAST];           ///< Key state tracking array
    CityGenerator* cityGen;                     ///< Pointer to city generator
    
    // Mouse state for building placement
    bool mouseButtonPressed;                    ///< Left mouse button state
    double lastMouseX, lastMouseY;             ///< Mouse position at click
    bool buildingPlacementRequested;            ///< Flag: building placement pending
    bool loadRequested;                         ///< Flag: load operation requested
    
public:
    /**
     * @brief Construct a new Input Handler
     * @param cfg Reference to city configuration
     * 
     * Initializes key state tracking and mouse state.
     * Config reference is stored for direct parameter updates.
     */
    InputHandler(CityConfig& cfg);
    
    /**
     * @brief Set the city generator pointer
     * @param gen Pointer to CityGenerator instance
     * 
     * Required for triggering city generation and save operations.
     * Should be called before processing input.
     */
    void setCityGenerator(CityGenerator* gen) { cityGen = gen; }
    
    /**
     * @brief Process all keyboard input
     * @param window GLFW window handle
     * 
     * Called every frame from main loop. Checks all relevant keys:
     * - ESC: Close window
     * - G: Request city generation
     * - V: Toggle 2D/3D view mode
     * - R: Cycle road patterns
     * - S: Cycle skyline types
     * - T: Advance/toggle time
     * - 1-4: Building count adjustment
     * - 5-6: Layout size adjustment
     * - 7-0: Park/fountain parameters
     * - C: Save city
     * - L: Load city (sets flag)
     * - H: Toggle help display
     * 
     * Updates config immediately and sets flags for deferred actions.
     */
    void processInput(GLFWwindow* window);
    
    /**
     * @brief Process mouse input for building placement
     * @param window GLFW window handle
     * @param screenWidth Screen width for coordinate conversion
     * @param screenHeight Screen height for coordinate conversion
     * 
     * Only active in 2D view mode. Detects left mouse button clicks
     * and converts screen coordinates to world coordinates for
     * building placement (Feature 4).
     * 
     * When click detected:
     * 1. Stores click position in lastMouseX, lastMouseY
     * 2. Sets buildingPlacementRequested flag
     * 3. Main loop will call BuildingPlacementSystem
     */
    void processMouseInput(GLFWwindow* window, int screenWidth, int screenHeight);
    
    /**
     * @brief Display controls/help to console
     * 
     * Static method that prints comprehensive control list to stdout.
     * Shows all keyboard shortcuts and their functions.
     * Called at startup and when H key is pressed.
     */
    static void displayControls();
    
    /**
     * @brief Check if city generation was requested
     * @return true if G key was pressed
     * 
     * Main loop checks this flag to know when to call
     * cityGenerator.generateCity().
     */
    bool generationRequested() const { return genRequested; }
    
    /**
     * @brief Clear generation request flag
     * 
     * Called by main loop after handling generation request.
     * Resets flag to false.
     */
    void clearGenerationRequest() { genRequested = false; }
    
    /**
     * @brief Check if building placement is pending
     * @return true if mouse click occurred in 2D mode
     */
    bool buildingPlacementPending() const { return buildingPlacementRequested; }
    
    /**
     * @brief Get the building placement position
     * @param x Output: X coordinate of click
     * @param y Output: Y coordinate of click
     * 
     * Returns the world coordinates where building should be placed.
     */
    void getBuildingPlacementPos(double& x, double& y) const { 
        x = lastMouseX; 
        y = lastMouseY; 
    }
    
    /**
     * @brief Clear building placement flag
     * 
     * Called by main loop after handling placement attempt.
     */
    void clearBuildingPlacement() { buildingPlacementRequested = false; }
    
    /**
     * @brief Check if city load was requested
     * @return true if L key was pressed
     */
    bool loadCityRequested() const { return loadRequested; }
    
    /**
     * @brief Clear load request flag
     */
    void clearLoadRequest() { loadRequested = false; }
    
private:
    /**
     * @brief Check if key transitioned from released to pressed
     * @param window GLFW window handle
     * @param key GLFW key code
     * @return true if key was just pressed (not held)
     * 
     * Prevents repeated actions when keys are held down.
     * Compares current key state with stored previous state.
     */
    bool isKeyJustPressed(GLFWwindow* window, int key);
    
    bool genRequested;  ///< Flag: city generation requested
};

#endif // INPUT_HANDLER_H
