# 🏙️ City Designer - Interactive 3D City Generation Tool

**SE3032 – Graphics and Visualization Assignment**  
_Interactive Algorithmic Art & 3D Environments_

[![C++](https://img.shields.io/badge/C++-11-blue.svg)](https://isocpp.org/)
[![OpenGL](https://img.shields.io/badge/OpenGL-3.3-green.svg)](https://www.opengl.org/)
[![Platform](https://img.shields.io/badge/Platform-macOS-lightgrey.svg)](https://www.apple.com/macos/)

---

## 📋 Project Overview

City Designer is a sophisticated OpenGL-based application that generates procedural cities with both 2D and 3D visualization modes. Built with C++11, it demonstrates advanced computer graphics techniques, software engineering best practices, and modular code organization.

City Designer is an interactive application that allows users to design and visualize custom futuristic cities by combining procedural 2D artwork with immersive 3D environments. The project demonstrates mastery of fundamental graphics algorithms including Bresenham's Line Algorithm, Midpoint Circle Algorithm, and 3D texture mapping.

### ✨ Key Features

- ✅ **Dual Rendering Modes**: Switch between 2D orthographic and 3D perspective views
- ✅ **Multiple City Patterns**: Grid, Radial, and Random road layouts
- ✅ **Dynamic Building Generation**: Configurable building heights and densities
- ✅ **Textured 3D Models**: JPG-based textures for buildings, roads, parks, and fountains
- ✅ **FPP Camera Controls**: WASD movement with mouse look in 3D mode
- ✅ **Real-time Configuration**: Modify city parameters on-the-fly with keyboard shortcuts
- ✅ **Professional Code Structure**: Organized, documented, and maintainable codebase

## ✅ Implemented Features

### Core Algorithms

- ✅ **Bresenham's Line Algorithm** - Pixel-perfect road generation
- ✅ **Midpoint Circle Algorithm** - Circular parks and fountains
- ✅ **Basic OpenGL Lines** - Road networks and city grids
- ⏳ **3D Models with Texture Mapping** - In progress

### Interactive Controls

- ✅ **Building Configuration** - Adjust number and layout size
- ✅ **Road Pattern Selection** - Grid, Radial, or Random layouts
- ✅ **Skyline Types** - Low-Rise, Mid-Rise, Skyscraper, Mixed
- ✅ **Texture Themes** - Modern, Classic, Industrial, Futuristic
- ✅ **Park & Fountain Controls** - Customizable sizes and quantities

---

## 🎯 Technologies Used

### Core Technologies

- **Language**: C++11
- **Graphics API**: OpenGL 3.3 Core Profile
- **Window Management**: GLFW3
- **Mathematics**: GLM 1.0.2
- **Image Loading**: STB Image (header-only library)
- **OpenGL Loader**: GLAD

### Algorithms Implemented

- **Bresenham's Line Algorithm** - For pixel-perfect road generation
- **Midpoint Circle Algorithm** - For parks and fountains
- **Perspective Projection** - 3D camera mathematics
- **Mesh Generation** - Procedural 3D geometry creation

---

## 🚀 Quick Start

### Prerequisites

- macOS (tested on macOS with Homebrew)
- Clang compiler with C++11 support
- GLFW3 library
- GLM library (included in lib/ folder)

### Installation

```bash
# Install GLFW via Homebrew (if not already installed)
brew install glfw

# Clone the repository
git clone https://github.com/Dilhara-Jayashan/GV-city_designer.git
cd GV-city_designer

# Build the project
chmod +x build.sh
./build.sh

# Run the application
./CityDesigner
```

---

## 🎮 Keyboard Controls

### Building Controls

| Key | Action                       |
| --- | ---------------------------- |
| `1` | Decrease number of buildings |
| `2` | Increase number of buildings |
| `3` | Decrease layout size         |
| `4` | Increase layout size         |

### Road Controls

| Key | Action                                      |
| --- | ------------------------------------------- |
| `R` | Cycle road pattern (Grid → Radial → Random) |
| `5` | Decrease road width                         |
| `6` | Increase road width                         |

### Skyline Controls

| Key | Action                                                        |
| --- | ------------------------------------------------------------- |
| `S` | Cycle skyline type (Low-Rise → Mid-Rise → Skyscraper → Mixed) |

### Texture Controls

| Key | Action                                                           |
| --- | ---------------------------------------------------------------- |
| `T` | Cycle texture theme (Modern → Classic → Industrial → Futuristic) |

### Park & Fountain Controls

| Key | Action                             |
| --- | ---------------------------------- |
| `7` | Decrease park radius               |
| `8` | Increase park radius               |
| `9` | Decrease number of parks           |
| `0` | Increase number of parks           |
| `F` | Toggle fountain size (small/large) |

### View & Generation

| Key   | Action                                  |
| ----- | --------------------------------------- |
| `V`   | Toggle 2D/3D view mode                  |
| `G`   | Generate new city with current settings |
| `P`   | Print current configuration to console  |
| `H`   | Display help menu                       |
| `ESC` | Exit application                        |

### 3D Camera Controls (3D Mode Only)

| Control | Action                      |
| ------- | --------------------------- |
| `W`     | Move forward                |
| `A`     | Strafe left                 |
| `S`     | Move backward               |
| `D`     | Strafe right                |
| `Mouse` | Look around (yaw and pitch) |
| `Shift` | Sprint (2x speed)           |

---

## 🛠️ Compilation & Running

### Requirements

- C++11 or higher
- OpenGL 3.3+
- GLFW3
- GLAD

### macOS Compilation

```bash
clang++ src/main.cpp src/glad.c src/algorithms.cpp src/city_config.cpp src/input_handler.cpp \
    -o CityDesigner \
    -Iinclude \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    -lglfw \
    -framework OpenGL \
    -std=c++11
```

### Running

```bash
./CityDesigner
```

---

## 📁 Project Structure

```
GV-city_designer/
├── assets/                 # Texture files (JPG)
├── include/               # Header files
│   ├── core/             # Configuration and data structures
│   ├── generation/       # City generation logic
│   ├── rendering/        # Rendering systems (2D, 3D, textures, camera)
│   └── utils/            # Algorithms and input handling
├── src/                  # Implementation files
│   ├── core/            # Core implementations
│   ├── generation/      # Generation implementations
│   ├── rendering/       # Rendering implementations
│   ├── utils/          # Utility implementations
│   └── main.cpp        # Application entry point
├── docs/                # Documentation
└── build.sh            # Build script
```

See `docs/PROJECT_STRUCTURE.md` for detailed architecture documentation.

---

## 🏗️ Architecture Highlights

### Modular Design

- **Core Module**: Configuration and data structures
- **Generation Module**: City generation algorithms
- **Rendering Module**: 2D/3D rendering, textures, camera
- **Utils Module**: Algorithms and input handling

### Code Quality

- ✅ Doxygen-style documentation
- ✅ Single Responsibility Principle
- ✅ Clear separation of concerns
- ✅ Professional naming conventions
- ✅ Comprehensive error handling

## 📊 Project Statistics

- **Total Lines of Code**: ~3,500+
- **Modules**: 4 (Core, Generation, Rendering, Utils)
- **Textures**: 6 JPG files
- **City Elements**: Roads, Buildings, Parks, Fountains
- **Road Patterns**: 3 (Grid, Radial, Random)
- **View Modes**: 2 (2D Orthographic, 3D Perspective)

---

## 🔬 Technical Implementation

### 1. Bresenham's Line Algorithm

**Purpose**: Generate pixel-perfect straight lines for roads and city grids

**Features**:

- Integer-only arithmetic for efficiency
- Handles all octants (8 directions)
- Used for: roads, building outlines, grid boundaries

**Performance**:

- Horizontal road: 601 points generated
- Vertical road: 401 points generated
- Diagonal road: 601 points generated

### 2. Midpoint Circle Algorithm

**Purpose**: Create circular shapes for parks, fountains, and roundabouts

**Features**:

- 8-way symmetry optimization
- Calculates one octant, mirrors for others
- Variable radius support

**Performance**:

- Large park (r=80): 464 points
- Medium fountain (r=50): 296 points
- Small roundabout (r=30): 176 points

### 3. User Input System

**Purpose**: Real-time interactive city configuration

**Features**:

- Non-blocking input processing
- Key state tracking (prevents repeated actions)
- Dynamic parameter adjustment
- Real-time feedback in console

---

## 🎨 Current Visualization

The application displays:

- **Yellow/Orange Lines**: Roads using Bresenham's Algorithm
- **Green Circles**: Parks using Midpoint Circle Algorithm
- **Blue Circles**: Fountains
- **Yellow Circles**: Roundabouts
- **Dark Blue Background**: City at dusk atmosphere

---

## 🎓 Academic Relevance

### Computer Graphics Concepts

✅ 2D Rasterization Algorithms  
✅ 3D Transformations & Matrices
✅ Texture Mapping & Mipmapping  
✅ Camera Mathematics  
✅ Procedural Mesh Generation  
✅ Depth Testing & Z-Buffer

### Software Engineering Principles

✅ Modularity & Encapsulation  
✅ Documentation Best Practices  
✅ Code Organization & Maintainability  
✅ Design Patterns (Singleton, Strategy, Factory)  
✅ Performance Optimization

---

## 🎯 Assignment Compliance

### Part 1 Requirements (50%)

✅ **Basic OpenGL Lines** - Implemented for rendering  
✅ **Bresenham's Line Algorithm** - Fully functional  
✅ **Midpoint Circle Algorithm** - Fully functional  
⏳ **3D Model with Texture Mapping** - Next milestone

---

## 📚 Documentation

- **README.md** (this file) - Quick start and overview
- **docs/PROJECT_STRUCTURE.md** - Comprehensive architecture documentation
- **docs/REORGANIZATION_GUIDE.md** - Development process guide
- **Header Files** - Inline API documentation with Doxygen comments

---

## 👥 Team Information

**Course**: SE3032 – Graphics and Visualization  
**Semester**: Semester 1, 2025  
**Submission Deadline**: November 26, 2025  
**Evaluation**: November 27, 2025

## 🙏 Acknowledgments

- Graphics Algorithms: Bresenham, Midpoint Circle Algorithm inventors
- GLM Library: OpenGL Mathematics library developers
- STB Image: Sean Barrett
- GLFW: Cross-platform window management
- OpenGL Community: Documentation and tutorials

---

**Built with using C++, OpenGL, and computational geometry!**

_Last Updated: November 25, 2025_

**Dilhara Jayashan**

GitHub: [@Dilhara-Jayashan](https://github.com/Dilhara-Jayashan)

## 🙏 Acknowledgments

- Graphics Algorithms: Bresenham, Midpoint Circle Algorithm inventors
- GLM Library: OpenGL Mathematics library developers
- STB Image: Sean Barrett
- GLFW: Cross-platform window management
- OpenGL Community: Documentation and tutorials

---

**Built with ❤️ using C++, OpenGL, and computational geometry!**

_Last Updated: November 25, 2025_
