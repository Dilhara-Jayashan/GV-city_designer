/**
 * @file road_mesh.cpp
 * @brief Implementation of Road 3D Mesh Generation
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include "rendering/mesh/road_mesh.h"
#include <glm/glm.hpp>
#include <cmath>

std::vector<float> roadTo3DMesh(const Road& road, int screenWidth, int screenHeight, bool is3D) {
    std::vector<float> vertices;
    
    if (road.points.size() < 2) return vertices;
    
    float roadWidth = 0.02f;  // Width in normalized coordinates
    int margin = 50;  // Boundary margin in pixels
    
    // Process each segment of the road
    for (size_t i = 0; i < road.points.size() - 1; i++) {
        // Skip points outside screen boundaries
        if (road.points[i].x < margin || road.points[i].x > screenWidth - margin ||
            road.points[i].y < margin || road.points[i].y > screenHeight - margin ||
            road.points[i + 1].x < margin || road.points[i + 1].x > screenWidth - margin ||
            road.points[i + 1].y < margin || road.points[i + 1].y > screenHeight - margin) {
            continue;  // Skip this segment if either endpoint is outside bounds
        }
        
        // Convert pixel coordinates to normalized device coordinates
        float x1 = (road.points[i].x / (screenWidth / 2.0f)) - 1.0f;
        float z1 = 1.0f - (road.points[i].y / (screenHeight / 2.0f));
        float x2 = (road.points[i + 1].x / (screenWidth / 2.0f)) - 1.0f;
        float z2 = 1.0f - (road.points[i + 1].y / (screenHeight / 2.0f));
        
        // Calculate direction and perpendicular
        glm::vec2 dir = glm::normalize(glm::vec2(x2 - x1, z2 - z1));
        glm::vec2 perp(-dir.y, dir.x);  // Perpendicular for width
        
        float halfWidth = roadWidth / 2.0f;
        
        // Four corners of road segment
        glm::vec2 v1(x1 + perp.x * halfWidth, z1 + perp.y * halfWidth);
        glm::vec2 v2(x1 - perp.x * halfWidth, z1 - perp.y * halfWidth);
        glm::vec2 v3(x2 + perp.x * halfWidth, z2 + perp.y * halfWidth);
        glm::vec2 v4(x2 - perp.x * halfWidth, z2 - perp.y * halfWidth);
        
        float roadHeight = 0.005f;  // Slightly above ground
        float texRepeat = glm::length(glm::vec2(x2 - x1, z2 - z1)) * 5.0f;  // Texture repeats along road
        
        if (is3D) {
            // 3D MODE: Y is UP
            // First triangle
            vertices.insert(vertices.end(), {
                v1.x, roadHeight, v1.y,  0.0f, 0.0f,
                v2.x, roadHeight, v2.y,  1.0f, 0.0f,
                v3.x, roadHeight, v3.y,  0.0f, texRepeat
            });
            
            // Second triangle
            vertices.insert(vertices.end(), {
                v2.x, roadHeight, v2.y,  1.0f, 0.0f,
                v4.x, roadHeight, v4.y,  1.0f, texRepeat,
                v3.x, roadHeight, v3.y,  0.0f, texRepeat
            });
        } else {
            // 2D MODE: Z is depth (for orthographic view)
            // First triangle
            vertices.insert(vertices.end(), {
                v1.x, v1.y, roadHeight,  0.0f, 0.0f,
                v2.x, v2.y, roadHeight,  1.0f, 0.0f,
                v3.x, v3.y, roadHeight,  0.0f, texRepeat
            });
            
            // Second triangle
            vertices.insert(vertices.end(), {
                v2.x, v2.y, roadHeight,  1.0f, 0.0f,
                v4.x, v4.y, roadHeight,  1.0f, texRepeat,
                v3.x, v3.y, roadHeight,  0.0f, texRepeat
            });
        }
    }
    
    return vertices;
}
