/**
 * @file day_night_cycle.cpp
 * @brief Implementation of Day/Night Cycle Feature
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include "features/day_night_cycle/day_night_cycle.h"
#include <algorithm>

DayNightCycle::DayNightCycle(float startTime, bool autoProgress)
    : timeOfDay(startTime)
    , autoTimeProgress(autoProgress)
    , timeSpeed(0.5f)  // 0.5 hours per real second (slower, more visible)
{
}

void DayNightCycle::update(float deltaTime) {
    if (autoTimeProgress) {
        timeOfDay += deltaTime * timeSpeed;
        if (timeOfDay >= 24.0f) {
            timeOfDay -= 24.0f;
        }
    }
}

void DayNightCycle::setTimeOfDay(float time) {
    timeOfDay = time;
    // Clamp to valid range
    while (timeOfDay < 0.0f) timeOfDay += 24.0f;
    while (timeOfDay >= 24.0f) timeOfDay -= 24.0f;
}

glm::vec3 DayNightCycle::getSkyColor() const {
    float t = timeOfDay;
    
    if (t >= 5.0f && t < 7.0f) {
        // Sunrise (5-7am): Soft lavender to peachy
        float blend = (t - 5.0f) / 2.0f;
        return glm::mix(glm::vec3(0.6f, 0.65f, 0.75f), glm::vec3(0.85f, 0.7f, 0.6f), blend);
    } else if (t >= 7.0f && t < 9.0f) {
        // Morning (7-9am): Peachy to light blue
        float blend = (t - 7.0f) / 2.0f;
        return glm::mix(glm::vec3(0.85f, 0.7f, 0.6f), glm::vec3(0.7f, 0.8f, 0.9f), blend);
    } else if (t >= 9.0f && t < 17.0f) {
        // Day (9am-5pm): Light sky blue (pastel)
        return glm::vec3(0.7f, 0.8f, 0.9f);
    } else if (t >= 17.0f && t < 19.0f) {
        // Sunset (5-7pm): Light blue to warm peach
        float blend = (t - 17.0f) / 2.0f;
        return glm::mix(glm::vec3(0.7f, 0.8f, 0.9f), glm::vec3(0.9f, 0.65f, 0.5f), blend);
    } else if (t >= 19.0f && t < 21.0f) {
        // Dusk (7-9pm): Warm peach to soft lavender
        float blend = (t - 19.0f) / 2.0f;
        return glm::mix(glm::vec3(0.9f, 0.65f, 0.5f), glm::vec3(0.6f, 0.65f, 0.75f), blend);
    } else {
        // Night (9pm-5am): Soft dark lavender
        return glm::vec3(0.3f, 0.35f, 0.5f);
    }
}

glm::vec3 DayNightCycle::getSkyColor2D() const {
    // Static dark background for 2D mode
    return glm::vec3(0.15f, 0.15f, 0.15f);
}

float DayNightCycle::getAmbientLightFactor() const {
    return calculateAmbientFactor(timeOfDay);
}

float DayNightCycle::calculateAmbientFactor(float time) const {
    // Day hours (7am - 7pm): Full brightness
    if (time >= 7.0f && time < 19.0f) {
        return 1.0f;
    }
    
    // Deep night (10pm - 4am): Dim lighting
    if (time >= 22.0f || time < 4.0f) {
        return 0.3f;
    }
    
    // Dawn transition (4am - 7am): Fade from dim to full
    if (time >= 4.0f && time < 7.0f) {
        float progress = (time - 4.0f) / 3.0f;
        return 0.3f + (0.7f * progress);  // 0.3 → 1.0
    }
    
    // Dusk transition (7pm - 10pm): Fade from full to dim
    if (time >= 19.0f && time < 22.0f) {
        float progress = (time - 19.0f) / 3.0f;
        return 1.0f - (0.7f * progress);  // 1.0 → 0.3
    }
    
    return 1.0f;
}
