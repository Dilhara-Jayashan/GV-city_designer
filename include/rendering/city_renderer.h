/**
 * @file city_renderer.h
 * @brief City Rendering System
 * 
 * Manages all rendering operations for the city including:
 * - VAO/VBO buffer management
 * - Mesh creation and updates
 * - Draw calls for all city elements
 * - Texture binding
 * - 2D/3D rendering modes
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef CITY_RENDERER_H
#define CITY_RENDERER_H

#include <glad/glad.h>
#include <vector>
#include "generation/city_generator.h"
#include "features/traffic_system/traffic_generator.h"
#include "rendering/shaders/shader_manager.h"
#include "core/city_config.h"

/**
 * @class CityRenderer
 * @brief Handles all rendering operations for city visualization
 * 
 * This class encapsulates the complex rendering logic including:
 * - Buffer creation and management for roads, parks, fountains, buildings
 * - Separate 2D point rendering and 3D mesh rendering
 * - Automatic buffer cleanup and regeneration
 * - Texture-based rendering for 3D mode
 */
class CityRenderer {
public:
    /**
     * @brief Construct a new City Renderer
     * @param screenWidth Window width in pixels
     * @param screenHeight Window height in pixels
     */
    CityRenderer(int screenWidth, int screenHeight);
    
    /**
     * @brief Destroy the City Renderer and cleanup all buffers
     */
    ~CityRenderer();
    
    /**
     * @brief Update rendering data when city changes or view mode changes
     * @param city City data to render
     * @param view3D Whether to use 3D rendering mode
     * 
     * Regenerates all VAO/VBO buffers for the current city and view mode.
     * Automatically cleans up old buffers before creating new ones.
     */
    void updateCity(const CityData& city, bool view3D);
    
    /**
     * @brief Update traffic rendering buffers
     * @param trafficData Traffic data to render
     * @param view3D Whether to use 3D rendering mode
     * 
     * Updates only traffic-related buffers without regenerating city.
     */
    void updateTraffic(const TrafficData& trafficData, bool view3D);
    
    /**
     * @brief Render the city
     * @param city City data
     * @param config City configuration (includes texture theme)
     * @param view3D Whether to use 3D mode
     * @param shaderManager Shader manager for rendering
     * @param brickTexture Texture ID for brick (low-rise buildings)
     * @param concreteTexture Texture ID for concrete (mid-rise buildings)
     * @param glassTexture Texture ID for glass (high-rise buildings)
     * @param roadTexture Texture ID for roads
     * @param grassTexture Texture ID for grass/parks
     * @param fountainTexture Texture ID for fountains
     */
    void render(const CityData& city, const CityConfig& config, bool view3D, ShaderManager& shaderManager,
                GLuint brickTexture, GLuint concreteTexture, GLuint glassTexture,
                GLuint roadTexture, GLuint grassTexture, GLuint fountainTexture);
    
    /**
     * @brief Render traffic (cars)
     * @param trafficData Traffic data
     * @param config City configuration
     * @param view3D Whether to use 3D mode
     * @param shaderManager Shader manager for rendering
     */
    void renderTraffic(const TrafficData& trafficData, const CityConfig& config, bool view3D, ShaderManager& shaderManager);
    
    /**
     * @brief Check if rendering data is ready
     * @return true if buffers are created and ready to render
     */
    bool isReady() const { return !VAOs.empty(); }
    
private:
    // Screen dimensions
    int screenWidth;
    int screenHeight;
    
    // 2D point rendering buffers (for 2D mode)
    std::vector<GLuint> VAOs;
    std::vector<GLuint> VBOs;
    std::vector<int> vertexCounts;
    
    // 3D mesh rendering buffers - Roads
    std::vector<GLuint> road3DVAOs;
    std::vector<GLuint> road3DVBOs;
    std::vector<int> road3DVertexCounts;
    
    // 3D mesh rendering buffers - Parks
    std::vector<GLuint> park3DVAOs;
    std::vector<GLuint> park3DVBOs;
    std::vector<int> park3DVertexCounts;
    
    // 3D mesh rendering buffers - Fountain
    GLuint fountain3DVAO;
    GLuint fountain3DVBO;
    int fountain3DVertexCount;
    
    // 3D mesh rendering buffers - Fountain Lights
    GLuint fountainLights3DVAO;
    GLuint fountainLights3DVBO;
    int fountainLights3DVertexCount;
    
    // Traffic rendering buffers
    std::vector<GLuint> traffic3DVAOs;
    std::vector<GLuint> traffic3DVBOs;
    std::vector<int> traffic3DVertexCounts;
    std::vector<GLuint> trafficVAOs;      // 2D traffic
    std::vector<GLuint> trafficVBOs;      // 2D traffic
    std::vector<int> trafficVertexCounts; // 2D traffic
    
    /**
     * @brief Cleanup all rendering buffers
     * 
     * Deletes all VAOs and VBOs, clears arrays.
     * Called before regenerating buffers or in destructor.
     */
    void cleanup();
    
    /**
     * @brief Create buffer for a mesh
     * @param vertices Vertex data (position + optional texture coordinates)
     * @param hasTexCoords Whether vertices include texture coordinates (5 floats vs 3 floats per vertex)
     * @return Pair of (VAO, VBO) handles
     */
    std::pair<GLuint, GLuint> createBuffer(const std::vector<float>& vertices, bool hasTexCoords);
    
    /**
     * @brief Render roads (both 2D and 3D)
     * @param city City data
     * @param view3D Render mode
     * @param shaderManager Shader manager
     * @param roadTexture Road texture ID
     * @param roadCount Number of roads
     */
    void renderRoads(const CityData& city, bool view3D, ShaderManager& shaderManager,
                     GLuint roadTexture, size_t roadCount);
    
    /**
     * @brief Render parks (both 2D and 3D)
     * @param city City data
     * @param view3D Render mode
     * @param shaderManager Shader manager
     * @param grassTexture Grass texture ID
     * @param roadCount Number of roads (for offset calculation)
     * @param parkCount Number of parks
     */
    void renderParks(const CityData& city, bool view3D, ShaderManager& shaderManager,
                     GLuint grassTexture, size_t roadCount, size_t parkCount);
    
    /**
     * @brief Render fountain (both 2D and 3D)
     * @param city City data
     * @param view3D Render mode
     * @param shaderManager Shader manager
     * @param fountainTexture Fountain texture ID
     * @param fountainOffset Offset in VAO array
     * @param fountainCount Number of fountains (0 or 1)
     * @param timeOfDay Current time of day (0-24 hours)
     */
    void renderFountain(const CityData& city, bool view3D, ShaderManager& shaderManager,
                        GLuint fountainTexture, size_t fountainOffset, size_t fountainCount, float timeOfDay);
    
    /**
     * @brief Render buildings (both 2D and 3D)
     * @param city City data
     * @param config City configuration (includes texture theme)
     * @param view3D Render mode
     * @param shaderManager Shader manager
     * @param brickTexture Brick texture ID
     * @param concreteTexture Concrete texture ID
     * @param glassTexture Glass texture ID
     * @param buildingStart Starting index in VAO array
     */
    void renderBuildings(const CityData& city, const CityConfig& config, bool view3D, ShaderManager& shaderManager,
                         GLuint brickTexture, GLuint concreteTexture, GLuint glassTexture,
                         size_t buildingStart);
};

#endif // CITY_RENDERER_H
