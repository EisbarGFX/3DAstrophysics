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

Cell::Cell(Particle *particle) {
    Particles.push_back(particle);

//    masses.push_back(particle->getMass());
    totalMass = particle->getMass();

//    positions.push_back(particle->getPPosition());
    avgPosition = particle->getPPosition();

    particle->grouped = true;
}

Cell::Cell() {
    avgPosition = glm::vec4(0,0,0,1);
}

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
    }
    return false;
}

void Cell::removeParticle(Particle *particle) {
    Particles.erase(std::remove_if(
            Particles.begin(),
            Particles.end(),
            [particle](Particle* p){
                return p==particle;
            }
            ));
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

    // TODO: are there more validation checks?

    avgPosition = std::accumulate(Particles.begin(), Particles.end(), Particles[0]->getPPosition(),
                                  [](glm::vec4 p1, Particle* p2) {
                                      auto x = (p1.x + p2->getPPosition().x) / 2.0f;
                                      auto y = (p1.y + p2->getPPosition().y) / 2.0f;
                                      auto z = (p1.z + p2->getPPosition().z) / 2.0f;
                                      return glm::vec4(x,y,z,1);
                                  });

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


glm::vec4 Cell::getPosition() {
    return avgPosition;
}
double Cell::getMass() {
    return totalMass;
}
