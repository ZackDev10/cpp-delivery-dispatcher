#include "../include/Velo.h"
Velo::Velo(const std::string &nom)
{
    this->nom = nom;
    this->capaciteMax = 5.0f;
    this->vitesse = 20.0f;
    this->coutParKm = 0.5f;
    this->disponible = true;
}
bool Velo::canDeliver(const Colis &c) const
{
    if (c.getPoids() > capaciteMax || c.getDistance() > 30.0f || c.getType() != TypeColis::STANDARD)
        return false;
    return true;
}
float Velo::computeCost(const Colis &c) const { return (coutParKm * c.getDistance()) + (c.getPoids() * 0.1f); }
float Velo::getDelay(const Colis &c) const { return (c.getDistance() / vitesse) + (5.0f / 60.0f); }
