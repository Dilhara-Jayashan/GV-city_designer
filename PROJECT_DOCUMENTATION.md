# 🏙️ City Designer - Project Documentation

## 1. Overview

City Designer is an interactive **2D/3D city generation and visualization** tool built in C++ and OpenGL. The project uses a **feature-based architecture** and implements **5 creative features**:

The goal is to provide a clean, extensible architecture where each feature is isolated in its own module, while still working together seamlessly in the main application.

---

## 2. Architecture

### 2.1 Directory Structure

```text
GV-city_designer/
├── src/
│   ├── features/                    # 🎯 The 5 Creative Features
│   │   ├── building_lights/         # Feature 1: Window Lighting
│   │   ├── day_night_cycle/         # Feature 2: Day/Night Cycle
│   │   ├── traffic_system/          # Feature 3: Traffic Animation
│   │   ├── building_placement/      # Feature 4: Click-to-Place
│   │   └── save_load/               # Feature 5: Save/Load System
│   ├── core/                        # Core application and config
│   ├── generation/                  # City + road generation
│   ├── rendering/                   # Rendering engine & meshes
│   ├── utils/                       # Input + algorithms
│   ├── main.cpp                     # Entry point
│   └── glad.c                       # OpenGL loader
├── include/                         # Header files (mirrors src/)
├── assets/                          # Textures
├── saves/                           # Saved cities (JSON)
├── build.sh                         # Build script
└── CityDesigner                     # Built executable
```

### 2.2 Main Subsystems

- **Core** (`core/`)
  - `Application`: GLFW window + OpenGL context setup
  - `CityConfig`: Central configuration (city size, roads, parks, cars, view mode, time, etc.)
- **Generation** (`generation/`)
  - `CityGenerator`: Creates full `CityData` (buildings, parks, fountain, roads)
  - `RoadGenerator`: Builds road networks using Bresenham and different patterns
- **Rendering** (`rendering/`)
  - `CityRenderer`: Draws city + traffic (2D and 3D)
  - `Camera`: FPP camera for 3D navigation (WASD + mouse)
  - `TextureManager`: Loads brick/road/grass/glass/fountain textures
  - `ShaderManager`: Compiles and manages GLSL shaders
  - Mesh modules: `building_mesh`, `road_mesh`, `park_mesh`, `traffic_mesh`, `mesh_utils`
- **Features** (`features/`)
  - `BuildingLightingSystem`: Time-based window glow
  - `DayNightCycle`: Time-of-day and sky colors
  - `TrafficGenerator`: Animated vehicles along roads
  - `BuildingPlacementSystem`: Mouse-based building placement
  - `CitySerializer`: Save/load entire city to/from JSON
- **Utils** (`utils/`)
  - `InputHandler`: Keyboard & mouse input
  - `Algorithms`: Bresenham line + midpoint circle

---

## 3. Application Flow

### 3.1 Startup Sequence (`main.cpp`)

1. **Configuration Setup**
   - Create `CityConfig`, `InputHandler`, and `CityGenerator` with screen size.
2. **Welcome & Controls**
   - Print ASCII art title and a **full keyboard controls table**.
   - Print current configuration (buildings, layout, roads, parks, etc.).
3. **OpenGL Initialization**
   - Create `Application` (GLFW window + GL context).
   - Create `Camera` and hook callbacks.
   - Configure cursor mode based on `view3D` (2D: normal cursor, 3D: disabled cursor).
4. **Subsystems Initialization**
   - Instantiate:
     - `CityRenderer`
     - `BuildingLightingSystem`
     - `DayNightCycle`
     - `TrafficGenerator`
     - `BuildingPlacementSystem`
   - Compile shaders (`ShaderManager`).
   - Enable depth testing & point size.
   - Load textures (`TextureManager`).
   - Attach `CityGenerator` to `InputHandler`.
5. **Final Startup Messages**
   - Print:
     - `All systems initialized!`
     - `Press 'G' to generate a city.`
     - `Press 'H' for keyboard controls.`

### 3.2 Main Loop

Each frame, the following happens:

1. **Time Step**
   - Compute `deltaTime` using GLFW time.
