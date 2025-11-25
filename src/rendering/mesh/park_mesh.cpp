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
    
    float baseHeight = 0.006f;    // Base park height (above roads)
    float hillHeight = 0.04f;     // Height of the raised hill in center
    int segments = 32;            // Number of segments for circular shape
    int rings = 4;                // Number of concentric rings for terraced effect
    
    if (!is3D) {
        // 2D MODE: Simple flat circle for visibility
        for (int i = 0; i < segments; i++) {
            float angle1 = (i * 2.0f * M_PI) / segments;
            float angle2 = ((i + 1) * 2.0f * M_PI) / segments;
            
            float x1 = centerX + radius * std::cos(angle1);
            float z1 = centerZ + radius * std::sin(angle1);
            float x2 = centerX + radius * std::cos(angle2);
            float z2 = centerZ + radius * std::sin(angle2);
            
            float u1 = 0.5f + 0.5f * std::cos(angle1);
            float v1 = 0.5f + 0.5f * std::sin(angle1);
            float u2 = 0.5f + 0.5f * std::cos(angle2);
            float v2 = 0.5f + 0.5f * std::sin(angle2);
            
            vertices.insert(vertices.end(), {
                centerX, centerZ, baseHeight,  0.5f, 0.5f,
                x1, z1, baseHeight,  u1, v1,
                x2, z2, baseHeight,  u2, v2
            });
        }
        return vertices;
    }
    
    // 3D MODE: Create a terraced park with gentle hill in the center
    
    // Texture tiling factor - higher = more texture repeats (adjust for your grass texture size)
    float textureTiling = 3.0f;  // Tile the texture 3 times across the radius
    
    // Create concentric rings from outside to center (creating a gentle hill effect)
    for (int ring = rings; ring >= 0; ring--) {
        float ringRadiusOuter = radius * (float)(ring + 1) / (rings + 1);
        float ringRadiusInner = (ring > 0) ? radius * (float)ring / (rings + 1) : 0.0f;
        
        // Calculate height for this ring (higher towards center)
        float heightFactor = 1.0f - (float)ring / (rings + 1);  // 0 at edge, 1 at center
        float ringHeight = baseHeight + hillHeight * heightFactor * heightFactor;  // Squared for smoother curve
        
        if (ring == 0) {
            // Center cap - flat top of the hill
            for (int i = 0; i < segments; i++) {
                float angle1 = (i * 2.0f * M_PI) / segments;
                float angle2 = ((i + 1) * 2.0f * M_PI) / segments;
                
                float x1 = centerX + ringRadiusOuter * std::cos(angle1);
                float z1 = centerZ + ringRadiusOuter * std::sin(angle1);
                float x2 = centerX + ringRadiusOuter * std::cos(angle2);
                float z2 = centerZ + ringRadiusOuter * std::sin(angle2);
                
                // UV mapping - use actual world space for proper tiling
                // Map X,Z coordinates to UV space with tiling
                float u_center = (centerX + 1.0f) * textureTiling;
                float v_center = (centerZ + 1.0f) * textureTiling;
                float u1 = (x1 + 1.0f) * textureTiling;
                float v1 = (z1 + 1.0f) * textureTiling;
                float u2 = (x2 + 1.0f) * textureTiling;
                float v2 = (z2 + 1.0f) * textureTiling;
                
                vertices.insert(vertices.end(), {
                    centerX, ringHeight, centerZ,  u_center, v_center,
                    x1, ringHeight, z1,  u1, v1,
                    x2, ringHeight, z2,  u2, v2
                });
            }
        } else {
            // Ring segments - create sloped surface between rings
            float heightInner = baseHeight + hillHeight * (1.0f - (float)(ring - 1) / (rings + 1)) * 
                                                          (1.0f - (float)(ring - 1) / (rings + 1));
            float heightOuter = baseHeight + hillHeight * (1.0f - (float)ring / (rings + 1)) * 
                                                          (1.0f - (float)ring / (rings + 1));
            
            for (int i = 0; i < segments; i++) {
                float angle1 = (i * 2.0f * M_PI) / segments;
                float angle2 = ((i + 1) * 2.0f * M_PI) / segments;
                
                // Outer ring points
                float xOut1 = centerX + ringRadiusOuter * std::cos(angle1);
                float zOut1 = centerZ + ringRadiusOuter * std::sin(angle1);
                float xOut2 = centerX + ringRadiusOuter * std::cos(angle2);
                float zOut2 = centerZ + ringRadiusOuter * std::sin(angle2);
                
                // Inner ring points
                float xIn1 = centerX + ringRadiusInner * std::cos(angle1);
                float zIn1 = centerZ + ringRadiusInner * std::sin(angle1);
                float xIn2 = centerX + ringRadiusInner * std::cos(angle2);
                float zIn2 = centerZ + ringRadiusInner * std::sin(angle2);
                
                // UV coordinates based on world position for proper tiling
                float uOut1 = (xOut1 + 1.0f) * textureTiling;
                float vOut1 = (zOut1 + 1.0f) * textureTiling;
                float uOut2 = (xOut2 + 1.0f) * textureTiling;
                float vOut2 = (zOut2 + 1.0f) * textureTiling;
                float uIn1 = (xIn1 + 1.0f) * textureTiling;
                float vIn1 = (zIn1 + 1.0f) * textureTiling;
                float uIn2 = (xIn2 + 1.0f) * textureTiling;
                float vIn2 = (zIn2 + 1.0f) * textureTiling;
                
                // Create quad with two triangles (sloped surface)
                // Triangle 1
                vertices.insert(vertices.end(), {
                    xOut1, heightOuter, zOut1,  uOut1, vOut1,
                    xOut2, heightOuter, zOut2,  uOut2, vOut2,
                    xIn1, heightInner, zIn1,    uIn1, vIn1
                });
                
                // Triangle 2
                vertices.insert(vertices.end(), {
                    xOut2, heightOuter, zOut2,  uOut2, vOut2,
                    xIn2, heightInner, zIn2,    uIn2, vIn2,
                    xIn1, heightInner, zIn1,    uIn1, vIn1
                });
            }
        }
    }
    
    // Add outer edge wall/border (gives the park some depth)
    float wallHeight = 0.015f;
    for (int i = 0; i < segments; i++) {
        float angle1 = (i * 2.0f * M_PI) / segments;
        float angle2 = ((i + 1) * 2.0f * M_PI) / segments;
        
        float x1 = centerX + radius * std::cos(angle1);
        float z1 = centerZ + radius * std::sin(angle1);
        float x2 = centerX + radius * std::cos(angle2);
        float z2 = centerZ + radius * std::sin(angle2);
        
        // UV coordinates for wall - wrap around circumference
        float u1 = (float)i / segments * textureTiling;
        float u2 = (float)(i + 1) / segments * textureTiling;
        
        // Outer wall (small vertical edge)
        vertices.insert(vertices.end(), {
            x1, baseHeight, z1,  u1, 0.0f,
            x2, baseHeight, z2,  u2, 0.0f,
            x1, baseHeight + wallHeight, z1,  u1, 1.0f
        });
        vertices.insert(vertices.end(), {
            x2, baseHeight, z2,  u2, 0.0f,
            x2, baseHeight + wallHeight, z2,  u2, 1.0f,
            x1, baseHeight + wallHeight, z1,  u1, 1.0f
        });
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
    
    float baseHeight = 0.008f;     // Base pool height (above parks)
    float poolDepth = 0.02f;       // Height of the pool walls
    float pedestalHeight = 0.1f;   // Height of center pedestal
    float pedestalRadius = radius * 0.15f;  // Pedestal is 15% of fountain radius
    float basinHeight = 0.03f;     // Height of top basin
    float basinRadius = radius * 0.4f;  // Basin is 40% of fountain radius
    
    int segments = 24;  // Number of segments for circular shapes
    
    if (!is3D) {
        // 2D MODE: Simple flat circle for visibility
        for (int i = 0; i < segments; i++) {
            float angle1 = (i * 2.0f * M_PI) / segments;
            float angle2 = ((i + 1) * 2.0f * M_PI) / segments;
            
            float x1 = centerX + radius * std::cos(angle1);
            float z1 = centerZ + radius * std::sin(angle1);
            float x2 = centerX + radius * std::cos(angle2);
            float z2 = centerZ + radius * std::sin(angle2);
            
            float u_center = 0.5f, v_center = 0.5f;
            float u1 = 0.5f + 0.5f * std::cos(angle1);
            float v1 = 0.5f + 0.5f * std::sin(angle1);
            float u2 = 0.5f + 0.5f * std::cos(angle2);
            float v2 = 0.5f + 0.5f * std::sin(angle2);
            
            vertices.insert(vertices.end(), {
                centerX, centerZ, baseHeight,  u_center, v_center,
                x1, z1, baseHeight,  u1, v1,
                x2, z2, baseHeight,  u2, v2
            });
        }
        return vertices;
    }
    
    // 3D MODE: Create a detailed fountain structure
    
    // === 1. BASE POOL (flat circular base) ===
    for (int i = 0; i < segments; i++) {
        float angle1 = (i * 2.0f * M_PI) / segments;
        float angle2 = ((i + 1) * 2.0f * M_PI) / segments;
        
        float x1 = centerX + radius * std::cos(angle1);
        float z1 = centerZ + radius * std::sin(angle1);
        float x2 = centerX + radius * std::cos(angle2);
        float z2 = centerZ + radius * std::sin(angle2);
        
        // Texture coordinates - radial pattern
        float u1 = 0.5f + 0.5f * std::cos(angle1);
        float v1 = 0.5f + 0.5f * std::sin(angle1);
        float u2 = 0.5f + 0.5f * std::cos(angle2);
        float v2 = 0.5f + 0.5f * std::sin(angle2);
        
        // Base circle
        vertices.insert(vertices.end(), {
            centerX, baseHeight, centerZ,  0.5f, 0.5f,
            x1, baseHeight, z1,  u1, v1,
            x2, baseHeight, z2,  u2, v2
        });
    }
    
    // === 2. POOL WALLS (cylinder around the base) ===
    for (int i = 0; i < segments; i++) {
        float angle1 = (i * 2.0f * M_PI) / segments;
        float angle2 = ((i + 1) * 2.0f * M_PI) / segments;
        
        float x1 = centerX + radius * std::cos(angle1);
        float z1 = centerZ + radius * std::sin(angle1);
        float x2 = centerX + radius * std::cos(angle2);
        float z2 = centerZ + radius * std::sin(angle2);
        
        float u1 = (float)i / segments;
        float u2 = (float)(i + 1) / segments;
        
        // Outer wall - two triangles per segment
        vertices.insert(vertices.end(), {
            x1, baseHeight, z1,  u1, 0.0f,
            x2, baseHeight, z2,  u2, 0.0f,
            x1, baseHeight + poolDepth, z1,  u1, 1.0f
        });
        vertices.insert(vertices.end(), {
            x2, baseHeight, z2,  u2, 0.0f,
            x2, baseHeight + poolDepth, z2,  u2, 1.0f,
            x1, baseHeight + poolDepth, z1,  u1, 1.0f
        });
    }
    
    // === 3. CENTER PEDESTAL (cylinder rising from base) ===
    float pedestalBase = baseHeight + poolDepth * 0.5f;  // Starts from middle of pool
    for (int i = 0; i < segments; i++) {
        float angle1 = (i * 2.0f * M_PI) / segments;
        float angle2 = ((i + 1) * 2.0f * M_PI) / segments;
        
        float x1 = centerX + pedestalRadius * std::cos(angle1);
        float z1 = centerZ + pedestalRadius * std::sin(angle1);
        float x2 = centerX + pedestalRadius * std::cos(angle2);
        float z2 = centerZ + pedestalRadius * std::sin(angle2);
        
        float u1 = (float)i / segments;
        float u2 = (float)(i + 1) / segments;
        
        // Pedestal side - two triangles per segment
        vertices.insert(vertices.end(), {
            x1, pedestalBase, z1,  u1, 0.0f,
            x2, pedestalBase, z2,  u2, 0.0f,
            x1, pedestalBase + pedestalHeight, z1,  u1, 1.0f
        });
        vertices.insert(vertices.end(), {
            x2, pedestalBase, z2,  u2, 0.0f,
            x2, pedestalBase + pedestalHeight, z2,  u2, 1.0f,
            x1, pedestalBase + pedestalHeight, z1,  u1, 1.0f
        });
    }
    
    // === 4. TOP BASIN (bowl at top of pedestal) ===
    float basinBase = pedestalBase + pedestalHeight;
    for (int i = 0; i < segments; i++) {
        float angle1 = (i * 2.0f * M_PI) / segments;
        float angle2 = ((i + 1) * 2.0f * M_PI) / segments;
        
        float x1 = centerX + basinRadius * std::cos(angle1);
        float z1 = centerZ + basinRadius * std::sin(angle1);
        float x2 = centerX + basinRadius * std::cos(angle2);
        float z2 = centerZ + basinRadius * std::sin(angle2);
        
        float u1 = 0.5f + 0.4f * std::cos(angle1);
        float v1 = 0.5f + 0.4f * std::sin(angle1);
        float u2 = 0.5f + 0.4f * std::cos(angle2);
        float v2 = 0.5f + 0.4f * std::sin(angle2);
        
        // Basin top (flat circle)
        vertices.insert(vertices.end(), {
            centerX, basinBase + basinHeight, centerZ,  0.5f, 0.5f,
            x1, basinBase + basinHeight, z1,  u1, v1,
            x2, basinBase + basinHeight, z2,  u2, v2
        });
        
        // Basin sides
        float u_seg1 = (float)i / segments;
        float u_seg2 = (float)(i + 1) / segments;
        vertices.insert(vertices.end(), {
            x1, basinBase, z1,  u_seg1, 0.0f,
            x2, basinBase, z2,  u_seg2, 0.0f,
            x1, basinBase + basinHeight, z1,  u_seg1, 1.0f
        });
        vertices.insert(vertices.end(), {
            x2, basinBase, z2,  u_seg2, 0.0f,
            x2, basinBase + basinHeight, z2,  u_seg2, 1.0f,
            x1, basinBase + basinHeight, z1,  u_seg1, 1.0f
        });
    }
    
    return vertices;
}

