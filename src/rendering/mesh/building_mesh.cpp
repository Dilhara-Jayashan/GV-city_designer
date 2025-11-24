/**
 * @file building_mesh.cpp
 * @brief Implementation of Building 3D Mesh Generation
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include "rendering/mesh/building_mesh.h"
#include <vector>

std::vector<float> buildingToVertices(const Building& building, int screenWidth, int screenHeight, bool is3D) {
    std::vector<float> vertices;
    
    // Convert pixel coordinates to world coordinates
    float centerX = (building.x / (screenWidth / 2.0f)) - 1.0f;
    float centerY = 1.0f - (building.y / (screenHeight / 2.0f));
    float halfWidth = building.width / (screenWidth / 2.0f);
    float halfDepth = building.depth / (screenHeight / 2.0f);
    float heightNorm = building.height / 300.0f;  // Normalize height for viewing
    
    if (!is3D) {
        // 2D MODE: Keep original coordinate system (Y for depth, Z for height)
        float x0 = centerX - halfWidth;
        float x1 = centerX + halfWidth;
        float y0 = centerY - halfDepth;
        float y1 = centerY + halfDepth;
        float z0 = 0.0f;
        float z1 = heightNorm;
        
        // Front face
        vertices.insert(vertices.end(), {
            x0, y0, z0,  0.0f, 0.0f,
            x1, y0, z0,  1.0f, 0.0f,
            x1, y0, z1,  1.0f, 1.0f,
            x0, y0, z0,  0.0f, 0.0f,
            x1, y0, z1,  1.0f, 1.0f,
            x0, y0, z1,  0.0f, 1.0f
        });
        
        // Back face
        vertices.insert(vertices.end(), {
            x1, y1, z0,  0.0f, 0.0f,
            x0, y1, z0,  1.0f, 0.0f,
            x0, y1, z1,  1.0f, 1.0f,
            x1, y1, z0,  0.0f, 0.0f,
            x0, y1, z1,  1.0f, 1.0f,
            x1, y1, z1,  0.0f, 1.0f
        });
        
        // Left face
        vertices.insert(vertices.end(), {
            x0, y1, z0,  0.0f, 0.0f,
            x0, y0, z0,  1.0f, 0.0f,
            x0, y0, z1,  1.0f, 1.0f,
            x0, y1, z0,  0.0f, 0.0f,
            x0, y0, z1,  1.0f, 1.0f,
            x0, y1, z1,  0.0f, 1.0f
        });
        
        // Right face
        vertices.insert(vertices.end(), {
            x1, y0, z0,  0.0f, 0.0f,
            x1, y1, z0,  1.0f, 0.0f,
            x1, y1, z1,  1.0f, 1.0f,
            x1, y0, z0,  0.0f, 0.0f,
            x1, y1, z1,  1.0f, 1.0f,
            x1, y0, z1,  0.0f, 1.0f
        });
        
        // Bottom face
        vertices.insert(vertices.end(), {
            x0, y0, z0,  0.0f, 0.0f,
            x0, y1, z0,  0.0f, 1.0f,
            x1, y1, z0,  1.0f, 1.0f,
            x0, y0, z0,  0.0f, 0.0f,
            x1, y1, z0,  1.0f, 1.0f,
            x1, y0, z0,  1.0f, 0.0f
        });
        
        // Top face
        vertices.insert(vertices.end(), {
            x0, y0, z1,  0.0f, 0.0f,
            x1, y0, z1,  1.0f, 0.0f,
            x1, y1, z1,  1.0f, 1.0f,
            x0, y0, z1,  0.0f, 0.0f,
            x1, y1, z1,  1.0f, 1.0f,
            x0, y1, z1,  0.0f, 1.0f
        });
        
        return vertices;
    }
    
    // 3D MODE: Use proper coordinate system (X=left/right, Y=up/down HEIGHT!, Z=depth)
    float centerZ = 1.0f - (building.y / (screenHeight / 2.0f));
    
    // Calculate the 8 corners of the cube
    float x0 = centerX - halfWidth;
    float x1 = centerX + halfWidth;
    float y0 = 0.0f;           // Ground level
    float y1 = heightNorm;     // Top of building (HEIGHT goes UP!)
    float z0 = centerZ - halfDepth;
    float z1 = centerZ + halfDepth;
    
    // Front face (facing -Z direction) - with UV coordinates
    vertices.insert(vertices.end(), {
        x0, y0, z0,  0.0f, 0.0f,
        x1, y0, z0,  1.0f, 0.0f,
        x1, y1, z0,  1.0f, 1.0f,
        x0, y0, z0,  0.0f, 0.0f,
        x1, y1, z0,  1.0f, 1.0f,
        x0, y1, z0,  0.0f, 1.0f
    });
    
    // Back face (facing +Z direction)
    vertices.insert(vertices.end(), {
        x1, y0, z1,  0.0f, 0.0f,
        x0, y0, z1,  1.0f, 0.0f,
        x0, y1, z1,  1.0f, 1.0f,
        x1, y0, z1,  0.0f, 0.0f,
        x0, y1, z1,  1.0f, 1.0f,
        x1, y1, z1,  0.0f, 1.0f
    });
    
    // Left face (facing -X direction)
    vertices.insert(vertices.end(), {
        x0, y0, z1,  0.0f, 0.0f,
        x0, y0, z0,  1.0f, 0.0f,
        x0, y1, z0,  1.0f, 1.0f,
        x0, y0, z1,  0.0f, 0.0f,
        x0, y1, z0,  1.0f, 1.0f,
        x0, y1, z1,  0.0f, 1.0f
    });
    
    // Right face (facing +X direction)
    vertices.insert(vertices.end(), {
        x1, y0, z0,  0.0f, 0.0f,
        x1, y0, z1,  1.0f, 0.0f,
        x1, y1, z1,  1.0f, 1.0f,
        x1, y0, z0,  0.0f, 0.0f,
        x1, y1, z1,  1.0f, 1.0f,
        x1, y0, z1,  0.0f, 1.0f
    });
    
    // Bottom face (ground, facing -Y direction)
    vertices.insert(vertices.end(), {
        x0, y0, z0,  0.0f, 0.0f,
        x0, y0, z1,  0.0f, 1.0f,
        x1, y0, z1,  1.0f, 1.0f,
        x0, y0, z0,  0.0f, 0.0f,
        x1, y0, z1,  1.0f, 1.0f,
        x1, y0, z0,  1.0f, 0.0f
    });
    
    // Top face (roof, facing +Y direction)
    vertices.insert(vertices.end(), {
        x0, y1, z0,  0.0f, 0.0f,
        x1, y1, z0,  1.0f, 0.0f,
        x1, y1, z1,  1.0f, 1.0f,
        x0, y1, z0,  0.0f, 0.0f,
        x1, y1, z1,  1.0f, 1.0f,
        x0, y1, z1,  0.0f, 1.0f
    });
    
    return vertices;
}
