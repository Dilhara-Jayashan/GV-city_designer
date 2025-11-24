#include "city_config.h"
#include <iostream>

void CityConfig::printConfig() const {
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║      CITY DESIGNER CONFIGURATION       ║\n";
    std::cout << "╠════════════════════════════════════════╣\n";
    std::cout << "║ Buildings:      " << numBuildings << " buildings" << std::string(18 - std::to_string(numBuildings).length(), ' ') << "║\n";
    std::cout << "║ Layout Size:    " << layoutSize << "x" << layoutSize << " grid" << std::string(17 - 2*std::to_string(layoutSize).length(), ' ') << "║\n";
    std::cout << "║ Road Pattern:   " << getRoadPatternString() << std::string(23 - getRoadPatternString().length(), ' ') << "║\n";
    std::cout << "║ Road Width:     " << roadWidth << " pixels" << std::string(17 - std::to_string(roadWidth).length(), ' ') << "║\n";
    std::cout << "║ Skyline Type:   " << getSkylineTypeString() << std::string(23 - getSkylineTypeString().length(), ' ') << "║\n";
    std::cout << "║ Texture Theme:  " << getTextureThemeString() << std::string(23 - getTextureThemeString().length(), ' ') << "║\n";
    std::cout << "║ Parks:          " << numParks << " parks (radius: " << parkRadius << ")" << std::string(8 - std::to_string(numParks).length() - std::to_string(parkRadius).length(), ' ') << "║\n";
    std::cout << "║ Fountains:      radius " << fountainRadius << std::string(15 - std::to_string(fountainRadius).length(), ' ') << "║\n";
    std::cout << "║ View Mode:      " << (view3D ? "3D View" : "2D View") << std::string(23 - (view3D ? 7 : 7), ' ') << "║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";
}
