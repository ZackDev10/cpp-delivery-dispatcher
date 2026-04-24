#pragma once

#include <string>

#include "Transporteur.h"

/**
 * @brief Transporteur aérien premium, rapide et coûteux.
 *
 * Positionnement coût/usage :
 * - accepte tout colis
 * - frais fixes élevés + coût distance important
 */
class Avion final : public Transporteur {
public:
    bool canDeliver(const Colis& c) const override;
    double computeCost(const Colis& c, double distance) const override;
    std::string getName() const override;

private:
    // Paramètres tarifaires.
    static constexpr double kBaseCost = 180.0;            // Opérations aéroportuaires
    static constexpr double kDistanceCostPerKm = 2.75;    // Coût fortement corrélé au km
    static constexpr double kHandlingCostPerKg = 0.12;    // Manutention, sûreté

    // Seuil au-delà duquel un supplément de fret lourd est appliqué.
    static constexpr double kHeavyFreightThresholdKg = 200.0;
    static constexpr double kHeavyFreightSurchargePerKg = 0.35;
};
