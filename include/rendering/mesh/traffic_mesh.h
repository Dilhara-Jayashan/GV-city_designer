#ifndef TRAFFIC_MESH_H
#define TRAFFIC_MESH_H

#include <vector>
#include "features/traffic_system/traffic_generator.h"

// Generate 3D mesh for a single car (small cube)
std::vector<float> carTo3DMesh(const Car& car, int screenWidth, int screenHeight);

// Generate 2D vertices for a car (single point)
std::vector<float> carTo2DVertices(const Car& car, int screenWidth, int screenHeight);

#endif
