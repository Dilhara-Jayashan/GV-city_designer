/**
 * @file city_renderer.cpp
 * @brief Implementation of City Rendering System
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include "rendering/city_renderer.h"
#include "rendering/mesh/building_mesh.h"
#include "rendering/mesh/road_mesh.h"
#include "rendering/mesh/park_mesh.h"
#include "rendering/mesh/traffic_mesh.h"
#include "rendering/mesh/mesh_utils.h"
#include "features/traffic_system/traffic_generator.h"

// Constructor
CityRenderer::CityRenderer(int screenWidth, int screenHeight)
    : screenWidth(screenWidth)
    , screenHeight(screenHeight)
    , fountain3DVAO(0)
    , fountain3DVBO(0)
    , fountain3DVertexCount(0)
    , fountainLights3DVAO(0)
    , fountainLights3DVBO(0)
    , fountainLights3DVertexCount(0)
{
}

// Destructor
CityRenderer::~CityRenderer() {
    cleanup();
}

// Cleanup all buffers
void CityRenderer::cleanup() {
    // Cleanup 2D buffers
    if (!VAOs.empty()) {
        glDeleteVertexArrays(VAOs.size(), VAOs.data());
        glDeleteBuffers(VBOs.size(), VBOs.data());
        VAOs.clear();
        VBOs.clear();
        vertexCounts.clear();
    }
    
    // Cleanup 3D road buffers
    if (!road3DVAOs.empty()) {
        glDeleteVertexArrays(road3DVAOs.size(), road3DVAOs.data());
        glDeleteBuffers(road3DVBOs.size(), road3DVBOs.data());
        road3DVAOs.clear();
        road3DVBOs.clear();
        road3DVertexCounts.clear();
    }
    
    // Cleanup 3D park buffers
    if (!park3DVAOs.empty()) {
        glDeleteVertexArrays(park3DVAOs.size(), park3DVAOs.data());
        glDeleteBuffers(park3DVBOs.size(), park3DVBOs.data());
        park3DVAOs.clear();
        park3DVBOs.clear();
        park3DVertexCounts.clear();
    }
    
    // Cleanup 3D fountain buffer
    if (fountain3DVAO != 0) {
        glDeleteVertexArrays(1, &fountain3DVAO);
        glDeleteBuffers(1, &fountain3DVBO);
        fountain3DVAO = 0;
        fountain3DVBO = 0;
        fountain3DVertexCount = 0;
    }
    
    if (fountainLights3DVAO != 0) {
        glDeleteVertexArrays(1, &fountainLights3DVAO);
        glDeleteBuffers(1, &fountainLights3DVBO);
        fountainLights3DVAO = 0;
        fountainLights3DVBO = 0;
        fountainLights3DVertexCount = 0;
    }
    
    // Cleanup traffic buffers
    if (!traffic3DVAOs.empty()) {
        glDeleteVertexArrays(traffic3DVAOs.size(), traffic3DVAOs.data());
        glDeleteBuffers(traffic3DVBOs.size(), traffic3DVBOs.data());
        traffic3DVAOs.clear();
        traffic3DVBOs.clear();
        traffic3DVertexCounts.clear();
    }
    if (!trafficVAOs.empty()) {
        glDeleteVertexArrays(trafficVAOs.size(), trafficVAOs.data());
        glDeleteBuffers(trafficVBOs.size(), trafficVBOs.data());
        trafficVAOs.clear();
        trafficVBOs.clear();
        trafficVertexCounts.clear();
    }
}

// Create buffer for mesh
std::pair<GLuint, GLuint> CityRenderer::createBuffer(const std::vector<float>& vertices, bool hasTexCoords) {
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), 
                vertices.data(), GL_STATIC_DRAW);
    
    if (hasTexCoords) {
        // Position attribute (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Texture coordinate attribute (location = 1)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 
                             (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    } else {
        // Position only
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
    
    return {VAO, VBO};
}

// Update city rendering data
void CityRenderer::updateCity(const CityData& city, bool view3D) {
    // Cleanup old buffers
    cleanup();
    
    // Create buffers for roads (2D points)
    for (const auto& road : city.roads) {
        auto vertices = pointsToVertices(road.points, screenWidth, screenHeight);
        auto [vao, vbo] = createBuffer(vertices, false);
        VAOs.push_back(vao);
        VBOs.push_back(vbo);
        vertexCounts.push_back(vertices.size() / 3);
    }
    
    // Create 3D textured road meshes
    for (const auto& road : city.roads) {
        auto vertices = roadTo3DMesh(road, screenWidth, screenHeight, view3D);
        if (!vertices.empty()) {
            auto [vao, vbo] = createBuffer(vertices, true);
            road3DVAOs.push_back(vao);
            road3DVBOs.push_back(vbo);
            road3DVertexCounts.push_back(vertices.size() / 5);
        }
    }
    
    // Create buffers for parks (2D points)
    for (const auto& park : city.parks) {
        auto vertices = pointsToVertices(park, screenWidth, screenHeight);
        auto [vao, vbo] = createBuffer(vertices, false);
        VAOs.push_back(vao);
        VBOs.push_back(vbo);
        vertexCounts.push_back(vertices.size() / 3);
    }
    
    // Create 3D textured park meshes
    for (const auto& park : city.parks) {
        auto vertices = parkTo3DMesh(park, screenWidth, screenHeight, view3D);
        if (!vertices.empty()) {
            auto [vao, vbo] = createBuffer(vertices, true);
            park3DVAOs.push_back(vao);
            park3DVBOs.push_back(vbo);
            park3DVertexCounts.push_back(vertices.size() / 5);
        }
    }
    
    // Create buffer for fountain (2D points)
    if (!city.fountain.empty()) {
        auto vertices = pointsToVertices(city.fountain, screenWidth, screenHeight);
        auto [vao, vbo] = createBuffer(vertices, false);
        VAOs.push_back(vao);
        VBOs.push_back(vbo);
        vertexCounts.push_back(vertices.size() / 3);
        
        // Create 3D textured fountain mesh
        auto vertices3D = fountainTo3DMesh(city.fountain, screenWidth, screenHeight, view3D);
        if (!vertices3D.empty()) {
            auto [vao3d, vbo3d] = createBuffer(vertices3D, true);
            fountain3DVAO = vao3d;
            fountain3DVBO = vbo3d;
            fountain3DVertexCount = vertices3D.size() / 5;
        }
        
        // Create 3D fountain lights mesh (light bulbs)
        auto lightVertices = fountainLightsTo3DMesh(city.fountain, screenWidth, screenHeight);
        if (!lightVertices.empty()) {
            auto [lightVao, lightVbo] = createBuffer(lightVertices, true);
            fountainLights3DVAO = lightVao;
            fountainLights3DVBO = lightVbo;
            fountainLights3DVertexCount = lightVertices.size() / 5;
        }
    }
    
    // Create buffers for buildings
    for (const auto& building : city.buildings) {
        auto vertices = buildingToVertices(building, screenWidth, screenHeight, view3D);
        auto [vao, vbo] = createBuffer(vertices, true);
        VAOs.push_back(vao);
        VBOs.push_back(vbo);
        vertexCounts.push_back(vertices.size() / 5);
    }
}

// Render roads
void CityRenderer::renderRoads(const CityData& city, bool view3D, ShaderManager& shaderManager,
                                GLuint roadTexture, size_t roadCount) {
    if (view3D) {
        // In 3D mode: Draw textured road meshes
        shaderManager.setIs2D(false);
        shaderManager.setUseTexture(true);
        glBindTexture(GL_TEXTURE_2D, roadTexture);
        
        for (size_t i = 0; i < road3DVAOs.size(); i++) {
            glBindVertexArray(road3DVAOs[i]);
            glDrawArrays(GL_TRIANGLES, 0, road3DVertexCounts[i]);
        }
        
        shaderManager.setUseTexture(false);
    } else {
        // In 2D mode: Draw roads as bright yellow points
        shaderManager.setIs2D(true);
        shaderManager.setColor(1.0f, 1.0f, 0.0f);  // Bright yellow
        glPointSize(2.0f);
        
        for (size_t i = 0; i < roadCount && i < VAOs.size(); i++) {
            glBindVertexArray(VAOs[i]);
            glDrawArrays(GL_POINTS, 0, vertexCounts[i]);
        }
    }
}

// Render parks
void CityRenderer::renderParks(const CityData& city, bool view3D, ShaderManager& shaderManager,
                                GLuint grassTexture, size_t roadCount, size_t parkCount) {
    if (view3D) {
        // In 3D mode: Draw textured grass-filled park meshes
        shaderManager.setIs2D(false);
        shaderManager.setUseTexture(true);
        
        if (grassTexture != 0) {
            glBindTexture(GL_TEXTURE_2D, grassTexture);
        } else {
            // Fallback: use color if texture not loaded
            shaderManager.setUseTexture(false);
            shaderManager.setColor(0.2f, 0.8f, 0.3f);
        }
        
        for (size_t i = 0; i < park3DVAOs.size(); i++) {
            glBindVertexArray(park3DVAOs[i]);
            glDrawArrays(GL_TRIANGLES, 0, park3DVertexCounts[i]);
        }
        
        shaderManager.setUseTexture(false);
    } else {
        // In 2D mode: Draw parks as bright green points
        shaderManager.setIs2D(true);
        shaderManager.setColor(0.0f, 1.0f, 0.0f);  // Bright lime green
        
        size_t fountainOffset = roadCount + parkCount;
        for (size_t i = roadCount; i < fountainOffset && i < VAOs.size(); i++) {
            glBindVertexArray(VAOs[i]);
            glDrawArrays(GL_POINTS, 0, vertexCounts[i]);
        }
    }
}

// Render fountain
void CityRenderer::renderFountain(const CityData& city, bool view3D, ShaderManager& shaderManager,
                                   GLuint fountainTexture, size_t fountainOffset, size_t fountainCount, float timeOfDay) {
    if (view3D) {
        // In 3D mode: Draw textured fountain mesh with nighttime lighting
        shaderManager.setIs2D(false);
        
        // Calculate if it's nighttime (18:00-6:00)
        bool isNight = (timeOfDay >= 18.0f || timeOfDay < 6.0f);
        
        // Calculate glow intensity based on time
        float glowIntensity = 0.0f;
        if (isNight) {
            // Smooth transition during sunset (18:00-20:00) and sunrise (4:00-6:00)
            if (timeOfDay >= 18.0f && timeOfDay < 20.0f) {
                // Sunset: fade in
                glowIntensity = (timeOfDay - 18.0f) / 2.0f;
            } else if (timeOfDay >= 20.0f || timeOfDay < 4.0f) {
                // Full night: full glow
                glowIntensity = 1.0f;
            } else if (timeOfDay >= 4.0f && timeOfDay < 6.0f) {
                // Sunrise: fade out
                glowIntensity = 1.0f - ((timeOfDay - 4.0f) / 2.0f);
            }
        }
        
        if (fountain3DVertexCount > 0) {
            // Always render the fountain structure with texture
            shaderManager.setUseTexture(true);
            if (fountainTexture != 0) {
                glBindTexture(GL_TEXTURE_2D, fountainTexture);
            }
            glBindVertexArray(fountain3DVAO);
            glDrawArrays(GL_TRIANGLES, 0, fountain3DVertexCount);
            
            // Render light bulbs at night
            if (glowIntensity > 0.0f && fountainLights3DVertexCount > 0) {
                // Enable additive blending for glowing effect
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // Additive blending
                glDepthMask(GL_FALSE);  // Don't write to depth buffer
                
                shaderManager.setUseTexture(false);
                
                // Warm white color for lights (like LED bulbs)
                // Compensate for night ambient dimming by making it VERY bright
                float warmR = 4.0f * glowIntensity;  // Bright warm white red
                float warmG = 3.5f * glowIntensity;  // Slightly less green for warmth
                float warmB = 2.5f * glowIntensity;  // Even less blue for warm tone
                shaderManager.setColor(warmR, warmG, warmB);
                
                // Render the light bulb spheres
                glBindVertexArray(fountainLights3DVAO);
                glDrawArrays(GL_TRIANGLES, 0, fountainLights3DVertexCount);
                
                glDepthMask(GL_TRUE);  // Re-enable depth writes
                glDisable(GL_BLEND);
            }
        }
        
        shaderManager.setUseTexture(false);
    } else {
        // In 2D mode: Draw fountain as bright cyan points
        shaderManager.setIs2D(true);
        if (fountainCount > 0 && fountainOffset < VAOs.size()) {
            shaderManager.setColor(0.0f, 1.0f, 1.0f);  // Bright cyan
            glBindVertexArray(VAOs[fountainOffset]);
            glDrawArrays(GL_POINTS, 0, vertexCounts[fountainOffset]);
        }
    }
}

// Render buildings
void CityRenderer::renderBuildings(const CityData& city, const CityConfig& config, bool view3D, ShaderManager& shaderManager,
                                    GLuint brickTexture, GLuint concreteTexture, GLuint glassTexture,
                                    size_t buildingStart) {
    shaderManager.setIs2D(false);
    
    if (view3D) {
        // Use textures in 3D mode based on texture theme
        shaderManager.setUseTexture(true);
        shaderManager.setShowWindowLights(true);  // Enable window lights in 3D
        
        for (size_t i = buildingStart; i < VAOs.size(); i++) {
            size_t buildingIndex = i - buildingStart;
            if (buildingIndex < city.buildings.size()) {
                const Building& building = city.buildings[buildingIndex];
                
                // Select texture based on BOTH building type AND texture theme
                GLuint selectedTexture;
                
                switch (config.textureTheme) {
                    case TextureTheme::MODERN:
                        // Modern: Glass dominant, some concrete
                        switch (building.type) {
                            case BuildingType::LOW_RISE:
                                selectedTexture = brickTexture;
                                break;
                            case BuildingType::MID_RISE:
                                selectedTexture = concreteTexture;
                                break;
                            case BuildingType::HIGH_RISE:
                                selectedTexture = glassTexture;
                                break;
                        }
                        break;
                        
                    case TextureTheme::CLASSIC:
                        // Classic: Brick dominant, traditional materials
                        switch (building.type) {
                            case BuildingType::LOW_RISE:
                                selectedTexture = brickTexture;
                                break;
                            case BuildingType::MID_RISE:
                                selectedTexture = brickTexture;  // More brick!
                                break;
                            case BuildingType::HIGH_RISE:
                                selectedTexture = concreteTexture;  // Less glass
                                break;
                        }
                        break;
                        
                    case TextureTheme::INDUSTRIAL:
                        // Industrial: Concrete/metal dominant
                        switch (building.type) {
                            case BuildingType::LOW_RISE:
                                selectedTexture = concreteTexture;  // Industrial materials
                                break;
                            case BuildingType::MID_RISE:
                                selectedTexture = concreteTexture;
                                break;
                            case BuildingType::HIGH_RISE:
                                selectedTexture = concreteTexture;  // Minimal glass
                                break;
                        }
                        break;
                        
                    case TextureTheme::FUTURISTIC:
                        // Futuristic: Glass everywhere
                        switch (building.type) {
                            case BuildingType::LOW_RISE:
                                selectedTexture = glassTexture;  // Even low buildings are glass
                                break;
                            case BuildingType::MID_RISE:
                                selectedTexture = glassTexture;
                                break;
                            case BuildingType::HIGH_RISE:
                                selectedTexture = glassTexture;
                                break;
                        }
                        break;
                }
                
                glBindTexture(GL_TEXTURE_2D, selectedTexture);
                glBindVertexArray(VAOs[i]);
                glDrawArrays(GL_TRIANGLES, 0, vertexCounts[i]);
            }
        }
    } else {
        // Use colors in 2D mode
        shaderManager.setUseTexture(false);
        shaderManager.setShowWindowLights(false);  // No window lights in 2D
        
        for (size_t i = buildingStart; i < VAOs.size(); i++) {
            size_t buildingIndex = i - buildingStart;
            if (buildingIndex < city.buildings.size()) {
                const Building& building = city.buildings[buildingIndex];
                
                // Set bright color based on building type
                switch (building.type) {
                    case BuildingType::LOW_RISE:
                        shaderManager.setColor(1.0f, 0.4f, 0.2f);  // Bright orange-red
                        break;
                    case BuildingType::MID_RISE:
                        shaderManager.setColor(0.9f, 0.9f, 0.9f);  // Bright white-gray
                        break;
                    case BuildingType::HIGH_RISE:
                        shaderManager.setColor(0.3f, 0.8f, 1.0f);  // Bright sky blue
                        break;
                }
                
                glBindVertexArray(VAOs[i]);
                glDrawArrays(GL_TRIANGLES, 0, vertexCounts[i]);
            }
        }
    }
}

// Main render function
void CityRenderer::render(const CityData& city, const CityConfig& config, bool view3D, ShaderManager& shaderManager,
                          GLuint brickTexture, GLuint concreteTexture, GLuint glassTexture,
                          GLuint roadTexture, GLuint grassTexture, GLuint fountainTexture) {
    if (!isReady()) return;
    
    size_t roadCount = city.roads.size();
    size_t parkCount = city.parks.size();
    size_t fountainOffset = roadCount + parkCount;
    size_t fountainCount = city.fountain.empty() ? 0 : 1;
    size_t buildingStart = fountainOffset + fountainCount;
    
    // Render each city element
    renderRoads(city, view3D, shaderManager, roadTexture, roadCount);
    renderParks(city, view3D, shaderManager, grassTexture, roadCount, parkCount);
    renderFountain(city, view3D, shaderManager, fountainTexture, fountainOffset, fountainCount, config.timeOfDay);
    renderBuildings(city, config, view3D, shaderManager, brickTexture, concreteTexture, glassTexture, buildingStart);
}

// Update traffic rendering buffers
void CityRenderer::updateTraffic(const TrafficData& trafficData, bool view3D) {
    // Clear old traffic buffers
    if (!traffic3DVAOs.empty()) {
        glDeleteVertexArrays(traffic3DVAOs.size(), traffic3DVAOs.data());
        glDeleteBuffers(traffic3DVBOs.size(), traffic3DVBOs.data());
        traffic3DVAOs.clear();
        traffic3DVBOs.clear();
        traffic3DVertexCounts.clear();
    }
    if (!trafficVAOs.empty()) {
        glDeleteVertexArrays(trafficVAOs.size(), trafficVAOs.data());
        glDeleteBuffers(trafficVBOs.size(), trafficVBOs.data());
        trafficVAOs.clear();
        trafficVBOs.clear();
        trafficVertexCounts.clear();
    }
    
    if (view3D) {
        // Generate 3D meshes for each car
        for (const Car& car : trafficData.cars) {
            std::vector<float> vertices = carTo3DMesh(car, screenWidth, screenHeight);
            if (!vertices.empty()) {
                auto [vao, vbo] = createBuffer(vertices, true);
                traffic3DVAOs.push_back(vao);
                traffic3DVBOs.push_back(vbo);
                traffic3DVertexCounts.push_back(vertices.size() / 5);
            }
        }
    } else {
        // Generate 2D points for each car
        for (const Car& car : trafficData.cars) {
            std::vector<float> vertices = carTo2DVertices(car, screenWidth, screenHeight);
            if (!vertices.empty()) {
                auto [vao, vbo] = createBuffer(vertices, false);
                trafficVAOs.push_back(vao);
                trafficVBOs.push_back(vbo);
                trafficVertexCounts.push_back(1); // One point per car
            }
        }
    }
}

// Render traffic
void CityRenderer::renderTraffic(const TrafficData& trafficData, const CityConfig& config, bool view3D, ShaderManager& shaderManager) {
    if (!config.showTraffic || trafficData.cars.empty()) return;
    
    if (view3D) {
        // Render 3D car meshes
        shaderManager.setIs2D(false);
        shaderManager.setUseTexture(false);
        
        for (size_t i = 0; i < traffic3DVAOs.size() && i < trafficData.cars.size(); i++) {
            const Car& car = trafficData.cars[i];
            shaderManager.setColor(car.color.r, car.color.g, car.color.b);
            glBindVertexArray(traffic3DVAOs[i]);
            glDrawArrays(GL_TRIANGLES, 0, traffic3DVertexCounts[i]);
        }
    } else {
        // Render 2D points
        shaderManager.setIs2D(true);
        shaderManager.setUseTexture(false);
        glPointSize(4.0f);  // Larger points for cars
        
        for (size_t i = 0; i < trafficVAOs.size() && i < trafficData.cars.size(); i++) {
            const Car& car = trafficData.cars[i];
            shaderManager.setColor(car.color.r, car.color.g, car.color.b);
            glBindVertexArray(trafficVAOs[i]);
            glDrawArrays(GL_POINTS, 0, trafficVertexCounts[i]);
        }
    }
}
