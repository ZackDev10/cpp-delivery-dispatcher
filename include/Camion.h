#pragma once
#include "Transporteur.h"

class Camion final : public Transporteur
{
public:
    Camion(const std::string &nom);
    bool canDeliver(const Colis &c) const override;
    float computeCost(const Colis &c) const override;
    float getDelay(const Colis &c) const override;
};
