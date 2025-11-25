/**
 * @file building_placement_system.cpp
 * @brief Implementation of Building Placement Feature
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include "features/building_placement/building_placement_system.h"
#include "generation/city_generator.h"
#include "core/city_config.h"
#include <cmath>
#include <algorithm>
#include <iostream>

BuildingPlacementSystem::BuildingPlacementSystem() {
}

bool BuildingPlacementSystem::tryPlaceBuilding(
    float worldX, float worldY,
    std::vector<Building>& buildings,
    const std::vector<Road>& roads,
    const std::vector<std::vector<Point>>& parks,
    const std::vector<Point>& fountain,
    const CityConfig& config,
    int screenWidth, int screenHeight)
{
    // Use standard building size from config
    float width = config.standardWidth;
    float depth = config.standardDepth;
    
    // Convert click position to building center
    float x = worldX;
    float y = worldY;
    
    // Check boundaries
    float halfWidth = width / 2.0f;
    float halfDepth = depth / 2.0f;
    const float screenMargin = 60.0f;
    
    if (x - halfWidth < screenMargin || x + halfWidth > screenWidth - screenMargin ||
        y - halfDepth < screenMargin || y + halfDepth > screenHeight - screenMargin) {
        std::cout << "❌ Cannot place building: too close to screen edge\n";
        return false;
    }
    
    // Check collisions
    if (collidesWithRoads(x, y, width, depth, roads)) {
        std::cout << "❌ Cannot place building: overlaps with road\n";
        return false;
    }
    
    if (collidesWithParks(x, y, width, depth, parks)) {
        std::cout << "❌ Cannot place building: overlaps with park\n";
        return false;
    }
    
    if (collidesWithFountain(x, y, width, depth, fountain)) {
        std::cout << "❌ Cannot place building: overlaps with fountain\n";
        return false;
    }
    
    if (collidesWithBuildings(x, y, width, depth, buildings)) {
        std::cout << "❌ Cannot place building: overlaps with existing building\n";
        return false;
    }
    
    // All checks passed - place the building
    float height = 0.15f;  // Default mid-rise height
    BuildingType type = BuildingType::MID_RISE;
    
    buildings.emplace_back(x, y, width, depth, height, type);
    
    std::cout << "✅ Building placed at (" << (int)x << ", " << (int)y << ")\n";
    std::cout << "   Total buildings: " << buildings.size() << "\n";
    
    return true;
}

bool BuildingPlacementSystem::collidesWithRoads(
    float x, float y, float width, float depth,
    const std::vector<Road>& roads) const
{
    float halfWidth = width / 2.0f;
    float halfDepth = depth / 2.0f;
    const float roadBuffer = 20.0f;
    
    float buildingLeft = x - halfWidth;
    float buildingRight = x + halfWidth;
    float buildingTop = y - halfDepth;
    float buildingBottom = y + halfDepth;
    
    for (const auto& road : roads) {
        for (const auto& point : road.points) {
            if (point.x >= buildingLeft - roadBuffer &&
                point.x <= buildingRight + roadBuffer &&
                point.y >= buildingTop - roadBuffer &&
                point.y <= buildingBottom + roadBuffer) {
                return true;
            }
        }
    }
    
    return false;
}

bool BuildingPlacementSystem::collidesWithParks(
    float x, float y, float width, float depth,
    const std::vector<std::vector<Point>>& parks) const
{
    const float parkBuffer = 35.0f;
    
    for (const auto& park : parks) {
        if (park.empty()) continue;
        
        // Calculate park center and radius
        float parkCenterX = 0, parkCenterY = 0;
        for (const auto& point : park) {
            parkCenterX += point.x;
            parkCenterY += point.y;
        }
        parkCenterX /= park.size();
        parkCenterY /= park.size();
        
        float parkRadius = 0;
        for (const auto& point : park) {
            float dx = point.x - parkCenterX;
            float dy = point.y - parkCenterY;
            float dist = std::sqrt(dx * dx + dy * dy);
            parkRadius = std::max(parkRadius, dist);
        }
        
        // Check if building box intersects with park circle
        float halfWidth = width / 2.0f;
        float halfDepth = depth / 2.0f;
        
        float closestX = std::max(x - halfWidth - parkBuffer,
                                 std::min(parkCenterX, x + halfWidth + parkBuffer));
        float closestY = std::max(y - halfDepth - parkBuffer,
                                 std::min(parkCenterY, y + halfDepth + parkBuffer));
        
        float dx = closestX - parkCenterX;
        float dy = closestY - parkCenterY;
        float distanceSquared = dx * dx + dy * dy;
        float radiusWithBuffer = parkRadius + parkBuffer;
        
        if (distanceSquared < radiusWithBuffer * radiusWithBuffer) {
            return true;
        }
    }
    
    return false;
}

bool BuildingPlacementSystem::collidesWithFountain(
    float x, float y, float width, float depth,
    const std::vector<Point>& fountain) const
{
    if (fountain.empty()) return false;
    
    const float fountainBuffer = 35.0f;
    
    // Calculate fountain center and radius
    float fountainCenterX = 0, fountainCenterY = 0;
    for (const auto& point : fountain) {
        fountainCenterX += point.x;
        fountainCenterY += point.y;
    }
    fountainCenterX /= fountain.size();
    fountainCenterY /= fountain.size();
    
    float fountainRadius = 0;
    for (const auto& point : fountain) {
        float dx = point.x - fountainCenterX;
        float dy = point.y - fountainCenterY;
        float dist = std::sqrt(dx * dx + dy * dy);
        fountainRadius = std::max(fountainRadius, dist);
    }
    
    // Check if building box intersects with fountain circle
    float halfWidth = width / 2.0f;
    float halfDepth = depth / 2.0f;
    
    float closestX = std::max(x - halfWidth - fountainBuffer,
                             std::min(fountainCenterX, x + halfWidth + fountainBuffer));
    float closestY = std::max(y - halfDepth - fountainBuffer,
                             std::min(fountainCenterY, y + halfDepth + fountainBuffer));
    
    float dx = closestX - fountainCenterX;
    float dy = closestY - fountainCenterY;
    float distanceSquared = dx * dx + dy * dy;
    float radiusWithBuffer = fountainRadius + fountainBuffer;
    
    return distanceSquared < radiusWithBuffer * radiusWithBuffer;
}

bool BuildingPlacementSystem::collidesWithBuildings(
    float x, float y, float width, float depth,
    const std::vector<Building>& buildings) const
{
    float halfWidth = width / 2.0f;
    float halfDepth = depth / 2.0f;
    const float buildingBuffer = 25.0f;
    
    float buildingLeft = x - halfWidth;
    float buildingRight = x + halfWidth;
    float buildingTop = y - halfDepth;
    float buildingBottom = y + halfDepth;
    
    for (const auto& existing : buildings) {
        float existingHalfWidth = existing.width / 2.0f;
        float existingHalfDepth = existing.depth / 2.0f;
        float existingLeft = existing.x - existingHalfWidth;
        float existingRight = existing.x + existingHalfWidth;
        float existingTop = existing.y - existingHalfDepth;
        float existingBottom = existing.y + existingHalfDepth;
        
        // Check AABB collision with buffer
        if (!(buildingRight + buildingBuffer < existingLeft ||
              buildingLeft - buildingBuffer > existingRight ||
              buildingBottom + buildingBuffer < existingTop ||
              buildingTop - buildingBuffer > existingBottom)) {
            return true;
        }
    }
    
    return false;
}

bool BuildingPlacementSystem::isInsideCircle(
    float px, float py,
    const std::vector<Point>& circlePoints) const
{
    if (circlePoints.empty()) return false;
    
    // Find center and radius
    float centerX = 0, centerY = 0;
    for (const auto& point : circlePoints) {
        centerX += point.x;
        centerY += point.y;
    }
    centerX /= circlePoints.size();
    centerY /= circlePoints.size();
    
    float radius = 0;
    for (const auto& point : circlePoints) {
        float dx = point.x - centerX;
        float dy = point.y - centerY;
        float dist = std::sqrt(dx * dx + dy * dy);
        radius = std::max(radius, dist);
    }
    
    // Check if point is inside
    float dx = px - centerX;
    float dy = py - centerY;
    float distSquared = dx * dx + dy * dy;
    
    return distSquared <= radius * radius;
}
