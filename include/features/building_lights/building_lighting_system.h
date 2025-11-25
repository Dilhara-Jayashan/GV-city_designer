/**
 * @file building_lighting_system.h
 * @brief Feature 1: Window Lighting System
 * 
 * Manages building window lights that turn on at night and off during day.
 * Handles light intensity calculations based on time of day.
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef BUILDING_LIGHTING_SYSTEM_H
#define BUILDING_LIGHTING_SYSTEM_H

#include <glm/glm.hpp>

/**
 * @class BuildingLightingSystem
 * @brief Manages window lighting for buildings based on time of day
 * 
 * This feature controls when building windows are lit:
 * - Day (6am-6pm): Windows off
 * - Night (6pm-6am): Windows on with warm yellow glow
 * - Smooth transitions at sunrise/sunset
 */
class BuildingLightingSystem {
public:
    /**
     * @brief Construct the lighting system
     */
    BuildingLightingSystem();
    
    /**
     * @brief Calculate window light emission color for current time
     * @param timeOfDay Current time in hours (0-24)
     * @return RGB color multiplier for window glow (0=off, >0=lit)
     * 
     * Returns warm yellow color during night hours, fades to black during day.
     * Smooth transitions occur at sunrise (4-6am) and sunset (18-20pm).
     */
    glm::vec3 getWindowEmissionColor(float timeOfDay) const;
    
    /**
     * @brief Get intensity multiplier for window lights
     * @param timeOfDay Current time in hours (0-24)
     * @return Intensity factor (0.0 = off, 1.0 = full brightness)
     * 
     * Controls the brightness of window lights:
     * - 0.0 during day (6am-6pm)
     * - 1.0 during night (8pm-4am)
     * - Smooth fade during twilight hours
     */
    float getLightIntensity(float timeOfDay) const;
    
    /**
     * @brief Check if lights should be on at given time
     * @param timeOfDay Current time in hours (0-24)
     * @return true if lights are active (even if fading)
     */
    bool areLightsActive(float timeOfDay) const;
    
private:
    glm::vec3 warmYellowColor;  ///< Warm yellow color for window glow
    float sunriseStart;         ///< Hour when sunrise begins (4.0)
    float sunriseEnd;           ///< Hour when sunrise ends (6.0)
    float sunsetStart;          ///< Hour when sunset begins (18.0)
    float sunsetEnd;            ///< Hour when sunset ends (20.0)
};

#endif // BUILDING_LIGHTING_SYSTEM_H
