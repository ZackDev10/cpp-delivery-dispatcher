#include "../include/RapportManager.h"
#include "../include/Colis.h"
#include "../include/Transporteur.h"
#include <iostream>
#include <fstream>
#include <iomanip>

namespace
{
    std::string typeToString(TypeColis type)
    {
        switch (type)
        {
        case TypeColis::STANDARD:
            return "Standard";
        case TypeColis::FRAGILE:
            return "Fragile";
        case TypeColis::PERISSABLE:
            return "Perissable";
        case TypeColis::DANGEREUX:
            return "Dangereux";
        default:
            return "-";
        }
    }

    std::string statutToString(StatutColis statut)
    {
        switch (statut)
        {
        case StatutColis::EN_ATTENTE:
            return "En attente";
        case StatutColis::EN_TRANSIT:
            return "En transit";
        case StatutColis::LIVRE:
            return "Livre";
        case StatutColis::ANNULE:
            return "Annule";
        default:
            return "-";
        }
    }
}

void RapportManager::logEvenement(const std::string &message) { historiqueSysteme.push_back(message); }

void RapportManager::genererRapportConsole() const
{
    std::cout << "\n========== RAPPORT CONSOLE ==========\n";
    for (const auto &log : historiqueSysteme)
        std::cout << log << "\n";
    std::cout << "=====================================\n";
}

void RapportManager::exporterFichier(const std::string &nomFichier) const
{
    std::ofstream fichier(nomFichier, std::ios::app);
    if (!fichier.is_open())
    {
        std::cerr << "Erreur fichier.\n";
        return;
    }
    fichier << "========== EXPORT DES LOGS ==========\n";
    for (const auto &log : historiqueSysteme)
        fichier << log << "\n";
    fichier << "=====================================\n\n";
    fichier.close();
}

void RapportManager::exporterFichier(const std::string &nomFichier,
                                     const std::vector<Colis> &colis,
                                     const std::vector<Transporteur *> &transporteurs,
                                     const std::vector<std::string> &assignments) const
{
    std::ofstream fichier(nomFichier, std::ios::trunc);
    if (!fichier.is_open())
    {
        std::cerr << "Erreur fichier.\n";
        return;
    }

    fichier << "========== RAPPORT LOGISTIQUE COMPLET ==========" << "\n\n";

    fichier << "--- EVENEMENTS SYSTEME ---\n";
    for (const auto &log : historiqueSysteme)
        fichier << "- " << log << "\n";
    fichier << "\n";

    fichier << "--- LISTE DES COLIS ---\n";
    fichier << std::left << std::setw(6) << "ID"
            << std::setw(12) << "Type"
            << std::setw(12) << "Statut"
            << std::setw(10) << "Poids"
            << std::setw(10) << "Dist"
            << std::setw(10) << "Volume"
            << "Destination" << "\n";
    fichier << std::string(80, '-') << "\n";
    for (const auto &c : colis)
    {
        fichier << std::left << std::setw(6) << c.getId()
                << std::setw(12) << typeToString(c.getType())
                << std::setw(12) << statutToString(c.getStatut())
                << std::setw(10) << std::fixed << std::setprecision(2) << c.getPoids()
                << std::setw(10) << std::fixed << std::setprecision(1) << c.getDistance()
                << std::setw(10) << std::fixed << std::setprecision(2) << c.getVolume()
                << c.getDestination() << "\n";
    }
    fichier << "\n";

    fichier << "--- TRANSPORTEURS DISPONIBLES ---\n";
    for (const auto *t : transporteurs)
    {
        if (!t)
            continue;
        fichier << "- " << t->getNom() << " | Vitesse: " << t->getVitesse() << " km/h\n";
    }
    fichier << "\n";

    fichier << "--- ASSIGNATIONS ---\n";
    if (assignments.empty())
    {
        fichier << "Aucune assignation enregistree.\n";
    }
    else
    {
        for (const auto &line : assignments)
            fichier << line << "\n";
    }

    fichier << "\n===============================================\n";
    fichier.close();
}
