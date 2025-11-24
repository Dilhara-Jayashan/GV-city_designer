# 🏙️ City Designer - Interactive 3D City Generation Tool# 🏙️ City Designer - Interactive 3D City Generation Tool



[![C++](https://img.shields.io/badge/C++-11-blue.svg)](https://isocpp.org/)**SE3032 – Graphics and Visualization Assignment**  

[![OpenGL](https://img.shields.io/badge/OpenGL-3.3-green.svg)](https://www.opengl.org/)*Interactive Algorithmic Art & 3D Environments*

[![Platform](https://img.shields.io/badge/Platform-macOS-lightgrey.svg)](https://www.apple.com/macos/)

---

## 📋 Project Overview

## 📋 Project Overview

City Designer is a sophisticated OpenGL-based application that generates procedural cities with both 2D and 3D visualization modes. Built with C++11, it demonstrates advanced computer graphics techniques, software engineering best practices, and modular code organization.

City Designer is an interactive application that allows users to design and visualize custom futuristic cities by combining procedural 2D artwork with immersive 3D environments. The project demonstrates mastery of fundamental graphics algorithms including Bresenham's Line Algorithm, Midpoint Circle Algorithm, and 3D texture mapping.

### ✨ Key Features

- ✅ **Dual Rendering Modes**: Switch between 2D orthographic and 3D perspective views---

- ✅ **Multiple City Patterns**: Grid, Radial, and Random road layouts  

- ✅ **Dynamic Building Generation**: Configurable building heights and densities## ✅ Implemented Features

- ✅ **Textured 3D Models**: JPG-based textures for buildings, roads, parks, and fountains

- ✅ **FPP Camera Controls**: WASD movement with mouse look in 3D mode### Core Algorithms

- ✅ **Real-time Configuration**: Modify city parameters on-the-fly with keyboard shortcuts- ✅ **Bresenham's Line Algorithm** - Pixel-perfect road generation

- ✅ **Professional Code Structure**: Organized, documented, and maintainable codebase- ✅ **Midpoint Circle Algorithm** - Circular parks and fountains

- ✅ **Basic OpenGL Lines** - Road networks and city grids

## 🎯 Technologies Used- ⏳ **3D Models with Texture Mapping** - In progress



### Core Technologies### Interactive Controls

- **Language**: C++11- ✅ **Building Configuration** - Adjust number and layout size

- **Graphics API**: OpenGL 3.3 Core Profile  - ✅ **Road Pattern Selection** - Grid, Radial, or Random layouts

- **Window Management**: GLFW3- ✅ **Skyline Types** - Low-Rise, Mid-Rise, Skyscraper, Mixed

- **Mathematics**: GLM 1.0.2- ✅ **Texture Themes** - Modern, Classic, Industrial, Futuristic

- **Image Loading**: STB Image (header-only library)- ✅ **Park & Fountain Controls** - Customizable sizes and quantities

- **OpenGL Loader**: GLAD- ✅ **View Mode Toggle** - Switch between 2D and 3D views



### Algorithms Implemented---

- **Bresenham's Line Algorithm** - For pixel-perfect road generation

- **Midpoint Circle Algorithm** - For parks and fountains## 🎮 Keyboard Controls

- **Perspective Projection** - 3D camera mathematics

- **Mesh Generation** - Procedural 3D geometry creation### Building Controls

| Key | Action |

## 🚀 Quick Start|-----|--------|

| `1` | Decrease number of buildings |

### Prerequisites| `2` | Increase number of buildings |

- macOS (tested on macOS with Homebrew)| `3` | Decrease layout size |

- Clang compiler with C++11 support| `4` | Increase layout size |

- GLFW3 library

- GLM library (included in lib/ folder)### Road Controls

| Key | Action |

### Installation|-----|--------|

| `R` | Cycle road pattern (Grid → Radial → Random) |

```bash| `5` | Decrease road width |

# Install GLFW via Homebrew (if not already installed)| `6` | Increase road width |

brew install glfw

### Skyline Controls

# Clone the repository  | Key | Action |

git clone https://github.com/Dilhara-Jayashan/GV-city_designer.git|-----|--------|

cd GV-city_designer| `S` | Cycle skyline type (Low-Rise → Mid-Rise → Skyscraper → Mixed) |



# Build the project### Texture Controls

chmod +x build.sh| Key | Action |

./build.sh|-----|--------|

| `T` | Cycle texture theme (Modern → Classic → Industrial → Futuristic) |

# Run the application

./CityDesigner### Park & Fountain Controls

```| Key | Action |

|-----|--------|

## 🎮 Controls| `7` | Decrease park radius |

| `8` | Increase park radius |

### View and Generation| `9` | Decrease number of parks |

| Key | Action || `0` | Increase number of parks |

|-----|--------|| `F` | Toggle fountain size (small/large) |

| `G` | Generate new city with current settings |

| `V` | Toggle between 2D and 3D view modes |### View & Generation

| `H` | Display help menu || Key | Action |

| `P` | Print current configuration ||-----|--------|

| `ESC` | Exit application || `V` | Toggle 2D/3D view mode |

| `G` | Generate new city with current settings |

### Building Configuration  | `P` | Print current configuration to console |

| Key | Action || `H` | Display help menu |

|-----|--------|| `ESC` | Exit application |

| `1` / `2` | Decrease / Increase number of buildings |

| `3` / `4` | Decrease / Increase layout grid size |---

| `B` | Toggle standard/random building sizes |

## 🛠️ Compilation & Running

### Road Configuration

| Key | Action |### Requirements

|-----|--------|- C++11 or higher

| `R` | Cycle through road patterns (Grid → Radial → Random) |- OpenGL 3.3+

| `5` / `6` | Decrease / Increase road width |- GLFW3

- GLAD

### Skyline and Textures

| Key | Action |### macOS Compilation

|-----|--------|```bash

| `L` | Cycle skyline types (Low-Rise → Mid-Rise → Skyscraper → Mixed) |clang++ src/main.cpp src/glad.c src/algorithms.cpp src/city_config.cpp src/input_handler.cpp \

| `T` | Cycle texture themes (Modern → Classic → Industrial → Futuristic) |    -o CityDesigner \

    -Iinclude \

### Parks and Fountains    -I/opt/homebrew/include \

| Key | Action |    -L/opt/homebrew/lib \

|-----|--------|    -lglfw \

| `7` / `8` | Decrease / Increase park radius |    -framework OpenGL \

| `9` / `0` | Decrease / Increase number of parks |    -std=c++11

| `F` | Toggle fountain size (small/large) |```



### 3D Camera Controls (3D Mode Only)### Running

| Control | Action |```bash

|---------|--------|./CityDesigner

| `W` | Move forward |```

| `A` | Strafe left |

| `S` | Move backward |---

| `D` | Strafe right |

| `Mouse` | Look around (yaw and pitch) |## 📁 Project Structure

| `Shift` | Sprint (2x speed) |

```

## 📁 Project StructureGV-city_designer/

├── include/

```│   ├── algorithms.h         # Algorithm declarations

GV-city_designer/│   ├── city_config.h        # City configuration structure

├── assets/                 # Texture files (JPG)│   ├── input_handler.h      # Input management

├── include/               # Header files│   ├── glad/                # OpenGL loader

│   ├── core/             # Configuration and data structures│   └── KHR/                 # Platform definitions

│   ├── generation/       # City generation logic├── src/

│   ├── rendering/        # Rendering systems (2D, 3D, textures, camera)│   ├── main.cpp             # Main program & rendering

│   └── utils/            # Algorithms and input handling│   ├── algorithms.cpp       # Bresenham & Midpoint Circle

├── src/                  # Implementation files│   ├── city_config.cpp      # Configuration management

│   ├── core/            # Core implementations│   ├── input_handler.cpp    # Keyboard input handling

│   ├── generation/      # Generation implementations│   └── glad.c               # GLAD implementation

│   ├── rendering/       # Rendering implementations├── lib/                     # External libraries

│   ├── utils/          # Utility implementations├── CityDesigner             # Executable

│   └── main.cpp        # Application entry point├── PROGRESS.md              # Development progress tracker

├── docs/                # Documentation└── README.md                # This file

└── build.sh            # Build script```

```

---

See `docs/PROJECT_STRUCTURE.md` for detailed architecture documentation.

## 🔬 Technical Implementation

## 🏗️ Architecture Highlights

### 1. Bresenham's Line Algorithm

### Modular Design**Purpose**: Generate pixel-perfect straight lines for roads and city grids

- **Core Module**: Configuration and data structures

- **Generation Module**: City generation algorithms  **Features**:

- **Rendering Module**: 2D/3D rendering, textures, camera- Integer-only arithmetic for efficiency

- **Utils Module**: Algorithms and input handling- Handles all octants (8 directions)

- Used for: roads, building outlines, grid boundaries

### Code Quality

- ✅ Doxygen-style documentation**Performance**: 

- ✅ Single Responsibility Principle- Horizontal road: 601 points generated

- ✅ Clear separation of concerns- Vertical road: 401 points generated

- ✅ Professional naming conventions- Diagonal road: 601 points generated

- ✅ Comprehensive error handling

### 2. Midpoint Circle Algorithm

## 📊 Project Statistics**Purpose**: Create circular shapes for parks, fountains, and roundabouts



- **Total Lines of Code**: ~3,500+**Features**:

- **Modules**: 4 (Core, Generation, Rendering, Utils)- 8-way symmetry optimization

- **Textures**: 6 JPG files- Calculates one octant, mirrors for others

- **City Elements**: Roads, Buildings, Parks, Fountains- Variable radius support

- **Road Patterns**: 3 (Grid, Radial, Random)

- **View Modes**: 2 (2D Orthographic, 3D Perspective)**Performance**:

- Large park (r=80): 464 points

## 🎓 Academic Relevance- Medium fountain (r=50): 296 points

- Small roundabout (r=30): 176 points

### Computer Graphics Concepts

✅ 2D Rasterization Algorithms  ### 3. User Input System

✅ 3D Transformations & Matrices**Purpose**: Real-time interactive city configuration

✅ Texture Mapping & Mipmapping  

✅ Camera Mathematics  **Features**:

✅ Procedural Mesh Generation  - Non-blocking input processing

✅ Depth Testing & Z-Buffer- Key state tracking (prevents repeated actions)

- Dynamic parameter adjustment

### Software Engineering Principles- Real-time feedback in console

✅ Modularity & Encapsulation  

✅ Documentation Best Practices  ---

✅ Code Organization & Maintainability  

✅ Design Patterns (Singleton, Strategy, Factory)  ## 🎨 Current Visualization

✅ Performance Optimization

The application displays:

## 📚 Documentation- **Yellow/Orange Lines**: Roads using Bresenham's Algorithm

- **Green Circles**: Parks using Midpoint Circle Algorithm

- **README.md** (this file) - Quick start and overview- **Blue Circles**: Fountains

- **docs/PROJECT_STRUCTURE.md** - Comprehensive architecture documentation- **Yellow Circles**: Roundabouts

- **docs/REORGANIZATION_GUIDE.md** - Development process guide- **Dark Blue Background**: City at dusk atmosphere

- **Header Files** - Inline API documentation with Doxygen comments

---

## 👥 Author

## 🎯 Assignment Compliance

**Dilhara Jayashan**  

GitHub: [@Dilhara-Jayashan](https://github.com/Dilhara-Jayashan)### Part 1 Requirements (50%)

✅ **Basic OpenGL Lines** - Implemented for rendering  

## 🙏 Acknowledgments✅ **Bresenham's Line Algorithm** - Fully functional  

✅ **Midpoint Circle Algorithm** - Fully functional  

- Graphics Algorithms: Bresenham, Midpoint Circle Algorithm inventors⏳ **3D Model with Texture Mapping** - Next milestone  

- GLM Library: OpenGL Mathematics library developers  

- STB Image: Sean Barrett---

- GLFW: Cross-platform window management

- OpenGL Community: Documentation and tutorials## 👥 Team Information



---**Course**: SE3032 – Graphics and Visualization  

**Semester**: Semester 1, 2025  

**Built with ❤️ using C++, OpenGL, and computational geometry!****Submission Deadline**: November 26, 2025  

**Evaluation**: November 27, 2025  

*Last Updated: November 25, 2025*

---

**Last Updated**: November 24, 2025
