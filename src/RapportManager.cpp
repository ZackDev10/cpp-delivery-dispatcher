#include "../include/RapportManager.h"
#include <iostream>
#include <fstream>

void RapportManager::logEvenement(const std::string &message)
{
    historiqueSysteme.push_back(message);
}

void RapportManager::genererRapportConsole() const
{
    std::cout << "\n========== RAPPORT CONSOLE ==========\n";
    for (const auto &log : historiqueSysteme)
    {
        std::cout << log << "\n";
    }
    std::cout << "=====================================\n";
}

void RapportManager::exporterFichier(const std::string &nomFichier) const
{
    // Mode ios::app pour ne pas Ã©craser les anciennes donnÃ©es
    std::ofstream fichier(nomFichier, std::ios::app);

    if (!fichier.is_open())
    {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << nomFichier << " pour l'exportation.\n";
        return;
    }

    fichier << "========== EXPORT DES LOGS ==========\n";
    for (const auto &log : historiqueSysteme)
    {
        fichier << log << "\n";
    }
    fichier << "=====================================\n\n";

    fichier.close();
}