std::vector<float> fountainLightsTo3DMesh(const std::vector<Point>& fountainPoints,
                                          int screenWidth,
                                          int screenHeight) {
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
    
    float baseHeight = 0.008f;
    float poolDepth = 0.02f;
    float pedestalHeight = 0.1f;
    float basinHeight = 0.03f;
    
    float lightRadius = 0.008f;  // Size of each light bulb
    int lightSegments = 12;      // Sphere resolution
    int lightRings = 8;
    
    // Create light positions array
    struct LightPosition {
        float x, y, z;
    };
    std::vector<LightPosition> lightPositions;
    
    // Add lights around the base pool rim (12 lights)
    int numPoolLights = 12;
    for (int i = 0; i < numPoolLights; i++) {
        float angle = (i * 2.0f * M_PI) / numPoolLights;
        float x = centerX + radius * 0.9f * std::cos(angle);
        float z = centerZ + radius * 0.9f * std::sin(angle);
        float y = baseHeight + poolDepth * 0.8f;  // Near top of pool wall
        lightPositions.push_back({x, y, z});
    }
    
    // Add lights around the top basin (8 lights)
    int numBasinLights = 8;
    float pedestalRadius = radius * 0.15f;
    float basinRadius = radius * 0.4f;
    float pedestalBase = baseHeight + poolDepth * 0.5f;
    float basinBase = pedestalBase + pedestalHeight;
    for (int i = 0; i < numBasinLights; i++) {
        float angle = (i * 2.0f * M_PI) / numBasinLights;
        float x = centerX + basinRadius * 0.9f * std::cos(angle);
        float z = centerZ + basinRadius * 0.9f * std::sin(angle);
        float y = basinBase + basinHeight * 0.9f;  // Near top of basin
        lightPositions.push_back({x, y, z});
    }
    
    // Generate sphere mesh for each light position
    for (const auto& lightPos : lightPositions) {
        // Create a simple sphere using latitude/longitude approach
        for (int lat = 0; lat < lightRings; lat++) {
            float theta1 = ((float)lat / lightRings) * M_PI;
            float theta2 = ((float)(lat + 1) / lightRings) * M_PI;
            
            for (int lon = 0; lon < lightSegments; lon++) {
                float phi1 = ((float)lon / lightSegments) * 2.0f * M_PI;
                float phi2 = ((float)(lon + 1) / lightSegments) * 2.0f * M_PI;
                
                // Four corners of the quad
                float x1 = lightPos.x + lightRadius * std::sin(theta1) * std::cos(phi1);
                float y1 = lightPos.y + lightRadius * std::cos(theta1);
                float z1 = lightPos.z + lightRadius * std::sin(theta1) * std::sin(phi1);
                
                float x2 = lightPos.x + lightRadius * std::sin(theta1) * std::cos(phi2);
                float y2 = lightPos.y + lightRadius * std::cos(theta1);
                float z2 = lightPos.z + lightRadius * std::sin(theta1) * std::sin(phi2);
                
                float x3 = lightPos.x + lightRadius * std::sin(theta2) * std::cos(phi2);
                float y3 = lightPos.y + lightRadius * std::cos(theta2);
                float z3 = lightPos.z + lightRadius * std::sin(theta2) * std::sin(phi2);
                
                float x4 = lightPos.x + lightRadius * std::sin(theta2) * std::cos(phi1);
                float y4 = lightPos.y + lightRadius * std::cos(theta2);
                float z4 = lightPos.z + lightRadius * std::sin(theta2) * std::sin(phi1);
                
                // Simple UV mapping (can be 0,0 since we'll use solid color)
                // Triangle 1
                vertices.insert(vertices.end(), {
                    x1, y1, z1,  0.0f, 0.0f,
                    x2, y2, z2,  1.0f, 0.0f,
                    x3, y3, z3,  1.0f, 1.0f
                });
                
                // Triangle 2
                vertices.insert(vertices.end(), {
                    x1, y1, z1,  0.0f, 0.0f,
                    x3, y3, z3,  1.0f, 1.0f,
                    x4, y4, z4,  0.0f, 1.0f
                });
            }
        }
    }
    
    return vertices;
}
