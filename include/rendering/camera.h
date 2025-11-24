/**
 * @file camera.h
 * @brief First-Person Perspective Camera for 3D View
 * 
 * Implements a first-person camera with:
 * - Mouse look (yaw and pitch rotation)
 * - WASD movement controls
 * - Sprint functionality (Shift key)
 * - Smooth movement and rotation
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

/**
 * @class Camera
 * @brief First-Person Perspective camera for 3D navigation
 * 
 * This class manages the 3D camera position, orientation, and movement.
 * It provides smooth FPP controls similar to modern 3D games.
 */
class Camera {
public:
    /**
     * @brief Construct a new Camera
     * @param startPos Initial camera position in world space
     * @param startYaw Initial yaw angle (horizontal rotation) in degrees
     * @param startPitch Initial pitch angle (vertical rotation) in degrees
     */
    Camera(glm::vec3 startPos = glm::vec3(0.0f, 0.5f, 2.0f), 
           float startYaw = -90.0f, 
           float startPitch = 0.0f);
    
    /**
     * @brief Get the view matrix for rendering
     * @return glm::mat4 View transformation matrix
     * 
     * The view matrix transforms world coordinates to camera space.
     * Used in the vertex shader: projection * view * model
     */
    glm::mat4 getViewMatrix() const;
    
    /**
     * @brief Get camera position in world space
     * @return glm::vec3 Current position
     */
    glm::vec3 getPosition() const { return position; }
    
    /**
     * @brief Get camera front direction vector
     * @return glm::vec3 Direction the camera is looking
     */
    glm::vec3 getFront() const { return front; }
    
    /**
     * @brief Process keyboard input for camera movement
     * @param window GLFW window handle
     * @param deltaTime Time elapsed since last frame (for framerate-independent movement)
     * 
     * Controls:
     * - W: Move forward
     * - S: Move backward
     * - A: Strafe left
     * - D: Strafe right
     * - Shift: Sprint (2x speed)
     */
    void processKeyboard(GLFWwindow* window, float deltaTime);
    
    /**
     * @brief Process mouse movement for camera rotation
     * @param xoffset Horizontal mouse movement
     * @param yoffset Vertical mouse movement
     * @param constrainPitch If true, prevents camera from flipping upside down
     * 
     * Mouse sensitivity is configurable. Pitch is typically constrained
     * between -89 and +89 degrees to prevent gimbal lock.
     */
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    
    /**
     * @brief Reset camera to initial position and orientation
     * 
     * Useful for resetting the view when generating a new city.
     */
    void reset();
    
    /**
     * @brief Set mouse sensitivity
     * @param sensitivity Mouse sensitivity multiplier (default: 0.1f)
     */
    void setMouseSensitivity(float sensitivity) { mouseSensitivity = sensitivity; }
    
    /**
     * @brief Set movement speed
     * @param speed Movement speed in units per second (default: 2.5f)
     */
    void setMovementSpeed(float speed) { movementSpeed = speed; }
    
private:
    /**
     * @brief Update camera vectors based on yaw and pitch
     * 
     * Recalculates front, right, and up vectors when rotation changes.
     * Called internally after mouse movement.
     */
    void updateCameraVectors();
    
    // Camera position and orientation
    glm::vec3 position;          ///< Camera position in world space
    glm::vec3 front;             ///< Direction camera is facing
    glm::vec3 up;                ///< Up vector for camera space
    glm::vec3 right;             ///< Right vector for strafing
    glm::vec3 worldUp;           ///< World's up direction (usually +Y)
    
    // Euler angles for rotation
    float yaw;                   ///< Horizontal rotation (left/right)
    float pitch;                 ///< Vertical rotation (up/down)
    
    // Camera settings
    float movementSpeed;         ///< Movement speed (units per second)
    float sprintMultiplier;      ///< Sprint speed multiplier
    float mouseSensitivity;      ///< Mouse look sensitivity
    
    // Initial values for reset
    glm::vec3 initialPosition;
    float initialYaw;
    float initialPitch;
};

#endif // CAMERA_H
