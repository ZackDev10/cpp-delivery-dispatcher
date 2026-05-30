#pragma once
#include <string>
#include <vector>

class Colis;
class Transporteur;

class RapportManager {
private:
    std::vector<std::string> historiqueSysteme;
public:
    RapportManager() = default;
    void logEvenement(const std::string &message);
    void genererRapportConsole() const;
    void exporterFichier(const std::string &nomFichier) const;
    void exporterFichier(const std::string &nomFichier,
                         const std::vector<Colis> &colis,
                         const std::vector<Transporteur *> &transporteurs,
                         const std::vector<std::string> &assignments) const;
};
