/**
 * @file building_placement_system.h
 * @brief Feature 4: Interactive Building Placement
 * 
 * Handles click-to-place building functionality in 2D view.
 * Manages collision detection with roads, parks, and existing buildings.
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef BUILDING_PLACEMENT_SYSTEM_H
#define BUILDING_PLACEMENT_SYSTEM_H

#include <vector>

// Forward declarations
struct Building;
struct Road;
struct Point;
struct CityConfig;

/**
 * @class BuildingPlacementSystem
 * @brief Manages interactive building placement via mouse clicks
 * 
 * This feature allows users to:
 * - Click on the 2D view to place buildings
 * - Automatically check for collisions with roads, parks, fountains
 * - Ensure buildings don't overlap
 * - Respect screen boundaries
 */
class BuildingPlacementSystem {
public:
    /**
     * @brief Construct a new Building Placement System
     */
    BuildingPlacementSystem();
    
    /**
     * @brief Attempt to place a building at screen coordinates
     * @param worldX X coordinate in world space
     * @param worldY Y coordinate in world space
     * @param buildings Existing buildings list (will be modified if placement succeeds)
     * @param roads Roads to check for collisions
     * @param parks Parks to check for collisions
     * @param fountain Fountain area to check for collision
     * @param config City configuration for building size
     * @param screenWidth Screen width for boundary checking
     * @param screenHeight Screen height for boundary checking
     * @return true if building was successfully placed
     * 
     * This function:
     * 1. Checks if position is within screen boundaries
     * 2. Checks for collision with roads
     * 3. Checks for collision with parks and fountain
     * 4. Checks for overlap with existing buildings
     * 5. If all checks pass, adds new building to the list
     */
    bool tryPlaceBuilding(float worldX, float worldY,
                          std::vector<Building>& buildings,
                          const std::vector<Road>& roads,
                          const std::vector<std::vector<Point>>& parks,
                          const std::vector<Point>& fountain,
                          const CityConfig& config,
                          int screenWidth, int screenHeight);
    
private:
    /**
     * @brief Check if position collides with any road
     * @param x X position
     * @param y Y position
     * @param width Building width
     * @param depth Building depth
     * @param roads List of roads
     * @return true if collision detected
     */
    bool collidesWithRoads(float x, float y, float width, float depth,
                           const std::vector<Road>& roads) const;
    
    /**
     * @brief Check if position collides with parks
     * @param x X position
     * @param y Y position
     * @param width Building width
     * @param depth Building depth
     * @param parks List of park areas
     * @return true if collision detected
     */
    bool collidesWithParks(float x, float y, float width, float depth,
                           const std::vector<std::vector<Point>>& parks) const;
    
    /**
     * @brief Check if position collides with fountain
     * @param x X position
     * @param y Y position
     * @param width Building width
     * @param depth Building depth
     * @param fountain Fountain area
     * @return true if collision detected
     */
    bool collidesWithFountain(float x, float y, float width, float depth,
                              const std::vector<Point>& fountain) const;
    
    /**
     * @brief Check if position collides with existing buildings
     * @param x X position
     * @param y Y position
     * @param width Building width
     * @param depth Building depth
     * @param buildings List of existing buildings
     * @return true if collision detected
     */
    bool collidesWithBuildings(float x, float y, float width, float depth,
                               const std::vector<Building>& buildings) const;
    
    /**
     * @brief Check if point is inside circle
     * @param px Point X
     * @param py Point Y
     * @param circlePoints Circle boundary points
     * @return true if point is inside
     */
    bool isInsideCircle(float px, float py, const std::vector<Point>& circlePoints) const;
};

#endif // BUILDING_PLACEMENT_SYSTEM_H
