#include "generation/city_generator.h"
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
    
    // GENERATION ORDER:
    // 1. Generate parks and fountains first (using Midpoint Circle Algorithm)
    generateParks(config);
    
    // 2. Generate roads (using Bresenham's Line Algorithm) - avoid parks/fountains
    cityData.roads = roadGen.generateRoadsAvoidingObstacles(config, cityData.parks, cityData.fountain);
    
    // 3. Generate buildings last (avoid parks, fountains, and roads)
    generateBuildings(config);
    
    // Mark as generated
    cityData.isGenerated = true;
    
    std::cout << "\n✅ City generation complete!\n";
    std::cout << "   - Total parks: " << cityData.parks.size() << "\n";
    std::cout << "   - Total buildings: " << cityData.buildings.size() << "\n";
    std::cout << "   - Total roads: " << cityData.roads.size() << "\n\n" << std::flush;
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
    
    int attempts = 0;
    int maxAttempts = config.numParks * 100; // More attempts for finding valid positions
    
    for (int i = 0; i < config.numParks && attempts < maxAttempts; ) {
        attempts++;
        
        // Random position for park with margins
        int marginX = config.parkRadius + 50;
        int marginY = config.parkRadius + 50;
        std::uniform_int_distribution<int> xDistMargin(marginX, screenWidth - marginX);
        std::uniform_int_distribution<int> yDistMargin(marginY, screenHeight - marginY);
        
        int x = xDistMargin(rng);
        int y = yDistMargin(rng);
        
        bool validPosition = true;
        
        // CHECK 1: Overlap with existing parks
        const float minParkDistance = config.parkRadius * 2.5f; // Good spacing between parks
        
        for (const auto& existingPark : cityData.parks) {
            if (existingPark.empty()) continue;
            
            // Calculate center of existing park
            float existingX = 0, existingY = 0;
            for (const auto& point : existingPark) {
                existingX += point.x;
                existingY += point.y;
            }
            existingX /= existingPark.size();
            existingY /= existingPark.size();
            
            // Check center-to-center distance
            float dx = x - existingX;
            float dy = y - existingY;
            float distance = std::sqrt(dx * dx + dy * dy);
            
            if (distance < minParkDistance) {
                validPosition = false;
                break;
            }
        }
        
        // CHECK 2: Overlap with fountain (reserved center space)
        if (validPosition && config.fountainRadius > 0) {
            int centerX = screenWidth / 2;
            int centerY = screenHeight / 2;
            float dx = x - centerX;
            float dy = y - centerY;
            float distance = std::sqrt(dx * dx + dy * dy);
            float minFountainDistance = config.parkRadius + config.fountainRadius + 30.0f;
            
            if (distance < minFountainDistance) {
                validPosition = false;
            }
        }
        
        if (validPosition) {
            // Use Midpoint Circle Algorithm to generate park
            std::vector<Point> park = midpointCircle(x, y, config.parkRadius);
            cityData.parks.push_back(park);
            
            std::cout << "   - Park " << (i + 1) << " at (" << x << ", " << y 
                      << ") with radius " << config.parkRadius << "\n";
            i++; // Successfully placed a park
        }
    }
    
    if (cityData.parks.size() < (size_t)config.numParks) {
        std::cout << "   ⚠️  Only placed " << cityData.parks.size() << " parks (strict overlap checking)\n";
    }
    
    // Add a central fountain if requested (stored separately for different rendering color)
    if (config.fountainRadius > 0) {
        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2;
        
        cityData.fountain = midpointCircle(centerX, centerY, config.fountainRadius);
        
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
    int maxAttempts = config.numBuildings * 50; // Increased attempts for stricter collision checks
    
    while (cityData.buildings.size() < (size_t)config.numBuildings && attempts < maxAttempts) {
        attempts++;
        
        // Generate random position with better margins
        std::uniform_int_distribution<int> xDistBetter(80, screenWidth - 80);
        std::uniform_int_distribution<int> yDistBetter(80, screenHeight - 80);
        
        float x = xDistBetter(rng);
        float y = yDistBetter(rng);
        
        // Use standard size or random size based on configuration
        float width, depth;
        if (config.useStandardSize) {
            width = config.standardWidth;
            depth = config.standardDepth;
        } else {
            width = widthDist(rng);
            depth = depthDist(rng);
        }
        
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
    // Calculate building bounding box with generous margins
    float halfWidth = width / 2.0f;
    float halfDepth = depth / 2.0f;
    const float buildingBuffer = 25.0f; // Increased buffer between buildings
    
    float buildingLeft = x - halfWidth;
    float buildingRight = x + halfWidth;
    float buildingTop = y - halfDepth;
    float buildingBottom = y + halfDepth;
    
    // Check screen boundaries with margin
    const float screenMargin = 60.0f;
    if (buildingLeft < screenMargin || buildingRight > screenWidth - screenMargin ||
        buildingTop < screenMargin || buildingBottom > screenHeight - screenMargin) {
        return false; // Too close to screen edges
    }
    
    // 1. Check overlap with existing buildings (STRICT - no touching)
    for (const auto& existingBuilding : cityData.buildings) {
        float existingHalfWidth = existingBuilding.width / 2.0f;
        float existingHalfDepth = existingBuilding.depth / 2.0f;
        float existingLeft = existingBuilding.x - existingHalfWidth;
        float existingRight = existingBuilding.x + existingHalfWidth;
        float existingTop = existingBuilding.y - existingHalfDepth;
        float existingBottom = existingBuilding.y + existingHalfDepth;
        
        // Check AABB collision with strict buffer
        // Buildings must have at least 'buildingBuffer' pixels between them
        if (!(buildingRight + buildingBuffer < existingLeft ||
              buildingLeft - buildingBuffer > existingRight ||
              buildingBottom + buildingBuffer < existingTop ||
              buildingTop - buildingBuffer > existingBottom)) {
            return false; // Buildings too close or overlapping
        }
    }
    
    // 2. Check overlap with parks (STRICT - check ALL park points)
    const float parkBuffer = 35.0f; // Increased buffer around parks
    
    for (const auto& park : cityData.parks) {
        if (park.empty()) continue;
        
        // Method 1: Check center distance (fast rejection)
        float parkCenterX = 0, parkCenterY = 0;
        for (const auto& point : park) {
            parkCenterX += point.x;
            parkCenterY += point.y;
        }
        parkCenterX /= park.size();
        parkCenterY /= park.size();
        
        // Calculate radius from center to furthest point
        float parkRadius = 0;
        for (const auto& point : park) {
            float dx = point.x - parkCenterX;
            float dy = point.y - parkCenterY;
            float dist = std::sqrt(dx * dx + dy * dy);
            parkRadius = std::max(parkRadius, dist);
        }
        
        // Check if building box intersects with park circle (with buffer)
        float closestX = std::max(buildingLeft - parkBuffer, 
                                 std::min(parkCenterX, buildingRight + parkBuffer));
        float closestY = std::max(buildingTop - parkBuffer, 
                                 std::min(parkCenterY, buildingBottom + parkBuffer));
        
        float dx = closestX - parkCenterX;
        float dy = closestY - parkCenterY;
        float distanceSquared = dx * dx + dy * dy;
        float radiusWithBuffer = parkRadius + parkBuffer;
        
        if (distanceSquared < radiusWithBuffer * radiusWithBuffer) {
            return false; // Building too close to park
        }
        
        // Method 2: Check individual park points (thorough verification)
        for (const auto& point : park) {
            // Check if any park point is inside or near building box
            if (point.x >= buildingLeft - parkBuffer && 
                point.x <= buildingRight + parkBuffer &&
                point.y >= buildingTop - parkBuffer && 
                point.y <= buildingBottom + parkBuffer) {
                return false; // Park point too close to building
            }
        }
    }
    
    // 3. Check overlap with fountain (same as parks)
    if (!cityData.fountain.empty()) {
        const float fountainBuffer = 35.0f;
        
        // Calculate fountain center
        float fountainCenterX = 0, fountainCenterY = 0;
        for (const auto& point : cityData.fountain) {
            fountainCenterX += point.x;
            fountainCenterY += point.y;
        }
        fountainCenterX /= cityData.fountain.size();
        fountainCenterY /= cityData.fountain.size();
        
        // Calculate fountain radius
        float fountainRadius = 0;
        for (const auto& point : cityData.fountain) {
            float dx = point.x - fountainCenterX;
            float dy = point.y - fountainCenterY;
            float dist = std::sqrt(dx * dx + dy * dy);
            fountainRadius = std::max(fountainRadius, dist);
        }
        
        // Check if building box intersects with fountain circle
        float closestX = std::max(buildingLeft - fountainBuffer, 
                                 std::min(fountainCenterX, buildingRight + fountainBuffer));
        float closestY = std::max(buildingTop - fountainBuffer, 
                                 std::min(fountainCenterY, buildingBottom + fountainBuffer));
        
        float dx = closestX - fountainCenterX;
        float dy = closestY - fountainCenterY;
        float distanceSquared = dx * dx + dy * dy;
        float radiusWithBuffer = fountainRadius + fountainBuffer;
        
        if (distanceSquared < radiusWithBuffer * radiusWithBuffer) {
            return false; // Building too close to fountain
        }
    }
    
    // 4. Check overlap with roads (since roads are now generated before buildings)
    const float roadBuffer = 5.0f; // Small buffer around roads
    
    for (const auto& road : cityData.roads) {
        if (road.points.empty()) continue;
        
        // Check if building intersects with any road point
        for (const auto& point : road.points) {
            // Expand road point by road width
            float roadHalfWidth = road.width / 2.0f;
            
            if (point.x >= buildingLeft - roadBuffer - roadHalfWidth && 
                point.x <= buildingRight + roadBuffer + roadHalfWidth &&
                point.y >= buildingTop - roadBuffer - roadHalfWidth && 
                point.y <= buildingBottom + roadBuffer + roadHalfWidth) {
                return false; // Building too close to road
            }
        }
    }
    
    return true; // Position is valid - no overlaps detected
}

// Place a building at specific coordinates (for interactive placement)
bool CityGenerator::placeBuilding(float x, float y, const CityConfig& config) {
    if (!cityData.isGenerated) {
        std::cout << "⚠️  Cannot place building: City not generated yet!\n";
        return false;
    }
    
    // Use standard building size from config
    float width = config.standardWidth;
    float depth = config.standardDepth;
    
    // Check if position is valid
    if (!isValidBuildingPosition(x, y, width, depth)) {
        std::cout << "❌ Cannot place building: Position overlaps with existing structures\n";
        return false;
    }
    
    // Determine building type based on skyline configuration
    BuildingType type;
    float height;
    
    switch (config.skylineType) {
        case SkylineType::LOW_RISE:
            type = BuildingType::LOW_RISE;
            height = 20.0f + (rand() % 20); // 20-40
            break;
        case SkylineType::MID_RISE:
            type = BuildingType::MID_RISE;
            height = 40.0f + (rand() % 40); // 40-80
            break;
        case SkylineType::SKYSCRAPER:
            type = BuildingType::HIGH_RISE;
            height = 80.0f + (rand() % 60); // 80-140
            break;
        case SkylineType::MIXED:
        default:
            // Random distribution
            int roll = rand() % 100;
            if (roll < 40) {
                type = BuildingType::LOW_RISE;
                height = 20.0f + (rand() % 20);
            } else if (roll < 75) {
                type = BuildingType::MID_RISE;
                height = 40.0f + (rand() % 40);
            } else {
                type = BuildingType::HIGH_RISE;
                height = 80.0f + (rand() % 60);
            }
            break;
    }
    
    // Create and add the building
    cityData.buildings.emplace_back(x, y, width, depth, height, type);
    
    std::cout << "✅ Building placed at (" << (int)x << ", " << (int)y << ") - Type: " 
              << (type == BuildingType::LOW_RISE ? "Low-Rise" : 
                  type == BuildingType::MID_RISE ? "Mid-Rise" : "High-Rise") << "\n";
    
    return true;
}
