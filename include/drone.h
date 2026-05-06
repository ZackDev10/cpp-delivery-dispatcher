#ifndef DRONE_H
#define DRONE_H

#include "Transporteur.h"
#include "Colis.h"

class Drone final : public Transporteur {
public:
    Drone(const std::string& nom);

    bool canDeliver(const Colis& c) const override; [cite: 2]
    float computeCost(const Colis& c) const override; [cite: 2]
    float getDelay(const Colis& c) const override; [cite: 2]
};

#endif