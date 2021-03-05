#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <assert.h>

#include "Point.h"
#include "Rows.h"
#include "VoxelGrid.h"

//define function for finding the actual min and max value of a bounding box
std::vector<Point> bbox(std::vector<Point> vertices) {

    // creating vector for each dimension x,y,z
    Point bbox_min = vertices.front();
    Point bbox_max = vertices.front();

    for (std::vector<Point>::const_iterator i = vertices.begin(); i != vertices.end(); ++i) {
        if (i->x < bbox_min.x) bbox_min.x = i->x;
        if (i->y < bbox_min.y) bbox_min.y = i->y;
        if (i->z < bbox_min.z) bbox_min.z = i->z;
        if (i->x > bbox_max.x) bbox_max.x = i->x;
        if (i->y > bbox_max.y) bbox_max.y = i->y;
        if (i->z > bbox_max.z) bbox_max.z = i->z;
    }

    std::vector<Point> bbox{ bbox_min, bbox_max };

    return bbox;
}

//define function for finding the target line left right
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

    std::vector<Point> points{ orig, dest };

    return points;
}

//define function for finding the target line front back
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

    std::vector<Point> points{ orig, dest };

    return points;
}

//define funtion for finding the target line bottom top 
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

    std::vector<Point> points{ orig, dest };

    return points;
}

//define formula for calculating the signed volume
float signed_volume(const Point& a, const Point& b, const Point& c, const Point& d) {
    return ((a - d).dot((b - d).cross(c - d))) / 6;
}

