#pragma once

#include <string>

/**
 * @brief Représente un colis à livrer.
 *
 * Ce modèle minimal expose l'API attendue par les transporteurs.
 * Il peut être enrichi ultérieurement (ID, destination, état, etc.)
 * sans casser le contrat de base.
 */
class Colis {
public:
    Colis(double poids = 0.0, std::string type = "Standard")
        : poids_(poids), type_(std::move(type)) {}

    double getPoids() const { return poids_; }
    const std::string& getType() const { return type_; }

private:
    double poids_;
    std::string type_;
};
