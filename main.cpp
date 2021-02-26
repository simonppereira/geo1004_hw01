#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <assert.h>
#include <cmath>
#include <algorithm>


#include "Point.h"
#include "Rows.h"
#include "VoxelGrid.h"

#include <stdio.h>

float signed_volume(const Point &a, const Point &b, const Point &c, const Point &d) {
  // to do
    return 0;
}

bool intersects(const Point &orig, const Point &dest, const Point &v0, const Point &v1, const Point &v2) {
  // to do
    return 0;
}

int main(int argc, const char* argv[]) {
    // I have added the path to the obj file. Local path so static and not dynamic. 
    const char* file_in = "C:/Users/s161887/Documents/GitHub/geo1004_hw01_test/geo1004_hw01_test/bag_bk.obj";
    const char* file_out = "vox.obj";
    float voxel_size = 1.0;

    // Read file

    std::vector<Point> vertices;
    std::vector<std::vector<unsigned int>> faces;
    std::string line;
    //std::string type;
    std::ifstream in(file_in, std::ios::in);
    
    if (!in)
    {
        std::cerr << "Cannot open" << file_in << std::endl;
        exit(1);
    }

    while (std::getline(in, line))
    {
        std::istringstream vert(line);
        vert >> line;
            if (line == "v") {
                float x, y, z;
                vert >> x >> y >> z;
                vertices.emplace_back(x, y, z);
        }
            if (line == "f") {
                unsigned int f1, f2, f3;
                vert >> f1 >> f2 >> f3;
                std::cout << f1;
                std::vector<unsigned int> temp = { f1, f2, f3 };
                faces.push_back(temp);
        }
    }

    //print the vertices for testing -> works
    for (std::vector<Point>::const_iterator i = vertices.begin();
        i != vertices.end();
        ++i)
        std::cout << *i << ' ';

    //works
    auto xExtremes = std::minmax_element(vertices.begin(), vertices.end(), [](Point const &lhs, Point const &rhs) {return lhs.x < rhs.x;});
    auto yExtremes = std::minmax_element(vertices.begin(), vertices.end(), [](Point const& lhs, Point const& rhs) {return lhs.y < rhs.y;});

    //does not work
    Point upperLeft(xExtremes.first->x, yExtremes.first->y);
    Point lowerRight(xExtremes.second->x, yExtremes.second->y);

    // Create grid
    // some testing
    Rows rows = {5, 5, 5};

    //some testing
    std::cout << rows;

    // to do
    VoxelGrid voxels(rows.x, rows.y, rows.z); 

    //some testing
    voxels = { 10, 10, 10 };

    // Voxelise
    for (auto const& triangle : faces) {
        // to do
    }

    // Fill model
    // to do

    // Write voxels
    // to do

    //return 0;
}
