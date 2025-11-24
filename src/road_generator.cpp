#include "road_generator.h"
#include <cmath>
#include <iostream>

RoadGenerator::RoadGenerator(int width, int height) 
    : screenWidth(width), screenHeight(height) {
    // Initialize random number generator with a seed
    std::random_device rd;
    rng.seed(rd());
}

std::vector<Road> RoadGenerator::generateRoads(const CityConfig& config) {
    std::cout << "\n🛣️  Generating roads (" << config.getRoadPatternString() << " pattern)...\n" << std::flush;
    
    switch(config.roadPattern) {
        case RoadPattern::GRID:
            return generateGridRoads(config);
        case RoadPattern::RADIAL:
            return generateRadialRoads(config);
        case RoadPattern::RANDOM:
            return generateRandomRoads(config);
        default:
            return generateGridRoads(config);
    }
}

std::vector<Road> RoadGenerator::generateGridRoads(const CityConfig& config) {
    std::vector<Road> roads;
    
    int margin = 50;
    int spacing = (screenWidth - 2 * margin) / config.layoutSize;
    
    std::cout << "   - Creating " << config.layoutSize << "x" << config.layoutSize << " grid\n";
    
    // Generate horizontal roads
    for (int i = 0; i <= config.layoutSize; i++) {
        int y = margin + i * spacing;
        Road road = createRoad(margin, y, screenWidth - margin, y, config.roadWidth);
        roads.push_back(road);
    }
    
    // Generate vertical roads
    for (int i = 0; i <= config.layoutSize; i++) {
        int x = margin + i * spacing;
        Road road = createRoad(x, margin, x, screenHeight - margin, config.roadWidth);
        roads.push_back(road);
    }
    
    std::cout << "   - Generated " << roads.size() << " road segments\n";
    return roads;
}

std::vector<Road> RoadGenerator::generateRadialRoads(const CityConfig& config) {
    std::vector<Road> roads;
    
    // Center of the city
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    
    // Number of radial roads (spokes)
    int numSpokes = config.layoutSize;
    
    // Radius for the roads
    int maxRadius = std::min(screenWidth, screenHeight) / 2 - 50;
    
    std::cout << "   - Creating " << numSpokes << " radial spokes\n";
    
    // Generate radial roads (spokes from center)
    for (int i = 0; i < numSpokes; i++) {
        double angle = (2.0 * M_PI * i) / numSpokes;
        int endX = centerX + static_cast<int>(maxRadius * cos(angle));
        int endY = centerY + static_cast<int>(maxRadius * sin(angle));
        
        Road road = createRoad(centerX, centerY, endX, endY, config.roadWidth);
        roads.push_back(road);
    }
    
    // Generate circular roads (rings)
    int numRings = config.layoutSize / 2;
    std::cout << "   - Creating " << numRings << " circular rings\n";
    
    for (int ring = 1; ring <= numRings; ring++) {
        int radius = (maxRadius * ring) / numRings;
        
        // Create circle using midpoint circle algorithm
        std::vector<Point> circlePoints = midpointCircle(centerX, centerY, radius);
        
        // Connect circle points to form a road
        // Sample every few points to create line segments
        for (size_t i = 0; i < circlePoints.size(); i += 8) {
            size_t nextIdx = (i + 8) % circlePoints.size();
            Road road = createRoad(
                circlePoints[i].x, circlePoints[i].y,
                circlePoints[nextIdx].x, circlePoints[nextIdx].y,
                config.roadWidth
            );
            roads.push_back(road);
        }
    }
    
    std::cout << "   - Generated " << roads.size() << " road segments\n";
    return roads;
}

std::vector<Road> RoadGenerator::generateRandomRoads(const CityConfig& config) {
    std::vector<Road> roads;
    
    // Number of random roads based on layout size
    int numRoads = config.layoutSize * 3;
    
    std::cout << "   - Creating " << numRoads << " random roads\n";
    
    // Generate random connection points
    std::vector<Point> nodes;
    for (int i = 0; i < config.layoutSize * 2; i++) {
        nodes.push_back(randomPoint());
    }
    
    // Add screen edge points for connectivity
    nodes.push_back(Point(100, 100));
    nodes.push_back(Point(screenWidth - 100, 100));
    nodes.push_back(Point(100, screenHeight - 100));
    nodes.push_back(Point(screenWidth - 100, screenHeight - 100));
    
    // Connect random nodes
    std::uniform_int_distribution<int> nodeDist(0, nodes.size() - 1);
    
    for (int i = 0; i < numRoads; i++) {
        int idx1 = nodeDist(rng);
        int idx2 = nodeDist(rng);
        
        if (idx1 != idx2) {
            Road road = createRoad(
                nodes[idx1].x, nodes[idx1].y,
                nodes[idx2].x, nodes[idx2].y,
                config.roadWidth
            );
            roads.push_back(road);
        }
    }
    
    std::cout << "   - Generated " << roads.size() << " road segments\n";
    return roads;
}

Road RoadGenerator::createRoad(int x0, int y0, int x1, int y1, int width) {
    // Use Bresenham's Line Algorithm to generate pixel-perfect road
    std::vector<Point> points = bresenhamLine(x0, y0, x1, y1);
    return Road(points, width);
}

Point RoadGenerator::randomPoint(int margin) {
    std::uniform_int_distribution<int> xDist(margin, screenWidth - margin);
    std::uniform_int_distribution<int> yDist(margin, screenHeight - margin);
    
    return Point(xDist(rng), yDist(rng));
}
