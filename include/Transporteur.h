#pragma once

#include <string>

#include "Colis.h"

/**
 * @brief Contrat polymorphique de base pour tous les transporteurs.
 *
 * Cette interface permet d'évaluer l'éligibilité d'un colis et de calculer
 * le coût de livraison sans aucun test de type explicite.
 */
class Transporteur {
public:
    virtual ~Transporteur() = default;

    /**
     * @brief Indique si ce transporteur peut prendre en charge le colis.
     */
    virtual bool canDeliver(const Colis& c) const = 0;

    /**
     * @brief Calcule le coût de livraison pour une distance donnée.
     */
    virtual double computeCost(const Colis& c, double distance) const = 0;

    /**
     * @brief Nom métier du transporteur (utile pour logs, rapports, UI).
     */
    virtual std::string getName() const = 0;
};
