#include "algorithms.h"

// Bresenham's Line Algorithm Implementation
// This algorithm calculates which pixels to draw for a straight line
// between two points using only integer arithmetic for efficiency
std::vector<Point> bresenhamLine(int x0, int y0, int x1, int y1) {
    std::vector<Point> points;
    
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    
    int sx = (x0 < x1) ? 1 : -1;  // Step direction in x
    int sy = (y0 < y1) ? 1 : -1;  // Step direction in y
    
    int err = dx - dy;  // Error term
    
    int x = x0;
    int y = y0;
    
    while (true) {
        // Add current point to the line
        points.push_back(Point(x, y));
        
        // Check if we've reached the end point
        if (x == x1 && y == y1) {
            break;
        }
        
        // Calculate error for next step
        int e2 = 2 * err;
        
        // Step in x direction if needed
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        
        // Step in y direction if needed
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
    
    return points;
}

// Midpoint Circle Algorithm Implementation
// This algorithm uses 8-way symmetry to efficiently draw circles
// by calculating points in one octant and mirroring them
std::vector<Point> midpointCircle(int centerX, int centerY, int radius) {
    std::vector<Point> points;
    
    int x = 0;
    int y = radius;
    int d = 1 - radius;  // Decision parameter
    
    // Function to add all 8 symmetric points
    auto addSymmetricPoints = [&](int x, int y) {
        points.push_back(Point(centerX + x, centerY + y));  // Octant 1
        points.push_back(Point(centerX - x, centerY + y));  // Octant 2
        points.push_back(Point(centerX + x, centerY - y));  // Octant 3
        points.push_back(Point(centerX - x, centerY - y));  // Octant 4
        points.push_back(Point(centerX + y, centerY + x));  // Octant 5
        points.push_back(Point(centerX - y, centerY + x));  // Octant 6
        points.push_back(Point(centerX + y, centerY - x));  // Octant 7
        points.push_back(Point(centerX - y, centerY - x));  // Octant 8
    };
    
    // Initial points
    addSymmetricPoints(x, y);
    
    // Calculate points for one octant, mirror for others
    while (x < y) {
        x++;
        
        if (d < 0) {
            // Midpoint is inside the circle, move right
            d += 2 * x + 1;
        } else {
            // Midpoint is outside the circle, move right and down
            y--;
            d += 2 * (x - y) + 1;
        }
        
        addSymmetricPoints(x, y);
    }
    
    return points;
}
