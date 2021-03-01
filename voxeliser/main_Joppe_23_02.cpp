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
// some testing
void centerOfVoxel_(int x, int y, int z, Point* rMin, Point* vSize, Point* voxelCenter) {
    voxelCenter->x = rMin->x + ((double)x) * vSize->x + 0.5 * vSize->x;
    voxelCenter->y = rMin->y + ((double)y) * vSize->y + 0.5 * vSize->y;
    voxelCenter->z = rMin->z + ((double)z) * vSize->z + 0.5 * vSize->z;
}

void centerOfVoxel(Rows* voxel, Point* rMin, Point *vSize, Point* voxelCenter) {
    voxelCenter->x = rMin->x + ((double)voxel->x) * vSize->x + 0.5 * vSize->x;
    voxelCenter->y = rMin->y + ((double)voxel->y) * vSize->y + 0.5 * vSize->y;
    voxelCenter->z = rMin->z + ((double)voxel->z) * vSize->z + 0.5 * vSize->z;
}

int main(int argc, const char* argv[]) {
    // I have added the path to the obj file. Local path so static and not dynamic. 
    const char* file_in = "C:/Users/s161887/Documents/GitHub/geo1004_hw01_test/geo1004_hw01_test/bag_bk.obj";
    const char* file_out = "vox.obj";
    float voxel_size = 1.0;
    Point vSize;
    vSize.x = 1.0;
    vSize.y = 1.0;
    vSize.z = 1.0;

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
                std::vector<unsigned int> temp = { f1, f2, f3 };
                faces.push_back(temp);
        }
    }

    //Obtaining the Bounding Box

    // creating vector for each dimension x,y,z
    std::vector<float> x_dim;
    std::vector<float> y_dim;
    std::vector<float> z_dim;

    for (std::vector<Point>::const_iterator i = vertices.begin(); i != vertices.end(); ++i) {
        //std::cout << i[0][1]<<' ';
        x_dim.push_back(i[0][0]);
        y_dim.push_back(i[0][1]);
        z_dim.push_back(i[0][2]);
    }

    // obtaining index of min/max
    std::vector<float>::iterator result_maxx;
    std::vector<float>::iterator result_maxy;
    std::vector<float>::iterator result_maxz;

    std::vector<float>::iterator result_minx;
    std::vector<float>::iterator result_miny;
    std::vector<float>::iterator result_minz;

    result_maxx = std::max_element(x_dim.begin(), x_dim.end());
    result_maxy = std::max_element(y_dim.begin(), y_dim.end());
    result_maxz = std::max_element(z_dim.begin(), z_dim.end());

    result_minx = std::min_element(x_dim.begin(), x_dim.end());
    result_miny = std::min_element(y_dim.begin(), y_dim.end());
    result_minz = std::min_element(z_dim.begin(), z_dim.end());

    int index_x = std::distance(x_dim.begin(), result_maxx);
    int index_y = std::distance(y_dim.begin(), result_maxy);
    int index_z = std::distance(z_dim.begin(), result_maxz);

    int index_minx = std::distance(x_dim.begin(), result_minx);
    int index_miny = std::distance(y_dim.begin(), result_miny);
    int index_minz = std::distance(z_dim.begin(), result_minz);

    //vector indexing
    float min_x = x_dim[index_minx];
    float min_y = y_dim[index_miny];
    float min_z = z_dim[index_minz];

    float max_x = x_dim[index_x];
    float max_y = y_dim[index_y];
    float max_z = z_dim[index_z];


    std::cout << "max_x: " << max_x << '\n' << "max_y: " << max_y << '\n' << "max_z: " << max_z << '\n' << '\n';
    std::cout << "min_x: " << min_x << '\n' << "min_y: " << min_y << '\n' << "min_z: " << min_z;

    //determine number of voxel cells between min and max of x,y,z
    int no_x = 0;
    int no_y = 0;
    int no_z = 0;

    if (std::fmod((max_x - min_x), voxel_size) == 0) {
        no_x = ((max_x - min_x) / voxel_size);
    }
    else {
        no_x = ((max_x - min_x) / voxel_size) + 1;
    }
    // shouldn't + voxel size not be + 1? no_x is the number of voxels in x direction right?

    if (std::fmod((max_y - min_y), voxel_size) == 0) {
        no_y = ((max_y - min_y) / voxel_size);
    }
    else {
        no_y = ((max_y - min_y) / voxel_size) + 1;
    }
    std::cout << '\n' << no_y;

    if (std::fmod((max_z - min_z), voxel_size) == 0) {
        no_z = int((max_z - min_z) / voxel_size);
    }
    else {
        no_z = int((max_z - min_z) / voxel_size) + 1;
    }

    std::cout << '\n' << "x:  " << no_x << "  y: " << no_y << "   z:  " << no_z;

    // create vector for bbox
    std::vector<Point> bbox;
    bbox.emplace_back(min_x, min_y, min_z);
    bbox.emplace_back(min_x + no_x * voxel_size, min_y + no_y * voxel_size, min_z + no_z * voxel_size);

    for (std::vector<Point>::const_iterator i = bbox.begin(); i != bbox.end(); ++i) {
        std::cout << '\n' << *i << ' ';
    }

    // some testing
    Rows rows = {no_x, no_y, no_z};

    // to do
    VoxelGrid voxels(rows.x, rows.y, rows.z); 

    //some testing
    int x, y, z;
    Point* vSize;
    Point rMin, rMax;
    Rows vMax;

    for (x = 0; x <= no_x; ++x) {
        for (y = 0; y <= no_y; ++y) {
            for (z = 0; z <= no_z; ++z) {
                Point voxelCenter;
                centerOfVoxel_(x, y, z, &rMin, vSize, &voxelCenter);
                printf("\nVoxel: ");
            }
        }
    }

    // Voxelise
    for (auto const& triangle : faces) {
        // to do
    }

    // intersection method code

    // Fill model
    // to do

    // Write voxels
    // to do

    return 0;
}
