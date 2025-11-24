#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <GLFW/glfw3.h>
#include "city_config.h"

// Forward declaration
class CityGenerator;

// Input Handler Class
// Manages keyboard input and updates city configuration
class InputHandler {
private:
    CityConfig& config;
    bool keysPressed[GLFW_KEY_LAST];  // Track key states to prevent repeated actions
    CityGenerator* cityGen;  // Pointer to city generator
    
public:
    InputHandler(CityConfig& cfg);
    
    // Set the city generator (for triggering generation)
    void setCityGenerator(CityGenerator* gen) { cityGen = gen; }
    
    // Process keyboard input
    void processInput(GLFWwindow* window);
    
    // Display help/controls
    static void displayControls();
    
    // Check if generation was requested
    bool generationRequested() const { return genRequested; }
    void clearGenerationRequest() { genRequested = false; }
    
private:
    // Helper to check if key was just pressed (not held)
    bool isKeyJustPressed(GLFWwindow* window, int key);
    
    bool genRequested;  // Flag for generation request
};

#endif // INPUT_HANDLER_H