2. **Day/Night Update (Feature 2)**
   - `dayNightCycle.update(deltaTime)` (if auto-progress enabled).
   - `cityConfig.timeOfDay = dayNightCycle.getTimeOfDay()`.
3. **Input Processing**
   - `InputHandler::processInput(window)` reads keyboard:
     - Tuning buildings, layout, road width, skyline, textures
     - Toggling auto time (`N`) and advancing 2 hours (`M`)
     - Switching between 2D/3D (`V`)
     - Generating (`G`), saving (`Z`), loading (`X`)
     - Viewing controls (`H`) and configuration (`P`)
   - `InputHandler::processMouseInput(...)` handles 2D clicks for building placement.
   - FPP `Camera` handles WASD + mouse in 3D.
   - If `M` is pressed, `config.timeOfDay` is updated and synced back to `DayNightCycle`.
4. **View Mode Handling**
   - Detect change in `view3D` and adjust GLFW cursor mode.
5. **Load Request (Feature 5)**
   - If `loadCityRequested`:
     - Call `CitySerializer::loadCity` into `CityGenerator`'s `CityData`.
     - Update city in renderer.
     - Optionally regenerate and upload traffic meshes.
6. **Generation Request (G or view mode change)**
   - If `generationRequested` or view mode changed and a city exists:
     - Re-upload city data to renderer.
     - Regenerate traffic if enabled.
     - **Print keyboard controls table again** after a successful generation.
7. **Traffic Update (Feature 3)**
   - If traffic is enabled and a city exists:
     - `trafficSystem.updateTraffic(deltaTime, city.roads)`.
     - Push new traffic data to renderer.
8. **Click-to-Place Buildings (Feature 4)**
   - If a 2D click was registered and a city exists:
     - Use `BuildingPlacementSystem::tryPlaceBuilding(...)` to validate and place.
     - If placement succeeds, upload updated buildings to renderer.
9. **Sky Color & Clear**
   - Compute `skyColor` from `DayNightCycle` (3D) or a static color (2D).
   - Clear color and depth buffers.
10. **Shader and Camera Setup**
    - Use `ShaderManager` program.
    - Pass time-of-day uniform.
    - Compute `view` + `projection` matrices for either 2D or 3D.
11. **Render**
    - If a city exists and renderer is ready:
      - `renderer.render(city, cityConfig, view3D, shaderManager, textures...)`.
      - If traffic exists: `renderer.renderTraffic(...)`.
12. **Window Update**
    - `app.update()` swaps buffers and polls GLFW events.

---

## 4. Feature Modules

This section describes not only the main 5 assignment features, but also
the important supporting features such as collision detection, validation
rules, mesh generation, and camera/navigation.

### 4.1 Building Window Lights (Feature 1)

- **Module**: `features/building_lights/`
- **Class**: `BuildingLightingSystem`
- **Responsibility**: Decide if building windows should glow and how strongly.

**Core Ideas**:
- Lights are **time-based**: on at night, off during the day.
- Uses a warm emissive color: `(3.0, 2.5, 1.5)` for windows.
- Smooth fade-in/out during sunrise and sunset.

**Key Methods**:
- `getWindowEmissionColor(timeOfDay)`
- `getLightIntensity(timeOfDay)`
- `areLightsActive(timeOfDay)`

These are used by the renderer/shader to apply emissive lighting to building windows.

---

### 4.2 Day/Night Cycle (Feature 2)

- **Module**: `features/day_night_cycle/`
- **Class**: `DayNightCycle`
- **Responsibility**: Track time-of-day and compute sky colors + ambient lighting.

**Behavior**:
- Time advances at `0.5` hours per real second (configurable).
- Wraps around at 24 hours.
- Produces smooth color transitions across:
  - Night → Sunrise → Morning → Day → Sunset → Dusk → Night
- Provides:
  - `getSkyColor()` for 3D background
  - `getSkyColor2D()` for the 2D view background
  - `getAmbientLightFactor()` for shaders (0.3 at night, 1.0 during day)

