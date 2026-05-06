#ifndef VELO_H
#define VELO_H

#include "Transporteur.h" 
#include "Colis.h"

class Velo final : public Transporteur {
public:
    Velo(const std::string& nom); 

    bool canDeliver(const Colis& c) const override; [cite: 15]
    float computeCost(const Colis& c) const override; [cite: 15]
    float getDelay(const Colis& c) const override; [cite: 15]
};

#endif