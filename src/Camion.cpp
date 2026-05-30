#include "../include/Camion.h"
Camion::Camion(const std::string &nom)
{
    this->nom = nom;
    this->capaciteMax = 5000.0f;
    this->vitesse = 90.0f;
    this->coutParKm = 3.0f;
    this->disponible = true;
}
bool Camion::canDeliver(const Colis &c) const
{
    return (c.getPoids() <= capaciteMax && c.getDistance() <= 500.0f);
}
float Camion::computeCost(const Colis &c) const
{
    float cost = (coutParKm * c.getDistance()) + (c.getPoids() * 0.2f);
    if (c.getType() == TypeColis::FRAGILE)
        cost += (cost * 0.05f);
    return cost;
}
float Camion::getDelay(const Colis &c) const
{
    float delay = (c.getDistance() / vitesse) + 0.5f;
    if (c.getType() == TypeColis::FRAGILE)
        delay += (delay * 0.20f);
    return delay;
}
