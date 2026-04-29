#ifndef DRONE_H
#define DRONE_H

#include "Transporteur.h"
#include "Colis.h"

class Drone : public Transporteur {
public:
    Drone();
    bool canDeliver(const Colis& c) override;
    float computeCost(const Colis& c) override;
    float getDelay(const Colis& c) override;
    
    virtual ~Drone() = default;
};

#endif