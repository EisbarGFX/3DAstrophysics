//
// Created by comppoter on 8/28/23.
//

#ifndef INC_3DASTRO_STELLARBODY_HPP
#define INC_3DASTRO_STELLARBODY_HPP


#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <map>



extern std::vector<std::pair<uint_fast64_t, glm::vec4>> transformQ;

class StellarBody {
private:
    double mass = 0.0;
    uint_fast64_t planetaryIndex = 0;
    glm::vec4 position = glm::vec4(0,0,0,1);
    glm::vec4 movement = glm::vec4(0,0,0,0);
    std::string name;

public:
    static double GravConstant;

    StellarBody();
    StellarBody(std::string name, double mass, glm::vec4 startingPosition, glm::vec4 startingMovement, uint_fast64_t index);

    uint_fast64_t getIndex();
    double getMass();
    glm::vec4 getPosition();
    glm::vec4 getMovement();
    std::string getName();

    StellarBody& operator=(const StellarBody& sb);
    bool operator<(const StellarBody& sb);
    bool operator>(const StellarBody& sb);
    bool operator==(const StellarBody& sb);

    void setPosition(glm::vec3 toPoint);
};


#endif //INC_3DASTRO_STELLARBODY_HPP
