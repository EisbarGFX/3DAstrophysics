//
// Created by comppoter on 8/28/23.
//

#include "StellarBody.hpp"


double StellarBody::GravConstant = 6.674 * pow(10, -11);

StellarBody::StellarBody(std::string name, double mass, glm::vec4 startingPosition, glm::vec4 startingMovement, uint_fast64_t index) {
    this->name = std::move(name);
    this->mass = mass;
    this->position = startingPosition;
    this->movement = startingMovement;
    this->planetaryIndex = index;

    if (startingPosition != glm::vec4(0,0,0,1)) {
        std::cout<<"Queue added"<<std::endl;
        transformQ.emplace_back(index, glm::vec4(startingPosition));
    }
}

StellarBody::StellarBody() {
    this->name = "SHOULD NOT BE SHOWN";
    this->mass = 0.0;
    this->position = glm::vec4(0,0,0,1);
    this->movement = glm::vec4(0,0,0,0);
    this->planetaryIndex = 0;
}

uint_fast64_t StellarBody::getIndex() {
    return this->planetaryIndex;
}

double StellarBody::getMass() {
    return this->mass;
}

glm::vec4 StellarBody::getPosition() {
    return this->position;
}
void StellarBody::setPosition(glm::vec3 toPoint) {
    this->position = glm::vec4(toPoint.x, toPoint.y, toPoint.z, 1);
}

glm::vec4 StellarBody::getMovement() {
    return this->movement;
}

std::string StellarBody::getName() {
    return this->name;
}

StellarBody &StellarBody::operator=(const StellarBody &sb) {
    this->name = sb.name;
    this->mass = sb.mass;
    this->planetaryIndex = sb.planetaryIndex;
    this->position = sb.position;
    this->movement = sb.movement;
    return *this;
}

bool StellarBody::operator<(const StellarBody &sb) {
    return this->planetaryIndex < sb.planetaryIndex;
}

bool StellarBody::operator>(const StellarBody &sb) {
    return this->planetaryIndex > sb.planetaryIndex;
}

bool StellarBody::operator==(const StellarBody &sb) {
    return (this->name == sb.name && this->planetaryIndex == sb.planetaryIndex);
}
