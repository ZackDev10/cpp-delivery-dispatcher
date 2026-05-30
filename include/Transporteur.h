#pragma once
#include <string>
#include "Colis.h"

class Transporteur {
protected:
    std::string nom; float capaciteMax; float vitesse;
    float coutParKm; bool disponible;
public:
    virtual ~Transporteur() = default;
    virtual bool canDeliver(const Colis& c) const = 0;
    virtual float computeCost(const Colis& c) const = 0;
    virtual float getDelay(const Colis& c) const = 0;
    bool isAvailable() const { return disponible; }
    std::string getNom() const { return nom; }
    float getVitesse() const { return vitesse; }
};
