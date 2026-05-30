#pragma once
#include <string>
#include <vector>
#include <stdexcept>

enum class TypeColis { STANDARD, FRAGILE, PERISSABLE, DANGEREUX };
enum class StatutColis { EN_ATTENTE, EN_TRANSIT, LIVRE, ANNULE };

struct Dimensions {
    float longueur; float largeur; float hauteur;
};

class Colis {
private:
    int id; float poids; Dimensions dimensions;
    TypeColis type; StatutColis statut; float distance_km;
    std::string adresse_destination; std::string date_creation;
    std::vector<std::string> historique_statuts;
    std::string genererHorodatage() const;

public:
    Colis(int id, float poids, Dimensions dim, TypeColis type, float distance_km, const std::string& adresse);
    Colis(int id, float poids, Dimensions dim, TypeColis type, float distance_km, const std::string& adresse, StatutColis statut);
    int getId() const; float getPoids() const; float getDistance() const;
    TypeColis getType() const; StatutColis getStatut() const; float getVolume() const;
    Dimensions getDimensions() const; const std::string &getDestination() const;
    void updateStatut(StatutColis nouveauStatut);
    void afficher() const;
};
