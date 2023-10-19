//
// Created by comppoter on 9/20/23.
//


#include "Cell.hpp"


/*
    std::vector<double> masses;
    double totalMass;
    std::vector<glm::vec4> positions;
    glm::vec4 avgPosition;
    std::vector<Particle*> Particles;
 */

Cell::Cell(Particle *particle, uint_fast64_t index) {
    this->Particles.push_back(particle);

//    masses.push_back(particle->getMass());
    this->totalMass = particle->getMass();

//    positions.push_back(particle->getPPosition());
    this->avgPosition = particle->getPPosition();
    this->index = index;

    this->particleIndex = particleMap.size();
    // TODO: finish converting this
//    CellParticle cp = CellParticle(this, particleIndex);
//    Particle* ptr = &cp;
    Particle* cp = new Particle(this, particleIndex);
    particleMap.emplace(particleIndex, cp);

    particle->grouped = true;
}

Cell::Cell() {
    avgPosition = glm::vec4(0,0,0,1);
    totalMass = 0;
    index = 0;
    particleIndex = 0;
}

/// Inputs a \c Particle object and updates the \c Cell info regarding position, mass, and \c Particle list. If the \c Cell already contains 8 or more \c Particles, attempts are made to remove invalid \c Particles and replace them. This is done recursively until all \c Particles are valid or there are fewer than 8.
/// \param particle Pointer to a \c Particle to add to \c Cell
/// \retval \c True if Particle was successfully added
/// \retval \c False if unsuccessful
bool Cell::addParticle(Particle *particle) {
    if (particle->grouped) {return false;}

    if (Particles.size()<8) {
        Particles.push_back(particle);
//        masses.push_back(particle->getMass());
//        positions.push_back(particle->getPPosition());

        totalMass += particle->getMass();
        avgPosition = std::accumulate(Particles.begin(), Particles.end(), Particles[0]->getPPosition(),
                                    [](glm::vec4 p1, Particle* p2) {
                        auto x = (p1.x + p2->getPPosition().x) / 2.0f;
                        auto y = (p1.y + p2->getPPosition().y) / 2.0f;
                        auto z = (p1.z + p2->getPPosition().z) / 2.0f;
                        return glm::vec4(x,y,z,1);
                                    });
        particle->grouped = true;
        return true;
    }
    else {
        auto validationCheck = validateParticle(particle);
        if (!validationCheck.first) {
            removeParticle(validationCheck.second);
            return addParticle(particle);
        }
        else {return false;}
    }
}

void Cell::removeParticle(Particle *particle) {
    Particles.erase(std::remove_if(
            Particles.begin(),
            Particles.end(),
            [particle](Particle* p){
                return p==particle;
            }
            ), Particles.end());
    totalMass-=particle->getMass();
//    positions.erase(std::remove_if(
//            positions.begin(),
//            positions.end(),
//            [particle](glm::vec4 pos) {
//                return pos==particle->getPPosition();
//            }
//            ));
    particle->grouped = false;
    avgPosition = std::accumulate(Particles.begin(), Particles.end(), Particles[0]->getPPosition(),
                                  [](glm::vec4 p1, Particle* p2) {
                                      auto x = (p1.x + p2->getPPosition().x) / 2.0f;
                                      auto y = (p1.y + p2->getPPosition().y) / 2.0f;
                                      auto z = (p1.z + p2->getPPosition().z) / 2.0f;
                                      return glm::vec4(x,y,z,1);
                                  });

    if (this->Particles.empty()) {
        cellMap.erase(this->index);
        delete this;
    }
}


[[nodiscard]] std::pair<bool, std::vector<Particle*>> Cell::validateCell() {
    std::vector<Particle*> invalid;
    bool validated = true;
    for (auto p : Particles) {
        if (glm::distance(p->getPPosition(), avgPosition) > Cell::cellWidth) {
            invalid.push_back(p);
            removeParticle(p);
            validated = false;
        }
    }

    if (Particles.size() >= 8) {
        Particle* furthest = nullptr;
        for (auto p : Particles) {
            if (furthest==nullptr ||
            glm::distance(p->getPPosition(), avgPosition) > glm::distance(furthest->getPPosition(), avgPosition)) {
                furthest = p;
            }
        }
        invalid.push_back(furthest);
        removeParticle(furthest);
        validated = false;
    }
    else if (!Particles.empty()) {
        // TODO: are there more validation checks?

        // TODO: CENTER OF MASS -- apply to all other avgPosition udates
        auto center = std::accumulate(Particles.begin(), Particles.end(), Particles[0]->getPPosition(),
                                      [](glm::vec4 p1, Particle *p2) {
                                          auto x = (p1.x + p2->getPPosition().x) / 2.0f;
                                          auto y = (p1.y + p2->getPPosition().y) / 2.0f;
                                          auto z = (p1.z + p2->getPPosition().z) / 2.0f;
                                          return glm::vec4(x, y, z, 1);
                                      });
        std::vector<glm::vec4> distancesFromCenter;
        for (auto p: Particles) {
            distancesFromCenter.push_back(
                    (center - p->getPPosition()) * (float) (p->getMass() / totalMass));
        }


        avgPosition = std::accumulate(distancesFromCenter.begin(), distancesFromCenter.end(), center,
                                      [](glm::vec4 p1, glm::vec4 p2) {
                                          auto x = p1.x + p2.x;
                                          auto y = p1.y + p2.y;
                                          auto z = p1.z + p2.z;
                                          return glm::vec4(x, y, z, 1);
                                      }
        );
    }
    else if (Particles.empty()) {avgPosition = glm::vec4(1.0);}

    return {validated, invalid};
}

