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

#include "../Graphics/loadShader.hpp"
#include "control.hpp"
#include "loadOBJ.hpp"
#include "loadDDS.hpp"
#include "../Physics/Particle.hpp"
#include "../Physics/Particle.hpp"
#include "structs.hpp"

bool createObject(
        std::map<uint_fast64_t,wrappedObject> & vboMap,
        const char * objectPath,
        const char * texturePath,
        [[maybe_unused]] IDList & idList
        );
bool createParticle(
        std::map<uint_fast64_t,Particle*> & particleMap,
        std::map<uint_fast64_t,wrappedObject> & vboMap,
        const char * objectPath,
        const char * texturePath,
        IDList & idList,
        const std::string& name,
        double mass,
        glm::vec4 position,
        glm::vec4 direction
        );
bool createPlanet(
        std::map<uint_fast64_t,Particle*> & particleMap,
        std::map<uint_fast64_t,wrappedObject> & vboMap,
        const char * objectPath,
        const char * texturePath,
        IDList & idList,
        const std::string& name,
        double mass,
        glm::vec4 position,
        glm::vec4 direction
        );
bool createSun(
        std::map<uint_fast64_t,Particle*> & particleMap,
        std::map<uint_fast64_t,wrappedObject> & vboMap,
        const char * objectPath,
        const char * texturePath,
        IDList & idList,
        const std::string& name,
        double mass,
        glm::vec4 position,
        glm::vec4 direction
        );
void drawObject(
        wrappedObject & vbo,
        IDList & idList,
        glm::mat4 & ProjectionMatrix,
        glm::mat4 & ViewMatrix
        );
void moveObject(
        wrappedObject & vbo,
        Particle* particle,
        IDList & idList,
        glm::vec4 gMove // A coordinate or direction to move the object in physical space
        );


#endif //INC_3DASTRO_COMMANDS_HPP
