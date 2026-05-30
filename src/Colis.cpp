#include "../include/Colis.h"
#include "../include/Exceptions.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string Colis::genererHorodatage() const
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

Colis::Colis(int id, float poids, Dimensions dim, TypeColis type, float distance_km, const std::string &adresse)
    : id(id), type(type), statut(StatutColis::EN_ATTENTE), adresse_destination(adresse)
{
    if (poids <= 0.0f)
        throw std::invalid_argument("Poids invalide.");
    if (distance_km <= 0.0f)
        throw std::invalid_argument("Distance invalide.");
    if (dim.longueur <= 0 || dim.largeur <= 0 || dim.hauteur <= 0)
        throw std::invalid_argument("Dimensions invalides.");

    this->poids = poids;
    this->dimensions = dim;
    this->distance_km = distance_km;
    this->date_creation = genererHorodatage();
    this->historique_statuts.push_back("[" + this->date_creation + "] Creation (EN_ATTENTE)");
}

Colis::Colis(int id, float poids, Dimensions dim, TypeColis type, float distance_km, const std::string &adresse, StatutColis statut)
    : id(id), type(type), statut(statut), adresse_destination(adresse)
{
    if (poids <= 0.0f)
        throw std::invalid_argument("Poids invalide.");
    if (distance_km <= 0.0f)
        throw std::invalid_argument("Distance invalide.");
    if (dim.longueur <= 0 || dim.largeur <= 0 || dim.hauteur <= 0)
        throw std::invalid_argument("Dimensions invalides.");

    this->poids = poids;
    this->dimensions = dim;
    this->distance_km = distance_km;
    this->date_creation = genererHorodatage();
    this->historique_statuts.push_back("[" + this->date_creation + "] Chargement (PERSISTENCE)");
}

int Colis::getId() const { return id; }
float Colis::getPoids() const { return poids; }
float Colis::getDistance() const { return distance_km; }
TypeColis Colis::getType() const { return type; }
StatutColis Colis::getStatut() const { return statut; }
float Colis::getVolume() const { return dimensions.longueur * dimensions.largeur * dimensions.hauteur; }
Dimensions Colis::getDimensions() const { return dimensions; }
const std::string &Colis::getDestination() const { return adresse_destination; }

void Colis::updateStatut(StatutColis nouveauStatut)
{
    if (nouveauStatut == statut)
        return;
    bool transitionValide = false;
    switch (statut)
    {
    case StatutColis::EN_ATTENTE:
        if (nouveauStatut == StatutColis::EN_TRANSIT || nouveauStatut == StatutColis::ANNULE)
            transitionValide = true;
        break;
    case StatutColis::EN_TRANSIT:
        if (nouveauStatut == StatutColis::LIVRE)
            transitionValide = true;
        break;
    case StatutColis::LIVRE:
    case StatutColis::ANNULE:
        transitionValide = false;
        break;
    }
    if (!transitionValide)
        throw InvalidStateTransitionException();
    statut = nouveauStatut;
    historique_statuts.push_back("[" + genererHorodatage() + "] Mise a jour du statut.");
}

void Colis::afficher() const
{
    std::cout << "Colis #" << id << " | Poids: " << poids << "kg | Dist: " << distance_km << "km | Dest: " << adresse_destination << "\n";
}
