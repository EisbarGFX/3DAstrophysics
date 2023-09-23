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

#include "../Common/structs.hpp"


extern std::vector<std::pair<uint_fast64_t, glm::vec4>> transformQ;
#ifndef cellMap
class Cell;
extern std::map<uint_fast64_t,Cell*> cellMap;
#endif // cellMap

class Particle{
protected:
    double mass = 0.0;
    uint_fast64_t planetaryIndex = 0;
    uint_fast64_t cellIndex = 0;
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
    // Constructor starting position/movement is given in physical space

    [[nodiscard]] uint_fast64_t getIndex() const;
    [[nodiscard]] double getMass() const;
    [[nodiscard]] glm::vec4 getGPosition() const; // Graphical Position expressed in OpenGL "voxels"
    [[nodiscard]] glm::vec4 getGMovement() const;
    [[nodiscard]] glm::vec4 getPPosition() const; // Physical Position expressed in meters
    [[nodiscard]] glm::vec4 getPMovement() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] uint_fast64_t getCell() const;

    Particle& operator=(const Particle& sb);
    bool operator<(const Particle& sb);
    bool operator>(const Particle& sb);
    bool operator==(const Particle& sb);

    void setGPosition(glm::vec3 toPoint);
    void setPPosition(glm::vec3 toPoint);
    void setGMovement(glm::vec3 nMove);
    void setPMovement(glm::vec3 nMove);
    void setCell(uint_fast64_t num);

    bool updateMovementOf(Particle* particle);
    void move(translationx44Matrix translation);
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
