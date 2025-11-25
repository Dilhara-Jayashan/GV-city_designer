/**
 * @file building_lighting_system.cpp
 * @brief Implementation of Building Window Lighting Feature
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include "features/building_lights/building_lighting_system.h"
#include <algorithm>

BuildingLightingSystem::BuildingLightingSystem()
    : warmYellowColor(3.0f, 2.5f, 1.5f)  // Warm yellow glow for windows
    , sunriseStart(4.0f)
    , sunriseEnd(6.0f)
    , sunsetStart(18.0f)
    , sunsetEnd(20.0f)
{
}

glm::vec3 BuildingLightingSystem::getWindowEmissionColor(float timeOfDay) const {
    float intensity = getLightIntensity(timeOfDay);
    return warmYellowColor * intensity;
}

float BuildingLightingSystem::getLightIntensity(float timeOfDay) const {
    // Night hours (8pm - 4am): full brightness
    if (timeOfDay >= sunsetEnd || timeOfDay < sunriseStart) {
        return 1.0f;
    }
    
    // Day hours (6am - 6pm): lights off
    if (timeOfDay >= sunriseEnd && timeOfDay < sunsetStart) {
        return 0.0f;
    }
    
    // Sunrise transition (4am - 6am): fade out lights
    if (timeOfDay >= sunriseStart && timeOfDay < sunriseEnd) {
        float progress = (timeOfDay - sunriseStart) / (sunriseEnd - sunriseStart);
        return 1.0f - progress;  // Fade from 1.0 to 0.0
    }
    
    // Sunset transition (6pm - 8pm): fade in lights
    if (timeOfDay >= sunsetStart && timeOfDay < sunsetEnd) {
        float progress = (timeOfDay - sunsetStart) / (sunsetEnd - sunsetStart);
        return progress;  // Fade from 0.0 to 1.0
    }
    
    return 0.0f;
}

bool BuildingLightingSystem::areLightsActive(float timeOfDay) const {
    // Lights are active from sunset start to sunrise end
    return (timeOfDay >= sunsetStart) || (timeOfDay < sunriseEnd);
}
