/**
 * @file park_mesh.cpp
 * @brief Implementation of Park and Fountain 3D Mesh Generation
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include "rendering/mesh/park_mesh.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

std::vector<float> parkTo3DMesh(const std::vector<Point>& parkPoints, 
                                 int screenWidth, int screenHeight, bool is3D) {
    std::vector<float> vertices;
    
    if (parkPoints.size() < 3) return vertices;
    
    // Find center and radius from the circle points
    float centerX = 0.0f, centerZ = 0.0f;
    for (const auto& pt : parkPoints) {
        float x = (pt.x / (screenWidth / 2.0f)) - 1.0f;
        float z = 1.0f - (pt.y / (screenHeight / 2.0f));
        centerX += x;
        centerZ += z;
    }
    centerX /= parkPoints.size();
    centerZ /= parkPoints.size();
    
    // Calculate radius from first point
    float x0 = (parkPoints[0].x / (screenWidth / 2.0f)) - 1.0f;
    float z0 = 1.0f - (parkPoints[0].y / (screenHeight / 2.0f));
    float radius = std::sqrt((x0 - centerX) * (x0 - centerX) + (z0 - centerZ) * (z0 - centerZ));
    
    float parkHeight = 0.006f;  // Above roads (roads at 0.005f) to prevent overlap
    int segments = 32;  // Number of triangles to form circle
    
    // Create a filled circle using triangle fan
    for (int i = 0; i < segments; i++) {
        float angle1 = (i * 2.0f * M_PI) / segments;
        float angle2 = ((i + 1) * 2.0f * M_PI) / segments;
        
        float x1 = centerX + radius * std::cos(angle1);
        float z1 = centerZ + radius * std::sin(angle1);
        float x2 = centerX + radius * std::cos(angle2);
        float z2 = centerZ + radius * std::sin(angle2);
        
        // UV coordinates for texture mapping
        float u_center = 0.5f;
        float v_center = 0.5f;
        float u1 = 0.5f + 0.5f * std::cos(angle1);
        float v1 = 0.5f + 0.5f * std::sin(angle1);
        float u2 = 0.5f + 0.5f * std::cos(angle2);
        float v2 = 0.5f + 0.5f * std::sin(angle2);
        
        if (is3D) {
            // 3D MODE: Y is UP (horizontal grass plane)
            vertices.insert(vertices.end(), {
                centerX, parkHeight, centerZ,  u_center, v_center,
                x1, parkHeight, z1,  u1, v1,
                x2, parkHeight, z2,  u2, v2
            });
        } else {
            // 2D MODE: Z is depth
            vertices.insert(vertices.end(), {
                centerX, centerZ, parkHeight,  u_center, v_center,
                x1, z1, parkHeight,  u1, v1,
                x2, z2, parkHeight,  u2, v2
            });
        }
    }
    
    return vertices;
}

std::vector<float> fountainTo3DMesh(const std::vector<Point>& fountainPoints, 
                                     int screenWidth, int screenHeight, bool is3D) {
    std::vector<float> vertices;
    
    if (fountainPoints.size() < 3) return vertices;
    
    // Find center and radius from the circle points
    float centerX = 0.0f, centerZ = 0.0f;
    for (const auto& pt : fountainPoints) {
        float x = (pt.x / (screenWidth / 2.0f)) - 1.0f;
        float z = 1.0f - (pt.y / (screenHeight / 2.0f));
        centerX += x;
        centerZ += z;
    }
    centerX /= fountainPoints.size();
    centerZ /= fountainPoints.size();
    
    // Calculate radius from first point
    float x0 = (fountainPoints[0].x / (screenWidth / 2.0f)) - 1.0f;
    float z0 = 1.0f - (fountainPoints[0].y / (screenHeight / 2.0f));
    float radius = std::sqrt((x0 - centerX) * (x0 - centerX) + (z0 - centerZ) * (z0 - centerZ));
    
    float fountainHeight = 0.008f;  // Above parks (parks at 0.006f) to make it stand out
    int segments = 32;  // Number of triangles to form circle
    
    // Create a filled circle using triangle fan
    for (int i = 0; i < segments; i++) {
        float angle1 = (i * 2.0f * M_PI) / segments;
        float angle2 = ((i + 1) * 2.0f * M_PI) / segments;
        
        float x1 = centerX + radius * std::cos(angle1);
        float z1 = centerZ + radius * std::sin(angle1);
        float x2 = centerX + radius * std::cos(angle2);
        float z2 = centerZ + radius * std::sin(angle2);
        
        // UV coordinates for texture mapping
        float u_center = 0.5f;
        float v_center = 0.5f;
        float u1 = 0.5f + 0.5f * std::cos(angle1);
        float v1 = 0.5f + 0.5f * std::sin(angle1);
        float u2 = 0.5f + 0.5f * std::cos(angle2);
        float v2 = 0.5f + 0.5f * std::sin(angle2);
        
        if (is3D) {
            // 3D MODE: Y is UP (horizontal fountain plane)
            vertices.insert(vertices.end(), {
                centerX, fountainHeight, centerZ,  u_center, v_center,
                x1, fountainHeight, z1,  u1, v1,
                x2, fountainHeight, z2,  u2, v2
            });
        } else {
            // 2D MODE: Z is depth
            vertices.insert(vertices.end(), {
                centerX, centerZ, fountainHeight,  u_center, v_center,
                x1, z1, fountainHeight,  u1, v1,
                x2, z2, fountainHeight,  u2, v2
            });
        }
    }
    
    return vertices;
}
