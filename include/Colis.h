#pragma once

// =============================================================================
// Colis.h  —  STUB (branche Zack)
// ----------------------------------------------------------------------------
// Version autonome pour que la branche du Lead compile sans dépendre du
// Membre 2. Contient uniquement les membres nécessaires au moteur
// SystemeLivraison. À REMPLACER par la version complète du Membre 2 lors
// du merge final dans dev.
// =============================================================================

#include <string>
#include <iostream>

class Colis
{
public:
    // Constructeur minimal : id, poids, dimensions, type
    Colis(int id, double poids, double longueur, double largeur,
          double hauteur, const std::string &type = "standard")
        : id_(id), poids_(poids),
          longueur_(longueur), largeur_(largeur), hauteur_(hauteur),
          type_(type), statut_("EN_ATTENTE")
    {
    }

    // --- Getters (const-correct) ---
    int getId() const { return id_; }
    double getPoids() const { return poids_; }
    double getLongueur() const { return longueur_; }
    double getLargeur() const { return largeur_; }
    double getHauteur() const { return hauteur_; }
    const std::string &getType() const { return type_; }
    const std::string &getStatut() const { return statut_; }

    // --- Setter statut ---
    void setStatut(const std::string &statut) { statut_ = statut; }

    // --- Volume utilitaire ---
    double getVolume() const { return longueur_ * largeur_ * hauteur_; }

    // --- Affichage minimal ---
    void afficher() const
    {
        std::cout << "  Colis #" << id_
                  << " | " << poids_ << " kg"
                  << " | " << longueur_ << "x" << largeur_ << "x" << hauteur_ << " cm"
                  << " | type: " << type_
                  << " | statut: " << statut_ << "\n";
    }

private:
    int id_;
    double poids_;
    double longueur_;
    double largeur_;
    double hauteur_;
    std::string type_;
    std::string statut_;
};
