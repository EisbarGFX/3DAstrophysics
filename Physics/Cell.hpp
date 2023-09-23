//
// Created by comppoter on 9/20/23.
//

#ifndef INC_3DASTRO_CELL_HPP
#define INC_3DASTRO_CELL_HPP

#include <glm/vec4.hpp>
#include <algorithm>
#include <numeric>
#include "Particle.hpp"

class Cell;
extern std::map<uint_fast64_t, Cell*> cellMap;

class Cell {
protected:
//    std::vector<double> masses;
    double totalMass;
//    std::vector<glm::vec4> positions;
    glm::vec4 avgPosition{};
    std::vector<Particle*> Particles;
    uint_fast64_t index;

public:
    static const uint_fast64_t cellWidth = (1000) / 2.0;//±1000km range from center of Cell

    Cell(Particle *particle, uint_fast64_t index);
    Cell();
    bool addParticle(Particle *particle);
    void removeParticle(Particle *particle);
    std::pair<bool, std::vector<Particle*>> validateCell();
    std::pair<bool, Particle*> validateParticle(Particle *particle);
    void moveParticle(Particle *particle);

    glm::vec4 getPosition();
    double getMass();
    uint_fast64_t getIndex();
};

//TODO: slight concern would be this can't go higher than Particle->Cell->cellParticle or it breaks the chain
class cellParticle : public Particle {
    //TODO: update position of particle when cell is changed
private:
    Cell *cell;
public:
    cellParticle(Cell *cell, uint_fast64_t index);
};



#endif //INC_3DASTRO_CELL_HPP
