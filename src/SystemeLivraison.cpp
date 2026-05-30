#include "../include/SystemeLivraison.h"
#include "../include/Transporteur.h"
#include "../include/Exceptions.h"
#include <iostream>
#include <limits>

SystemeLivraison::SystemeLivraison() {}
SystemeLivraison::~SystemeLivraison()
{
    for (Transporteur *t : transporteurs_)
        delete t;
    transporteurs_.clear();
}

void SystemeLivraison::ajouterTransporteur(Transporteur *transporteur)
{
    if (!transporteur)
        throw std::invalid_argument("nullptr recu.");
    transporteurs_.push_back(transporteur);
}
void SystemeLivraison::ajouterColis(const Colis &colis) { colis_.push_back(colis); }

Transporteur *SystemeLivraison::assignerTransporteur(const Colis &colis) const
{
    Transporteur *meilleur = nullptr;
    float meilleurCout = std::numeric_limits<float>::max();
    float meilleurVit = 0.0f;

    for (Transporteur *t : transporteurs_)
    {
        if (!t->canDeliver(colis) || !t->isAvailable())
            continue;
        const float cout = t->computeCost(colis);
        const float vitesse = t->getVitesse();

        if (cout < meilleurCout)
        {
            meilleur = t;
            meilleurCout = cout;
            meilleurVit = vitesse;
        }
        else if (cout == meilleurCout && vitesse > meilleurVit)
        {
            meilleur = t;
            meilleurVit = vitesse;
        }
    }

    if (!meilleur)
        std::cerr << "Aucun transporteur dispo pour le colis #" << colis.getId() << ".\n";
    else
        std::cout << "Colis #" << colis.getId() << " -> " << meilleur->getNom() << " | " << meilleurCout << " MAD | " << meilleurVit << " km/h\n";

    return meilleur;
}

void SystemeLivraison::mettreAJourStatut(int idColis, StatutColis nouveauStatut)
{
    for (Colis &c : colis_)
    {
        if (c.getId() == idColis)
        {
            c.updateStatut(nouveauStatut);
            return;
        }
    }
    throw ColisNotFoundException(idColis);
}

const std::vector<Colis> &SystemeLivraison::getColis() const { return colis_; }
const std::vector<Transporteur *> &SystemeLivraison::getTransporteurs() const { return transporteurs_; }
