//
// Created by comppoter on 8/26/23.
//

#ifndef INC_3DASTRO_LOADOBJ_HPP
#define INC_3DASTRO_LOADOBJ_HPP

#include <vector>
#include <glm/glm.hpp>
#include <filesystem>
#include <cstring>
#include <iostream>
#include <map>
#include <cstring>
using namespace std;



void indexVBO(
        std::vector<glm::vec3> & in_vertices,
        std::vector<glm::vec2> & in_uvs,
        std::vector<glm::vec3> & in_normals,

        std::vector<unsigned short> & out_indices,
        std::vector<glm::vec3> & out_vertices,
        std::vector<glm::vec2> & out_uvs,
        std::vector<glm::vec3> & out_normals
);

bool loadOBJ (
    const char * path,
    vector < glm::vec3 > & out_vertices,
    vector < glm::vec2 > & out_uvs,
    vector < glm::vec3 > & out_normals
);


#endif //INC_3DASTRO_LOADOBJ_HPP
