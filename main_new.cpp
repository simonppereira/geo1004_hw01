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
    return ((a-d).dot((b-d).cross(c-d)))/6;
}
//const Point target1(float voxel_size, int i, int j, int k, float min_x, float min_y, float min_z) {
std::vector<Point> target1(float voxel_size, int i, int j, int k, float min_x, float min_y, float min_z) {
    Point orgBB;
    Point orig;
    Point dest;

    orgBB.x = min_x;
    orgBB.y = min_y;
    orgBB.z = min_z;

    orig.x = orgBB.x + i * voxel_size;
    orig.y = orgBB.y + j * voxel_size + 0.5 * voxel_size;
    orig.z = orgBB.z + k * voxel_size + 0.5 * voxel_size;
    dest.x = orig.x + voxel_size;
    dest.y = orig.y;
    dest.z = orig.z;

    std::vector<Point> points{orig, dest};

    return points;
}
//const Point target2(float voxel_size, int i, int j, int k, float min_x, float min_y, float min_z) {
std::vector<Point> target2(float voxel_size, int i, int j, int k, float min_x, float min_y, float min_z) {
    Point orgBB;
    Point orig;
    Point dest;

    orgBB.x = min_x;
    orgBB.y = min_y;
    orgBB.z = min_z;

    orig.x = orgBB.x + i * voxel_size + 0.5 * voxel_size;
    orig.y = orgBB.y + j * voxel_size;
    orig.z = orgBB.z + k * voxel_size + 0.5 * voxel_size;
    dest.x = orig.x;
    dest.y = orig.y + voxel_size;
    dest.z = orig.z;

    std::vector<Point> points{orig, dest};

    return points;
}
//const Point target3(float voxel_size, int i, int j, int k, float min_x, float min_y, float min_z) {
std::vector<Point> target3(float voxel_size, int i, int j, int k, float min_x, float min_y, float min_z) {
    Point orgBB;
    Point orig;
    Point dest;

    orgBB.x = min_x;
    orgBB.y = min_y;
    orgBB.z = min_z;

    orig.x = orgBB.x + i * voxel_size + 0.5 * voxel_size;
    orig.y = orgBB.y + j * voxel_size + 0.5 * voxel_size;
    orig.z = orgBB.z + k * voxel_size;
    dest.x = orig.x;
    dest.y = orig.y;
    dest.z = orig.z + voxel_size;

    std::vector<Point> points {orig, dest};

    return points;
}

