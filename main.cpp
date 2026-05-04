// =============================================================================
// main.cpp  
// =============================================================================

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <stdexcept>

#include "SystemeLivraison.h"
#include "Colis.h"
#include "Transporteur.h"

// =============================================================================
// Couleurs ANSI
// =============================================================================
namespace Couleur
{
    constexpr const char *RESET = "\033[0m";
    constexpr const char *BOLD = "\033[1m";
    constexpr const char *DIM = "\033[2m";
    constexpr const char *BLANC = "\033[97m";
    constexpr const char *CYAN = "\033[96m";
    constexpr const char *VERT = "\033[92m";
    constexpr const char *ROUGE = "\033[91m";
    constexpr const char *JAUNE = "\033[93m";
    constexpr const char *BLEU = "\033[94m";
    constexpr const char *BG_BLEU = "\033[44m";
}

// =============================================================================
// Helpers affichage
// =============================================================================

static void separateur(char c = '-', int n = 50)
{
    std::cout << Couleur::DIM;
    for (int i = 0; i < n; ++i)
        std::cout << c;
    std::cout << Couleur::RESET << "\n";
}

static void titreSection(const std::string &titre)
{
    std::cout << "\n";
    separateur();
    std::cout << Couleur::BOLD << Couleur::CYAN << "  " << titre << "\n"
              << Couleur::RESET;
    separateur();
}

static void ok(const std::string &msg)
{
    std::cout << Couleur::VERT << "  [OK] " << Couleur::RESET << msg << "\n";
}
static void erreur(const std::string &msg)
{
    std::cerr << Couleur::ROUGE << "  [!!] " << Couleur::RESET << msg << "\n";
}
static void info(const std::string &msg)
{
    std::cout << Couleur::BLEU << "  [--] " << Couleur::RESET << msg << "\n";
}

// =============================================================================
// Menu
// =============================================================================

static int afficherMenu()
{
    std::cout << "\n";
    separateur('=');
    std::cout << Couleur::BOLD << Couleur::BG_BLEU << Couleur::BLANC
              << "   SIMULATEUR DE SYSTEME DE LIVRAISON   "
              << Couleur::RESET << "\n";
    std::cout << Couleur::DIM
              << "   Mode standalone  |  Moteur polymorphique C++17\n"
              << Couleur::RESET;
    separateur('=');

    std::cout << Couleur::CYAN << "  [1]" << Couleur::RESET << "  Creer un colis\n";
    std::cout << Couleur::CYAN << "  [2]" << Couleur::RESET << "  Mettre a jour le statut d'un colis\n";
    std::cout << Couleur::CYAN << "  [3]" << Couleur::RESET << "  Assigner un transporteur\n";
    std::cout << Couleur::CYAN << "  [4]" << Couleur::RESET << "  Afficher tous les colis\n";
    std::cout << Couleur::CYAN << "  [5]" << Couleur::RESET << "  Afficher la flotte\n";
    std::cout << Couleur::JAUNE << "  [9]" << Couleur::RESET << "  Lancer les scenarios de validation\n";
    std::cout << Couleur::ROUGE << "  [0]" << Couleur::RESET << "  Quitter\n";

    separateur();
    std::cout << Couleur::BOLD << "  Votre choix : " << Couleur::RESET;

    int choix{};
    std::cin >> choix;
    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choix;
}

// =============================================================================
// Saisie
// =============================================================================

static int lireEntier(const std::string &invite)
{
    std::cout << Couleur::JAUNE << "  > " << Couleur::RESET << invite;
    int v{};
    std::cin >> v;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return v;
}
static double lireDouble(const std::string &invite)
{
    std::cout << Couleur::JAUNE << "  > " << Couleur::RESET << invite;
    double v{};
    std::cin >> v;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return v;
}
static std::string lireLigne(const std::string &invite)
{
    std::cout << Couleur::JAUNE << "  > " << Couleur::RESET << invite;
    std::string v{};
    std::getline(std::cin, v);
    return v;
}

// =============================================================================
// Scenarios de validation
// =============================================================================

