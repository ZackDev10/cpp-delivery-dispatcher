#pragma once

// =============================================================================
// SystemeLivraison.h
// =============================================================================

#include <vector>
#include <string>
#include <stdexcept>

class Colis;
class Transporteur;

class SystemeLivraison
{
public:
    SystemeLivraison();
    ~SystemeLivraison();

    // Pas de copie — ownership exclusif des pointeurs
    SystemeLivraison(const SystemeLivraison &) = delete;
    SystemeLivraison &operator=(const SystemeLivraison &) = delete;

    // --- Flotte ---
    /// @throws std::invalid_argument si nullptr.
    void ajouterTransporteur(Transporteur *transporteur);

    // --- Colis ---
    void ajouterColis(const Colis &colis);

    // --- Moteur polymorphique ---
    /// @return nullptr si aucun transporteur ne peut livrer.
    Transporteur *assignerTransporteur(const Colis &colis) const;

    /// @throws std::out_of_range si l'ID est inconnu.
    void mettreAJourStatut(int idColis, const std::string &statut);

    // --- Accesseurs ---
    const std::vector<Colis> &getColis() const;
    const std::vector<Transporteur *> &getTransporteurs() const;

private:
    std::vector<Colis> colis_;
    std::vector<Transporteur *> transporteurs_;
};
