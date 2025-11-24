#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <cmath>

// Structure to hold a 2D point
struct Point {
    int x;
    int y;
    
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

// Bresenham's Line Algorithm
// Generates pixel-perfect line points between two coordinates
// Used for: roads, grid lines, building outlines
std::vector<Point> bresenhamLine(int x0, int y0, int x1, int y1);

// Midpoint Circle Algorithm
// Generates points for a circle using the midpoint algorithm
// Used for: parks, fountains, roundabouts
std::vector<Point> midpointCircle(int centerX, int centerY, int radius);

#endif // ALGORITHMS_H
