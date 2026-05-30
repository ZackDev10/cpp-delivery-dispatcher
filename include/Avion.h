#pragma once
#include "Transporteur.h"

class Avion final : public Transporteur
{
public:
    Avion(const std::string &nom);
    bool canDeliver(const Colis &c) const override;
    float computeCost(const Colis &c) const override;
    float getDelay(const Colis &c) const override;
};
