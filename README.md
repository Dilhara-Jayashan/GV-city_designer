# 🏙️ City Designer - Feature-Based Architecture

An interactive 3D city generation and visualization tool with 5 creative features.

## 📦 Project Structure (Feature-Based)

```
GV-city_designer/
├── src/
│   ├── features/                    # 🎯 THE 5 CREATIVE FEATURES
│   │   ├── building_lights/         # Feature 1: Window Lighting
│   │   │   ├── building_lighting_system.cpp
│   │   │   └── building_lighting_system.h
│   │   ├── day_night_cycle/         # Feature 2: Day/Night Cycle
│   │   │   ├── day_night_cycle.cpp
│   │   │   └── day_night_cycle.h
│   │   ├── traffic_system/          # Feature 3: Traffic Animation
│   │   │   ├── traffic_generator.cpp
│   │   │   └── traffic_generator.h
│   │   ├── building_placement/      # Feature 4: Click-to-Place
│   │   │   ├── building_placement_system.cpp
│   │   │   └── building_placement_system.h
│   │   └── save_load/               # Feature 5: Save/Load System
│   │       ├── city_serializer.cpp
│   │       └── city_serializer.h
│   │
│   ├── core/                        # Core Application
│   │   ├── application.cpp          # GLFW window management
│   │   └── city_config.cpp          # Configuration system
│   │
│   ├── generation/                  # City Generation
│   │   ├── city_generator.cpp       # Main city generation logic
│   │   └── road_generator.cpp       # Road network generation
│   │
│   ├── rendering/                   # Rendering Engine
│   │   ├── city_renderer.cpp        # Main renderer
│   │   ├── texture_manager.cpp      # Texture loading
│   │   ├── 3d/camera.cpp            # FPP camera
│   │   ├── shaders/
│   │   │   └── shader_manager.cpp   # Shader compilation
│   │   └── mesh/                    # 3D mesh generation
│   │       ├── building_mesh.cpp
│   │       ├── road_mesh.cpp
│   │       ├── park_mesh.cpp
│   │       ├── traffic_mesh.cpp
│   │       └── mesh_utils.cpp
│   │
│   ├── utils/                       # Utilities
│   │   ├── algorithms.cpp           # Bresenham, midpoint circle
│   │   └── input_handler.cpp        # Keyboard/mouse input
│   │
│   ├── main.cpp                     # Main entry point
│   └── glad.c                       # OpenGL loader
│
├── include/                         # Header files (mirrors src/)
├── assets/                          # Textures
│   ├── brick.jpg
│   ├── concrete.jpg
│   ├── glass.jpg
│   ├── road.jpg
│   ├── grass.jpg
│   └── fountains.jpg
├── saves/                           # Save files
│   └── city_save.json
├── build.sh                         # Build script
└── README.md                        # This file
```

## 🎯 The 5 Creative Features

### Feature 1: Building Window Lights 💡
**Location**: `src/features/building_lights/`

- **What it does**: Building windows light up at night and turn off during the day
- **Key class**: `BuildingLightingSystem`
- **Methods**:
  - `getWindowEmissionColor(timeOfDay)` - Returns warm yellow glow color
  - `getLightIntensity(timeOfDay)` - Returns 0.0-1.0 intensity
  - `areLightsActive(timeOfDay)` - Checks if lights should be on
- **Transitions**: Smooth fade-in at sunset (6pm-8pm), fade-out at sunrise (4am-6am)
- **Visual effect**: Warm yellow (RGB: 3.0, 2.5, 1.5) emissive lighting on building windows

### Feature 2: Day/Night Cycle 🌅
**Location**: `src/features/day_night_cycle/`

- **What it does**: Automatic time progression with sky color transitions
- **Key class**: `DayNightCycle`
- **Methods**:
  - `update(deltaTime)` - Advances time automatically
  - `getSkyColor()` - Returns sky color for 3D view
  - `getAmbientLightFactor()` - Returns 0.3-1.0 ambient multiplier
- **Time periods**:
  - Night (9pm-5am): Soft dark lavender
  - Sunrise (5am-9am): Lavender → Peachy → Light blue
  - Day (9am-5pm): Light sky blue
  - Sunset (5pm-9pm): Light blue → Warm peach → Lavender
- **Speed**: 0.5 hours per real second

### Feature 3: Traffic System 🚗
**Location**: `src/features/traffic_system/`

- **What it does**: Animated vehicles that follow roads
- **Key class**: `TrafficGenerator`
- **Features**:
  - Vehicles follow road networks
  - Collision avoidance (roads, parks, fountains, other cars)
  - Boundary checking (stays within 50px margin)
  - Smooth movement with progress tracking
  - Random colored vehicles
- **Methods**:
  - `generateTraffic()` - Spawns vehicles on roads
  - `updateTraffic(deltaTime)` - Updates vehicle positions
- **Cars**: Default 15 vehicles, configurable 0-50

### Feature 4: Click-to-Place Buildings 🏢
**Location**: `src/features/building_placement/`

- **What it does**: Interactive building placement via mouse clicks (2D view only)
- **Key class**: `BuildingPlacementSystem`
- **Methods**:
  - `tryPlaceBuilding()` - Attempts to place building at click position
  - Collision checks with roads, parks, fountain, existing buildings
