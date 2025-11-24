/**
 * @file algorithms.h
 * @brief Core 2D Graphics Algorithms
 * 
 * Implements classic computer graphics algorithms for 2D shape generation.
 * These algorithms are used throughout the city generator for creating
 * precise, pixel-perfect geometric shapes.
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <cmath>

/**
 * @struct Point
 * @brief Represents a 2D integer coordinate
 * 
 * Used for pixel-accurate positioning in the 2D generation phase.
 * All city elements start as collections of Points before being
 * converted to OpenGL vertices.
 */
struct Point {
    int x;  ///< X coordinate (horizontal position)
    int y;  ///< Y coordinate (vertical position)
    
    /**
     * @brief Construct a new Point
     * @param x X coordinate (default: 0)
     * @param y Y coordinate (default: 0)
     */
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

/**
 * @brief Bresenham's Line Algorithm
 * 
 * Generates pixel-perfect line points between two coordinates using
 * only integer arithmetic for efficiency. This is the classic algorithm
 * used in computer graphics for rasterizing lines.
 * 
 * @param x0 Starting X coordinate
 * @param y0 Starting Y coordinate
 * @param x1 Ending X coordinate
 * @param y1 Ending Y coordinate
 * @return std::vector<Point> Ordered list of points forming the line
 * 
 * **Used for**:
 * - Road segments (Grid, Radial, Random patterns)
 * - Grid lines in layout
 * - Building outlines
 * 
 * **Time Complexity**: O(max(dx, dy)) where dx, dy are coordinate differences
 * **Space Complexity**: O(max(dx, dy)) for result vector
 * 
 * @note This algorithm guarantees no gaps between pixels
 * @see https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 */
std::vector<Point> bresenhamLine(int x0, int y0, int x1, int y1);

/**
 * @brief Midpoint Circle Algorithm
 * 
 * Generates points for a circle using the midpoint algorithm (also known
 * as Bresenham's circle algorithm). Uses 8-way symmetry to efficiently
 * compute all circle points from one octant.
 * 
 * @param centerX X coordinate of circle center
 * @param centerY Y coordinate of circle center
 * @param radius Radius of circle in pixels
 * @return std::vector<Point> Ordered list of points forming the circle perimeter
 * 
 * **Used for**:
 * - Park boundaries (green circles in 2D)
 * - Fountain boundaries (cyan circles in 2D)
 * - Circular road patterns (radial layout)
 * 
 * **Time Complexity**: O(radius)
 * **Space Complexity**: O(radius) for result vector
 * 
 * @note Points are returned in clockwise order starting from rightmost point
 * @note Uses integer-only arithmetic for speed and precision
 * @see https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 */
std::vector<Point> midpointCircle(int centerX, int centerY, int radius);

#endif // ALGORITHMS_H
