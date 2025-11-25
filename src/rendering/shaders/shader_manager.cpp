/**
 * @file shader_manager.cpp
 * @brief Implementation of OpenGL Shader Management
 * 
 * @author City Designer Team
 * @date November 2025
 */

#include "rendering/shaders/shader_manager.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

// Vertex Shader Source (supports both 2D and 3D with textures)
const char* ShaderManager::getVertexShaderSource() {
    return R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;
uniform bool is2D;

void main() {
    FragPos = aPos;
    if (is2D) {
        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    } else {
        gl_Position = projection * view * vec4(aPos, 1.0);
    }
    TexCoord = aTexCoord;
}
)";
}

// Fragment Shader Source (supports both color and texture)
const char* ShaderManager::getFragmentShaderSource() {
    return R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;

uniform vec3 color;
uniform bool useTexture;
uniform sampler2D buildingTex;
uniform bool showWindowLights;
uniform float timeOfDay;  // Time in hours (0-24)

// Simple pseudo-random function based on position
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main() {
    vec4 baseColor;
    
    if (useTexture) {
        baseColor = texture(buildingTex, TexCoord);
    } else {
        baseColor = vec4(color, 1.0);
    }
    
    // Calculate ambient light based on time of day (default to 1.0 for safety)
    float ambientStrength = 1.0;
    if (timeOfDay > 0.0) {  // Only apply if timeOfDay is set
        if (timeOfDay >= 5.0 && timeOfDay < 7.0) {
            // Sunrise
            ambientStrength = mix(0.3, 1.0, (timeOfDay - 5.0) / 2.0);
        } else if (timeOfDay >= 19.0 && timeOfDay < 21.0) {
            // Sunset
            ambientStrength = mix(1.0, 0.3, (timeOfDay - 19.0) / 2.0);
        } else if (timeOfDay >= 21.0 || timeOfDay < 5.0) {
            // Night
            ambientStrength = 0.3;
        }
    }
    
    // Add window lights effect for buildings in 3D view
    if (showWindowLights && useTexture) {
        // Create window grid pattern (8x8 windows per building face)
        vec2 windowGrid = fract(TexCoord * 8.0);
        
        // Window frame (dark borders)
        bool isFrame = windowGrid.x < 0.1 || windowGrid.x > 0.9 || 
                       windowGrid.y < 0.1 || windowGrid.y > 0.9;
        
        if (!isFrame) {
            // Use fragment position for consistent random per window
            vec2 windowId = floor(TexCoord * 8.0);
            float randomValue = random(windowId + FragPos.xy * 0.1);
            
            // 60% of windows are lit (random per window)
            if (randomValue > 0.4) {
                // Window light intensity based on time (brighter at night)
                float windowBrightness = 0.25;
                if (timeOfDay >= 19.0 || timeOfDay < 7.0) {
                    // Night/early morning: windows glow brighter
                    windowBrightness = 0.5;
                }
                
                // Subtle warm glow for windows
                vec3 windowLight = vec3(1.0, 0.95, 0.7) * 0.8;
                // Blend with texture
                baseColor.rgb = mix(baseColor.rgb, windowLight, windowBrightness);
            }
        }
    }
    
    // Apply ambient lighting to final color
    FragColor = vec4(baseColor.rgb * ambientStrength, baseColor.a);
}
)";
}

ShaderManager::ShaderManager() 
    : shaderProgram(0), isCompiled(false),
      colorLocation(-1), viewLocation(-1), projectionLocation(-1),
      useTextureLocation(-1), is2DLocation(-1), showWindowLightsLocation(-1),
      timeOfDayLocation(-1) {
}

ShaderManager::~ShaderManager() {
    if (shaderProgram != 0) {
        glDeleteProgram(shaderProgram);
    }
}

GLuint ShaderManager::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    // Check compilation status
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR: Shader compilation failed (" 
                  << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") 
                  << ")\n" << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

bool ShaderManager::compileShaders() {
    std::cout << "🔧 Compiling shaders...\n";
    
    // Compile vertex shader
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, getVertexShaderSource());
    if (vertexShader == 0) {
        std::cerr << "❌ Vertex shader compilation failed!\n";
        return false;
    }
    std::cout << "✓ Vertex shader compiled\n";
    
    // Compile fragment shader
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, getFragmentShaderSource());
    if (fragmentShader == 0) {
        std::cerr << "❌ Fragment shader compilation failed!\n";
        glDeleteShader(vertexShader);
        return false;
    }
    std::cout << "✓ Fragment shader compiled\n";
    
    // Link shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Check linking status
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR: Shader program linking failed\n" << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
        return false;
    }
    
    // Clean up shaders (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Cache uniform locations
    cacheUniformLocations();
    
    isCompiled = true;
    std::cout << "✅ Shaders compiled and linked successfully\n";
    return true;
}

void ShaderManager::cacheUniformLocations() {
    colorLocation = glGetUniformLocation(shaderProgram, "color");
    viewLocation = glGetUniformLocation(shaderProgram, "view");
    projectionLocation = glGetUniformLocation(shaderProgram, "projection");
    useTextureLocation = glGetUniformLocation(shaderProgram, "useTexture");
    is2DLocation = glGetUniformLocation(shaderProgram, "is2D");
    showWindowLightsLocation = glGetUniformLocation(shaderProgram, "showWindowLights");
    timeOfDayLocation = glGetUniformLocation(shaderProgram, "timeOfDay");
}

void ShaderManager::use() const {
    if (isCompiled) {
        glUseProgram(shaderProgram);
    }
}

void ShaderManager::setColor(float r, float g, float b) const {
    if (colorLocation != -1) {
        glUniform3f(colorLocation, r, g, b);
    }
}

void ShaderManager::setView(const float* viewMatrix) const {
    if (viewLocation != -1) {
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewMatrix);
    }
}

void ShaderManager::setProjection(const float* projectionMatrix) const {
    if (projectionLocation != -1) {
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix);
    }
}

void ShaderManager::setUseTexture(bool use) const {
    if (useTextureLocation != -1) {
        glUniform1i(useTextureLocation, use ? 1 : 0);
    }
}

void ShaderManager::setIs2D(bool is2D) const {
    if (is2DLocation != -1) {
        glUniform1i(is2DLocation, is2D ? 1 : 0);
    }
}

void ShaderManager::setShowWindowLights(bool show) const {
    if (showWindowLightsLocation != -1) {
        glUniform1i(showWindowLightsLocation, show ? 1 : 0);
    }
}

void ShaderManager::setTimeOfDay(float time) const {
    if (timeOfDayLocation != -1) {
        glUniform1f(timeOfDayLocation, time);
    }
}
