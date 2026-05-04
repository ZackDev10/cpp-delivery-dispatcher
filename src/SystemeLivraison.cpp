// =============================================================================
// SystemeLivraison.cpp
// =============================================================================

#include "../include/SystemeLivraison.h"
#include "../include/Colis.h"
#include "../include/Transporteur.h"

#include <stdexcept>
#include <limits>
#include <iostream>

// -----------------------------------------------------------------------------
// Constructeur / Destructeur
// -----------------------------------------------------------------------------

SystemeLivraison::SystemeLivraison()
{
    colis_.reserve(32);
    transporteurs_.reserve(8);
}

SystemeLivraison::~SystemeLivraison()
{
    // Libération de toute la flotte.
    for (Transporteur *t : transporteurs_)
        delete t;

    transporteurs_.clear();
}

// -----------------------------------------------------------------------------
// Gestion de la flotte
// -----------------------------------------------------------------------------

void SystemeLivraison::ajouterTransporteur(Transporteur *transporteur)
{
    if (!transporteur)
        throw std::invalid_argument("[SystemeLivraison] ajouterTransporteur : nullptr reçu.");

    transporteurs_.push_back(transporteur);
}

// -----------------------------------------------------------------------------
// Gestion des colis
// -----------------------------------------------------------------------------

void SystemeLivraison::ajouterColis(const Colis &colis)
{
    colis_.push_back(colis);
}

// -----------------------------------------------------------------------------
// Cœur du moteur — attribution polymorphique pure
// -----------------------------------------------------------------------------

Transporteur *SystemeLivraison::assignerTransporteur(const Colis &colis) const
{
    Transporteur *meilleur = nullptr;
    double meilleurCout = std::numeric_limits<double>::max();
    double meilleurVit = 0.0;

    for (Transporteur *t : transporteurs_)
    {
        // Filtrage : compatibilité ET disponibilité — délégués aux virtuels
        if (!t->canDeliver(colis) || !t->isAvailable())
            continue;

        const double cout = t->computeCost(colis);
        const double vitesse = t->getVitesse();

        // Règle 1 : candidat moins cher
        if (cout < meilleurCout)
        {
            meilleur = t;
            meilleurCout = cout;
            meilleurVit = vitesse;
        }
        // Règle 2 : coût identique → départage par vitesse
        else if (cout == meilleurCout && vitesse > meilleurVit)
        {
            meilleur = t;
            meilleurVit = vitesse;
        }
    }

    if (!meilleur)
        std::cerr << "[SystemeLivraison] Aucun transporteur dispo pour le colis #"
                  << colis.getId() << ".\n";
    else
        std::cout << "[SystemeLivraison] Colis #" << colis.getId()
                  << " → " << meilleur->getNom()
                  << " | coût: " << meilleurCout << " €"
                  << " | vitesse: " << meilleurVit << " km/h\n";

    return meilleur;
}

// -----------------------------------------------------------------------------
// Mise à jour de statut
// -----------------------------------------------------------------------------

void SystemeLivraison::mettreAJourStatut(int idColis, const std::string &statut)
{
    for (Colis &c : colis_)
    {
        if (c.getId() == idColis)
        {
            c.setStatut(statut);
            std::cout << "[SystemeLivraison] Colis #" << idColis
                      << " → statut: " << statut << "\n";
            return;
        }
    }
    throw std::out_of_range(
        "[SystemeLivraison] mettreAJourStatut : colis #" + std::to_string(idColis) + " introuvable.");
}

// -----------------------------------------------------------------------------
// Accesseurs
// -----------------------------------------------------------------------------

const std::vector<Colis> &SystemeLivraison::getColis() const
{
    return colis_;
}

const std::vector<Transporteur *> &SystemeLivraison::getTransporteurs() const
{
    return transporteurs_;
}
