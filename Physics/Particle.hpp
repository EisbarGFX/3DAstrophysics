//
// Created by comppoter on 8/28/23.
//

#ifndef INC_3DASTRO_PARTICLE_HPP
#define INC_3DASTRO_PARTICLE_HPP


#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <map>

// TODO: safe destructors to fix possible memory leaks


extern std::vector<std::pair<uint_fast64_t, glm::vec4>> transformQ;

class Particle{
protected:
    double mass = 0.0;
    uint_fast64_t planetaryIndex = 0;
    uint_fast64_t cellIndex = NULL;
    glm::vec4 graphicalPosition = glm::vec4(0, 0, 0, 1);
    glm::vec4 physicalPosition = glm::vec4(0,0,0,1);
    glm::vec4 graphicalMovement = glm::vec4(0, 0, 0, 0);
    glm::vec4 physicalMovement = glm::vec4(0,0,0,0);
    std::string name;

public:
    static const double GravConstant;
    static float graphToPhysConversion;
    static float physToGraphConversion;
    bool grouped = false;

    Particle();
    Particle(std::string name, double mass, glm::vec4 startingPosition, glm::vec4 startingMovement, uint_fast64_t index);
    // Initiator starting position/movement is given in physical space

    uint_fast64_t getIndex();
    double getMass();
    glm::vec4 getGPosition(); // Graphical Position expressed in OpenGL "voxels"
    glm::vec4 getGMovement();
    glm::vec4 getPPosition(); // Physical Position expressed in meters
    glm::vec4 getPMovement();
    std::string getName();

    Particle& operator=(const Particle& sb);
    bool operator<(const Particle& sb);
    bool operator>(const Particle& sb);
    bool operator==(const Particle& sb);

    void setGPosition(glm::vec3 toPoint);
    void setPPosition(glm::vec3 toPoint);
    void setGMovement(glm::vec3 nMove);
    void setPMovement(glm::vec3 nMove);

    void setCell(uint_fast64_t num);
    uint_fast64_t getCell();
};

class Planet : public Particle {
public:
    using Particle::Particle;
};

class Sun : public Particle {
public:
    using Particle::Particle;
};

glm::vec4 scalePos(glm::vec4 vec, float scalar);
glm::vec3 scalePos(glm::vec3 vec, float scalar);


#endif //INC_3DASTRO_PARTICLE_HPP
