#!/bin/bash
# Build script for City Designer

echo "🏗️  Building City Designer..."
echo ""

clang++ src/main.cpp \
        src/glad.c \
        src/algorithms.cpp \
        src/city_config.cpp \
        src/input_handler.cpp \
        src/road_generator.cpp \
        src/city_generator.cpp \
        -o CityDesigner \
        -Iinclude \
        -I/opt/homebrew/include \
        -L/opt/homebrew/lib \
        -lglfw \
        -framework OpenGL \
        -std=c++11

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
