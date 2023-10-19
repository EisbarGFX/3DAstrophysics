//
// Created by comppoter on 8/28/23.
//

#include "Particle.hpp"


double const Particle::GravConstant = 6.674 * pow(10, -11);
float Particle::graphToPhysConversion = 100.0f;
float Particle::physToGraphConversion = 1.0f/graphToPhysConversion;

Particle::Particle(const std::string& name, double mass, glm::vec4 startingPosition, glm::vec4 startingMovement, uint_fast64_t index) { // Phys Space
    this->name = name;
    this->mass = mass;
    this->physicalMovement = startingMovement;
    this->planetaryIndex = index;

    if (startingPosition != glm::vec4(0,0,0,1)) {
        transformQ.emplace_back(index, glm::vec4(startingPosition));
    }
    else {
        this->physicalPosition = startingPosition;
        this->graphicalPosition = scalex44Matrix(physToGraphConversion) * startingPosition;
    }
}

Particle::Particle() {
//    this->name = static_cast<std::string *>(malloc(sizeof(char) * 256));
    this->name = "SHOULD NOT BE SHOWN";
    this->mass = 0.0;
    this->physicalPosition = glm::vec4(1, 1, 1, 1);
    this->graphicalPosition = glm::vec4(1, 1, 1, 1);
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

glm::vec4 Particle::getGMovement() const {
    return this->graphicalMovement;
}

glm::vec4 Particle::getPPosition() const {
    return this->physicalPosition;
}

glm::vec4 Particle::getPMovement() const {
    return this->physicalMovement;
}

std::string Particle::getName() const {
    return this->name;
}

uint_fast64_t Particle::getCell() const {
    return this->cellIndex;
}

Cell *Particle::getRepCell() const {
    return this->representedCell;
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


///\deprecated Use transformGPosition instead, unless where absolutely necessary.
__attribute_deprecated_msg__("Use transformGPosition instead.")
void Particle::setGPosition(glm::vec3 toPoint) {
    this->graphicalPosition = glm::vec4(toPoint.x, toPoint.y, toPoint.z, 1);
}

///\deprecated Use transformPPosition instead, unless where absolutely necessary.
__attribute_deprecated_msg__("Use transformPPosition instead.")
void Particle::setPPosition(glm::vec3 toPoint) {
    this->physicalPosition = glm::vec4(toPoint.x, toPoint.y, toPoint.z, 1);
}

void Particle::setGMovement(glm::vec3 nMove) {
    this->graphicalMovement = glm::vec4(nMove.x, nMove.y, nMove.z, 0);
}

void Particle::transformPPosition(glm::mat4 trans) {
//    if (this->physicalPosition.x == 0 && (trans[0][0] != 0 || trans[0][3] != 0)) {
//        this->physicalPosition.x = 1;
//    }
//    if (this->physicalPosition.y == 0 && (trans[1][1] != 0 || trans[1][3] != 0)) {
//        this->physicalPosition.y = 1;
//    }
//    if (this->physicalPosition.z == 0 && (trans[2][2] != 0 || trans[2][3] != 0)) {
//        this->physicalPosition.z = 1;
//    }

    this->physicalPosition = trans * this->physicalPosition;
}

void Particle::transformGPosition(glm::mat4 trans) {
//    if (this->graphicalPosition.x == 0 && (trans[0][0] != 0 || trans[3][0] != 0)) {
//        this->graphicalPosition.x = 1;
//    }
//    if (this->graphicalPosition.y == 0 && (trans[1][1] != 0 || trans[3][1] != 0)) {
//        this->graphicalPosition.y = 1;
//    }
//    if (this->graphicalPosition.z == 0 && (trans[2][2] != 0 || trans[3][2] != 0)) {
//        this->graphicalPosition.z = 1;
//    }

    this->graphicalPosition = trans * this->graphicalPosition;
}

void Particle::setPMovement(glm::vec3 nMove) {
    this->physicalMovement = glm::vec4(nMove.x, nMove.y, nMove.z, 0);
}

void Particle::setCell(uint_fast64_t num) {
    this->cellIndex = num;
}

bool Particle::updateMovementOf(Particle *particle) {
    auto distance = glm::distance(physicalPosition, particle->getPPosition());
    // TODO: code physics evaluations
}


///\deprecated Use a Scale Matrix Instead
__attribute_deprecated_msg__("Use a Scale Matrix Instead")
glm::vec4 scalePos(glm::vec4 vec, float scalar) {
    return {
            vec.x*scalar,
            vec.y*scalar,
            vec.z*scalar,
            vec.w
    };
}

/*
 * Translation: 1,0,0,X
 *              0,1,0,Y
 *              0,0,1,Z
 *              0,0,0,1
 *
 * GLM matrix multiplication and/or construction
 * treats them in COLUMN-MAJOR ORDER. Meaning
 * construction of a translation matrix must list
 * the translation components in the final row,
 * rather than the final column.
 *
 *
 * Scale:       X,0,0,0
 *              0,Y,0,0
 *              0,0,Z,0
 *              0,0,0,W
 *
 * The same as above applies in multiplication and
 * construction, but in a scale-only matrix the order
 * does not matter due to the nature of scaling being
 * done along the diagonal.
 */

glm::mat4 scalex44Matrix(float x, float y, float z) {
    return {x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1};
}

glm::mat4 scalex44Matrix(float unif) {
    return {unif, 0, 0, 0,
            0, unif, 0, 0,
            0, 0, unif, 0,
            0, 0, 0, 1};
}

glm::mat4 translationx44Matrix(float x, float y, float z) {
    return {1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            x, y, z, 1};
}

glm::mat4 translationx44Matrix(float unif) {
    return {1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            unif, unif, unif, 1};
}