**Manual Time Control**:
- Pressing `M` advances the time by **2 hours**.
- `InputHandler` updates `config.timeOfDay` and this value is
  synchronized back into `DayNightCycle` so visuals stay consistent.

---

### 4.3 Traffic System (Feature 3)

- **Module**: `features/traffic_system/`
- **Class**: `TrafficGenerator`
- **Responsibility**: Create and animate cars that move along roads.

**Generation**:
- Picks random roads and positions along them for each car.
- Checks against:
  - Screen margins
  - Park circles
  - Fountain circle
- Computes initial velocity from the road direction.

**Update**:
- Stores per-car `roadIndex` and `roadProgress`.
- Moves cars along roads over time.
- When reaching a road end, picks a next road and continues.
- Avoids collisions and respects boundaries.

Traffic data is sent to `TrafficMesh` for 3D geometry, and then rendered with its own draw call.

---

### 4.4 Click-to-Place Buildings (Feature 4)

- **Module**: `features/building_placement/`
- **Class**: `BuildingPlacementSystem`
- **Responsibility**: Allow the user to place extra buildings using mouse clicks in 2D.

**Flow**:
1. `InputHandler::processMouseInput(...)` detects a left-click in 2D mode.
2. It records the cursor position and sets `buildingPlacementRequested`.
3. In the main loop, if `buildingPlacementPending()` and a city exists:
   - Fetch the stored mouse position.
   - Call `tryPlaceBuilding(...)` with:
     - Click position (screen space)
     - References to current buildings, roads, parks, and fountain
     - Configuration and screen size
4. If placement is valid (no collisions or boundary issues),
   the building is added and the renderer is updated.

**Validation Rules**:
- Respect screen margins.
- Avoid overlaps with:
  - Existing buildings (AABB collision + padding)
  - Road points (distance-based)
  - Park/fountain circles (circle-box / point-in-circle checks)

This feature relies heavily on the **collision detection" feature
implemented in `CityGenerator` and `BuildingPlacementSystem`.

---

### 4.5 Save/Load System (Feature 5)

- **Module**: `features/save_load/`
- **Class**: `CitySerializer` (static API)
- **Responsibility**: Serialize/deserialize `CityData` to/from JSON.

**Data Saved**:
- Buildings: position, dimensions, height classification, type
- Roads: list of road segments and their sample points
- Parks: center + radius
- Fountain: center + radius

**Usage**:
- Press `Z` → `saveCity(city, "city_save")` writes `saves/city_save.json`.
- Press `X` → `loadCity(city, "city_save")` repopulates `CityGenerator`'s `CityData`.

After a load, the renderer and traffic system are refreshed so the screen matches the saved state.

---

### 4.6 Collision Detection & Spatial Validation (Supporting Feature)

While not counted as one of the "5 creative features", the project has
a **rich collision detection and validation system** that underpins
city generation and interactive placement.

**What this feature does:**
- Ensures buildings never overlap each other
- Keeps buildings away from parks and the central fountain
- Prevents buildings from being placed on top of roads
- Enforces a safe margin inside the screen boundaries

**Where it is used:**
- `CityGenerator::generateCity(...)` – automatic building placement
- `CityGenerator::placeBuilding(...)` – interactive placement (2D)
- `BuildingPlacementSystem::tryPlaceBuilding(...)` – high-level API

**Validation / collision techniques:**
- **Screen boundary checks**
  - Simple range checks with a margin (e.g. 60px) from each screen edge
- **Axis-Aligned Bounding Box (AABB) collision** between buildings
  - Treat each building footprint as a rectangle
  - Two rectangles collide if their projections on X and Y overlap
- **Circle vs Building checks** for parks and the fountain
  - Treat parks/fountain as circles (center + radius)
  - Use distance-based tests or circle-box overlap
- **Road proximity checks**
  - Roads are discrete points (from Bresenham line)
  - Skip or reject buildings whose footprint is too close to any road point

This collision feature is crucial for producing a city that **looks
plausible**, with clean separation between roads, green spaces, and
buildings.

---

## 5. Generation System

### 5.1 CityGenerator

