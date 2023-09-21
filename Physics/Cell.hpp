//
// Created by comppoter on 9/20/23.
//

#ifndef INC_3DASTRO_CELL_HPP
#define INC_3DASTRO_CELL_HPP

#include <glm/vec4.hpp>
#include <algorithm>
#include <numeric>
#include "Particle.hpp"
;

class Cell {
protected:
//    std::vector<double> masses;
    double totalMass;
//    std::vector<glm::vec4> positions;
    glm::vec4 avgPosition{};
    std::vector<Particle*> Particles;

public:
    static const uint_fast64_t cellWidth = (1000) / 2.0;//±1000km range from center of Cell

    Cell(Particle *particle);
    Cell();
    bool addParticle(Particle *particle);
    void removeParticle(Particle *particle);
    std::pair<bool, std::vector<Particle*>> validateCell();
    std::pair<bool, Particle*> validateParticle(Particle *particle);
    void moveParticle(Particle *particle);

    glm::vec4 getPosition();
    double getMass();
};


#endif //INC_3DASTRO_CELL_HPP
