#include "Avion.h"

#include <algorithm>

bool Avion::canDeliver(const Colis& /*c*/) const {
    // Choix métier : l'avion peut transporter tous les types de colis.
    return true;
}

double Avion::computeCost(const Colis& c, const double distance) const {
    // Validation légère : empêche des coûts incohérents avec des entrées invalides.
    const double safeDistance = std::max(0.0, distance);
    const double safeWeight = std::max(0.0, c.getPoids());

    const double distanceComponent = kDistanceCostPerKm * safeDistance;
    const double handlingComponent = kHandlingCostPerKg * safeWeight;

    // Surcoût pour fret lourd (chargement spécialisé, contraintes de volume/masse).
    const double heavyWeight = std::max(0.0, safeWeight - kHeavyFreightThresholdKg);
    const double heavyFreightSurcharge = heavyWeight * kHeavyFreightSurchargePerKg;

    return kBaseCost + distanceComponent + handlingComponent + heavyFreightSurcharge;
}

std::string Avion::getName() const {
    return "Avion";
}
