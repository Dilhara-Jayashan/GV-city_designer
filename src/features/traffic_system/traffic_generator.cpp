/**
 * @file traffic_generator.cpp
 * @brief Implementation of Traffic System Feature
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include "features/traffic_system/traffic_generator.h"
#include <cmath>
#include <iostream>

TrafficGenerator::TrafficGenerator() : rng(std::random_device{}()), dist01(0.0f, 1.0f), screenWidth(800), screenHeight(600) {}

bool TrafficGenerator::isInsideCircle(float x, float y, const std::vector<Point>& circlePoints) {
    if (circlePoints.empty()) return false;
    
    // Find the center and radius of the circle
    float minX = circlePoints[0].x, maxX = circlePoints[0].x;
    float minY = circlePoints[0].y, maxY = circlePoints[0].y;
    
    for (const Point& p : circlePoints) {
        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
    }
    
    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;
    float radius = (maxX - minX) / 2.0f;
    
    // Check if point is inside circle
    float dx = x - centerX;
    float dy = y - centerY;
    float distSquared = dx * dx + dy * dy;
    float radiusSquared = radius * radius;
    
    return distSquared <= radiusSquared;
}

glm::vec3 TrafficGenerator::getRandomCarColor() {
    // Generate vibrant car colors
    std::vector<glm::vec3> carColors = {
        glm::vec3(1.0f, 0.0f, 0.0f),   // Red
        glm::vec3(0.0f, 0.0f, 1.0f),   // Blue
        glm::vec3(1.0f, 1.0f, 0.0f),   // Yellow
        glm::vec3(0.0f, 1.0f, 0.0f),   // Green
        glm::vec3(1.0f, 0.5f, 0.0f),   // Orange
        glm::vec3(0.8f, 0.8f, 0.8f),   // Silver
        glm::vec3(0.2f, 0.2f, 0.2f),   // Dark gray
        glm::vec3(1.0f, 1.0f, 1.0f),   // White
    };
    
    int index = static_cast<int>(dist01(rng) * carColors.size());
    if (index >= carColors.size()) index = carColors.size() - 1;
    return carColors[index];
}

void TrafficGenerator::generateTraffic(const std::vector<Road>& roads, int numCars,
                                      const std::vector<std::vector<Point>>& parks,
                                      const std::vector<Point>& fountain,
                                      int screenWidth, int screenHeight) {
    trafficData.cars.clear();
    parkAreas = parks;
    fountainArea = fountain;
    
    // Store screen dimensions for boundary checking
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    
    if (roads.empty() || numCars <= 0) {
        return;
    }
    
    std::cout << "\n🚗 Generating " << numCars << " cars on roads...\n";
    
    // Screen boundaries with margin
    const float margin = 50.0f;
    const float minX = margin;
    const float maxX = screenWidth - margin;
    const float minY = margin;
    const float maxY = screenHeight - margin;
    
    int attemptedCars = 0;
    for (int i = 0; i < numCars && attemptedCars < numCars * 3; i++) {
        attemptedCars++;
        Car car;
        
        // Pick a random road
        car.roadIndex = static_cast<int>(dist01(rng) * roads.size());
        if (car.roadIndex >= roads.size()) car.roadIndex = roads.size() - 1;
        
        const Road& road = roads[car.roadIndex];
        
        // Random position along the road
        car.roadProgress = dist01(rng);
        
        if (!road.points.empty()) {
            // Place car on road
            int pointIndex = static_cast<int>(car.roadProgress * (road.points.size() - 1));
            if (pointIndex >= road.points.size()) pointIndex = road.points.size() - 1;
            
            const Point& pt = road.points[pointIndex];
            car.x = pt.x;
            car.y = pt.y;
            
            // Check if car is within screen boundaries
            if (car.x < minX || car.x > maxX || car.y < minY || car.y > maxY) {
                i--;  // Try again with a different position
                continue;
            }
            
            // Check if car position is inside any park or fountain
            bool insideObstacle = false;
            
            // Check parks
            for (const auto& park : parkAreas) {
                if (isInsideCircle(car.x, car.y, park)) {
                    insideObstacle = true;
                    break;
                }
            }
            
            // Check fountain
            if (!insideObstacle && isInsideCircle(car.x, car.y, fountainArea)) {
                insideObstacle = true;
            }
            
            // Skip this car if it's inside an obstacle
            if (insideObstacle) {
                i--;  // Try again with a different position
                continue;
            }
            
            // Calculate velocity direction from road direction
            if (pointIndex < road.points.size() - 1) {
                const Point& nextPt = road.points[pointIndex + 1];
                float dx = nextPt.x - pt.x;
                float dy = nextPt.y - pt.y;
                float length = std::sqrt(dx * dx + dy * dy);
                
                if (length > 0.0f) {
                    car.speed = 20.0f + dist01(rng) * 30.0f;  // Random speed 20-50 pixels/sec
                    car.vx = (dx / length) * car.speed;
                    car.vy = (dy / length) * car.speed;
                } else {
                    car.vx = 0.0f;
                    car.vy = 0.0f;
                    car.speed = 0.0f;
                }
            } else {
                car.vx = 0.0f;
                car.vy = 0.0f;
                car.speed = 0.0f;
            }
        } else {
            car.x = 0.0f;
            car.y = 0.0f;
            car.vx = 0.0f;
            car.vy = 0.0f;
            car.speed = 0.0f;
        }
        
        car.color = getRandomCarColor();
        trafficData.cars.push_back(car);
    }
    
    std::cout << "   ✓ Spawned " << trafficData.cars.size() << " cars\n";
}

void TrafficGenerator::updateTraffic(float deltaTime, const std::vector<Road>& roads) {
    if (roads.empty()) return;
    
    for (Car& car : trafficData.cars) {
        // Calculate new position
        float newX = car.x + car.vx * deltaTime;
        float newY = car.y + car.vy * deltaTime;
        
        // Check if new position would collide with parks or fountain
        bool wouldCollide = false;
        
        for (const auto& park : parkAreas) {
            if (isInsideCircle(newX, newY, park)) {
                wouldCollide = true;
                break;
            }
        }
        
        if (!wouldCollide && isInsideCircle(newX, newY, fountainArea)) {
            wouldCollide = true;
        }
        
        // Only update position if no collision
        if (!wouldCollide) {
            car.x = newX;
            car.y = newY;
        } else {
            // Skip ahead on the road to get past the obstacle
            car.roadProgress += 0.1f;  // Jump forward
        }
        
        car.roadProgress += (car.speed / 500.0f) * deltaTime;  // Normalized progress
        
        // If car reaches end of road, wrap to beginning
        if (car.roadProgress >= 1.0f) {
            car.roadProgress = 0.0f;
            
            // Optionally switch to a random connected road or same road
            if (dist01(rng) < 0.2f) {  // 20% chance to switch roads
                car.roadIndex = static_cast<int>(dist01(rng) * roads.size());
                if (car.roadIndex >= roads.size()) car.roadIndex = roads.size() - 1;
            }
            
            // Reset position to start of road with boundary checking
            const Road& road = roads[car.roadIndex];
            if (!road.points.empty()) {
                // Try to find a valid starting point within boundaries
                // Screen boundaries with margin
                const float margin = 50.0f;
                const float minX = margin;
                const float maxX = static_cast<float>(screenWidth) - margin;
                const float minY = margin;
                const float maxY = static_cast<float>(screenHeight) - margin;
                
                bool foundValidPosition = false;
                
                // Try multiple points along the road to find one within bounds
                for (int attempt = 0; attempt < 5; attempt++) {
                    float testProgress = (float)attempt / 5.0f;
                    int pointIndex = static_cast<int>(testProgress * (road.points.size() - 1));
                    if (pointIndex >= road.points.size()) pointIndex = road.points.size() - 1;
                    
                    const Point& pt = road.points[pointIndex];
                    
                    // Check if this point is within boundaries
                    if (pt.x >= minX && pt.x <= maxX && pt.y >= minY && pt.y <= maxY) {
                        // Also check it's not in a park or fountain
                        bool insideObstacle = false;
                        
                        for (const auto& park : parkAreas) {
                            if (isInsideCircle(pt.x, pt.y, park)) {
                                insideObstacle = true;
                                break;
                            }
                        }
                        
                        if (!insideObstacle && isInsideCircle(pt.x, pt.y, fountainArea)) {
                            insideObstacle = true;
                        }
                        
                        if (!insideObstacle) {
                            // Valid position found!
                            car.x = pt.x;
                            car.y = pt.y;
                            car.roadProgress = testProgress;
                            foundValidPosition = true;
                            
                            // Recalculate velocity
                            if (pointIndex < road.points.size() - 1) {
                                const Point& nextPt = road.points[pointIndex + 1];
                                float dx = nextPt.x - pt.x;
                                float dy = nextPt.y - pt.y;
                                float length = std::sqrt(dx * dx + dy * dy);
                                
                                if (length > 0.0f) {
                                    car.vx = (dx / length) * car.speed;
                                    car.vy = (dy / length) * car.speed;
                                }
                            }
                            break;
                        }
                    }
                }
                
                // If no valid position found, try a different road
                if (!foundValidPosition) {
                    car.roadIndex = (car.roadIndex + 1) % roads.size();
                    car.roadProgress = 0.0f;
                }
            }
        }
    }
}
