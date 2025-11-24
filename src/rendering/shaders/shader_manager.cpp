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

uniform mat4 view;
uniform mat4 projection;
uniform bool is2D;

void main() {
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

uniform vec3 color;
uniform bool useTexture;
uniform sampler2D buildingTex;

void main() {
    if (useTexture) {
        FragColor = texture(buildingTex, TexCoord);
    } else {
        FragColor = vec4(color, 1.0);
    }
}
)";
}

ShaderManager::ShaderManager() 
    : shaderProgram(0), isCompiled(false),
      colorLocation(-1), viewLocation(-1), projectionLocation(-1),
      useTextureLocation(-1), is2DLocation(-1) {
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
    // Compile vertex shader
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, getVertexShaderSource());
    if (vertexShader == 0) {
        return false;
    }
    
    // Compile fragment shader
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, getFragmentShaderSource());
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        return false;
    }
    
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
