#include "Velo.h"

Velo::Velo(const std::string& nom) : Transporteur(nom) { [cite: 16]
    this->capaciteMax = 5.0f; [cite: 17]
    this->vitesse = 20.0f; [cite: 17]
    this->coutParKm = 0.5f; [cite: 17]
    this->disponible = true; [cite: 17]
}

bool Velo::canDeliver(const Colis& c) const { [cite: 15]
    if (c.getPoids() > 5.0f) return false; [cite: 18]
    if (c.getDistance() > 30.0f) return false; [cite: 18]
    if (c.getType() != TypeColis::STANDARD) return false; [cite: 18]
    return true; [cite: 19]
}

float Velo::computeCost(const Colis& c) const { [cite: 15]
    return (coutParKm * c.getDistance()) + (c.getPoids() * 0.1f); [cite: 20]
}

float Velo::getDelay(const Colis& c) const { [cite: 15]
    float tempsChargement = 5.0f / 60.0f; 
    return (c.getDistance() / vitesse) + tempsChargement; [cite: 21]
}