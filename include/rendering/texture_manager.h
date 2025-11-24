/**
 * @file texture_manager.h
 * @brief Texture Management System for City Designer
 * 
 * Handles loading, caching, and management of all textures used in the application.
 * Supports JPG and PNG formats using STB Image library.
 * Provides fallback procedural textures if file loading fails.
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <glad/glad.h>
#include <string>
#include <map>

/**
 * @class TextureManager
 * @brief Manages texture loading, generation, and lifecycle
 * 
 * This class provides a centralized system for handling all textures in the application.
 * It supports:
 * - Loading textures from image files (JPG, PNG)
 * - Generating procedural textures as fallbacks
 * - Caching loaded textures to avoid redundant loading
 * - Proper cleanup of GPU resources
 */
class TextureManager {
public:
    /**
     * @brief Construct a new Texture Manager
     */
    TextureManager();
    
    /**
     * @brief Destroy the Texture Manager and cleanup all textures
     */
    ~TextureManager();
    
    /**
     * @brief Load all required textures for the city designer
     * 
     * Loads the following textures:
     * - brick.jpg (for low-rise buildings)
     * - concrete.jpg (for mid-rise buildings)
     * - glass.jpg (for high-rise buildings)
     * - road.jpg (for 3D road surfaces)
     * - grass.jpg (for 3D park surfaces)
     * - fountains.jpg (for 3D fountain surfaces)
     * 
     * If any texture fails to load, a procedural texture is generated as fallback.
     */
    void loadAllTextures();
    
    /**
     * @brief Get a texture by name
     * @param name Texture identifier (e.g., "brick", "road", "grass")
     * @return GLuint OpenGL texture ID (0 if texture doesn't exist)
     */
    GLuint getTexture(const std::string& name) const;
    
    /**
     * @brief Check if a texture is loaded
     * @param name Texture identifier
     * @return true if texture exists and is loaded
     */
    bool hasTexture(const std::string& name) const;
    
    /**
     * @brief Cleanup all loaded textures
     * 
     * Deletes all OpenGL texture objects and clears the cache.
     * Called automatically in destructor.
     */
    void cleanup();
    
private:
    /**
     * @brief Load a texture from an image file
     * @param filepath Path to the image file (JPG or PNG)
     * @return GLuint OpenGL texture ID (0 if loading fails)
     * 
     * Uses STB Image to load the file and creates an OpenGL texture with:
     * - Mipmapping enabled
     * - Linear filtering for smooth appearance
     * - Repeat wrapping mode
     */
    GLuint loadTextureFromFile(const std::string& filepath);
    
    /**
     * @brief Generate a procedural texture as fallback
     * @param type Type of texture to generate ("brick", "concrete", "glass", "asphalt", "grass")
     * @return GLuint OpenGL texture ID
     * 
     * Creates a simple colored texture when file loading fails.
     * Colors are chosen to match the material type.
     */
    GLuint generateProceduralTexture(const std::string& type);
    
    /**
     * @brief Cache of loaded textures
     * Maps texture name to OpenGL texture ID
     */
    std::map<std::string, GLuint> textureCache;
};

#endif // TEXTURE_MANAGER_H
