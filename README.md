# 🏙️ City Designer - Interactive 3D City Generation Tool

**SE3032 – Graphics and Visualization Assignment**  
*Interactive Algorithmic Art & 3D Environments*

---

## 📋 Project Overview

City Designer is an interactive application that allows users to design and visualize custom futuristic cities by combining procedural 2D artwork with immersive 3D environments. The project demonstrates mastery of fundamental graphics algorithms including Bresenham's Line Algorithm, Midpoint Circle Algorithm, and 3D texture mapping.

---

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
- ✅ **View Mode Toggle** - Switch between 2D and 3D views

---

## 🎮 Keyboard Controls

### Building Controls
| Key | Action |
|-----|--------|
| `1` | Decrease number of buildings |
| `2` | Increase number of buildings |
| `3` | Decrease layout size |
| `4` | Increase layout size |

### Road Controls
| Key | Action |
|-----|--------|
| `R` | Cycle road pattern (Grid → Radial → Random) |
| `5` | Decrease road width |
| `6` | Increase road width |

### Skyline Controls
| Key | Action |
|-----|--------|
| `S` | Cycle skyline type (Low-Rise → Mid-Rise → Skyscraper → Mixed) |

### Texture Controls
| Key | Action |
|-----|--------|
| `T` | Cycle texture theme (Modern → Classic → Industrial → Futuristic) |

### Park & Fountain Controls
| Key | Action |
|-----|--------|
| `7` | Decrease park radius |
| `8` | Increase park radius |
| `9` | Decrease number of parks |
| `0` | Increase number of parks |
| `F` | Toggle fountain size (small/large) |

### View & Generation
| Key | Action |
|-----|--------|
| `V` | Toggle 2D/3D view mode |
| `G` | Generate new city with current settings |
| `P` | Print current configuration to console |
| `H` | Display help menu |
| `ESC` | Exit application |

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
├── include/
│   ├── algorithms.h         # Algorithm declarations
│   ├── city_config.h        # City configuration structure
│   ├── input_handler.h      # Input management
│   ├── glad/                # OpenGL loader
│   └── KHR/                 # Platform definitions
├── src/
│   ├── main.cpp             # Main program & rendering
│   ├── algorithms.cpp       # Bresenham & Midpoint Circle
│   ├── city_config.cpp      # Configuration management
│   ├── input_handler.cpp    # Keyboard input handling
│   └── glad.c               # GLAD implementation
├── lib/                     # External libraries
├── CityDesigner             # Executable
├── PROGRESS.md              # Development progress tracker
└── README.md                # This file
```

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

## 🎯 Assignment Compliance

### Part 1 Requirements (50%)
✅ **Basic OpenGL Lines** - Implemented for rendering  
✅ **Bresenham's Line Algorithm** - Fully functional  
✅ **Midpoint Circle Algorithm** - Fully functional  
⏳ **3D Model with Texture Mapping** - Next milestone  

---

## 👥 Team Information

**Course**: SE3032 – Graphics and Visualization  
**Semester**: Semester 1, 2025  
**Submission Deadline**: November 26, 2025  
**Evaluation**: November 27, 2025  

---

**Last Updated**: November 24, 2025
