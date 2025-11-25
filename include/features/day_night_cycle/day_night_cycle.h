/**
 * @file day_night_cycle.h
 * @brief Feature 2: Day/Night Cycle System
 * 
 * Manages time progression and calculates time-dependent visual effects:
 * - Sky color transitions (sunrise, day, sunset, night)
 * - Ambient lighting factors
 * - Time of day progression
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef DAY_NIGHT_CYCLE_H
#define DAY_NIGHT_CYCLE_H

#include <glm/glm.hpp>

/**
 * @class DayNightCycle
 * @brief Manages time progression and day/night visual effects
 * 
 * This feature handles:
 * - Automatic time progression
 * - Sky color calculation based on time
 * - Ambient light factor for shader lighting
 * - Smooth color transitions between time periods
 */
class DayNightCycle {
public:
    /**
     * @brief Construct a new Day/Night Cycle system
     * @param startTime Initial time of day in hours (0-24)
     * @param autoProgress Whether time advances automatically
     */
    DayNightCycle(float startTime = 14.0f, bool autoProgress = true);
    
    /**
     * @brief Update time progression
     * @param deltaTime Time elapsed since last update in seconds
     * 
     * If auto-progress is enabled, advances timeOfDay.
     * Time wraps around from 24.0 back to 0.0.
     */
    void update(float deltaTime);
    
    /**
     * @brief Get current time of day
     * @return Time in hours (0-24)
     */
    float getTimeOfDay() const { return timeOfDay; }
    
    /**
     * @brief Set current time of day
     * @param time Time in hours (0-24)
     */
    void setTimeOfDay(float time);
    
    /**
     * @brief Get sky color for current time (3D view)
     * @return RGB color for sky background
     * 
     * Returns beautiful pastel colors that transition through:
     * - Night: Soft dark lavender (9pm-5am)
     * - Sunrise: Lavender → Peachy → Light blue (5am-9am)
     * - Day: Light sky blue (9am-5pm)
     * - Sunset: Light blue → Warm peach → Lavender (5pm-9pm)
     */
    glm::vec3 getSkyColor() const;
    
    /**
     * @brief Get sky color for 2D view (static)
     * @return Dark gray color for 2D mode
     */
    glm::vec3 getSkyColor2D() const;
    
    /**
     * @brief Get ambient light factor for shader
     * @return Multiplier for ambient lighting (0.3 at night, 1.0 in day)
     * 
     * This affects how bright objects appear:
     * - Day: 1.0 (full brightness)
     * - Night: 0.3 (dimmed for realism)
     * - Smooth transitions at twilight
     */
    float getAmbientLightFactor() const;
    
    /**
     * @brief Toggle automatic time progression
     */
    void toggleAutoProgress() { autoTimeProgress = !autoTimeProgress; }
    
    /**
     * @brief Check if auto-progress is enabled
     * @return true if time advances automatically
     */
    bool isAutoProgressEnabled() const { return autoTimeProgress; }
    
private:
    float timeOfDay;            ///< Current time in hours (0-24)
    bool autoTimeProgress;      ///< Whether time advances automatically
    float timeSpeed;            ///< Speed of time progression (hours per real second)
    
    /**
     * @brief Calculate ambient factor based on time
     * @param time Time of day in hours
     * @return Ambient light multiplier
     */
    float calculateAmbientFactor(float time) const;
};

#endif // DAY_NIGHT_CYCLE_H
