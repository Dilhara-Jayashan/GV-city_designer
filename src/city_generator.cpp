#include "city_generator.h"
#include <iostream>
#include <random>
#include <cmath>

CityGenerator::CityGenerator(int width, int height) 
    : roadGen(width, height), screenWidth(width), screenHeight(height) {
}

void CityGenerator::generateCity(const CityConfig& config) {
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║     🏗️  GENERATING CITY...  🏗️        ║\n";
    std::cout << "╚════════════════════════════════════════╝\n" << std::flush;
    
    // Clear previous city data
    cityData.clear();
    
    // Generate roads using Bresenham's Line Algorithm
    cityData.roads = roadGen.generateRoads(config);
    
    // Generate parks using Midpoint Circle Algorithm
    generateParks(config);
    
    // Generate buildings
    generateBuildings(config);
    
    // Mark as generated
    cityData.isGenerated = true;
    
    std::cout << "\n✅ City generation complete!\n";
    std::cout << "   - Total roads: " << cityData.roads.size() << "\n";
    std::cout << "   - Total parks: " << cityData.parks.size() << "\n";
    std::cout << "   - Total buildings: " << cityData.buildings.size() << "\n\n" << std::flush;
}

void CityGenerator::generateParks(const CityConfig& config) {
    if (config.numParks == 0) {
        std::cout << "\n🌳 No parks requested\n";
        return;
    }
    
    std::cout << "\n🌳 Generating " << config.numParks << " parks...\n";
    
    // Random number generator for park placement
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> xDist(100, screenWidth - 100);
    std::uniform_int_distribution<int> yDist(100, screenHeight - 100);
    
    for (int i = 0; i < config.numParks; i++) {
        // Random position for park
        int x = xDist(rng);
        int y = yDist(rng);
        
        // Use Midpoint Circle Algorithm to generate park
        std::vector<Point> park = midpointCircle(x, y, config.parkRadius);
        cityData.parks.push_back(park);
        
        std::cout << "   - Park " << (i + 1) << " at (" << x << ", " << y 
                  << ") with radius " << config.parkRadius << "\n";
    }
    
    // Add a central fountain if requested
    if (config.fountainRadius > 0) {
        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2;
        
        std::vector<Point> fountain = midpointCircle(centerX, centerY, config.fountainRadius);
        cityData.parks.push_back(fountain);
        
        std::cout << "   - Central fountain at (" << centerX << ", " << centerY 
                  << ") with radius " << config.fountainRadius << "\n";
    }
}

void CityGenerator::generateBuildings(const CityConfig& config) {
    if (config.numBuildings == 0) {
        std::cout << "\n🏢 No buildings requested\n";
        return;
    }
    
    std::cout << "\n🏢 Generating " << config.numBuildings << " buildings...\n";
    
    // Random number generator
    std::random_device rd;
    std::mt19937 rng(rd());
    
    // Position distribution (avoid edges)
    std::uniform_int_distribution<int> xDist(50, screenWidth - 50);
    std::uniform_int_distribution<int> yDist(50, screenHeight - 50);
    
    // Building dimensions
    std::uniform_real_distribution<float> widthDist(20.0f, 60.0f);
    std::uniform_real_distribution<float> depthDist(20.0f, 60.0f);
    
    // Height distribution based on skyline type
    std::uniform_real_distribution<float> lowRiseHeight(10.0f, 30.0f);
    std::uniform_real_distribution<float> midRiseHeight(40.0f, 100.0f);
    std::uniform_real_distribution<float> highRiseHeight(120.0f, 250.0f);
    std::uniform_int_distribution<int> typeDist(0, 2);
    
    int attempts = 0;
    int maxAttempts = config.numBuildings * 10;
    
    while (cityData.buildings.size() < (size_t)config.numBuildings && attempts < maxAttempts) {
        attempts++;
        
        // Generate random position
        float x = xDist(rng);
        float y = yDist(rng);
        float width = widthDist(rng);
        float depth = depthDist(rng);
        
        // Check if position is valid (doesn't overlap roads/parks)
        if (!isValidBuildingPosition(x, y, width, depth)) {
            continue;
        }
        
        // Determine building type and height based on skyline configuration
        BuildingType type;
        float height;
        
        switch (config.skylineType) {
            case SkylineType::LOW_RISE:
                // All low-rise buildings
                type = BuildingType::LOW_RISE;
                height = lowRiseHeight(rng);
                break;
                
            case SkylineType::MID_RISE:
                // All mid-rise buildings
                type = BuildingType::MID_RISE;
                height = midRiseHeight(rng);
                break;
                
            case SkylineType::MIXED:
                // Mix of all types
                {
                    int typeChoice = typeDist(rng);
                    if (typeChoice == 0) {
                        type = BuildingType::LOW_RISE;
                        height = lowRiseHeight(rng);
                    } else if (typeChoice == 1) {
                        type = BuildingType::MID_RISE;
                        height = midRiseHeight(rng);
                    } else {
                        type = BuildingType::HIGH_RISE;
                        height = highRiseHeight(rng);
                    }
                }
                break;
                
            case SkylineType::SKYSCRAPER:
                // Mostly high-rise with some mid-rise
                {
                    int typeChoice = typeDist(rng);
                    if (typeChoice <= 1) {
                        type = BuildingType::HIGH_RISE;
                        height = highRiseHeight(rng);
                    } else {
                        type = BuildingType::MID_RISE;
                        height = midRiseHeight(rng);
                    }
                }
                break;
        }
        
        // Create and add building
        cityData.buildings.emplace_back(x, y, width, depth, height, type);
        
        if (cityData.buildings.size() % 5 == 0) {
            std::cout << "   - Generated " << cityData.buildings.size() << " buildings...\n" << std::flush;
        }
    }
    
    std::cout << "   ✓ Completed " << cityData.buildings.size() << " buildings\n";
    
    // Count by type
    int lowRise = 0, midRise = 0, highRise = 0;
    for (const auto& building : cityData.buildings) {
        switch (building.type) {
            case BuildingType::LOW_RISE: lowRise++; break;
            case BuildingType::MID_RISE: midRise++; break;
            case BuildingType::HIGH_RISE: highRise++; break;
        }
    }
    
    std::cout << "   - Low-rise: " << lowRise << " | Mid-rise: " << midRise << " | High-rise: " << highRise << "\n";
}

bool CityGenerator::isValidBuildingPosition(float x, float y, float width, float depth) const {
    // Simple check: ensure building doesn't overlap with park centers
    // In a more sophisticated version, you'd check actual road/park boundaries
    
    const float minDistance = 80.0f; // Minimum distance from parks
    
    for (const auto& park : cityData.parks) {
        if (!park.empty()) {
            // Get approximate park center (use first point as reference)
            float parkCenterX = park[0].x;
            float parkCenterY = park[0].y;
            
            float dx = x - parkCenterX;
            float dy = y - parkCenterY;
            float distance = std::sqrt(dx * dx + dy * dy);
            
            if (distance < minDistance) {
                return false;
            }
        }
    }
    
    return true;
}
