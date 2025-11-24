/**
 * @file camera.cpp
 * @brief Implementation of First-Person Perspective Camera
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include "rendering/camera.h"
#include <algorithm>

// Constructor
Camera::Camera(glm::vec3 startPos, float startYaw, float startPitch)
    : position(startPos)
    , worldUp(0.0f, 1.0f, 0.0f)
    , yaw(startYaw)
    , pitch(startPitch)
    , movementSpeed(2.5f)
    , sprintMultiplier(2.0f)
    , mouseSensitivity(0.15f)
    , initialPosition(startPos)
    , initialYaw(startYaw)
    , initialPitch(startPitch)
{
    updateCameraVectors();
}

// Get view matrix for rendering
glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

// Process keyboard input for camera movement
void Camera::processKeyboard(GLFWwindow* window, float deltaTime) {
    // Calculate movement speed (with sprint modifier)
    float velocity = movementSpeed * deltaTime;
    
    // Check for sprint (Shift key)
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        velocity *= sprintMultiplier;
    }
    
    // Forward/Backward movement (W/S keys)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += front * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= front * velocity;
    }
    
    // Strafe Left/Right (A/D keys)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= right * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * velocity;
    }
}

// Process mouse movement for camera rotation
void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    // Apply mouse sensitivity
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    
    // Update rotation angles
    yaw   += xoffset;
    pitch += yoffset;
    
    // Constrain pitch to prevent screen flip
    if (constrainPitch) {
        pitch = std::max(-89.0f, std::min(89.0f, pitch));
    }
    
    // Update camera vectors based on new rotation
    updateCameraVectors();
}

// Reset camera to initial state
void Camera::reset() {
    position = initialPosition;
    yaw = initialYaw;
    pitch = initialPitch;
    updateCameraVectors();
}

// Update camera direction vectors based on Euler angles
void Camera::updateCameraVectors() {
    // Calculate new front vector from yaw and pitch
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    
    // Recalculate right and up vectors
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}
