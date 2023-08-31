//
// Created by comppoter on 8/28/23.
//

#ifndef INC_3DASTRO_STELLARBODY_HPP
#define INC_3DASTRO_STELLARBODY_HPP


#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>

class StellarBody {
private:
    double mass = 0.0;
    unsigned short planetaryIndex = 0;
    glm::vec4 position = glm::vec4(0,0,0,1);
    glm::vec4 movement = glm::vec4(0,0,0,0);
    std::string name;

public:
    static double GravConstant;

    StellarBody();
    StellarBody(std::string name, double mass, glm::vec4 startingPosition, glm::vec4 startingMovement, unsigned short index);

    unsigned short getIndex() const;
    double getMass() const;
    glm::vec4 getPosition();
    glm::vec4 getMovement();
    std::string getName();

    StellarBody& operator=(const StellarBody& sb);
    bool operator<(const StellarBody& sb);
    bool operator>(const StellarBody& sb);
    bool operator==(const StellarBody& sb);
};


#endif //INC_3DASTRO_STELLARBODY_HPP
