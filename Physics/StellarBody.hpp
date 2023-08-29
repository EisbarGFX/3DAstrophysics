//
// Created by comppoter on 8/28/23.
//

#ifndef INC_3DASTRO_STELLARBODY_HPP
#define INC_3DASTRO_STELLARBODY_HPP


#include <glm/glm.hpp>
#include <vector>

class StellarBody {
private:
    double mass = 0.0;
    unsigned short planetaryIndex = 0;
    glm::vec4 position = glm::vec4(0,0,0,1);
    glm::vec4 movement = glm::vec4(0,0,0,0);

public:
    static double GravConstant;
    StellarBody(double mass, glm::vec4 startingPosition, glm::vec4 startingMovement);
    unsigned short getIndex();
    double getMass();
    glm::vec4 getPosition();
    glm::vec4 getMovement();
    void
};


#endif //INC_3DASTRO_STELLARBODY_HPP
