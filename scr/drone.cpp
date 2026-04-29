#include "Drone.h"

Drone::Drone() {
    this->nom = "Drone";
    this->capaciteMax = 3.0f;
    this->vitesse = 80.0f;
    this->coutParkm = 2.0f;
    this->disponible = true;
}

bool Drone::canDeliver(const Colis& c) {
    if (c.getPoids() > 3.0f) return false;
    
    if (c.getDistance() > 50.0f) return false;
    
    if (c.getType() != TypeColis::STANDARD && c.getType() != TypeColis::FRAGILE) {
        return false;
    }
    
    return true;
}

float Drone::computeCost(const Colis& c) {
    float coutBase = (coutParkm * c.getDistance()) + (c.getPoids() * 0.5f);
    
    if (c.getType() == TypeColis::FRAGILE) {
        coutBase += coutBase * 0.10f; 
    }
    
    return coutBase;
}

float Drone::getDelay(const Colis& c) {
    float tempsChargement = 2.0f / 60.0f; 
    
    float delaiBase = (c.getDistance() / vitesse) + tempsChargement;
    
    if (c.getType() == TypeColis::FRAGILE) {
        delaiBase += delaiBase * 0.20f;
    }
    
    return delaiBase;
}