static void lancerScenariosTest(SystemeLivraison &sys)
{
    titreSection("SCENARIOS DE VALIDATION  -  MOTEUR POLYMORPHIQUE");
    int pass = 0, fail = 0;

    auto resultat = [&](bool ok_)
    {
        std::cout << (ok_ ? Couleur::VERT : Couleur::ROUGE)
                  << "         " << (ok_ ? "[PASS]" : "[FAIL]")
                  << Couleur::RESET << "\n";
        ok_ ? ++pass : ++fail;
    };

    // Test 1 : departage par vitesse a cout egal
    std::cout << Couleur::BOLD << "\n  [Test 1]" << Couleur::RESET
              << " Colis 5 kg - departage par vitesse a cout egal\n";
    Colis c1(101, 5.0, 20.0, 15.0, 10.0, "standard");
    sys.ajouterColis(c1);
    Transporteur *r1 = sys.assignerTransporteur(c1);
    std::cout << "         Attendu : FakeSamePrice  |  Obtenu : "
              << (r1 ? r1->getNom() : "nullptr") << "\n";
    resultat(r1 && r1->getNom() == "FakeSamePrice");

    // Test 2 : seul candidat sur colis lourd
    std::cout << Couleur::BOLD << "\n  [Test 2]" << Couleur::RESET
              << " Colis 35 kg - seul FakeRapide accepte\n";
    Colis c2(102, 35.0, 60.0, 40.0, 30.0, "standard");
    sys.ajouterColis(c2);
    Transporteur *r2 = sys.assignerTransporteur(c2);
    std::cout << "         Attendu : FakeRapide  |  Obtenu : "
              << (r2 ? r2->getNom() : "nullptr") << "\n";
    resultat(r2 && r2->getNom() == "FakeRapide");

    // Test 3 : statut sur ID existant
    std::cout << Couleur::BOLD << "\n  [Test 3]" << Couleur::RESET
              << " mettreAJourStatut() sur colis existant (#101)\n";
    bool t3 = false;
    try
    {
        sys.mettreAJourStatut(101, "EN_TRANSIT");
        t3 = true;
    }
    catch (const std::exception &e)
    {
        std::cout << "         Exception inattendue : " << e.what() << "\n";
    }
    resultat(t3);

    // Test 4 : exception sur ID inconnu
    std::cout << Couleur::BOLD << "\n  [Test 4]" << Couleur::RESET
              << " mettreAJourStatut() sur ID inconnu (#999) - exception attendue\n";
    bool t4 = false;
    try
    {
        sys.mettreAJourStatut(999, "LIVRE");
    }
    catch (const std::out_of_range &)
    {
        t4 = true;
    }
    resultat(t4);

    // Bilan
    separateur();
    std::cout << Couleur::BOLD << "  Bilan : "
              << Couleur::VERT << pass << " PASS  "
              << (fail > 0 ? Couleur::ROUGE : Couleur::DIM)
              << fail << " FAIL"
              << Couleur::RESET << "\n";
    separateur();
}

// =============================================================================
// main
// =============================================================================

int main()
{

    // Fix encodage Windows : active UTF-8 + sequences ANSI
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

    SystemeLivraison systeme;

    // Flotte de test (stubs) - remplacer par les vraies classes a l'integration
    systeme.ajouterTransporteur(new FakeRapide());
    systeme.ajouterTransporteur(new FakePasCher());
    systeme.ajouterTransporteur(new FakeSamePrice());

    int choix{};
    do
    {
        choix = afficherMenu();

        switch (choix)
        {

        case 1:
        {
            titreSection("NOUVEAU COLIS");
            const int id = lireEntier("ID            : ");
            const double poids = lireDouble("Poids (kg)    : ");
            const double longueur = lireDouble("Longueur (cm) : ");
            const double largeur = lireDouble("Largeur (cm)  : ");
            const double hauteur = lireDouble("Hauteur (cm)  : ");
            const std::string type = lireLigne("Type (fragile/standard) : ");
            systeme.ajouterColis(Colis(id, poids, longueur, largeur, hauteur, type));
            ok("Colis #" + std::to_string(id) + " enregistre avec succes.");
            break;
        }

        case 2:
        {
            titreSection("MISE A JOUR STATUT");
            const int id = lireEntier("ID du colis    : ");
            const std::string statut = lireLigne("Nouveau statut : ");
            try
            {
                systeme.mettreAJourStatut(id, statut);
                ok("Colis #" + std::to_string(id) + " -> " + statut);
            }
            catch (const std::out_of_range &e)
            {
                erreur(e.what());
            }
            break;
        }

        case 3:
        {
            titreSection("ATTRIBUTION TRANSPORTEUR");
            const int id = lireEntier("ID du colis : ");
            const Colis *cible = nullptr;
            for (const Colis &c : systeme.getColis())
                if (c.getId() == id)
                {
                    cible = &c;
                    break;
                }
            if (!cible)
            {
                erreur("Colis #" + std::to_string(id) + " introuvable.");
                break;
            }
            Transporteur *choisi = systeme.assignerTransporteur(*cible);
            if (!choisi)
                erreur("Aucun transporteur disponible pour ce colis.");
            else
                ok("Transporte par : " + choisi->getNom());
            break;
        }

        case 4:
        {
            titreSection("STOCK DE COLIS");
            const auto &liste = systeme.getColis();
            if (liste.empty())
                info("Aucun colis enregistre.");
            else
                for (const Colis &c : liste)
                    c.afficher();
            break;
        }

        case 5:
        {
            titreSection("FLOTTE DE TRANSPORTEURS");
            const auto &flotte = systeme.getTransporteurs();
            if (flotte.empty())
                info("Aucun transporteur enregistre.");
            else
                for (const Transporteur *t : flotte)
                    t->afficher();
            break;
        }

        case 9:
            lancerScenariosTest(systeme);
            break;

        case 0:
            std::cout << "\n"
                      << Couleur::DIM << "  Au revoir.\n\n"
                      << Couleur::RESET;
            break;

        default:
            erreur("Option invalide.");
            break;
        }

    } while (choix != 0);

    return 0;
}
