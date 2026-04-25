#pragma once
#include <string>
#include <vector>

class RapportManager
{
private:
    std::vector<std::string> historiqueSysteme;

public:
    RapportManager() = default;

    void logEvenement(const std::string &message);
    void genererRapportConsole() const;
    void exporterFichier(const std::string &nomFichier) const;
};