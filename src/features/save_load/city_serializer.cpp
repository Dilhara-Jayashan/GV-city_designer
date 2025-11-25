/**
 * @file city_serializer.cpp
 * @brief Implementation of Save/Load System Feature
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include "features/save_load/city_serializer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

std::string CitySerializer::getSaveDirectory() {
    return "saves/";
}

std::string CitySerializer::buildingTypeToString(BuildingType type) {
    switch (type) {
        case BuildingType::LOW_RISE: return "LOW_RISE";
        case BuildingType::MID_RISE: return "MID_RISE";
        case BuildingType::HIGH_RISE: return "HIGH_RISE";
        default: return "LOW_RISE";
    }
}

BuildingType CitySerializer::stringToBuildingType(const std::string& str) {
    if (str == "LOW_RISE") return BuildingType::LOW_RISE;
    if (str == "MID_RISE") return BuildingType::MID_RISE;
    if (str == "HIGH_RISE") return BuildingType::HIGH_RISE;
    return BuildingType::LOW_RISE;
}

std::string CitySerializer::escapeJson(const std::string& str) {
    std::string escaped;
    for (char c : str) {
        if (c == '"' || c == '\\') escaped += '\\';
        escaped += c;
    }
    return escaped;
}

bool CitySerializer::saveCity(const CityData& city, const std::string& filename) {
    if (!city.isGenerated) {
        std::cout << "❌ Cannot save: No city generated yet!\n";
        return false;
    }
    
    // Create saves directory if it doesn't exist
    std::string saveDir = getSaveDirectory();
    mkdir(saveDir.c_str(), 0755);
    
    std::string filepath = saveDir + filename + ".json";
    std::ofstream file(filepath);
    
    if (!file.is_open()) {
        std::cout << "❌ Failed to open file for writing: " << filepath << "\n";
        return false;
    }
    
    std::cout << "\n💾 Saving city to " << filepath << "...\n";
    
    // Write JSON manually (simple format)
    file << "{\n";
    file << "  \"version\": \"1.0\",\n";
    file << "  \"timestamp\": \"" << time(nullptr) << "\",\n";
    
    // Save buildings
    file << "  \"buildings\": [\n";
    for (size_t i = 0; i < city.buildings.size(); i++) {
        const Building& b = city.buildings[i];
        file << "    {\n";
        file << "      \"x\": " << b.x << ",\n";
        file << "      \"y\": " << b.y << ",\n";
        file << "      \"width\": " << b.width << ",\n";
        file << "      \"depth\": " << b.depth << ",\n";
        file << "      \"height\": " << b.height << ",\n";
        file << "      \"type\": \"" << buildingTypeToString(b.type) << "\"\n";
        file << "    }" << (i < city.buildings.size() - 1 ? "," : "") << "\n";
    }
    file << "  ],\n";
    
    // Save roads
    file << "  \"roads\": [\n";
    for (size_t i = 0; i < city.roads.size(); i++) {
        const Road& r = city.roads[i];
        file << "    {\n";
        file << "      \"width\": " << r.width << ",\n";
        file << "      \"points\": [\n";
        for (size_t j = 0; j < r.points.size(); j++) {
            const Point& p = r.points[j];
            file << "        {\"x\": " << p.x << ", \"y\": " << p.y << "}"
                 << (j < r.points.size() - 1 ? "," : "") << "\n";
        }
        file << "      ]\n";
        file << "    }" << (i < city.roads.size() - 1 ? "," : "") << "\n";
    }
    file << "  ],\n";
    
    // Save parks
    file << "  \"parks\": [\n";
    for (size_t i = 0; i < city.parks.size(); i++) {
        const auto& park = city.parks[i];
        file << "    [\n";
        for (size_t j = 0; j < park.size(); j++) {
            const Point& p = park[j];
            file << "      {\"x\": " << p.x << ", \"y\": " << p.y << "}"
                 << (j < park.size() - 1 ? "," : "") << "\n";
        }
        file << "    ]" << (i < city.parks.size() - 1 ? "," : "") << "\n";
    }
    file << "  ],\n";
    
    // Save fountain
    file << "  \"fountain\": [\n";
    for (size_t i = 0; i < city.fountain.size(); i++) {
        const Point& p = city.fountain[i];
        file << "    {\"x\": " << p.x << ", \"y\": " << p.y << "}"
             << (i < city.fountain.size() - 1 ? "," : "") << "\n";
    }
    file << "  ]\n";
    
    file << "}\n";
    
    file.close();
    
    std::cout << "✅ City saved successfully!\n";
    std::cout << "   - " << city.buildings.size() << " buildings\n";
    std::cout << "   - " << city.roads.size() << " roads (";
    int totalRoadPoints = 0;
    for (const auto& road : city.roads) {
        totalRoadPoints += road.points.size();
    }
    std::cout << totalRoadPoints << " total points)\n";
    std::cout << "   - " << city.parks.size() << " parks\n";
    std::cout << "   - " << city.fountain.size() << " fountain points\n";
    std::cout << "   - File: " << filepath << "\n\n";
    
    return true;
}

bool CitySerializer::loadCity(CityData& city, const std::string& filename) {
    std::string filepath = getSaveDirectory() + filename + ".json";
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        std::cout << "❌ Failed to open file for reading: " << filepath << "\n";
        return false;
    }
    
    std::cout << "\n📂 Loading city from " << filepath << "...\n";
    
    city.clear();
    
    std::string line;
    std::string section = "";
    Building currentBuilding(0, 0, 0, 0, 0, BuildingType::LOW_RISE);
    Road currentRoad;
    std::vector<Point> currentPark;
    
    int braceDepth = 0;
    bool inBuilding = false;
    bool inRoad = false;
    bool inPark = false;
    bool inFountain = false;
    bool inPoints = false;
    
    while (std::getline(file, line)) {
        // Track brace depth
        for (char c : line) {
            if (c == '{') braceDepth++;
            if (c == '}') braceDepth--;
        }
        
        // Detect sections
        if (line.find("\"buildings\"") != std::string::npos) {
            section = "buildings";
        } else if (line.find("\"roads\"") != std::string::npos) {
            section = "roads";
        } else if (line.find("\"parks\"") != std::string::npos) {
            section = "parks";
        } else if (line.find("\"fountain\"") != std::string::npos) {
            section = "fountain";
        }
        
        // Parse buildings
        if (section == "buildings") {
            if (line.find("\"x\":") != std::string::npos) {
                size_t pos = line.find(":") + 1;
                currentBuilding.x = std::stof(line.substr(pos));
            } else if (line.find("\"y\":") != std::string::npos) {
                size_t pos = line.find(":") + 1;
                currentBuilding.y = std::stof(line.substr(pos));
            } else if (line.find("\"width\":") != std::string::npos) {
                size_t pos = line.find(":") + 1;
                currentBuilding.width = std::stof(line.substr(pos));
            } else if (line.find("\"depth\":") != std::string::npos) {
                size_t pos = line.find(":") + 1;
                currentBuilding.depth = std::stof(line.substr(pos));
            } else if (line.find("\"height\":") != std::string::npos) {
                size_t pos = line.find(":") + 1;
                currentBuilding.height = std::stof(line.substr(pos));
            } else if (line.find("\"type\":") != std::string::npos) {
                size_t start = line.find("\"", line.find(":")) + 1;
                size_t end = line.find("\"", start);
                std::string typeStr = line.substr(start, end - start);
                currentBuilding.type = stringToBuildingType(typeStr);
                city.buildings.push_back(currentBuilding);
            }
        }
        
        // Parse roads
        if (section == "roads") {
            if (line.find("\"width\":") != std::string::npos && !inPoints) {
                size_t pos = line.find(":") + 1;
                currentRoad.width = std::stoi(line.substr(pos));
            } else if (line.find("\"points\":") != std::string::npos) {
                inPoints = true;
                currentRoad.points.clear();
            } else if (inPoints && line.find("\"x\":") != std::string::npos) {
                Point p;
                size_t xPos = line.find("\"x\":") + 4;
                size_t xEnd = line.find(",", xPos);
                p.x = std::stoi(line.substr(xPos, xEnd - xPos));
                
                size_t yPos = line.find("\"y\":") + 4;
                size_t yEnd = line.find("}", yPos);
                p.y = std::stoi(line.substr(yPos, yEnd - yPos));
                
                currentRoad.points.push_back(p);
            } else if (inPoints && line.find("]") != std::string::npos) {
                inPoints = false;
                if (!currentRoad.points.empty()) {
                    city.roads.push_back(currentRoad);
                }
            }
        }
        
        // Parse parks
        if (section == "parks") {
            if (line.find("[") != std::string::npos && line.find("\"parks\"") == std::string::npos && section == "parks") {
                currentPark.clear();
            } else if (line.find("\"x\":") != std::string::npos && section == "parks") {
                Point p;
                size_t xPos = line.find("\"x\":") + 4;
                size_t xEnd = line.find(",", xPos);
                p.x = std::stoi(line.substr(xPos, xEnd - xPos));
                
                size_t yPos = line.find("\"y\":") + 4;
                size_t yEnd = line.find("}", yPos);
                p.y = std::stoi(line.substr(yPos, yEnd - yPos));
                
                currentPark.push_back(p);
            } else if (line.find("]") != std::string::npos && !currentPark.empty() && section == "parks") {
                city.parks.push_back(currentPark);
                currentPark.clear();
            }
        }
        
        // Parse fountain
        if (section == "fountain" && line.find("\"x\":") != std::string::npos) {
            Point p;
            size_t xPos = line.find("\"x\":") + 4;
            size_t xEnd = line.find(",", xPos);
            p.x = std::stoi(line.substr(xPos, xEnd - xPos));
            
            size_t yPos = line.find("\"y\":") + 4;
            size_t yEnd = line.find("}", yPos);
            p.y = std::stoi(line.substr(yPos, yEnd - yPos));
            
            city.fountain.push_back(p);
        }
    }
    
    file.close();
    
    city.isGenerated = true;
    
    std::cout << "✅ City loaded successfully!\n";
    std::cout << "   - " << city.buildings.size() << " buildings\n";
    std::cout << "   - " << city.roads.size() << " roads (";
    int totalRoadPoints = 0;
    for (const auto& road : city.roads) {
        totalRoadPoints += road.points.size();
    }
    std::cout << totalRoadPoints << " total points)\n";
    std::cout << "   - " << city.parks.size() << " parks\n";
    std::cout << "   - " << city.fountain.size() << " fountain points\n\n";
    
    return true;
}
