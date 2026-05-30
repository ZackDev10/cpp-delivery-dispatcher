#pragma once
#include "Transporteur.h"

class Velo final : public Transporteur
{
public:
    Velo(const std::string &nom);
    bool canDeliver(const Colis &c) const override;
    float computeCost(const Colis &c) const override;
    float getDelay(const Colis &c) const override;
};
