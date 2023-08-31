//
// Created by comppoter on 8/28/23.
//

#include "StellarBody.hpp"

#include <utility>

double StellarBody::GravConstant = 6.674 * pow(10, -11);

StellarBody::StellarBody(std::string name, double mass, glm::vec4 startingPosition, glm::vec4 startingMovement, unsigned short index) {
    this->name = std::move(name);
    this->mass = mass;
    this->position = startingPosition;
    this->movement = startingMovement;
    this->planetaryIndex = index;
}

StellarBody::StellarBody() {
    this->name = "SHOULD NOT BE SHOWN";
    this->mass = 0.0;
    this->position = glm::vec4(0,0,0,1);
    this->movement = glm::vec4(0,0,0,0);
    this->planetaryIndex = 0;
}

unsigned short StellarBody::getIndex() const {
    return this->planetaryIndex;
}

double StellarBody::getMass() const {
    return this->mass;
}

glm::vec4 StellarBody::getPosition() {
    return this->position;
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
