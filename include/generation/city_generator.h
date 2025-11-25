/**
 * @file city_generator.h
 * @brief City Generation System - Core City Builder
 * 
 * This module is responsible for generating all city elements:
 * - Buildings with various heights and types
 * - Parks using Midpoint Circle Algorithm
 * - Fountains (special circular park at center)
 * - Collision detection and space management
 * 
 * The generator ensures that:
 * - Buildings don't overlap with roads, parks, or each other
 * - Parks are placed in valid locations
 * - The city layout respects screen boundaries
 * - Interactive building placement is supported
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef CITY_GENERATOR_H
#define CITY_GENERATOR_H

#include <vector>
#include "core/city_config.h"
#include "generation/road_generator.h"
#include "utils/algorithms.h"

/**
 * @enum BuildingType
 * @brief Classification of buildings by height
 * 
 * Used to determine visual appearance and height in 3D view:
 * - LOW_RISE: Residential buildings (1-3 floors, height ~0.08)
 * - MID_RISE: Commercial buildings (4-10 floors, height ~0.15)
 * - HIGH_RISE: Skyscrapers (11+ floors, height ~0.30)
 */
enum BuildingType {
    LOW_RISE,      ///< 1-3 floors (residential), warm colors
    MID_RISE,      ///< 4-10 floors (commercial), neutral colors
    HIGH_RISE      ///< 11+ floors (skyscrapers), cool colors
};

/**
 * @struct Building
 * @brief Represents a single 3D building in the city
 * 
 * Buildings are rectangular prisms positioned at (x, y) on the ground plane.
 * The height determines how tall the building appears in 3D view.
 * 
 * Coordinate system:
 * - x, y: Ground position (center of building footprint)
 * - width: Size along X-axis (perpendicular to camera in 2D)
 * - depth: Size along Y-axis (towards/away from camera in 2D)
 * - height: Vertical size (Z-axis, visible in 3D view)
 */
struct Building {
    float x, y;           ///< Base position - center of building footprint
    float width;          ///< X-axis dimension (horizontal width)
    float depth;          ///< Y-axis dimension (horizontal depth)
    float height;         ///< Z-axis dimension (vertical height in 3D)
    BuildingType type;    ///< Classification determining visual style
    
    /**
     * @brief Construct a new Building
     * @param px X position (center)
     * @param py Y position (center)
     * @param w Width (X-axis size)
     * @param d Depth (Y-axis size)
     * @param h Height (Z-axis size, visible in 3D)
     * @param t Building type classification
     */
    Building(float px, float py, float w, float d, float h, BuildingType t)
        : x(px), y(py), width(w), depth(d), height(h), type(t) {}
};

/**
 * @struct CityData
 * @brief Container for all generated city elements
 * 
 * This structure holds the complete city state including:
 * - Roads: Network of connected road segments
 * - Parks: Circular green spaces (array of boundary points)
 * - Fountain: Special central circular feature
 * - Buildings: 3D structures with positions and heights
 * 
 * The isGenerated flag tracks whether a valid city exists.
 * This data can be serialized to JSON for save/load functionality.
 */
struct CityData {
    std::vector<Road> roads;                    ///< Road network (Bresenham lines)
    std::vector<std::vector<Point>> parks;      ///< Parks (each is circle of points)
    std::vector<Point> fountain;                ///< Central fountain (circle points)
    std::vector<Building> buildings;            ///< 3D building structures
    bool isGenerated;                           ///< True if city has been generated
    
    /**
     * @brief Construct empty city data
     */
    CityData() : isGenerated(false) {}
    
    /**
     * @brief Clear all city data
     * Resets to empty state, ready for new generation
     */
    void clear() {
        roads.clear();
        parks.clear();
        fountain.clear();
        buildings.clear();
        isGenerated = false;
    }
};

/**
 * @class CityGenerator
 * @brief Main city generation orchestrator
 * 
 * This class coordinates the entire city generation process:
 * 1. Road network generation (via RoadGenerator)
 * 2. Park placement (using Midpoint Circle Algorithm)
 * 3. Fountain creation (special central park)
 * 4. Building generation with collision avoidance
 * 5. Interactive building placement support
 * 
 * Generation algorithm:
 * - First generates road network (grid/radial/random pattern)
 * - Then places parks in valid locations (avoiding roads)
 * - Creates central fountain
 * - Finally generates buildings in remaining space
 * - Ensures no overlaps using AABB and circle-box collision
 * 
 * The generator maintains screen boundaries and proper spacing
 * between all elements using configurable buffer zones.
 */
