//
// Created by comppoter on 8/28/23.
//

#include "Particle.hpp"


double const Particle::GravConstant = 6.674 * pow(10, -11);
float Particle::graphToPhysConversion = 100.0f;
float Particle::physToGraphConversion = 1.0f/graphToPhysConversion;

Particle::Particle(std::string name, double mass, glm::vec4 startingPosition, glm::vec4 startingMovement, uint_fast64_t index) { // Phys Space
    this->name = std::move(name);
    this->mass = mass;
    this->physicalMovement = startingMovement;
    this->planetaryIndex = index;

    if (startingPosition != glm::vec4(0,0,0,1)) {
        transformQ.emplace_back(index, glm::vec4(startingPosition));
    }
    else {
        this->physicalPosition = startingPosition;
        this->graphicalPosition = scalePos(startingPosition, physToGraphConversion);
    }
}

//TODO: convert to matrices? idk.
Particle::Particle() {
    this->name = "SHOULD NOT BE SHOWN";
    this->mass = 0.0;
    this->physicalPosition = glm::vec4(0, 0, 0, 1);
    this->graphicalPosition = glm::vec4(0, 0, 0, 1);
    this->physicalMovement = glm::vec4(0, 0, 0, 0);
    this->graphicalMovement = glm::vec4(0, 0, 0, 0);
    this->planetaryIndex = 0;
}

uint_fast64_t Particle::getIndex() const {
    return this->planetaryIndex;
}

double Particle::getMass() const {
    return this->mass;
}

glm::vec4 Particle::getGPosition() const {
    return this->graphicalPosition;
}

void Particle::setGPosition(glm::vec3 toPoint) {
    this->graphicalPosition = glm::vec4(toPoint.x, toPoint.y, toPoint.z, 1);
}

glm::vec4 Particle::getGMovement() const {
    return this->graphicalMovement;
}

glm::vec4 Particle::getPPosition() const {
    return this->physicalPosition;
}

std::string Particle::getName() const {
    return this->name;
}

uint_fast64_t Particle::getCell() const {
    return this->cellIndex;
}


Particle &Particle::operator=(const Particle &sb) {
    this->name = sb.name;
    this->mass = sb.mass;
    this->planetaryIndex = sb.planetaryIndex;
    this->graphicalPosition = sb.graphicalPosition;
    this->graphicalMovement = sb.graphicalMovement;
    return *this;
}
bool Particle::operator<(const Particle &sb) {
    return this->planetaryIndex < sb.planetaryIndex;
}
bool Particle::operator>(const Particle &sb) {
    return this->planetaryIndex > sb.planetaryIndex;
}
bool Particle::operator==(const Particle &sb) {
    return (this->name == sb.name && this->planetaryIndex == sb.planetaryIndex);
}


void Particle::setPPosition(glm::vec3 toPoint) {
    this->physicalPosition = glm::vec4(toPoint.x, toPoint.y, toPoint.z, 1);
}

void Particle::setGMovement(glm::vec3 nMove) {
    this->graphicalMovement = glm::vec4(nMove.x, nMove.y, nMove.z, 0);
}

glm::vec4 Particle::getPMovement() const {
    return this->physicalMovement;
}

void Particle::setPMovement(glm::vec3 nMove) {
    this->physicalMovement = glm::vec4(nMove.x, nMove.y, nMove.z, 0);
}

void Particle::setCell(uint_fast64_t num) {
    this->cellIndex = num;
}


bool Particle::updateMovementOf(Particle *particle) {
    auto distance = glm::distance(physicalPosition, particle->getPPosition());

}



glm::vec4 scalePos(glm::vec4 vec, float scalar) {
    return {
            vec.x*scalar,
            vec.y*scalar,
            vec.z*scalar,
            vec.w
    };
}
glm::vec3 scalePos(glm::vec3 vec, float scalar) {
    return {
            vec.x*scalar,
            vec.y*scalar,
            vec.z*scalar
    };
}
