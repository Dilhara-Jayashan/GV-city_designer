#!/bin/bash
# ====================================================================
# City Designer - Feature-Based Build Script
# ====================================================================
# 
# This project is organized by features:
# - Feature 1: Building Window Lights
# - Feature 2: Day/Night Cycle
# - Feature 3: Traffic System
# - Feature 4: Click-to-Place Buildings
# - Feature 5: Save/Load System
#
# Directory Structure:
#   src/features/     - All 5 feature implementations
#   src/core/         - Application core (window, config)
#   src/generation/   - City/road generation
#   src/rendering/    - Rendering engine
#   src/utils/        - Utilities (input, algorithms)
#
# ====================================================================

echo "🏗️  Building City Designer (Feature-Based Architecture)..."
echo ""

# Feature System Files
FEATURES=(
    "src/features/building_lights/building_lighting_system.cpp"
    "src/features/day_night_cycle/day_night_cycle.cpp"
    "src/features/traffic_system/traffic_generator.cpp"
    "src/features/building_placement/building_placement_system.cpp"
    "src/features/save_load/city_serializer.cpp"
)

# Core System Files
CORE=(
    "src/core/application.cpp"
    "src/core/city_config.cpp"
)

# Generation System Files
GENERATION=(
    "src/generation/city_generator.cpp"
    "src/generation/road_generator.cpp"
)

# Rendering System Files
RENDERING=(
    "src/rendering/city_renderer.cpp"
    "src/rendering/texture_manager.cpp"
    "src/rendering/3d/camera.cpp"
    "src/rendering/shaders/shader_manager.cpp"
    "src/rendering/mesh/building_mesh.cpp"
    "src/rendering/mesh/road_mesh.cpp"
    "src/rendering/mesh/park_mesh.cpp"
    "src/rendering/mesh/traffic_mesh.cpp"
    "src/rendering/mesh/mesh_utils.cpp"
)

# Utility System Files
UTILS=(
    "src/utils/algorithms.cpp"
    "src/utils/input_handler.cpp"
)

# Combine all source files
ALL_SOURCES=(
    "src/main.cpp"
    "src/glad.c"
    "${FEATURES[@]}"
    "${CORE[@]}"
    "${GENERATION[@]}"
    "${RENDERING[@]}"
    "${UTILS[@]}"
)

# Compile
clang++ "${ALL_SOURCES[@]}" \
        -o CityDesigner \
        -Iinclude \
        -I/opt/homebrew/include \
        -L/opt/homebrew/lib \
        -lglfw \
        -framework OpenGL \
        -std=c++17

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Build successful!"
    echo ""
    echo ""
    echo "Run with: ./CityDesigner"
    echo ""
else
    echo ""
    echo "❌ Build failed!"
    exit 1
fi