- Creates complete `CityData` (roads, parks, fountain, buildings)
- Uses `RoadGenerator` internally for road layouts
- Applies multiple validation layers for buildings:
  - Screen bounds
  - Parks
  - Fountain
  - Roads
  - Other buildings

**Algorithms and techniques used by CityGenerator:**
- **Midpoint Circle Algorithm** (via `midpointCircle`) for:
  - Generating point sets for park boundaries
  - Generating the central fountain circle
- **AABB collision** for building-building overlap detection
- **Circle-box and point-in-circle tests** for buildings vs.
  parks/fountain
- **Random sampling with rejection** for choosing building locations
  until a valid (non-colliding) spot is found

### 5.2 RoadGenerator

- Implements 3 layout patterns:
  - **Grid**: Regular Manhattan-style
  - **Radial**: Centered radial spokes
  - **Random**: Organic road network
- Uses **Bresenham's Line Algorithm** to sample precise road points.
- Carves out roads around parks/fountain by removing points that fall inside circles.

**Algorithms used by RoadGenerator:**
- **Bresenham's Line Algorithm** for rasterizing each road segment as a
  sequence of integer points. This guarantees clean, gap-free roads.
- **Midpoint Circle Algorithm** (indirectly) for radial patterns and for
  avoiding circular regions (parks/fountain) by filtering road points.

---

## 6. Rendering System

### 6.1 CityRenderer

- Holds VAOs/VBOs for buildings, roads, parks, fountain, and traffic.
- Rebuilds meshes when city or traffic data changes.
- Has separate 2D and 3D rendering paths.

**Rendering-related features and algorithms:**
- **Procedural mesh generation**
  - Buildings: generated as textured boxes based on `Building` width,
    depth, and height.
  - Roads: quads or strips built from 2D road points.
  - Parks & fountain: disks/low-poly cylinders created from circle
    point sets.
- **View-dependent rendering**
  - 2D mode: orthographic projection, top-down view, simplified meshes.
  - 3D mode: perspective projection, FPP camera.

### 6.2 Camera (3D Navigation)

- FPP camera with:
  - `W/A/S/D` for planar movement
  - `SHIFT` for sprint
  - Mouse for yaw/pitch
- Integrated via `Application` callbacks and processed each frame.

This forms a **3D navigation feature** on top of the static city:
- Smooth movement with configurable speed
- Mouse-look using accumulated yaw/pitch
- Sprinting when `SHIFT` is held

### 6.3 Shaders & Textures

- `ShaderManager` compiles vertex + fragment shaders and exposes uniform setters.
- `TextureManager` loads 6 textures (brick, concrete, glass, road, grass, fountain).
- Building meshes use different textures for walls, windows, roads, and ground.

---

## 7. Input & Controls

### 7.1 Keyboard

See `InputHandler::displayControls()` for the full ASCII table. Key groups:

- **Building Controls**: `1/2/3/4/B`
- **Road Controls**: `R`, `5/6`
- **Skyline & Texture**: `L`, `T`
- **Parks/Fountain**: `7/8/9/0/F`
- **Time of Day**: `N` (auto), `M` (+2 hours)
- **View & Generation**: `V`, `G`, `Z`, `X`, `P`, `H`, `ESC`
- **3D Navigation**: `W/A/S/D`, `SHIFT`, mouse look
- **2D Building Placement**: Left click

Behind these controls, `InputHandler` implements a small **input state
machine**:
- Tracks which keys were just pressed vs. held (edge-triggered events)
- Uses flags to request actions (generate, load, place building) which
  are then handled in the main loop, keeping rendering logic clean.

### 7.2 Mouse

- **2D Mode**: Left click places a building (if valid).
- **3D Mode**: Mouse moves the camera view.

---

## 8. Build & Run

### 8.1 Prerequisites (macOS)

- Xcode Command Line Tools
- Homebrew-installed GLFW3:

```bash
brew install glfw
```

### 8.2 Build

```bash
bash build.sh
```

### 8.3 Run

```bash
./CityDesigner
```

---

## 9. Extensibility

To add a new feature:

