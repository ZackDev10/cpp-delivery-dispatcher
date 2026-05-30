#pragma once
#include <vector>
#include <string>
#include "Colis.h"

class Transporteur;

class SystemeLivraison
{
public:
    SystemeLivraison();
    ~SystemeLivraison();
    SystemeLivraison(const SystemeLivraison &) = delete;
    SystemeLivraison &operator=(const SystemeLivraison &) = delete;

    void ajouterTransporteur(Transporteur *transporteur);
    void ajouterColis(const Colis &colis);
    Transporteur *assignerTransporteur(const Colis &colis) const;
    void mettreAJourStatut(int idColis, StatutColis nouveauStatut);

    const std::vector<Colis> &getColis() const;
    const std::vector<Transporteur *> &getTransporteurs() const;

private:
    std::vector<Colis> colis_;
    std::vector<Transporteur *> transporteurs_;
};
