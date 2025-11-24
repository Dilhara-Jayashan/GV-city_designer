#!/bin/bash
# Build script for City Designer - Feature-Separated Structure

echo "🏗️  Building City Designer..."
echo ""

clang++ src/main.cpp \
        src/glad.c \
        src/core/application.cpp \
        src/core/city_config.cpp \
        src/generation/city_generator.cpp \
        src/generation/road_generator.cpp \
        src/rendering/texture_manager.cpp \
        src/rendering/3d/camera.cpp \
        src/rendering/city_renderer.cpp \
        src/rendering/shaders/shader_manager.cpp \
        src/rendering/mesh/building_mesh.cpp \
        src/rendering/mesh/road_mesh.cpp \
        src/rendering/mesh/park_mesh.cpp \
        src/rendering/mesh/mesh_utils.cpp \
        src/utils/algorithms.cpp \
        src/utils/input_handler.cpp \
        -o CityDesigner \
        -Iinclude \
        -Ilib/glm \
        -I/opt/homebrew/include \
        -L/opt/homebrew/lib \
        -lglfw \
        -framework OpenGL \
        -std=c++17

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Build successful!"
    echo ""
    echo "Run with: ./CityDesigner"
    echo ""
else
    echo ""
    echo "❌ Build failed!"
    exit 1
fi
