#include "Camion.h"

#include <algorithm>

bool Camion::canDeliver(const Colis& c) const {
    // Tolérance défensive : un poids négatif (donnée invalide) est ramené à 0.
    const double safeWeight = std::max(0.0, c.getPoids());
    return safeWeight <= kMaxWeightKg;
}

double Camion::computeCost(const Colis& c, const double distance) const {
    // Validation légère : évite des coûts négatifs si des données amont sont incorrectes.
    const double safeDistance = std::max(0.0, distance);
    const double safeWeight = std::max(0.0, c.getPoids());

    // Formule réaliste (transport routier) :
    //   coût fixe + coût linéaire à la distance + impact du poids sur la distance.
    const double distanceComponent = kDistanceCostPerKm * safeDistance;
    const double weightDistanceComponent = kWeightCostPerKgPerKm * safeWeight * safeDistance;

    return kBaseCost + distanceComponent + weightDistanceComponent;
}

std::string Camion::getName() const {
    return "Camion";
}
