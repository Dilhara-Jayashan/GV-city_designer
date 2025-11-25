/**
 * @file park_mesh.h
 * @brief Park and Fountain 3D Mesh Generation
 * 
 * Generates 3D circular filled meshes for parks and fountains with proper UV coordinates.
 * Supports both 2D and 3D view modes with appropriate coordinate systems.
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef PARK_MESH_H
#define PARK_MESH_H

#include <vector>
#include "utils/algorithms.h" // For Point struct

/**
 * @brief Generate 3D mesh for a park (filled circle)
 * 
 * Creates a circular filled mesh using a triangle fan approach.
 * The mesh represents a grass-covered park area.
 * 
 * @param parkPoints Points defining the circular boundary of the park
 * @param screenWidth Width of the viewport in pixels
 * @param screenHeight Height of the viewport in pixels
 * @param is3D If true, uses 3D coordinate system (Y is up); if false, uses 2D system (Z is up)
 * @return std::vector<float> Vertex data with positions and UV coordinates (x, y, z, u, v)
 * 
 * Coordinate systems:
 * - 3D mode: X=left/right, Y=height (park on ground plane), Z=depth
 * - 2D mode: X=left/right, Y=depth, Z=height
 * 
 * Each vertex has 5 floats: (x, y, z, u, v)
 * Creates 32 triangles forming a filled circle
 */
std::vector<float> parkTo3DMesh(const std::vector<Point>& parkPoints, 
                                 int screenWidth, 
                                 int screenHeight, 
                                 bool is3D);

/**
 * @brief Generate 3D mesh for a fountain (filled circle)
 * 
 * Similar to parkTo3DMesh but slightly raised above the ground plane
 * to make it visually distinct from parks.
 * 
 * @param fountainPoints Points defining the circular boundary of the fountain
 * @param screenWidth Width of the viewport in pixels
 * @param screenHeight Height of the viewport in pixels
 * @param is3D If true, uses 3D coordinate system (Y is up); if false, uses 2D system (Z is up)
 * @return std::vector<float> Vertex data with positions and UV coordinates (x, y, z, u, v)
 * 
 * Height hierarchy (to prevent z-fighting):
 * - Roads: 0.005f
 * - Parks: 0.006f
 * - Fountains: 0.008f
 */
std::vector<float> fountainTo3DMesh(const std::vector<Point>& fountainPoints, 
                                     int screenWidth, 
                                     int screenHeight, 
                                     bool is3D);

/**
 * @brief Generate 3D mesh for fountain light bulbs (small spheres)
 * 
 * Creates small spherical meshes representing light bulbs placed around
 * and on the fountain structure for nighttime illumination effect.
 * 
 * @param fountainPoints Points defining the circular boundary of the fountain
 * @param screenWidth Width of the viewport in pixels
 * @param screenHeight Height of the viewport in pixels
 * @return std::vector<float> Vertex data with positions and UV coordinates (x, y, z, u, v)
 */
std::vector<float> fountainLightsTo3DMesh(const std::vector<Point>& fountainPoints,
                                          int screenWidth,
                                          int screenHeight);

#endif // PARK_MESH_H