std::vector<Point> bbox(std::vector<Point> fvertices, float voxel_size) {

    // creating vector for each dimension x,y,z
    std::vector<float> x_dim;
    std::vector<float> y_dim;
    std::vector<float> z_dim;

    for (std::vector<Point>::const_iterator i = fvertices.begin(); i != fvertices.end(); ++i) {
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
    //std::cout << '\n'<< no_y;

    if (std::fmod((max_z - min_z), voxel_size) == 0) {
        no_z = int((max_z - min_z) / voxel_size);
    }
    else {
        no_z = int((max_z - min_z) / voxel_size) + 1;
    }

    float bbx_max_x = min_x + no_x * voxel_size;
    float bbx_max_y = min_y + no_y * voxel_size;
    float bbx_max_z = min_z + no_z * voxel_size;

    Point bbox_min;
    Point bbox_max;

    bbox_min.x = min_x;
    bbox_min.y = min_y;
    bbox_min.z = min_z;

    bbox_max.x = bbx_max_x;
    bbox_max.y = bbx_max_y;
    bbox_max.z = bbx_max_z;

    std::vector<Point> bbox {bbox_min, bbox_max};

    return bbox;
}

bool intersects(const Point &orig, const Point &dest, const Point &v0, const Point &v1, const Point &v2) {
    if (signed_volume(v0, v1, v2, orig) > 0 && signed_volume(v0, v1, v2, dest) < 0) {
        return true;
    } //next if statement
    else if (signed_volume(v0, v1, v2, orig) < 0 && signed_volume(v0, v1, v2, dest) > 0) {
        return true;
    } //next if statement
    else if (signed_volume(v0, v1, v2, orig) == 0 || signed_volume(v0, v1, v2, dest) == 0) {
        return false; //check on silvers
    }
    else { return false; }
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

    // Set some iteration variables
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
    unsigned int no_x = 0;
    unsigned int no_y = 0;
    unsigned int no_z = 0;

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
        no_z =  ((max_z - min_z) / voxel_size);
    }
    else {
        no_z =  ((max_z - min_z) / voxel_size) + 1;
    }

    std::cout << '\n' << "x:  " << no_x << "  y: " << no_y << "   z:  " << no_z;

    // create vector for bbox
    float bbx_max_x = min_x + no_x * voxel_size;
    float bbx_max_y = min_y + no_y * voxel_size;
    float bbx_max_z = min_z + no_z * voxel_size;

    Point bbox_min_p;
    Point bbox_max_p;

    bbox_min_p.x = min_x;
    bbox_min_p.y = min_y;
    bbox_min_p.z = min_z;

    bbox_max_p.x = bbx_max_x;
    bbox_max_p.y = bbx_max_y;
    bbox_max_p.z = bbx_max_z;

    Rows rows = {no_x, no_y, no_z};
    VoxelGrid voxels(rows.x, rows.y, rows.z);
    //no x,y,z174 244 54
    std::cout<< '\n'<< "no x,y,z"<< no_x <<' '<<no_y <<' ' <<no_z;

    int p;
    p = 0;

    for (int i = 0; i < no_x; i++) {
        for (int j = 0; j < no_y; j++) {
            for (int k = 0; k < no_z; k++) {
                // std::cout<<"something";
                voxels(i, j, k) = 0;
                }
            }
        }

    int q = 0;
    // to prevent double variable names

    float testmin_x = min_x;
    float testmin_y = min_y;
    float testmin_z = min_z;

    //faces.size()
    for (int t = 4600; t < faces.size(); t++) { //100 should be changed to max face which is somewhere in the 4000
        int Iv0 = faces[t][0];
        int Iv1 = faces[t][1];
        int Iv2 = faces[t][2];
        Point v0 = vertices[Iv0];
        Point v1 = vertices[Iv1];
        Point v2 = vertices[Iv2];

        std::vector<Point> triangle;
        triangle.emplace_back(v0);
        triangle.emplace_back(v1);
        triangle.emplace_back(v2);

        std::vector<Point> bbox_tri = bbox(triangle, voxel_size);
        Point min;
        Point max;

        float min_x, min_y, min_z;
        float max_x, max_y, max_z;

        min = bbox_tri[0];
        max = bbox_tri[1];

        //std::cout << '\n' << " min: " << min << "max: " << max;

        min_x = min.x;
        min_y = min.y;
        min_z = min.z;

        max_x = max.x;
        max_y = max.y;
        max_z = max.z;

        int testminx, testminy, testminz, testmaxx, testmaxy, testmaxz;
        testminx = std::floor((min_x-testmin_x) / voxel_size);
        testminy = std::floor((min_y-testmin_y) / voxel_size);
        testminz = std::floor((min_z-testmin_z) / voxel_size);
        testmaxx = std::floor((max_x- testmin_x) / voxel_size);
        testmaxy = std::floor((max_y- testmin_y) / voxel_size);
        testmaxz = std::floor((max_z- testmin_z) / voxel_size);

        //::cout << '\n' << "minindexofface: " << testminx << ", actualvalueofface: " << min_x;

        for (int i = testminx; i < testmaxx; i++) {
            for (int j = testminy; j < testmaxy; j++) {
                for (int k = testminz; k < testmaxz; k++) {

                    std::vector<Point> tar1 = target1(voxel_size, i, j, k, testmin_x, testmin_y, testmin_z);
                    Point orig1 = tar1[0];
                    Point dest1 = tar1[1];

                    std::vector<Point> tar2 = target2(voxel_size, i, j, k, testmin_x, testmin_y, testmin_z);
                    Point orig2 = tar2[0];
                    Point dest2 = tar2[1];

                    std::vector<Point> tar3 = target3(voxel_size, i, j, k, testmin_x, testmin_y, testmin_z);
                    Point orig3 = tar3[0];
                    Point dest3 = tar3[1];

                    p++;

                    //FirstTarget
                    if (intersects(orig1, dest1, v0, v1, v2)) {
                        //std::cout << '\n' << " #p " << p << " first intersection left-right - check";
                        if (intersects(v0, v1, orig1, dest1, v2) && intersects(v2, v0, orig1, dest1, v1) &&
                            intersects(v1, v2, orig1, dest1, v0)) {
                            q++;
                            std::cout << '\n' << " #q " << q << " #p " << p << " Second intersection - check" << " x: " << i << " y: " << j << " z: " << k;
                            voxels(i, j, k) = 1;
                            continue;
                        }
                        else {
                            //std::cout << ", second intersection - fail";
                            goto SecondTarget;
                        }
                    }
                    else {
                        //std::cout << '\n' << " #p " << p << " first intersection left-right - fail";
                        goto SecondTarget; }

                SecondTarget:
                    if (intersects(orig2, dest2, v0, v1, v2)) {
                        //std::cout << '\n' << " #p " << p << " first intersection front-back - check";
                        if (intersects(v0, v1, orig2, dest2, v2) && intersects(v2, v0, orig2, dest2, v1) &&
                            intersects(v1, v2, orig2, dest2, v0)) {
                            q++;
                            std::cout << '\n' << " #q " << q << " #p " << p << " Second intersection - check" << " x: " << i << " y: " << j << " z: " << k;
                            voxels(i, j, k) = 1;
                            continue;
                        }
                        else {
                            //std::cout << ", second intersection - fail";
                            goto ThirdTarget;
                        }
                    }
                    else {
                        //std::cout << '\n' << " #p " << p << " first intersection front-back - fail";
                        goto ThirdTarget; }

                ThirdTarget:
                    if (intersects(orig3, dest3, v0, v1, v2)) {
                        //std::cout << '\n' << " #p " << p << " first intersection bottom-top - check";
                        if (intersects(v0, v1, orig3, dest3, v2) && intersects(v2, v0, orig3, dest3, v1) &&
                            intersects(v1, v2, orig3, dest3, v0)) {
                            q++;
                            std::cout << '\n' << " #q " << q << " #p " << p << " Second intersection - check" << " x: " << i << " y: " << j << " z: " << k;
                            voxels(i, j, k) = 1;
                            continue;
                        }
                        else {
                            //std::cout << ", second intersection - fail";
                        }
                    }
                    else {
                        //std::cout << '\n' << " #p " << p << " first intersection bottom-top - fail";

                    }
                }
            }
        }
    }

    /*int counter = 0;
    for (int i = 0; i < no_x; i++) {
        for (int j = 0; j < no_y; j++) {
            for (int k = 0; k < no_z; k++) {
                if(voxels(i, j, k) == 1) {
                    ++counter;
                    std::cout << '\n' << counter << " Value of 1 -> intersection";
                }
            }
        }
    }*/

    // Voxelise
    // Meaby this is what I did abovve but than different?
    /*for (auto const& triangle : faces) {
        // to do}*/

    // Fill model
    // to do

    // Write voxels
    // to do

    return 0;
}