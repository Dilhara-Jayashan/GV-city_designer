/**
 * @file road_mesh.h
 * @brief Road 3D Mesh Generation
 * 
 * Generates 3D road meshes from line segments with proper UV coordinates for texturing.
 * Supports both 2D and 3D view modes with appropriate coordinate systems.
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef ROAD_MESH_H
#define ROAD_MESH_H

#include <vector>
#include "generation/road_generator.h" // For Road struct
#include "utils/algorithms.h" // For Point struct

/**
 * @brief Generate 3D mesh for a road from points
 * 
 * Creates a textured 3D mesh representing a road surface from a series of points.
 * The road is rendered as a strip of connected quads (2 triangles each).
 * 
 * @param road Road structure containing points defining the road path
 * @param screenWidth Width of the viewport in pixels
 * @param screenHeight Height of the viewport in pixels
 * @param is3D If true, uses 3D coordinate system (Y is up); if false, uses 2D system (Z is up)
 * @return std::vector<float> Vertex data with positions and UV coordinates (x, y, z, u, v)
 * 
 * Coordinate systems:
 * - 3D mode: X=left/right, Y=height (roads on ground plane), Z=depth
 * - 2D mode: X=left/right, Y=depth, Z=height
 * 
 * Each vertex has 5 floats: (x, y, z, u, v)
 * Each road segment produces 6 vertices (2 triangles)
 */
std::vector<float> roadTo3DMesh(const Road& road, 
                                 int screenWidth, 
                                 int screenHeight, 
                                 bool is3D);

#endif // ROAD_MESH_H
