#include "../include/Drone.h"
Drone::Drone(const std::string &nom)
{
    this->nom = nom;
    this->capaciteMax = 3.0f;
    this->vitesse = 80.0f;
    this->coutParKm = 2.0f;
    this->disponible = true;
}
bool Drone::canDeliver(const Colis &c) const
{
    if (c.getPoids() > capaciteMax || c.getDistance() > 50.0f)
        return false;
    if (c.getType() != TypeColis::STANDARD && c.getType() != TypeColis::FRAGILE)
        return false;
    return true;
}
float Drone::computeCost(const Colis &c) const
{
    float coutBase = (coutParKm * c.getDistance()) + (c.getPoids() * 0.5f);
    if (c.getType() == TypeColis::FRAGILE)
        coutBase += (coutBase * 0.10f);
    return coutBase;
}
float Drone::getDelay(const Colis &c) const
{
    float delaiBase = (c.getDistance() / vitesse) + (2.0f / 60.0f);
    if (c.getType() == TypeColis::FRAGILE)
        delaiBase += (delaiBase * 0.20f);
    return delaiBase;
}