[[nodiscard]] std::pair<bool, Particle*> Cell::validateParticle(Particle *particle) {
    Particle* furthest = nullptr;
    for (auto p : Particles) {
        if (furthest==nullptr ||
            glm::distance(p->getPPosition(), avgPosition) > glm::distance(furthest->getPPosition(), avgPosition)) {
            furthest = p;
        }
    }
    if (glm::distance(particle->getPPosition(), avgPosition) > glm::distance(furthest->getPPosition(), avgPosition)) {
        return {false, furthest};
    }
    return {true, particle};
}

void Cell::moveParticle(Particle *particle) {
//    this->removeParticle(particle);
//    this->addParticle(particle);

}


Cell &Cell::operator=(const Cell &sb) {
    this->index = sb.index;
    this->particleIndex = sb.particleIndex;
    this->totalMass = sb.totalMass;
    this->Particles.clear();
    for (auto p : sb.Particles) {this->Particles.push_back(p);}
    this->avgPosition = sb.avgPosition;
    return *this;
}
bool Cell::operator<(const Cell &sb) {
    return this->index < sb.index;
}
bool Cell::operator>(const Cell &sb) {
    return this->index > sb.index;
}
bool Cell::operator==(const Cell &sb) {
    return (this->avgPosition == sb.avgPosition && this->index == sb.index);
}


glm::vec4 Cell::getPosition() const {
    return avgPosition;
}

double Cell::getMass() const {
    return totalMass;
}

uint_fast64_t Cell::getIndex() const {
    return index;
}

std::vector<Particle*> Cell::getParticles() const {
    return Particles;
}

uint_fast64_t Cell::getParticleIndex() const {
    return particleIndex;
}



/*
 * Particle Values
    double mass = 0.0;
    uint_fast64_t planetaryIndex = 0;
    uint_fast64_t cellIndex = 0;
    glm::vec4 graphicalPosition = glm::vec4(0, 0, 0, 1);
    glm::vec4 physicalPosition = glm::vec4(0,0,0,1);
    glm::vec4 graphicalMovement = glm::vec4(0, 0, 0, 0);
    glm::vec4 physicalMovement = glm::vec4(0,0,0,0);
    std::string name;
 */

/*
 *
 * CellParticle
 *
 *
*/

__attribute_deprecated_msg__("Use Particle instead.")
CellParticle::CellParticle(Cell *cell, uint_fast64_t index) {
    this->mass = cell->getMass();
    this->planetaryIndex = index;
    this->cell = cell;
    this->physicalPosition = cell->getPosition();
    graphicalPosition = scalePos(cell->getPosition(), physToGraphConversion);
//    this->name = static_cast<std::string *>(malloc(sizeof(char) * 256));
//    *this->name = std::string("Particle Representing Cell %i", cell->getIndex());
}

__attribute_deprecated_msg__("Use Particle::update instead.")
void CellParticle::update() {
    physicalPosition = cell->getPosition();
    graphicalPosition = scalePos(cell->getPosition(), physToGraphConversion);
    auto Particles = cell->getParticles();

    auto mvmnt = std::accumulate(Particles.begin(), Particles.end(), Particles.at(0)->getPMovement(),
                                 [](glm::vec4 m1, Particle *p2) {
                                     auto m2 = p2->getPMovement();
                                     auto x = (m1.x + m2.x) / 2.0f;
                                     auto y = (m1.y + m2.y) / 2.0f;
                                     auto z = (m1.z + m2.z) / 2.0f;
                                     return glm::vec4(x, y, z, 0);
                                 });

    physicalMovement = mvmnt;
    graphicalMovement = scalePos(mvmnt, physToGraphConversion);
}



/*
 *
 *
 * Cell dependant Particle functions
 *
 *
 */

Particle::Particle(Cell *c, uint_fast64_t index) {
    this->representedCell = c;
    this->mass = c->getMass();
    this->planetaryIndex = index;
    this->physicalPosition = c->getPosition();
    this->graphicalPosition = scalex44Matrix(physToGraphConversion)*c->getPosition();
    this->name = "Particle Representing Cell " + std::to_string(c->getIndex());
    this->cellParticle = true;
}

//TODO: update position of particle when representedCell is changed - transfer from CellParticle::update()
void Particle::update() {
    physicalPosition = representedCell->getPosition();
    graphicalPosition = scalePos(representedCell->getPosition(), physToGraphConversion);
    auto Particles = representedCell->getParticles();

    auto mvmnt = std::accumulate(Particles.begin(), Particles.end(), Particles.at(0)->getPMovement(),
                                 [](glm::vec4 m1, Particle* p2){
                                     auto m2 = p2->getPMovement();
                                     auto x = (m1.x + m2.x) / 2.0f;
                                     auto y = (m1.y + m2.y) / 2.0f;
                                     auto z = (m1.z + m2.z) / 2.0f;
                                     return glm::vec4(x,y,z,0);
                                 });

    physicalMovement = mvmnt;
    graphicalMovement = scalePos(mvmnt, physToGraphConversion);
}
