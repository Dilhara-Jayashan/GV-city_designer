/**
 * @file texture_manager.cpp
 * @brief Implementation of Texture Management System
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include "rendering/texture_manager.h"
#include "stb_image.h"
#include <iostream>
#include <vector>
#include <cstdlib>

// Constructor
TextureManager::TextureManager() {
    // Initialize empty texture cache
}

// Destructor
TextureManager::~TextureManager() {
    cleanup();
}

// Load all required textures for the application
void TextureManager::loadAllTextures() {
    std::cout << "\n🎨 Loading Textures...\n";
    
    // Load building textures
    GLuint brickTex = loadTextureFromFile("assets/brick.jpg");
    if (brickTex == 0) {
        std::cout << "⚠️  Warning: Could not load brick.jpg, generating procedural\n";
        brickTex = generateProceduralTexture("brick");
    } else {
        std::cout << "✅ Loaded brick texture from assets/brick.jpg\n";
    }
    textureCache["brick"] = brickTex;
    
    GLuint concreteTex = loadTextureFromFile("assets/concrete.jpg");
    if (concreteTex == 0) {
        std::cout << "⚠️  Warning: Could not load concrete.jpg, generating procedural\n";
        concreteTex = generateProceduralTexture("concrete");
    } else {
        std::cout << "✅ Loaded concrete texture from assets/concrete.jpg\n";
    }
    textureCache["concrete"] = concreteTex;
    
    GLuint glassTex = loadTextureFromFile("assets/glass.jpg");
    if (glassTex == 0) {
        std::cout << "⚠️  Warning: Could not load glass.jpg, generating procedural\n";
        glassTex = generateProceduralTexture("glass");
    } else {
        std::cout << "✅ Loaded glass texture from assets/glass.jpg\n";
    }
    textureCache["glass"] = glassTex;
    
    // Load road texture
    GLuint roadTex = loadTextureFromFile("assets/road.jpg");
    if (roadTex == 0) {
        std::cout << "⚠️  Warning: Could not load road.jpg, generating procedural\n";
        roadTex = generateProceduralTexture("asphalt");
    } else {
        std::cout << "✅ Loaded road texture from assets/road.jpg\n";
    }
    textureCache["road"] = roadTex;
    
    // Load grass texture for parks
    GLuint grassTex = loadTextureFromFile("assets/grass.jpg");
    if (grassTex == 0) {
        std::cout << "⚠️  Warning: Could not load grass.jpg, using green color\n";
        grassTex = generateProceduralTexture("grass");
    } else {
        std::cout << "✅ Loaded grass texture from assets/grass.jpg\n";
    }
    textureCache["grass"] = grassTex;
    
    // Load fountain texture
    GLuint fountainTex = loadTextureFromFile("assets/fountains.jpg");
    if (fountainTex == 0) {
        std::cout << "⚠️  Warning: Could not load fountains.jpg, using cyan color\n";
        fountainTex = generateProceduralTexture("water");
    } else {
        std::cout << "✅ Loaded fountain texture from assets/fountains.jpg\n";
    }
    textureCache["fountain"] = fountainTex;
}

// Get texture by name
GLuint TextureManager::getTexture(const std::string& name) const {
    auto it = textureCache.find(name);
    if (it != textureCache.end()) {
        return it->second;
    }
    return 0; // Texture not found
}

// Check if texture exists
bool TextureManager::hasTexture(const std::string& name) const {
    return textureCache.find(name) != textureCache.end();
}

// Cleanup all textures
void TextureManager::cleanup() {
    for (auto& pair : textureCache) {
        if (pair.second != 0) {
            glDeleteTextures(1, &pair.second);
        }
    }
    textureCache.clear();
}

// Load texture from image file using STB Image
GLuint TextureManager::loadTextureFromFile(const std::string& filepath) {
    int width, height, nrChannels;
    
    // Flip textures vertically to match OpenGL coordinate system
    stbi_set_flip_vertically_on_load(true);
    
    // Load image data
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
    
    if (!data) {
        return 0; // Failed to load
    }
    
    // Generate OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Determine format (RGB or RGBA)
    GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
    
    // Upload texture data to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    
    // Generate mipmaps for better quality at distance
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Free image data
    stbi_image_free(data);
    
    return textureID;
}

// Generate procedural texture as fallback
GLuint TextureManager::generateProceduralTexture(const std::string& type) {
    const int width = 256;
    const int height = 256;
    std::vector<unsigned char> data(width * height * 3);
    
    if (type == "brick") {
        // Red brick pattern with mortar
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = (y * width + x) * 3;
                bool isMortar = (y % 32 < 2) || (x % 64 < 2);
                if (isMortar) {
                    // Gray mortar lines
                    data[idx] = 180; 
                    data[idx+1] = 180; 
                    data[idx+2] = 180;
                } else {
                    // Red brick with variation for realism
                    data[idx] = 160 + (rand() % 40);
                    data[idx+1] = 50 + (rand() % 30);
                    data[idx+2] = 40 + (rand() % 20);
                }
            }
        }
    } 
    else if (type == "concrete") {
        // Gray concrete with subtle variation
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = (y * width + x) * 3;
                unsigned char gray = 120 + (rand() % 60);
                data[idx] = gray; 
                data[idx+1] = gray; 
                data[idx+2] = gray;
            }
        }
    } 
    else if (type == "glass") {
        // Blue glass with window grid pattern
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = (y * width + x) * 3;
                bool isFrame = (y % 32 < 2) || (x % 32 < 2);
                if (isFrame) {
                    // Dark frame
                    data[idx] = 60; 
                    data[idx+1] = 60; 
                    data[idx+2] = 80;
                } else {
                    // Blue tinted glass
                    data[idx] = 100 + (rand() % 30);
                    data[idx+1] = 150 + (rand() % 30);
                    data[idx+2] = 200 + (rand() % 30);
                }
            }
        }
    } 
    else if (type == "asphalt") {
        // Dark gray asphalt with road markings
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = (y * width + x) * 3;
                // Yellow dashed center line
                bool isRoadLine = (y > height / 2 - 2 && y < height / 2 + 2) && 
                                  ((x / 16) % 4 == 0);
                if (isRoadLine) {
                    // Yellow road marking
                    data[idx] = 220; 
                    data[idx+1] = 200; 
                    data[idx+2] = 50;
                } else {
                    // Dark gray asphalt with variation
                    unsigned char gray = 40 + (rand() % 30);
                    data[idx] = gray;
                    data[idx+1] = gray;
                    data[idx+2] = gray + 5; // Slightly bluish tint
                }
            }
        }
    }
    else if (type == "grass") {
        // Green grass with variation
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = (y * width + x) * 3;
                data[idx] = 40 + (rand() % 50);      // Red channel
                data[idx+1] = 120 + (rand() % 60);   // Green channel (dominant)
                data[idx+2] = 40 + (rand() % 40);    // Blue channel
            }
        }
    }
    else if (type == "water") {
        // Cyan/blue water with wave pattern
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = (y * width + x) * 3;
                data[idx] = 70 + (rand() % 50);      // Red channel
                data[idx+1] = 150 + (rand() % 60);   // Green channel
                data[idx+2] = 200 + (rand() % 55);   // Blue channel (dominant)
            }
        }
    }
    
    // Create OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    return textureID;
}
