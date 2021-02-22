#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <assert.h>

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

    // What I did myself
    std::string line;

    std::fstream objfile;
    objfile.open(file_in);
    // Test if the file is opened
    if (objfile.is_open()) { std::cout << "true"; /* ok, proceed with output*/ }
    else { std::cout << "false"; }

    if (objfile.is_open())
    {
        while (getline(objfile, line))
        {
            std::cout << line << '\n';
        }
    }

    objfile.close();

    // Create grid
    Rows rows;
    // to do
    VoxelGrid voxels(rows.x, rows.y, rows.z);

    voxels.max_x = 2;
    voxels.max_y = 2;
    voxels.max_z = 2;

    std::cout << voxels;

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
