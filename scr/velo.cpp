#include "Velo.h"


Velo::Velo() {
    this->nom = "Velo";
    this->capaciteMax = 5.0f; 
    this->vitesse = 20.0f; 
    this->coutParkm = 0.5f;   
    this->disponible = true;  
}


bool Velo::canDeliver(const Colis& c) {

    if (c.getPoids() > 5.0f) return false;
    
    
    if (c.getDistance() > 30.0f) return false;

   
    if (c.getType() != TypeColis::STANDARD) return false;
    
    
    return true;
}

float Velo::computeCost(const Colis& c) {
   
    return (coutParkm * c.getDistance()) + (c.getPoids() * 0.1f);
}

float Velo::getDelay(const Colis& c) {

    float tempsChargement = 5.0f / 60.0f; 
    return (c.getDistance() / vitesse) + tempsChargement;
}