#pragma once
#include "Transporteur.h"

class Drone final : public Transporteur
{
public:
    Drone(const std::string &nom);
    bool canDeliver(const Colis &c) const override;
    float computeCost(const Colis &c) const override;
    float getDelay(const Colis &c) const override;
};