1. Create a new folder under `src/features/` and matching headers under `include/features/`.
2. Implement your feature as a self-contained class with a clear responsibility.
3. Wire it into `main.cpp` only via high-level methods (init, update, render hooks).
4. Optionally extend `CityConfig` to add configuration knobs.
5. Document your feature in this file and in a dedicated header comment.

Examples of future features:
- Weather system (rain, fog, snow)
- Pedestrian system (people walking on sidewalks)
- Traffic lights and intersections
- Advanced camera paths (cinematic flythroughs)

---

## 10. Algorithms by Feature / System

The following table summarizes which algorithms are used where, and for what purpose.

| Area / Feature                                | Algorithms / Techniques                         | What They Do                                                                                 |
|----------------------------------------------|-------------------------------------------------|----------------------------------------------------------------------------------------------|
| **Road generation (RoadGenerator)**          | **Bresenham’s Line Algorithm**                  | Rasterizes each road segment into contiguous integer points; used for Grid/Radial/Random.    |
|                                              | Midpoint Circle (indirect, via circle masks)   | Helps carve roads around circular regions like parks/fountain by filtering road points.      |
| **Parks & fountain (CityGenerator)**         | **Midpoint Circle Algorithm**                   | Generates park and fountain perimeters as precise circles of points.                         |
|                                              | Point-in-circle checks                          | Ensures roads/buildings don’t occupy park/fountain areas.                                    |
| **Building generation (CityGenerator)**      | Random sampling + rejection                     | Picks random candidate positions and keeps only those that pass all collision checks.        |
|                                              | **AABB collision (building vs building)**       | Prevents overlapping building footprints using rectangle–rectangle overlap tests.            |
|                                              | Circle-box / point-in-circle (vs parks/fountain)| Ensures buildings stay outside circular green/fountain zones.                                |
|                                              | Distance checks vs road points                  | Keeps buildings a safe buffer away from all road samples.                                    |
| **Click-to-place buildings (Feature 4)**     | Same as above (AABB, circle, distance checks)   | Reuses all CityGenerator collision logic for interactive placement in 2D.                    |
| **Traffic system (Feature 3)**               | Road-following along Bresenham points           | Cars move along road point chains with progress values instead of continuous geometry only.  |
|                                              | Distance-based separation between cars          | Simple collision-avoidance so cars don’t overlap visually.                                   |
| **Day/Night cycle (Feature 2)**              | Piecewise linear interpolation (`glm::mix`)     | Blends sky colors smoothly between time ranges (night, sunrise, day, sunset, dusk).          |
|                                              | Time wrapping & step integration                | Advances time at 0.5 hours per real second; wraps at 24 hours.                               |
| **Building window lights (Feature 1)**       | Threshold + interpolation over time             | Turns lights on/off with smooth fades around sunrise/sunset by interpolating intensities.    |
| **Rendering / meshes**                       | Procedural mesh generation                      | Builds vertex data for buildings, roads, parks, fountain, and traffic directly from data.    |
|                                              | 2D orthographic + 3D perspective transforms     | Uses different projection matrices for 2D map vs 3D FPP camera views.                        |
| **Camera & navigation (3D)**                 | Euler-based yaw/pitch camera                    | Computes view matrix from position + yaw/pitch controlled by mouse and WASD/Shift keys.      |
| **Input handling**                           | Edge-triggered key detection                    | Tracks previous vs current key state to detect just-pressed events reliably.                 |
|                                              | Flag-based command queue                        | Sets flags (generate, load, place) for main loop to consume, keeping logic decoupled.        |

For deeper details of each algorithm, see `include/utils/algorithms.h` and `COMPLETE_CODE_DOCUMENTATION.md`.

---

## 11. Summary

City Designer is now a **clean, fully documented, feature-based** C++/OpenGL project. This document summarizes:

- High-level architecture
- Main application flow
- All 5 creative features
- Generation and rendering systems
- Input scheme and controls
- Build and extensibility guidelines

For deeper technical details (per-class and per-method), see:
- `COMPLETE_CODE_DOCUMENTATION.md`
- Header comments in `include/`

This `PROJECT_DOCUMENTATION.md` is meant as your **go-to overview** when you (or another developer) want to quickly understand how the entire project fits together.
