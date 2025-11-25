/**
 * @file road_generator.h
 * @brief Road Network Generation System
 * 
 * This module generates road networks using Bresenham's Line Algorithm.
 * It supports three different road patterns:
 * 
 * 1. GRID Pattern (Manhattan style):
 *    - Horizontal and vertical roads forming a grid
 *    - Evenly spaced based on layout size
 *    - Perfect for organized city layouts
 * 
 * 2. RADIAL Pattern (spoke and ring):
 *    - Roads emanating from center like spokes
 *    - Concentric rings connecting spokes
 *    - Creates a circular city layout
 * 
 * 3. RANDOM Pattern (organic):
 *    - Random connection of nodes
 *    - Creates irregular, natural-looking networks
 *    - Good for varied, less structured cities
 * 
 * All roads use Bresenham's algorithm for perfect line rasterization
 * without floating-point arithmetic.
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef ROAD_GENERATOR_H
#define ROAD_GENERATOR_H

#include <vector>
#include <random>
#include "utils/algorithms.h"
#include "core/city_config.h"

/**
 * @struct Road
 * @brief Represents a single road segment
 * 
 * A road is defined by a sequence of points generated using
 * Bresenham's Line Algorithm. Each road has a width that
 * determines how thick it appears when rendered.
 * 
 * In 2D view, roads are rendered as lines.
 * In 3D view, roads become textured 3D meshes (flat rectangles).
 */
struct Road {
    std::vector<Point> points;  ///< Points along road (Bresenham output)
    int width;                  ///< Road width in pixels (default: 14)
    
    /**
     * @brief Construct empty road with default width
     */
    Road() : width(8) {}
    
    /**
     * @brief Construct road with specific points and width
     * @param pts Points along the road path
     * @param w Road width in pixels
     */
    Road(const std::vector<Point>& pts, int w) : points(pts), width(w) {}
};

/**
 * @class RoadGenerator
 * @brief Road network generator using Bresenham's Line Algorithm
 * 
 * This class is responsible for creating road networks in various patterns.
 * All roads use Bresenham's algorithm for pixel-perfect line rasterization.
 * 
 * Pattern Types:
 * 
 * GRID (Manhattan):
 * - Creates (layoutSize + 1) horizontal roads
 * - Creates (layoutSize + 1) vertical roads
 * - Total roads: 2 * (layoutSize + 1)
 * - Example: layoutSize=10 → 22 roads in 10x10 grid
 * 
 * RADIAL (Spoke and Ring):
 * - Spokes: layoutSize roads from center to edges
 * - Rings: (layoutSize / 2) concentric circles
 * - Creates circular city layout
 * 
 * RANDOM (Organic):
 * - Nodes: layoutSize * 2 random points
 * - Connections: layoutSize * 3 random road segments
 * - Creates irregular, natural-looking network
 * 
 * Bresenham's Algorithm:
 * - Integer-only arithmetic (no floating point)
 * - Perfect pixel coverage (no gaps or overlaps)
 * - O(max(dx, dy)) complexity
 * - Generates points from (x0,y0) to (x1,y1)
 */
class RoadGenerator {
private:
    int screenWidth;    ///< Screen width (800) for boundary checks
    int screenHeight;   ///< Screen height (600) for boundary checks
    std::mt19937 rng;   ///< Mersenne Twister RNG for random patterns
    
public:
    /**
     * @brief Construct a new Road Generator
     * @param width Screen width in pixels
     * @param height Screen height in pixels
     * 
     * Initializes RNG with random seed for non-deterministic generation.
     */
    RoadGenerator(int width, int height);
    
    /**
     * @brief Generate roads based on configuration pattern
     * @param config City configuration (pattern, size, width)
     * @return std::vector<Road> Generated road network
     * 
     * This is the main generation method. It delegates to:
     * - generateGridRoads() for GRID pattern
     * - generateRadialRoads() for RADIAL pattern
     * - generateRandomRoads() for RANDOM pattern
     * 
     * Does NOT avoid obstacles - used for initial generation.
     */
    std::vector<Road> generateRoads(const CityConfig& config);
    
    /**
     * @brief Generate roads with obstacle avoidance (FUTURE FEATURE)
     * @param config City configuration
     * @param parks Existing parks to avoid
     * @param fountain Existing fountain to avoid
     * @return std::vector<Road> Road network avoiding obstacles
     * 
     * This method can be used for adaptive road generation
     * that routes around pre-placed parks and fountains.
     * Currently not used in main generation flow.
     */
    std::vector<Road> generateRoadsAvoidingObstacles(const CityConfig& config, 
                                                       const std::vector<std::vector<Point>>& parks,
                                                       const std::vector<Point>& fountain);
    
private:
    /**
     * @brief Generate grid-based (Manhattan) road network
     * @param config City configuration
     * @return std::vector<Road> Grid road network
     * 
     * Creates a regular grid of roads:
     * - Horizontal roads: top to bottom at regular intervals
     * - Vertical roads: left to right at regular intervals
     * - Spacing: (screenWidth - 2*margin) / layoutSize
     * 
     * Example for layoutSize=10:
     * - 11 horizontal roads (0 to 10)
     * - 11 vertical roads (0 to 10)
     * - Total: 22 roads forming 10x10 grid of blocks
     */
    std::vector<Road> generateGridRoads(const CityConfig& config);
    
    /**
     * @brief Generate radial (spoke and ring) road network
     * @param config City configuration
     * @return std::vector<Road> Radial road network
     * 
     * Creates roads emanating from city center:
     * 1. Spoke roads: From center to edge at regular angles
     *    - Number of spokes: layoutSize
     *    - Angle between spokes: 360° / layoutSize
     * 
     * 2. Ring roads: Concentric circles connecting spokes
     *    - Number of rings: layoutSize / 2
     *    - Ring spacing: increases from center outward
     * 
     * Creates a radial city layout like Paris or Moscow.
     */
    std::vector<Road> generateRadialRoads(const CityConfig& config);
    
    /**
     * @brief Generate random (organic) road network
     * @param config City configuration
     * @return std::vector<Road> Random road network
     * 
     * Creates an irregular road network:
     * 1. Generate (layoutSize * 2) random nodes
     * 2. Create (layoutSize * 3) random connections between nodes
     * 3. Each connection becomes a road segment
     * 
     * Results in natural-looking, non-grid layouts.
     */
    std::vector<Road> generateRandomRoads(const CityConfig& config);
    
    /**
     * @brief Create a road using Bresenham's Line Algorithm
     * @param x0 Starting X coordinate
     * @param y0 Starting Y coordinate
     * @param x1 Ending X coordinate
     * @param y1 Ending Y coordinate
     * @param width Road width in pixels
     * @return Road Road segment with all points
     * 
     * Implementation details:
     * - Uses Bresenham's algorithm from algorithms.cpp
     * - Integer-only arithmetic (fast)
     * - Perfect line with no gaps
     * - Handles all octants (all directions)
     * - Width parameter stored but not used in point generation
     */
    Road createRoad(int x0, int y0, int x1, int y1, int width);
    
    /**
     * @brief Generate random point within screen bounds
     * @param margin Distance from edge (default: 50px)
     * @return Point Random point with margin from edges
     * 
     * Used by random road generation to create node positions.
     * Ensures nodes stay away from screen boundaries.
     */
    Point randomPoint(int margin = 50);
};

#endif // ROAD_GENERATOR_H
