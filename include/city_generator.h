#ifndef CITY_GENERATOR_H
#define CITY_GENERATOR_H

#include <vector>
#include "city_config.h"
#include "road_generator.h"
#include "algorithms.h"

// Building types based on height
enum BuildingType {
    LOW_RISE,      // 1-3 floors (residential)
    MID_RISE,      // 4-10 floors (commercial)
    HIGH_RISE      // 11+ floors (skyscrapers)
};

// Structure to represent a 3D building
struct Building {
    float x, y;           // Base position (center of building)
    float width;          // X-axis dimension
    float depth;          // Y-axis dimension
    float height;         // Z-axis dimension (vertical)
    BuildingType type;    // Building classification
    
    Building(float px, float py, float w, float d, float h, BuildingType t)
        : x(px), y(py), width(w), depth(d), height(h), type(t) {}
};

// Structure to hold all generated city elements
struct CityData {
    std::vector<Road> roads;
    std::vector<std::vector<Point>> parks;  // Each park is a vector of points
    std::vector<Building> buildings;         // 3D buildings
    bool isGenerated;
    
    CityData() : isGenerated(false) {}
    
    void clear() {
        roads.clear();
        parks.clear();
        buildings.clear();
        isGenerated = false;
    }
};

// City Generator Class
// Manages the overall city generation process
class CityGenerator {
private:
    RoadGenerator roadGen;
    CityData cityData;
    int screenWidth;
    int screenHeight;
    
public:
    CityGenerator(int width, int height);
    
    // Generate a complete city based on configuration
    void generateCity(const CityConfig& config);
    
    // Get the generated city data
    const CityData& getCityData() const { return cityData; }
    
    // Check if city is generated
    bool hasCity() const { return cityData.isGenerated; }
    
private:
    // Generate parks using Midpoint Circle Algorithm
    void generateParks(const CityConfig& config);
    
    // Generate buildings based on configuration and available space
    void generateBuildings(const CityConfig& config);
    
    // Helper function to check if a position overlaps with roads or parks
    bool isValidBuildingPosition(float x, float y, float width, float depth) const;
};

#endif // CITY_GENERATOR_H
