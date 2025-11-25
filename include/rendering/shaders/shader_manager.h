/**
 * @file shader_manager.h
 * @brief OpenGL Shader Compilation and Management
 * 
 * Manages shader source code, compilation, linking, and program management.
 * Separates shader logic from main rendering code for better organization.
 * 
 * @author City Designer Team
 * @date November 2025
 */

#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <glad/glad.h>
#include <string>

/**
 * @class ShaderManager
 * @brief Manages OpenGL shader programs
 * 
 * Handles all shader-related operations including:
 * - Vertex and fragment shader source code
 * - Shader compilation
 * - Program linking
 * - Uniform location caching
 */
class ShaderManager {
private:
    GLuint shaderProgram;           ///< Compiled shader program ID
    bool isCompiled;                ///< Compilation status flag
    
    // Cached uniform locations
    GLint colorLocation;
    GLint viewLocation;
    GLint projectionLocation;
    GLint useTextureLocation;
    GLint is2DLocation;
    GLint showWindowLightsLocation;
    GLint timeOfDayLocation;
    
public:
    /**
     * @brief Construct a new Shader Manager
     */
    ShaderManager();
    
    /**
     * @brief Destroy the Shader Manager and cleanup OpenGL resources
     */
    ~ShaderManager();
    
    /**
     * @brief Compile and link the city designer shader program
     * @return true if compilation successful, false otherwise
     * 
     * Compiles vertex and fragment shaders, links them into a program,
     * and caches uniform locations for faster access.
     */
    bool compileShaders();
    
    /**
     * @brief Use this shader program for rendering
     * 
     * Calls glUseProgram with the compiled shader program ID.
     */
    void use() const;
    
    /**
     * @brief Get the shader program ID
     * @return OpenGL shader program ID
     */
    GLuint getProgram() const { return shaderProgram; }
    
    /**
     * @brief Check if shaders are compiled
     * @return true if compiled, false otherwise
     */
    bool isReady() const { return isCompiled; }
    
    // Uniform setters for convenience
    void setColor(float r, float g, float b) const;
    void setView(const float* viewMatrix) const;
    void setProjection(const float* projectionMatrix) const;
    void setUseTexture(bool use) const;
    void setIs2D(bool is2D) const;
    void setShowWindowLights(bool show) const;
    void setTimeOfDay(float time) const;
    
    // Get uniform locations (for advanced usage)
    GLint getColorLocation() const { return colorLocation; }
    GLint getViewLocation() const { return viewLocation; }
    GLint getProjectionLocation() const { return projectionLocation; }
    GLint getUseTextureLocation() const { return useTextureLocation; }
    GLint getIs2DLocation() const { return is2DLocation; }
    
private:
    /**
     * @brief Get vertex shader source code
     * @return GLSL vertex shader source as string
     */
    static const char* getVertexShaderSource();
    
    /**
     * @brief Get fragment shader source code
     * @return GLSL fragment shader source as string
     */
    static const char* getFragmentShaderSource();
    
    /**
     * @brief Compile a shader from source
     * @param type GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
     * @param source GLSL source code
     * @return Compiled shader ID, or 0 on failure
     */
    GLuint compileShader(GLenum type, const char* source);
    
    /**
     * @brief Cache uniform locations for faster access
     */
    void cacheUniformLocations();
};

#endif // SHADER_MANAGER_H