- **Rules**:
  - Must be within screen boundaries (60px margin)
  - Must not overlap roads (20px buffer)
  - Must not overlap parks/fountain (35px buffer)
  - Must not overlap buildings (25px buffer)
- **Usage**: Click anywhere in 2D view to place a mid-rise building

### Feature 5: Save/Load System 💾
**Location**: `src/features/save_load/`

- **What it does**: Persists city data to JSON file
- **Key class**: `CitySerializer` (static methods)
- **Methods**:
  - `saveCity(cityData, filename)` - Saves to JSON
  - `loadCity(cityData, filename)` - Loads from JSON
- **Saves**:
  - Buildings (position, size, height, type)
  - Roads (all point coordinates)
  - Parks (circular boundary points)
  - Fountain (circular boundary points)
- **Location**: `saves/city_save.json`
- **Usage**: Press 'C' to save, 'L' to load

## 🛠️ Build Instructions

### Prerequisites
- macOS (tested on macOS)
- Xcode Command Line Tools
- GLFW3 (`brew install glfw`)
- C++17 compiler

### Build
```bash
bash build.sh
```

### Run
```bash
./CityDesigner
```

## 🎮 Controls

### View Controls
- **V**: Toggle 2D/3D view
- **G**: Generate new city
- **H**: Show/hide help

### Time Controls  
- **T**: Cycle time of day manually
- **A**: Toggle auto time progression

### Generation Controls
- **1-4**: Change building count
- **5-6**: Change layout size
- **R**: Cycle road patterns (Grid/Radial/Random)
- **S**: Cycle skyline types
- **7-0**: Adjust park/fountain settings

### 2D View Only
- **Left Click**: Place building at cursor position

### 3D View Only
- **WASD**: Move camera
- **Mouse**: Look around
- **Shift**: Sprint (faster movement)

### Save/Load
- **C**: Save city to `saves/city_save.json`
- **L**: Load city from `saves/city_save.json`

## 📊 Code Quality

### Architecture Principles
✅ **Feature-Based Organization**: Each of the 5 features is self-contained in its own directory  
✅ **No Code Duplication**: All duplicate files removed, single source of truth  
✅ **Clear Separation of Concerns**: Features, rendering, generation, and core are separated  
✅ **Single Responsibility**: Each class has one clear purpose  
✅ **Clean main.cpp**: Only 245 lines, just initialization and main loop  
✅ **Well-Documented**: Every feature class has comprehensive header documentation  

### File Count
- **Feature files**: 10 files (5 features × 2 files each)
- **Core files**: 4 files
- **Generation files**: 4 files
- **Rendering files**: 20 files
- **Utility files**: 4 files
- **Total source files**: 42 files (+ headers)

### Lines of Code
- **Features**: ~1,200 lines
- **Main.cpp**: 245 lines
- **Total project**: ~8,000 lines

## 🎨 Feature Integration

All 5 features work together seamlessly:

1. **Day/Night Cycle** controls the time of day
2. **Building Lights** respond to time of day (lights on at night)
3. **Traffic System** animates based on time (smooth movement)
4. **Click-to-Place** allows interactive city editing
5. **Save/Load** preserves everything for later

## 🔧 Technical Details

### Algorithms Used
- **Bresenham's Line Algorithm**: Road generation
- **Midpoint Circle Algorithm**: Parks and fountain generation
- **AABB Collision Detection**: Building placement
- **Circle-Box Collision**: Park/fountain collision checks

### Rendering
- **OpenGL 3.3 Core**: Modern OpenGL
- **Shader-Based**: Vertex and fragment shaders
- **Texture Mapping**: 6 texture types (brick, glass, concrete, road, grass, fountain)
- **3D Meshes**: Procedurally generated buildings, roads, parks
- **Emissive Lighting**: Window glow, fountain lights
- **Time-Based Ambient**: Dynamic lighting based on time of day

### Performance
- **60 FPS**: Smooth rendering
- **Real-time traffic**: 15 vehicles updating every frame
- **Efficient mesh generation**: Cached VAO/VBO buffers
- **Minimal allocations**: Pre-allocated buffers

## 📝 Development Notes

### Why Feature-Based Architecture?
1. **Clarity**: Each feature is easy to find and understand
2. **Maintainability**: Changes to one feature don't affect others
3. **Scalability**: Easy to add new features
4. **Testing**: Each feature can be tested independently
5. **Team Collaboration**: Multiple developers can work on different features

### Design Decisions
- **Static methods for serialization**: No state needed
- **Separate lighting system**: Decouples lighting from rendering
- **Config-driven**: All parameters in one place
- **RAII pattern**: Automatic resource cleanup
- **Forward declarations**: Minimize include dependencies

## 🚀 Future Enhancements
- [ ] More building types
- [ ] Weather effects (rain, fog)
- [ ] Day/night sounds
- [ ] Traffic signals
- [ ] Pedestrians
- [ ] Multiple save slots
- [ ] Export to image/video

## 👥 Credits
**Author**: City Designer Team  
**Date**: November 2025  
**Course**: Computer Graphics Project

## 📄 License
Educational project - Free to use and modify
