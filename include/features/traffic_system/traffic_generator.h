/**
 * @file traffic_generator.h
 * @brief Feature 3: Traffic System
 * 
 * Manages vehicle generation, movement, and collision avoidance.
 * Vehicles follow roads and avoid parks, fountains, and other cars.
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef TRAFFIC_GENERATOR_H
#define TRAFFIC_GENERATOR_H

#include <vector>
#include <random>
#include <glm/glm.hpp>
#include "generation/road_generator.h"

// Single car entity
struct Car {
    float x, y;          // Current position
    float vx, vy;        // Velocity components
    float speed;         // Speed magnitude
    int roadIndex;       // Which road segment it's on
    float roadProgress;  // Progress along the road (0-1)
    glm::vec3 color;     // Car color
};

// Collection of all traffic data
struct TrafficData {
    std::vector<Car> cars;
};

class TrafficGenerator {
private:
    TrafficData trafficData;
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist01;
    
    // Collision data
    std::vector<std::vector<Point>> parkAreas;
    std::vector<Point> fountainArea;
    
    // Screen dimensions for boundary checking
    int screenWidth;
    int screenHeight;
    
    // Helper to get random color for cars
    glm::vec3 getRandomCarColor();
    
    // Helper to check if a position is inside a circular area
    bool isInsideCircle(float x, float y, const std::vector<Point>& circlePoints);
    
public:
    TrafficGenerator();
    
    // Generate cars along roads (with collision avoidance)
    void generateTraffic(const std::vector<Road>& roads, int numCars,
                        const std::vector<std::vector<Point>>& parks,
                        const std::vector<Point>& fountain,
                        int screenWidth, int screenHeight);
    
    // Update car positions based on time (with collision avoidance)
    void updateTraffic(float deltaTime, const std::vector<Road>& roads);
    
    // Get traffic data
    const TrafficData& getTrafficData() const { return trafficData; }
    
    // Check if traffic exists
    bool hasTraffic() const { return !trafficData.cars.empty(); }
    
    // Clear traffic
    void clear() { trafficData.cars.clear(); }
};

#endif
