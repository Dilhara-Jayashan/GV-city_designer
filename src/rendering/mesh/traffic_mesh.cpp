#include "rendering/mesh/traffic_mesh.h"

std::vector<float> carTo3DMesh(const Car& car, int screenWidth, int screenHeight) {
    std::vector<float> vertices;
    
    // Normalize car position to OpenGL coordinates
    float nx = (car.x / (float)screenWidth) * 2.0f - 1.0f;
    float ny = 1.0f - (car.y / (float)screenHeight) * 2.0f;
    float nz = 0.01f;  // Slightly above ground
    
    // Car dimensions (small cube)
    float carWidth = 0.015f;
    float carDepth = 0.025f;  // Longer in direction of travel
    float carHeight = 0.012f;
    
    // Create a small rectangular box for the car (6 faces, 2 triangles each)
    
    // Bottom face
    vertices.insert(vertices.end(), {
        nx - carWidth, nz, ny - carDepth,  0.0f, 0.0f,
        nx + carWidth, nz, ny - carDepth,  1.0f, 0.0f,
        nx + carWidth, nz, ny + carDepth,  1.0f, 1.0f,
        nx - carWidth, nz, ny - carDepth,  0.0f, 0.0f,
        nx + carWidth, nz, ny + carDepth,  1.0f, 1.0f,
        nx - carWidth, nz, ny + carDepth,  0.0f, 1.0f
    });
    
    // Top face
    vertices.insert(vertices.end(), {
        nx - carWidth, nz + carHeight, ny + carDepth,  0.0f, 0.0f,
        nx + carWidth, nz + carHeight, ny + carDepth,  1.0f, 0.0f,
        nx + carWidth, nz + carHeight, ny - carDepth,  1.0f, 1.0f,
        nx - carWidth, nz + carHeight, ny + carDepth,  0.0f, 0.0f,
        nx + carWidth, nz + carHeight, ny - carDepth,  1.0f, 1.0f,
        nx - carWidth, nz + carHeight, ny - carDepth,  0.0f, 1.0f
    });
    
    // Front face
    vertices.insert(vertices.end(), {
        nx - carWidth, nz, ny + carDepth,  0.0f, 0.0f,
        nx + carWidth, nz, ny + carDepth,  1.0f, 0.0f,
        nx + carWidth, nz + carHeight, ny + carDepth,  1.0f, 1.0f,
        nx - carWidth, nz, ny + carDepth,  0.0f, 0.0f,
        nx + carWidth, nz + carHeight, ny + carDepth,  1.0f, 1.0f,
        nx - carWidth, nz + carHeight, ny + carDepth,  0.0f, 1.0f
    });
    
    // Back face
    vertices.insert(vertices.end(), {
        nx + carWidth, nz, ny - carDepth,  0.0f, 0.0f,
        nx - carWidth, nz, ny - carDepth,  1.0f, 0.0f,
        nx - carWidth, nz + carHeight, ny - carDepth,  1.0f, 1.0f,
        nx + carWidth, nz, ny - carDepth,  0.0f, 0.0f,
        nx - carWidth, nz + carHeight, ny - carDepth,  1.0f, 1.0f,
        nx + carWidth, nz + carHeight, ny - carDepth,  0.0f, 1.0f
    });
    
    // Left face
    vertices.insert(vertices.end(), {
        nx - carWidth, nz, ny - carDepth,  0.0f, 0.0f,
        nx - carWidth, nz, ny + carDepth,  1.0f, 0.0f,
        nx - carWidth, nz + carHeight, ny + carDepth,  1.0f, 1.0f,
        nx - carWidth, nz, ny - carDepth,  0.0f, 0.0f,
        nx - carWidth, nz + carHeight, ny + carDepth,  1.0f, 1.0f,
        nx - carWidth, nz + carHeight, ny - carDepth,  0.0f, 1.0f
    });
    
    // Right face
    vertices.insert(vertices.end(), {
        nx + carWidth, nz, ny + carDepth,  0.0f, 0.0f,
        nx + carWidth, nz, ny - carDepth,  1.0f, 0.0f,
        nx + carWidth, nz + carHeight, ny - carDepth,  1.0f, 1.0f,
        nx + carWidth, nz, ny + carDepth,  0.0f, 0.0f,
        nx + carWidth, nz + carHeight, ny - carDepth,  1.0f, 1.0f,
        nx + carWidth, nz + carHeight, ny + carDepth,  0.0f, 1.0f
    });
    
    return vertices;
}

std::vector<float> carTo2DVertices(const Car& car, int screenWidth, int screenHeight) {
    // Normalize car position to OpenGL coordinates
    float nx = (car.x / (float)screenWidth) * 2.0f - 1.0f;
    float ny = 1.0f - (car.y / (float)screenHeight) * 2.0f;
    
    // Return simple point vertices (x, y, z, u, v)
    return {
        nx, ny, 0.0f, 0.0f, 0.0f
    };
}
