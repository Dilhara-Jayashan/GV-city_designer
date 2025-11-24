/**
 * @file building_mesh.h
 * @brief Building 3D Mesh Generation
 * 
 * Generates 3D cube meshes for buildings with proper UV coordinates for texturing.
 * Supports both 2D and 3D view modes with appropriate coordinate systems.
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef BUILDING_MESH_H
#define BUILDING_MESH_H

#include <vector>
#include "generation/city_generator.h" // For Building struct

/**
 * @brief Generate 3D cube vertices for a building
 * 
 * Creates a 3D cube mesh with 36 vertices (6 faces * 2 triangles * 3 vertices each)
 * in format (x, y, z, u, v) where (u, v) are texture coordinates.
 * 
 * @param building Building structure containing position and dimensions
 * @param screenWidth Width of the viewport in pixels
 * @param screenHeight Height of the viewport in pixels
 * @param is3D If true, uses 3D coordinate system (Y is up); if false, uses 2D system (Z is up)
 * @return std::vector<float> Vertex data with positions and UV coordinates
 * 
 * Coordinate systems:
 * - 3D mode: X=left/right, Y=height(UP!), Z=depth
 * - 2D mode: X=left/right, Y=depth, Z=height
 * 
 * Each vertex has 5 floats: (x, y, z, u, v)
 * Total vertices: 36 (6 faces * 6 vertices per face)
 */
std::vector<float> buildingToVertices(const Building& building, 
                                      int screenWidth, 
                                      int screenHeight, 
                                      bool is3D = true);

#endif // BUILDING_MESH_H
