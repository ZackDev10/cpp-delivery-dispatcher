#pragma once

#include <string>

#include "Transporteur.h"

/**
 * @brief Transporteur routier adapté aux charges lourdes.
 *
 * Positionnement coût/usage :
 * - capable de transporter des colis lourds (jusqu'à une limite)
 * - coût intermédiaire
 */
class Camion final : public Transporteur {
public:
    bool canDeliver(const Colis& c) const override;
    double computeCost(const Colis& c, double distance) const override;
    std::string getName() const override;

private:
    // Capacité maximale en kg.
    static constexpr double kMaxWeightKg = 1000.0;

    // Paramètres tarifaires.
    static constexpr double kBaseCost = 25.0;               // Frais fixes de prise en charge
    static constexpr double kDistanceCostPerKm = 0.90;      // Coût km (carburant, péage, usure)
    static constexpr double kWeightCostPerKgPerKm = 0.002;  // Surcoût charge lourde par km
};
