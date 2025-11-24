/**
 * @file application.h
 * @brief Application and Window Management
 * 
 * Handles:
 * - GLFW initialization and termination
 * - Window creation and configuration
 * - OpenGL context setup
 * - Callback registration
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

// Forward declare callback types
class Camera;

/**
 * @class Application
 * @brief Manages GLFW window and OpenGL context
 * 
 * This class encapsulates all window and context management,
 * providing a clean RAII wrapper around GLFW.
 */
class Application {
public:
    /**
     * @brief Construct a new Application
     * @param width Window width in pixels
     * @param height Window height in pixels
     * @param title Window title
     */
    Application(int width, int height, const std::string& title);
    
    /**
     * @brief Destroy the Application and cleanup GLFW
     */
    ~Application();
    
    /**
     * @brief Check if application initialized successfully
     * @return true if window and OpenGL context are ready
     */
    bool isValid() const { return window != nullptr; }
    
    /**
     * @brief Get the GLFW window handle
     * @return GLFWwindow* Window pointer
     */
    GLFWwindow* getWindow() const { return window; }
    
    /**
     * @brief Check if window should close
     * @return true if close was requested
     */
    bool shouldClose() const;
    
    /**
     * @brief Swap buffers and poll events
     * 
     * Call this at the end of each frame.
     */
    void update();
    
    /**
     * @brief Set up camera-related callbacks
     * @param camera Pointer to camera for mouse callback
     * 
     * Sets up framebuffer size and mouse callbacks.
     * Camera pointer is stored for use in callbacks.
     */
    void setupCallbacks(Camera* camera);
    
    /**
     * @brief Get window width
     * @return int Width in pixels
     */
    int getWidth() const { return width; }
    
    /**
     * @brief Get window height
     * @return int Height in pixels
     */
    int getHeight() const { return height; }
    
private:
    GLFWwindow* window;
    int width;
    int height;
    std::string title;
    
    /**
     * @brief Initialize GLFW library
     * @return true if successful
     */
    bool initGLFW();
    
    /**
     * @brief Create window and OpenGL context
     * @return true if successful
     */
    bool createWindow();
    
    /**
     * @brief Load OpenGL function pointers using GLAD
     * @return true if successful
     */
    bool loadGL();
};

#endif // APPLICATION_H
