#include "../include/Avion.h"
Avion::Avion(const std::string &nom)
{
    this->nom = nom;
    this->capaciteMax = 10000.0f;
    this->vitesse = 900.0f;
    this->coutParKm = 15.0f;
    this->disponible = true;
}
bool Avion::canDeliver(const Colis &c) const { return (c.getPoids() <= capaciteMax); }
float Avion::computeCost(const Colis &c) const
{
    float cost = (coutParKm * c.getDistance()) + (c.getPoids() * 1.0f);
    if (c.getType() == TypeColis::FRAGILE)
        cost += (cost * 0.15f);
    return cost;
}
float Avion::getDelay(const Colis &c) const
{
    float delay = (c.getDistance() / vitesse) + 2.0f;
    if (c.getType() == TypeColis::FRAGILE)
        delay += (delay * 0.20f);
    return delay;
}
