//
// Created by comppoter on 8/30/23.
//

#ifndef INC_3DASTRO_COMMANDS_HPP
#define INC_3DASTRO_COMMANDS_HPP

#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <map>
#include <filesystem>
#include <regex>

#include "loadShader.hpp"
#include "../Common/control.hpp"
#include "../Common/loadOBJ.hpp"
#include "../Common/loadDDS.hpp"
#include "../Physics/StellarBody.hpp"
#include "../Common/structs.hpp"

bool createObject(
        std::map<uint_fast64_t,StellarBody> & planetMap,
        std::map<uint_fast64_t,wrappedObject> & vboMap,
        const char * objectPath,
        const char * texturePath,
        IDList & idList,
        const std::string& name,
        double mass,
        glm::vec4 position,
        glm::vec4 direction);
void drawObject(
        wrappedObject & vbo,
        IDList & idList,
        glm::mat4 & ProjectionMatrix,
        glm::mat4 & ViewMatrix);
void moveObject(
        wrappedObject & vbo,
        IDList & idList,
        glm::vec4 moveCommand);


#endif //INC_3DASTRO_COMMANDS_HPP
