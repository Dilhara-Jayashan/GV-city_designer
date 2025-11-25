/**
 * @file city_config.h
 * @brief City Configuration System
 * 
 * Defines all configuration parameters, enums, and settings for city generation.
 * This is the central configuration point for the entire application.
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef CITY_CONFIG_H
#define CITY_CONFIG_H

#include <string>

/**
 * @enum RoadPattern
 * @brief Road network patterns for city generation
 */
enum class RoadPattern {
    GRID,       ///< Traditional grid layout (Manhattan-style)
    RADIAL,     ///< Radial pattern with spokes from center
    RANDOM      ///< Random organic road network
};

/**
 * @enum SkylineType
 * @brief Building height distribution patterns
 */
enum class SkylineType {
    LOW_RISE,       ///< Mostly low buildings (1-3 floors)
    MID_RISE,       ///< Mostly medium buildings (4-10 floors)
    SKYSCRAPER,     ///< Mostly tall buildings (11+ floors)
    MIXED           ///< Mixed heights for variety
};

/**
 * @enum TextureTheme
 * @brief Visual themes for building textures
 */
enum class TextureTheme {
    MODERN,         ///< Modern glass and steel aesthetic
    CLASSIC,        ///< Classic brick and stone
    INDUSTRIAL,     ///< Industrial concrete and metal
    FUTURISTIC      ///< Futuristic high-tech appearance
};

/**
 * @struct CityConfig
 * @brief Comprehensive city generation configuration
 * 
 * This structure holds all user-controlled parameters that affect
 * how cities are generated, rendered, and displayed. All values can
 * be modified at runtime through keyboard controls.
 */
struct CityConfig {
    // ===== Building Parameters =====
    int numBuildings;           ///< Number of buildings to generate (1-100)
    int layoutSize;             ///< Size of the city grid, e.g., 10 = 10x10 (5-20)
    
    // ===== Road Parameters =====
    RoadPattern roadPattern;    ///< Type of road network pattern
    int roadWidth;              ///< Width of roads in pixels (2-20)
    
    // ===== Skyline Parameters =====
    SkylineType skylineType;    ///< Building height distribution strategy
    
    // ===== Texture Parameters =====
    TextureTheme textureTheme;  ///< Building facade visual theme
    
    // ===== Park/Fountain Parameters =====
    int parkRadius;             ///< Radius for circular parks in pixels (10-100)
    int numParks;               ///< Number of parks to generate (0-10)
    int fountainRadius;         ///< Radius for central fountain (25 or 40)
    
    // ===== Building Size Parameters =====
    bool useStandardSize;       ///< If true, all buildings use standard dimensions
    float standardWidth;        ///< Standard building width when useStandardSize=true
    float standardDepth;        ///< Standard building depth when useStandardSize=true
    
    // ===== View Mode =====
    bool view3D;                ///< Toggle: false=2D orthographic, true=3D perspective
    
    // ===== Time of Day =====
    float timeOfDay;            ///< Time in hours (0-24): 0=midnight, 6=sunrise, 12=noon, 18=sunset
    bool autoTimeProgress;      ///< If true, time advances automatically
    
    // ===== Traffic Parameters =====
    int numCars;                ///< Number of cars for traffic animation (0-50)
    bool showTraffic;           ///< Toggle traffic visibility
    
    /**
     * @brief Construct a new City Config with sensible defaults
     * 
     * Default configuration creates a medium-sized city with:
     * - 20 buildings in a 10x10 grid
     * - Grid road pattern
     * - Mixed building heights
     * - 3 parks and a fountain
     * - Starting in 2D view mode
     */
    CityConfig() 
        : numBuildings(20),
          layoutSize(10),
          roadPattern(RoadPattern::GRID),
          roadWidth(14),
          skylineType(SkylineType::MIXED),
          textureTheme(TextureTheme::MODERN),
          parkRadius(40),
          numParks(3),
          fountainRadius(25),
          useStandardSize(true),
          standardWidth(50.0f),
          standardDepth(50.0f),
          view3D(false),
          timeOfDay(14.0f),         // Start at 2 PM (afternoon)
          autoTimeProgress(true),   // Automatic time progression enabled
          numCars(15),              // Default 15 cars
          showTraffic(true)         // Traffic enabled by default
    {
        // Initialize building size based on default layout
        updateStandardBuildingSize();
    }
    
    /**
     * @brief Convert road pattern enum to human-readable string
     * @return String representation of the road pattern
     */
    std::string getRoadPatternString() const {
        switch(roadPattern) {
            case RoadPattern::GRID: return "Grid";
            case RoadPattern::RADIAL: return "Radial";
            case RoadPattern::RANDOM: return "Random";
            default: return "Unknown";
        }
    }
    
    /**
     * @brief Convert skyline type enum to human-readable string
     * @return String representation of the skyline type
     */
    std::string getSkylineTypeString() const {
        switch(skylineType) {
            case SkylineType::LOW_RISE: return "Low-Rise";
            case SkylineType::MID_RISE: return "Mid-Rise";
            case SkylineType::SKYSCRAPER: return "Skyscraper";
            case SkylineType::MIXED: return "Mixed";
            default: return "Unknown";
        }
    }
    
    /**
     * @brief Convert texture theme enum to human-readable string
     * @return String representation of the texture theme
     */
    std::string getTextureThemeString() const {
        switch(textureTheme) {
            case TextureTheme::MODERN: return "Modern";
            case TextureTheme::CLASSIC: return "Classic";
            case TextureTheme::INDUSTRIAL: return "Industrial";
            case TextureTheme::FUTURISTIC: return "Futuristic";
            default: return "Unknown";
        }
    }
    
    /**
     * @brief Calculate optimal building size based on layout grid
     * @param screenWidth Width of the screen/window in pixels
     * @param margin Margin from screen edges in pixels
     * 
     * Automatically adjusts standardWidth and standardDepth to fit
     * buildings within the grid cells, accounting for roads.
     * Buildings are sized to ~40% of cell size for proper spacing.
     */
    void updateStandardBuildingSize(int screenWidth = 800, int margin = 50) {
        // Calculate grid cell size
        float cellSize = static_cast<float>(screenWidth - 2 * margin) / layoutSize;
        // Buildings should be about 40% of cell size to fit within one grid square
        // This accounts for road width and proper spacing
        standardWidth = cellSize * 0.40f;
        standardDepth = cellSize * 0.40f;
    }
    
    /**
     * @brief Print current configuration to console
     * 
     * Displays a formatted table of all current configuration values.
     * Useful for debugging and showing the user current settings.
     */
    void printConfig() const;
};

#endif // CITY_CONFIG_H
