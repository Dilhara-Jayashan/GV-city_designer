/**
 * @file mesh_utils.cpp
 * @brief Implementation of Mesh Utility Functions
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include "rendering/mesh/mesh_utils.h"

std::vector<float> pointsToVertices(const std::vector<Point>& points, 
                                     int screenWidth, 
                                     int screenHeight) {
    std::vector<float> vertices;
    int margin = 50;  // Boundary margin in pixels
    
    for (const auto& point : points) {
        // Skip points outside screen boundaries
        if (point.x < margin || point.x > screenWidth - margin ||
            point.y < margin || point.y > screenHeight - margin) {
            continue;
        }
        
        // Convert pixel coordinates to normalized device coordinates (-1 to 1)
        float x = (point.x / (screenWidth / 2.0f)) - 1.0f;
        float y = 1.0f - (point.y / (screenHeight / 2.0f));
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);  // Z coordinate for 2D elements
    }
    return vertices;
}
