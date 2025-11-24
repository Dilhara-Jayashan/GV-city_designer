#ifndef CITY_CONFIG_H
#define CITY_CONFIG_H

#include <string>

// Enum for road patterns
enum class RoadPattern {
    GRID,
    RADIAL,
    RANDOM
};

// Enum for skyline types
enum class SkylineType {
    LOW_RISE,
    MID_RISE,
    SKYSCRAPER,
    MIXED
};

// Enum for texture themes
enum class TextureTheme {
    MODERN,
    CLASSIC,
    INDUSTRIAL,
    FUTURISTIC
};

// City Configuration Structure
// Holds all user-controlled parameters for city generation
struct CityConfig {
    // Building parameters
    int numBuildings;           // Number of buildings to generate
    int layoutSize;             // Size of the city grid (e.g., 10 = 10x10)
    
    // Road parameters
    RoadPattern roadPattern;    // Type of road network
    int roadWidth;              // Width of roads in pixels
    
    // Skyline parameters
    SkylineType skylineType;    // Building height distribution
    
    // Texture parameters
    TextureTheme textureTheme;  // Building facade style
    
    // Park/Circle parameters
    int parkRadius;             // Radius for circular parks
    int numParks;               // Number of parks to generate
    int fountainRadius;         // Radius for fountains
    
    // View mode
    bool view3D;                // Toggle between 2D and 3D view
    
    // Constructor with default values
    CityConfig() 
        : numBuildings(20),
          layoutSize(10),
          roadPattern(RoadPattern::GRID),
          roadWidth(8),
          skylineType(SkylineType::MIXED),
          textureTheme(TextureTheme::MODERN),
          parkRadius(40),
          numParks(3),
          fountainRadius(25),
          view3D(false)
    {}
    
    // Helper methods to convert enums to strings for display
    std::string getRoadPatternString() const {
        switch(roadPattern) {
            case RoadPattern::GRID: return "Grid";
            case RoadPattern::RADIAL: return "Radial";
            case RoadPattern::RANDOM: return "Random";
            default: return "Unknown";
        }
    }
    
    std::string getSkylineTypeString() const {
        switch(skylineType) {
            case SkylineType::LOW_RISE: return "Low-Rise";
            case SkylineType::MID_RISE: return "Mid-Rise";
            case SkylineType::SKYSCRAPER: return "Skyscraper";
            case SkylineType::MIXED: return "Mixed";
            default: return "Unknown";
        }
    }
    
    std::string getTextureThemeString() const {
        switch(textureTheme) {
            case TextureTheme::MODERN: return "Modern";
            case TextureTheme::CLASSIC: return "Classic";
            case TextureTheme::INDUSTRIAL: return "Industrial";
            case TextureTheme::FUTURISTIC: return "Futuristic";
            default: return "Unknown";
        }
    }
    
    // Method to print current configuration
    void printConfig() const;
};

#endif // CITY_CONFIG_H
