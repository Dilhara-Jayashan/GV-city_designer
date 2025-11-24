/**
 * @file application.cpp
 * @brief Implementation of Application and Window Management
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include "core/application.h"
#include "rendering/camera.h"
#include <iostream>

// Static camera pointer for callbacks
static Camera* g_callbackCamera = nullptr;
static bool g_firstMouse = true;
static float g_lastX = 400.0f;
static float g_lastY = 300.0f;

// Framebuffer size callback
static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Mouse callback
static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (g_firstMouse) {
        g_lastX = xpos;
        g_lastY = ypos;
        g_firstMouse = false;
    }

    float xoffset = xpos - g_lastX;
    float yoffset = g_lastY - ypos;
    g_lastX = xpos;
    g_lastY = ypos;

    if (g_callbackCamera) {
        g_callbackCamera->processMouseMovement(xoffset, yoffset);
    }
}

// Constructor
Application::Application(int width, int height, const std::string& title)
    : window(nullptr)
    , width(width)
    , height(height)
    , title(title)
{
    if (!initGLFW()) {
        std::cout << "Failed to initialize GLFW\n";
        return;
    }
    
    if (!createWindow()) {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return;
    }
    
    if (!loadGL()) {
        std::cout << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        window = nullptr;
        return;
    }
    
    // Set viewport
    glViewport(0, 0, width, height);
}

// Destructor
Application::~Application() {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

// Initialize GLFW
bool Application::initGLFW() {
    if (!glfwInit()) {
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    return true;
}

// Create window
bool Application::createWindow() {
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window) {
        return false;
    }
    
    glfwMakeContextCurrent(window);
    return true;
}

// Load OpenGL
bool Application::loadGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return false;
    }
    return true;
}

// Setup callbacks
void Application::setupCallbacks(Camera* camera) {
    g_callbackCamera = camera;
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    // Enable FPP camera controls
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// Check if should close
bool Application::shouldClose() const {
    return glfwWindowShouldClose(window);
}

// Update (swap buffers and poll events)
void Application::update() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}
