/**
 * @file mesh_utils.h
 * @brief Utility Functions for Mesh Generation
 * 
 * Common utility functions used across different mesh generators.
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef MESH_UTILS_H
#define MESH_UTILS_H

#include <vector>
#include "utils/algorithms.h" // For Point struct

/**
 * @brief Convert 2D points to OpenGL vertices
 * 
 * Converts a series of 2D pixel coordinates to normalized device coordinates (-1 to 1)
 * for use with OpenGL. Filters out points outside the screen boundaries.
 * 
 * @param points Vector of 2D points in pixel coordinates
 * @param screenWidth Width of the viewport in pixels
 * @param screenHeight Height of the viewport in pixels
 * @return std::vector<float> Vertex data in format (x, y, z) where z=0 for 2D elements
 * 
 * Each vertex has 3 floats: (x, y, 0.0)
 * Points outside the screen margins are filtered out
 */
std::vector<float> pointsToVertices(const std::vector<Point>& points, 
                                     int screenWidth, 
                                     int screenHeight);

#endif // MESH_UTILS_H
