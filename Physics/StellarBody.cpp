//
// Created by comppoter on 8/28/23.
//

#include "StellarBody.hpp"

double StellarBody::GravConstant = 6.674 * pow(10, -11);

StellarBody::StellarBody(double mass, glm::vec4 startingPosition, glm::vec4 startingMovement) {
    this->mass = mass;
    this->position = startingPosition;
    this-> movement = startingMovement;
}