//define intersection condition, return true if intersection condition is met
bool intersects(const Point & orig, const Point & dest, const Point & v0, const Point & v1, const Point & v2) {
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
    //I have added the path to the obj file. Local path so static and not dynamic.
    const char* file_in = "C:/Users/s161887/Documents/GitHub/geo1004_hw01_test/geo1004_hw01_test/bag_bk.obj";
    const char* file_out = "C:/Users/s161887/Documents/GitHub/geo1004_hw01_test/geo1004_hw01_test/output.obj";
    float voxel_size = 1.0;

    //read file
    std::vector<Point> vertices;
    std::vector<std::vector<unsigned int>> faces;
    std::string line;
    std::ifstream in(file_in, std::ios::in);

    //give an error if the file cannot be opened
    if (!in)
    {
        std::cerr << "Cannot open" << file_in << std::endl;
        exit(1);
    }

    //read and parse the vertices and faces in the obj file
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

    //find the exact values of the bounding box
    std::vector<Point> bbox_overal = bbox(vertices);
    Point min_overal = bbox_overal[0];
    Point max_overal = bbox_overal[1];

    //find the number of rows in every direction, also described as the number of indices of the voxels in every direction. 
    int row_x = ceil((max_overal.x - min_overal.x) / voxel_size);
    int row_y = ceil((max_overal.y - min_overal.y) / voxel_size);
    int row_z = ceil((max_overal.z - min_overal.z) / voxel_size);
    Rows rows(row_x, row_y, row_z);
    VoxelGrid voxels(rows.x, rows.y, rows.z);

    //print some data on the overal bounding box of the model
    std::cout << '\n' << "maxoveralx: " << max_overal.x << " maxoveraly " << max_overal.y << " maxoveralz " << max_overal.z;
    std::cout << '\n' << "minoveralx: " << min_overal.x << " minoveraly " << min_overal.y << " minoveralz " << min_overal.z;
    std::cout << '\n' << "row_x: " << row_x << " row_y " << row_y << " row_z " << row_z;

    //give every voxel in the overal voxelgrid a value of 0. Later 0 a value of 0 is used to identify a voxel as part of the interior. 
    //If a voxel intersects it will be given a value of 1 later in the code. If a voxel is outside of the building it will be given a value of 2 later in the code.
    for (int i = 0; i < row_x; i++) {
        for (int j = 0; j < row_y; j++) {
            for (int k = row_z - 1; k >= 0; k--) {
                voxels(i, j, k) == 0;
            }
        }
    }

    //iterate through all faces of the original model. For every face find the bounding box. Iterate through the voxels in the bounding box of every face. Find the voxels that intersect with the face. 
    for (int f = 0; f < faces.size(); f++) {

        //getting the exact value of the vertices of the face
        int index_v0 = faces[f][0];
        int index_v1 = faces[f][1];
        int index_v2 = faces[f][2];

        Point v0 = vertices[index_v0-1];
        Point v1 = vertices[index_v1-1];
        Point v2 = vertices[index_v2-1];

        //getting the exact min and max of the bounding box of the face
        std::vector<Point> vertices_face{ v0, v1, v2 };

        std::vector<Point> bbox_face = bbox(vertices_face);
        Point min_face = bbox_face[0];
        Point max_face = bbox_face[1];

        //setting the index values of the voxels that are the min and max of the bounding box of the face
        int min_face_x = floor((min_face.x - min_overal.x) / voxel_size);
        int min_face_y = floor((min_face.y - min_overal.y) / voxel_size);
        int min_face_z = floor((min_face.z - min_overal.z) / voxel_size);
        int max_face_x = ceil((max_face.x - min_overal.x) / voxel_size);
        int max_face_y = ceil((max_face.y - min_overal.y) / voxel_size);
        int max_face_z = ceil((max_face.z - min_overal.z) / voxel_size);

        //iterate through the voxels that are within the bounding box of the face
        for (int i = min_face_x; i < max_face_x; i++) {
            for (int j = min_face_y; j < max_face_y; j++) {
                for (int k = min_face_z; k < max_face_z; k++) {

                    //define the three target lines for every voxel. The target lines are checked on intersection. 
                    //The target lines go from the center of every face of the voxel to the center of the opposite face of the voxel
                    std::vector<Point> tar1 = target1(voxel_size, i, j, k, min_overal.x, min_overal.y, min_overal.z);
                    assert(tar1.size() == 2);
                    Point orig1 = tar1[0];
                    Point dest1 = tar1[1];

                    std::vector<Point> tar2 = target2(voxel_size, i, j, k, min_overal.x, min_overal.y, min_overal.z);
                    assert(tar2.size() == 2);
                    Point orig2 = tar2[0];
                    Point dest2 = tar2[1];

                    std::vector<Point> tar3 = target3(voxel_size, i, j, k, min_overal.x, min_overal.y, min_overal.z);
                    assert(tar3.size() == 2);
                    Point orig3 = tar3[0];
                    Point dest3 = tar3[1];

                    //Check the target lines on intersection. If a target line is found to intersect, define the voxel to intersect by giving it a value of 1 and go to the next voxel.
                    //FirstTarget
                    if (intersects(orig1, dest1, v0, v1, v2)) {
                        if (intersects(v0, v1, orig1, dest1, v2) && intersects(v2, v0, orig1, dest1, v1) &&
                            intersects(v1, v2, orig1, dest1, v0)) {
                            voxels(i, j, k) = 1;
                            continue;
                        }
                        else {
                            goto SecondTarget;
                        }
                    }
                    else {
                        goto SecondTarget;
                    }

                SecondTarget:
                    if (intersects(orig2, dest2, v0, v1, v2)) {
                        if (intersects(v0, v1, orig2, dest2, v2) && intersects(v2, v0, orig2, dest2, v1) &&
                            intersects(v1, v2, orig2, dest2, v0)) {
                            voxels(i, j, k) = 1;
                            continue;
                        }
                        else {
                            goto ThirdTarget;
                        }
                    }
                    else {
                        goto ThirdTarget;
                    }

                ThirdTarget:
                    if (intersects(orig3, dest3, v0, v1, v2)) {
                        if (intersects(v0, v1, orig3, dest3, v2) && intersects(v2, v0, orig3, dest3, v1) &&
                            intersects(v1, v2, orig3, dest3, v0)) {
                            voxels(i, j, k) = 1;
                            continue;
                        }
                        else {
                        }
                    }
                    else {
                    }
                }
            }
        }
    }
  
    //giving the exterior voxels a value of 2
    for (int i = 0; i < row_x; i++) {
        for (int j = 0; j < row_y; j++) {
            for (int k = row_z - 1; k >= 0; k--) {
                if (voxels(i, j, k) == 0) {
                    voxels(i, j, k) = 2;
                }
                else {
                    break;
                }
            }
        }
    }

    //calculating the volume. The boundary has been assumed to be half in the building and half outside the building. 

    float vol_inside = 0;
    float vol_boundary = 0;

    for (int i = 0; i < row_x; i++) {
        for (int j = 0; j < row_y; j++) {
            for (int k = 0; k < row_z; k++) {
                if (voxels(i, j, k) == 2) {
                    continue;
                }
                else if (voxels(i, j, k) == 0) {
                    vol_inside = vol_inside + pow(voxel_size, 3.0);
                }
                else if (voxels(i, j, k) == 1) {
                    vol_boundary = vol_boundary + pow(voxel_size, 3.0);

                }
            }
        }
    }

    float vol_total = vol_inside + 0.5 * vol_boundary;

    std::cout << "total volume = " << vol_total;

    //exporting the voxelized model to an obj file. Faces of voxels are transformed to triangles. 

    std::fstream fl;
    fl.open(file_out, std::fstream::in | std::fstream::out | std::fstream::trunc);

    for (int i = 0; i < row_x; i++) {
        for (int j = 0; j < row_y; j++) {
            for (int k = 0; k < row_z; k++) {
                if (voxels(i, j, k) != 2) {

                    Point LFB;
                    Point RFB;
                    Point LBB;
                    Point RBB;
                    Point LFT;
                    Point RFT;
                    Point LBT;
                    Point RBT;

                    //origin / left-front-bottom
                    LFB.x = min_overal.x + i * voxel_size;
                    LFB.y = min_overal.y + j * voxel_size;
                    LFB.z = min_overal.z + k * voxel_size;

                    //left-front-top
                    LFT.x = min_overal.x + i * voxel_size;
                    LFT.y = min_overal.y + j * voxel_size;
                    LFT.z = min_overal.z + k * voxel_size + voxel_size;

                    //left-back-bottom
                    LBB.x = min_overal.x + i * voxel_size;
                    LBB.y = min_overal.y + j * voxel_size + voxel_size;
                    LBB.z = min_overal.z + k * voxel_size;

                    //left-back-top
                    LBT.x = min_overal.x + i * voxel_size;
                    LBT.y = min_overal.y + j * voxel_size + voxel_size;
                    LBT.z = min_overal.z + k * voxel_size + voxel_size;

                    //right-front-bottom
                    RFB.x = min_overal.x + i * voxel_size + voxel_size;
                    RFB.y = min_overal.y + j * voxel_size;
                    RFB.z = min_overal.z + k * voxel_size;

                    //right-front-top
                    RFT.x = min_overal.x + i * voxel_size + voxel_size;
                    RFT.y = min_overal.y + j * voxel_size;
                    RFT.z = min_overal.z + k * voxel_size + voxel_size;

                    //right-back-bottom
                    RBB.x = min_overal.x + i * voxel_size + voxel_size;
                    RBB.y = min_overal.y + j * voxel_size + voxel_size;
                    RBB.z = min_overal.z + k * voxel_size;

                    RBT.x = min_overal.x + i * voxel_size + voxel_size;
                    RBT.y = min_overal.y + j * voxel_size + voxel_size;
                    RBT.z = min_overal.z + k * voxel_size + voxel_size;

                    fl << "v " << LFB.x << ' ' << LFB.y << ' ' << LFB.z << std::endl;
                    fl << "v " << LFT.x << ' ' << LFT.y << ' ' << LFT.z << std::endl;
                    fl << "v " << LBB.x << ' ' << LBB.y << ' ' << LBB.z << std::endl;
                    fl << "v " << LBT.x << ' ' << LBT.y << ' ' << LBT.z << std::endl;
                    fl << "v " << RFB.x << ' ' << RFB.y << ' ' << RFB.z << std::endl;
                    fl << "v " << RFT.x << ' ' << RFT.y << ' ' << RFT.z << std::endl;
                    fl << "v " << RBB.x << ' ' << RBB.y << ' ' << RBB.z << std::endl;
                    fl << "v " << RBT.x << ' ' << RBT.y << ' ' << RBT.z << std::endl;
                }
            }
        }
    }

    int index = 1;
    for (int i = 0; i < row_x; i++) {
        for (int j = 0; j < row_y; j++) {
            for (int k = 0; k < row_z; k++) {
                if (voxels(i, j, k) != 2) {

                    Point LFB;
                    Point RFB;
                    Point LBB;
                    Point RBB;
                    Point LFT;
                    Point RFT;
                    Point LBT;
                    Point RBT;

                    //writing f LFB RFT LFT
                    fl << "f " << (index + 0) << ' ' << (index + 5) << ' ' << (index + 1) << std::endl;
                    //writing f LFB RFB RFT
                    fl << "f " << (index + 0) << ' ' << (index + 4) << ' ' << (index + 5) << std::endl;

                    //writing f RFB RBB RBT
                    fl << "f " << (index + 4) << ' ' << (index + 6) << ' ' << (index + 7) << std::endl;
                    //writing f RFB RBT RFT
                    fl << "f " << (index + 4) << ' ' << (index + 7) << ' ' << (index + 5) << std::endl;

                    //writing f RBB LBT RBT
                    fl << "f " << (index + 6) << ' ' << (index + 3) << ' ' << (index + 7) << std::endl;
                    //writing f RBB LBB LBT
                    fl << "f " << (index + 6) << ' ' << (index + 2) << ' ' << (index + 3) << std::endl;

                    //writing f LBB LFT LBT
                    fl << "f " << (index + 2) << ' ' << (index + 1) << ' ' << (index + 3) << std::endl;
                    //writing f LBB LFB LFT
                    fl << "f " << (index + 2) << ' ' << (index + 0) << ' ' << (index + 1) << std::endl;

                    //writing f LFT RBT LBT
                    fl << "f " << (index + 1) << ' ' << (index + 7) << ' ' << (index + 3) << std::endl;
                    //writing f LFT RFT RBT
                    fl << "f " << (index + 1) << ' ' << (index + 5) << ' ' << (index + 7) << std::endl;

                    //writing f LFB RBB LBB
                    fl << "f " << (index + 0) << ' ' << (index + 6) << ' ' << (index + 2) << std::endl;
                    //writing f LFB RFB RBB
                    fl << "f " << (index + 0) << ' ' << (index + 4) << ' ' << (index + 6) << std::endl;

                    index = index + 8;
                }
            }
        }
    }
    fl.close();
    
    return 0;
}