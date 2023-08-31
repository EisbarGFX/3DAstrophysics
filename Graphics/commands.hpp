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

#include "loadShader.hpp"
#include "../Common/control.hpp"
#include "../Common/loadOBJ.hpp"
#include "../Common/loadDDS.hpp"
#include "../Physics/StellarBody.hpp"
#include "../Common/structs.hpp"

bool createBody(
        std::map<unsigned short,StellarBody> & planetMap,
        std::map<unsigned short,wrappedObject> & vboMap,
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


#endif //INC_3DASTRO_COMMANDS_HPP
