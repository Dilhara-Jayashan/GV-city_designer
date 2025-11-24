#ifndef ROAD_GENERATOR_H
#define ROAD_GENERATOR_H

#include <vector>
#include <random>
#include "utils/algorithms.h"
#include "core/city_config.h"

// Structure to represent a road segment
struct Road {
    std::vector<Point> points;  // Points along the road (from Bresenham)
    int width;                  // Width of the road in pixels
    
    Road() : width(8) {}
    Road(const std::vector<Point>& pts, int w) : points(pts), width(w) {}
};

// Road Generator Class
// Generates different road patterns using Bresenham's Line Algorithm
class RoadGenerator {
private:
    int screenWidth;
    int screenHeight;
    std::mt19937 rng;  // Random number generator
    
public:
    RoadGenerator(int width, int height);
    
    // Generate roads based on the configuration
    std::vector<Road> generateRoads(const CityConfig& config);
    
    // Generate roads avoiding parks and fountains
    std::vector<Road> generateRoadsAvoidingObstacles(const CityConfig& config, 
                                                       const std::vector<std::vector<Point>>& parks,
                                                       const std::vector<Point>& fountain);
    
private:
    // Generate grid-based road network
    std::vector<Road> generateGridRoads(const CityConfig& config);
    
    // Generate radial road network (roads emanating from center)
    std::vector<Road> generateRadialRoads(const CityConfig& config);
    
    // Generate random road network
    std::vector<Road> generateRandomRoads(const CityConfig& config);
    
    // Helper: Create a road between two points using Bresenham's algorithm
    Road createRoad(int x0, int y0, int x1, int y1, int width);
    
    // Helper: Generate random position within screen bounds
    Point randomPoint(int margin = 50);
};

#endif // ROAD_GENERATOR_H
