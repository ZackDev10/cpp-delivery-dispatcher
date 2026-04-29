#ifndef VELO_H
#define VELO_H

#include "Transporteur.h" 
#include "Colis.h"


class Velo : public Transporteur {
public:
    
    Velo();

    
    bool canDeliver(const Colis& c) override;
    float computeCost(const Colis& c) override;
    float getDelay(const Colis& c) override;
};

#endif