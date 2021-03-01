#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <assert.h>
#include <cmath>
#include <algorithm>
#include <list>
#include <numeric>
#include <random>

#include "Point.h"
#include "Rows.h"
#include "VoxelGrid.h"

#include <stdio.h>

float signed_volume(const Point &a, const Point &b, const Point &c, const Point &d) {
  // to do
    return (((a-d)*((b-d)*(c-d))/6));
}

bool intersects(const Point &orig, const Point &dest, const Point &v0, const Point &v1, const Point &v2) {
    if signed_volume() > 0 on one side && < 0 on other side {
        if()//next if statement
    }
    else go to next voxel-face combination

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

    // Create rows from the boudningbox
    // change to no_x, no_y, no_z but limited to 20 now for the sake of computation time.
    Rows rows;
    rows.x = 20;
    rows.y = 20;
    rows.z = 20;

    // Create the grid with just this line of code? yes but only a grid consisting out of indices
    VoxelGrid voxels(rows.x, rows.y, rows.z);
    
    //const Point& orig, const Point& dest, const Point& v0, const Point& v1, const Point& v2);

    orig.x = min_x;
    orig.y = min_y;
    orig.z = min_z;

    //bool intersects(const Point & orig, const Point & dest, const Point & v0, const Point & v1, const Point & v2) {

    // Iterate over the grid
    for (int i = 0; i < voxels.max_x; i++) {
        for (int j = 0; j < voxels.max_y; j++) {
            for (int k = 0; k < voxels.max_z; k++) {
                std::cout << '\n' << "x:  " << i << "  y: " << j << "   z:  " << k;
                for (std::vector<Point>::const_iterator i = faces.begin(); i != faces.end(); ++i) {
                    if intersects(orig, xxxxxxxxx) = 1 {
                        store the voxel(indices.x, y, z)
                    }
                    else go to next one 
                }
            }
        }

    }

    voxel(5,6,9)

    x_center = min_x + voxel_size*5 + 0.5*voxel_size 
    y_center = min_y + voxel_size*6 + 0.5*voxel_size 
    Z_center = min_z + voxel_size*9 + 0.5*voxel_size

    // Voxelise
    // Meaby this is what I did abovve but than different?
    for (auto const& triangle : faces) {
        // to do
    }

    // Fill model
    // to do

    // Write voxels
    // to do

    return 0;
}
