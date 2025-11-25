/**
 * @file city_serializer.h
 * @brief Feature 5: Save/Load System
 * 
 * Handles JSON serialization and deserialization of city data.
 * Saves/loads buildings, roads, parks, and fountain to/from disk.
 * 
 * Provides JSON serialization and deserialization for CityData.
 * Allows saving cities to files and loading them back.
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef CITY_SERIALIZER_H
#define CITY_SERIALIZER_H

#include <string>
#include "generation/city_generator.h"

class CitySerializer {
public:
    /**
     * @brief Save city data to a JSON file
     * @param city The city data to save
     * @param filename Name of the file (without path or extension)
     * @return true if save was successful, false otherwise
     */
    static bool saveCity(const CityData& city, const std::string& filename);
    
    /**
     * @brief Load city data from a JSON file
     * @param city Reference to city data object to populate
     * @param filename Name of the file to load (without path or extension)
     * @return true if load was successful, false otherwise
     */
    static bool loadCity(CityData& city, const std::string& filename);
    
    /**
     * @brief Get the default save directory path
     * @return Path to saves directory
     */
    static std::string getSaveDirectory();
    
private:
    /**
     * @brief Convert building type enum to string
     */
    static std::string buildingTypeToString(BuildingType type);
    
    /**
     * @brief Convert string to building type enum
     */
    static BuildingType stringToBuildingType(const std::string& str);
    
    /**
     * @brief Escape special characters for JSON strings
     */
    static std::string escapeJson(const std::string& str);
};

#endif // CITY_SERIALIZER_H
