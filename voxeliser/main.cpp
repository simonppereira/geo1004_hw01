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
    const char* file_in = "C:\\Users\\simon\\Desktop\\Stuff\\1. TU Delft\\2. Semester\\3. GEO1004 3D Modelling of the Built Environment\\1. Assignment\\repo\\geo1004_hw01\\bag_bk.obj";
    const char* file_out = "vox.obj";
    float voxel_size = 1.0;

    // Read file

    std::vector<Point> vertices;
    std::vector<std::vector<unsigned int>> faces;
    std::string line;
    std::string type;
    std::ifstream in(file_in, std::ios::in);

    if (!in)
    {
        std::cerr << "Cannot open" << file_in << std::endl;
        exit(1);
    }

    while (std::getline(in, line))
    {
        std::istringstream vert(line);
        vert >> type;
        if (type == "v") {
            float x, y, z;
            vert >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        }
        if (type == "f") {
            unsigned int f1, f2, f3;
            vert >> f1 >> f2 >> f3;
            std::vector<unsigned int> temp = { f1, f2, f3 };
            faces.push_back(temp);
        }
    }

    //print the vertices for testing -> works
    /*for (std::vector<Point>::const_iterator i = vertices.begin(); i != vertices.end(); ++i)
        std::cout << *i << ' ';*/

    //print the faces for testing -> does not work. Logical answer probably but I do not know why
    /*for (auto i = faces.begin(); i != faces.end(); ++i)
        std::cout << *i << ' ';*/

    // Create grid ////////////////////////////////////////////////////////////////////////////////

    // creating vector for each dimension x,y,z
    std::vector<float> x_dim;
    std::vector<float> y_dim;
    std::vector<float> z_dim;

    for (std::vector<Point>::const_iterator i = vertices.begin(); i != vertices.end(); ++i){
        //std::cout << i[0][1]<<' ';
        x_dim.push_back(i[0][0]);
        y_dim.push_back(i[0][1]);
        z_dim.push_back(i[0][2]);}

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


    std::cout <<"max_x: " << max_x << '\n' <<"max_y: " << max_y << '\n' << "max_z: " << max_z << '\n'<< '\n';
    std::cout <<"min_x: " << min_x << '\n' <<"min_y: " << min_y << '\n' << "min_z: " << min_z;



    // to do
    //VoxelGrid voxels(rows.x, rows.y, rows.z);

    //  Vertex vSize

    //  voxels = { 10, 10, 10 };

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
