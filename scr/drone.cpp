#include "Drone.h"

Drone::Drone(const std::string& nom) : Transporteur(nom) { [cite: 3]
    this->capaciteMax = 3.0f; [cite: 3]
    this->vitesse = 80.0f; [cite: 3]
    this->coutParKm = 2.0f; [cite: 3]
    this->disponible = true; [cite: 4]
}

bool Drone::canDeliver(const Colis& c) const { [cite: 2]
    if (c.getPoids() > 3.0f) return false; [cite: 4]
    if (c.getDistance() > 50.0f) return false; [cite: 5]
    if (c.getType() != TypeColis::STANDARD && c.getType() != TypeColis::FRAGILE) { [cite: 5]
        return false; [cite: 6]
    }
    return true; [cite: 7]
}

float Drone::computeCost(const Colis& c) const { [cite: 2]
    float coutBase = (coutParKm * c.getDistance()) + (c.getPoids() * 0.5f); [cite: 7]
    if (c.getType() == TypeColis::FRAGILE) { [cite: 8]
        coutBase += coutBase * 0.10f; [cite: 8]
    }
    return coutBase; [cite: 10]
}

float Drone::getDelay(const Colis& c) const { [cite: 2]
    float tempsChargement = 2.0f / 60.0f; 
    float delaiBase = (c.getDistance() / vitesse) + tempsChargement; [cite: 11]
    if (c.getType() == TypeColis::FRAGILE) { [cite: 12]
        delaiBase += delaiBase * 0.20f; [cite: 12]
    }
    return delaiBase; [cite: 13]
}