class CityGenerator {
private:
    RoadGenerator roadGen;      ///< Road network generator instance
    CityData cityData;          ///< Container for all generated elements
    int screenWidth;            ///< Screen width in pixels (800)
    int screenHeight;           ///< Screen height in pixels (600)
    
public:
    /**
     * @brief Construct a new City Generator
     * @param width Screen width in pixels
     * @param height Screen height in pixels
     * 
     * Initializes the road generator with screen dimensions.
     */
    CityGenerator(int width, int height);
    
    /**
     * @brief Generate a complete city from scratch
     * @param config City configuration (parameters for generation)
     * 
     * This is the main generation method that:
     * 1. Clears existing city data
     * 2. Generates road network based on pattern (grid/radial/random)
     * 3. Places parks avoiding roads
     * 4. Creates central fountain
     * 5. Generates buildings in remaining valid spaces
     * 6. Reports generation statistics to console
     * 
     * The entire process takes ~100-500ms depending on complexity.
     */
    void generateCity(const CityConfig& config);
    
    /**
     * @brief Get read-only access to city data
     * @return const CityData& Reference to generated city
     */
    const CityData& getCityData() const { return cityData; }
    
    /**
     * @brief Get mutable access to city data
     * @return CityData& Reference for modification (used by save/load)
     * 
     * This non-const version is needed for the load system
     * to populate city data from saved JSON files.
     */
    CityData& getCityData() { return cityData; }
    
    /**
     * @brief Check if a city has been generated
     * @return true if city exists and is ready to render
     */
    bool hasCity() const { return cityData.isGenerated; }
    
    /**
     * @brief Interactively place a building at coordinates
     * @param x World X coordinate (center of building)
     * @param y World Y coordinate (center of building)
     * @param config City config for building size parameters
     * @return true if building was successfully placed
     * 
     * This method supports Feature 4 (Click-to-Place Buildings).
     * It performs comprehensive collision checks:
     * - Screen boundaries (60px margin)
     * - Roads (20px buffer)
     * - Parks (35px buffer)
     * - Fountain (35px buffer)
     * - Existing buildings (25px buffer)
     * 
     * If all checks pass, creates a MID_RISE building.
     * Used only in 2D view mode.
     */
    bool placeBuilding(float x, float y, const CityConfig& config);
    
private:
    /**
     * @brief Generate parks using Midpoint Circle Algorithm
     * @param config City configuration (park count, radius)
     * 
     * Creates circular parks by:
     * 1. Finding valid random positions (avoiding roads)
     * 2. Using midpoint circle algorithm to generate boundary
     * 3. Adding to cityData.parks vector
     * 
     * Also creates the central fountain as a special park.
     */
    void generateParks(const CityConfig& config);
    
    /**
     * @brief Generate buildings in available space
     * @param config City configuration (count, size, skyline type)
     * 
     * Attempts to place buildings randomly while:
     * - Avoiding roads (with buffer)
     * - Avoiding parks (with buffer)
     * - Avoiding other buildings (with buffer)
     * - Staying within screen boundaries
     * 
     * Uses multiple attempts per building (up to 100 tries)
     * to find valid positions. Buildings vary in height
     * based on skyline type configuration.
     */
    void generateBuildings(const CityConfig& config);
    
    /**
     * @brief Check if position is valid for building placement
     * @param x Center X position
     * @param y Center Y position
     * @param width Building width
     * @param depth Building depth
     * @return true if position is valid (no collisions)
     * 
     * Performs comprehensive collision detection:
     * 1. Screen boundary check (60px margin)
     * 2. Building-building AABB collision (25px buffer)
     * 3. Building-park circle collision (35px buffer)
     * 4. Building-fountain circle collision (35px buffer)
     * 5. Building-road point collision (checked per road point)
     * 
     * This is the core validation used by both automatic
     * generation and interactive placement.
     */
    bool isValidBuildingPosition(float x, float y, float width, float depth) const;
};

#endif // CITY_GENERATOR_H
