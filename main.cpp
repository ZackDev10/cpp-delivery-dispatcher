#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <chrono>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include "include/SystemeLivraison.h"
#include "include/Colis.h"
#include "include/Transporteur.h"
#include "include/Camion.h"
#include "include/Avion.h"
#include "include/Velo.h"
#include "include/Drone.h"
#include "include/RapportManager.h"

namespace ui
{
    struct Theme
    {
        std::string name;
        std::string header;
        std::string accent;
        std::string success;
        std::string warning;
        std::string error;
        std::string dim;
    };

    const std::string reset = "\x1b[0m";
    const std::string bold = "\x1b[1m";

    const Theme darkTheme{ "Sombre", "\x1b[36m", "\x1b[35m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[2m" };
    const Theme lightTheme{ "Clair", "\x1b[34m", "\x1b[35m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[90m" };

    Theme currentTheme = darkTheme;

    void setTheme(bool light)
    {
        currentTheme = light ? lightTheme : darkTheme;
    }

    void clearScreen()
    {
        std::cout << "\x1b[2J\x1b[H";
    }

    void separator(const std::string &token = "─", int width = 60)
    {
        std::cout << currentTheme.dim;
        for (int i = 0; i < width; ++i)
            std::cout << token;
        std::cout << reset << "\n";
    }

    void header()
    {
        std::cout << currentTheme.header << bold;
        std::cout << "╔══════════════════════════════════════════════════════╗\n";
        std::cout << "║   SIMULATEUR LOGISTIQUE — Centre de Livraison        ║\n";
        std::cout << "╚══════════════════════════════════════════════════════╝\n";
        std::cout << reset;
    }

    void subtitle(const std::string &text)
    {
        std::cout << currentTheme.dim << text << reset << "\n";
    }

    void section(const std::string &title)
    {
        std::cout << "\n" << currentTheme.accent << bold << "╭─ " << title << reset << "\n";
    separator("─", 54);
    }

    void success(const std::string &message)
    {
        std::cout << currentTheme.success << "✔ " << reset << message << "\n";
    }

    void warning(const std::string &message)
    {
        std::cout << currentTheme.warning << "⚠ " << reset << message << "\n";
    }

    void error(const std::string &message)
    {
        std::cout << currentTheme.error << "✖ " << reset << message << "\n";
    }

    void statBox(const std::string &title, const std::string &value)
    {
        std::cout << "╭───────────────╮  ";
        std::cout << currentTheme.dim << title << reset << "\n";
        std::cout << "│ " << currentTheme.accent << std::setw(11) << std::left << value << reset << "│\n";
        std::cout << "╰───────────────╯\n";
    }

    void loading(const std::string &message, int cycles = 12, int delayMs = 80)
    {
        const char *frames[] = { "⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏" };
        std::cout << currentTheme.accent << message << " " << reset;
        for (int i = 0; i < cycles; ++i)
        {
            std::cout << currentTheme.accent << frames[i % 10] << reset << "\r";
            std::cout.flush();
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
            std::cout << " \r";
        }
        std::cout << "\n";
    }

    template <typename T>
    void readNumber(const std::string &label, T &value, const std::string &hint = "")
    {
        while (true)
        {
            std::cout << currentTheme.dim << "• " << reset << label;
            if (!hint.empty())
                std::cout << currentTheme.dim << " " << hint << reset;
            std::cout << " : ";
            if (std::cin >> value)
                return;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            warning("Entrée invalide, veuillez réessayer.");
        }
    }

    void readLine(const std::string &label, std::string &value)
    {
        std::cout << currentTheme.dim << "• " << reset << label << " : ";
        std::getline(std::cin >> std::ws, value);
    }

    void pause(const std::string &message = "Appuyez sur Entrée pour continuer")
    {
        std::cout << "\n" << currentTheme.dim << message << reset;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    void menu(bool isLightTheme, const std::string &resume, const std::string &etat)
    {
        clearScreen();
        header();
        std::cout << "\n";
        std::cout << currentTheme.accent << bold << "Tableau de bord" << reset << "\n";
        separator("─", 60);
        std::cout << "Résumé : " << currentTheme.accent << resume << reset << "\n";
        std::cout << "État    : " << currentTheme.accent << etat << reset << "\n";
        separator("─", 60);
        std::cout << bold << "Actions" << reset << "\n";
        std::cout << "╭───────────────────────────────╮  ╭───────────────────────────────╮\n";
        std::cout << "│ [1] Créer un colis             │  │ [2] Mettre à jour un statut    │\n";
        std::cout << "│ [3] Assigner transporteur      │  │ [4] Afficher le stock          │\n";
        std::cout << "│ [8] Exporter les logs (.txt)   │  │ [9] Thème (" << (isLightTheme ? "Clair" : "Sombre") << ")             │\n";
        std::cout << "│ [0] Quitter                    │  │                                 │\n";
        std::cout << "╰───────────────────────────────╯  ╰───────────────────────────────╯\n";
        std::cout << currentTheme.dim << "Choisissez une action" << reset << " > ";
    }
}

struct StockStats
{
    int total = 0;
    int attente = 0;
    int transit = 0;
    int livre = 0;
    int annule = 0;
};

StockStats computeStats(const std::vector<Colis> &colis)
{
    StockStats stats;
    stats.total = static_cast<int>(colis.size());
    for (const auto &c : colis)
    {
        switch (c.getStatut())
        {
        case StatutColis::EN_ATTENTE:
            ++stats.attente;
            break;
        case StatutColis::EN_TRANSIT:
            ++stats.transit;
            break;
        case StatutColis::LIVRE:
            ++stats.livre;
            break;
        case StatutColis::ANNULE:
            ++stats.annule;
            break;
        }
    }
    return stats;
}

std::string formatNumber(float value, int precision = 1)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}

std::string formatTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &tt);
#else
    tm = *std::localtime(&tt);
#endif
    std::ostringstream out;
    out << std::put_time(&tm, "%H:%M");
    return out.str();
}

std::string toString(TypeColis type)
{
    switch (type)
    {
    case TypeColis::STANDARD:
        return "Standard";
    case TypeColis::FRAGILE:
        return "Fragile";
    case TypeColis::PERISSABLE:
        return "Périssable";
    case TypeColis::DANGEREUX:
        return "Dangereux";
    default:
        return "-";
    }
}

std::string toString(StatutColis statut)
{
    switch (statut)
    {
    case StatutColis::EN_ATTENTE:
        return "En attente";
    case StatutColis::EN_TRANSIT:
        return "En transit";
    case StatutColis::LIVRE:
        return "Livré";
    case StatutColis::ANNULE:
        return "Annulé";
    default:
        return "-";
    }
}

void afficherTableStock(const std::vector<Colis> &colis)
{
    const std::vector<std::string> headers = { "ID", "Type", "Statut", "Poids (kg)", "Distance (km)", "Volume" };
    std::vector<std::vector<std::string>> rows;
    rows.reserve(colis.size());

    for (const auto &c : colis)
    {
        rows.push_back({
            std::to_string(c.getId()),
            toString(c.getType()),
            toString(c.getStatut()),
            formatNumber(c.getPoids(), 2),
            formatNumber(c.getDistance(), 1),
            formatNumber(c.getVolume(), 2)
        });
    }

    std::vector<size_t> widths(headers.size(), 0);
    for (size_t i = 0; i < headers.size(); ++i)
        widths[i] = headers[i].size();
    for (const auto &row : rows)
        for (size_t i = 0; i < row.size(); ++i)
            widths[i] = std::max(widths[i], row[i].size());

    auto drawLine = [&](const std::string &left, const std::string &mid, const std::string &right)
    {
        std::cout << left;
        for (size_t i = 0; i < widths.size(); ++i)
        {
            for (size_t j = 0; j < widths[i] + 2; ++j)
                std::cout << "─";
            std::cout << (i + 1 == widths.size() ? right : mid);
        }
        std::cout << "\n";
    };

    drawLine("┌", "┬", "┐");
    std::cout << "│" << ui::currentTheme.accent << ui::bold;
    for (size_t i = 0; i < headers.size(); ++i)
    {
        std::cout << " " << std::left << std::setw(static_cast<int>(widths[i])) << headers[i] << " │";
    }
    std::cout << ui::reset << "\n";
    drawLine("├", "┼", "┤");
    for (const auto &row : rows)
    {
        std::cout << "│";
        for (size_t i = 0; i < row.size(); ++i)
        {
            std::cout << " " << std::left << std::setw(static_cast<int>(widths[i])) << row[i] << " │";
        }
        std::cout << "\n";
    }
    drawLine("└", "┴", "┘");
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

    SystemeLivraison systeme;
    RapportManager rapporteur;
    bool themeClair = false;
    ui::setTheme(themeClair);

    systeme.ajouterTransporteur(new Velo("Velo Express"));
    systeme.ajouterTransporteur(new Drone("Drone Alpha"));
    systeme.ajouterTransporteur(new Camion("Camion Nord"));
    systeme.ajouterTransporteur(new Avion("Air Rapide"));
    rapporteur.logEvenement("Systeme demarre avec 4 vehicules.");

    int choix = -1;
    do
    {
        StockStats stats = computeStats(systeme.getColis());
        std::ostringstream resume;
        resume << "Total " << stats.total
               << " | Attente " << stats.attente
               << " | Transit " << stats.transit
               << " | Livré " << stats.livre
               << " | Annulé " << stats.annule;

        std::ostringstream etat;
        etat << "Thème " << (themeClair ? "Clair" : "Sombre")
             << " • " << "Heure " << formatTime();

        ui::menu(themeClair, resume.str(), etat.str());
        std::cin >> choix;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            ui::warning("Choix invalide. Veuillez saisir un numéro du menu.");
            continue;
        }

        switch (choix)
        {
        case 1:
        {
            int id, t;
            float p, d, l, w, h;
            std::string dest;
            ui::section("Création d'un colis");
            ui::readNumber("Identifiant", id);
            ui::readNumber("Poids", p, "(kg)");
            ui::readNumber("Distance", d, "(km)");
            ui::readNumber("Longueur", l, "(cm)");
            ui::readNumber("Largeur", w, "(cm)");
            ui::readNumber("Hauteur", h, "(cm)");
            ui::readNumber("Type", t, "(0:Std, 1:Frag, 2:Peris, 3:Dang)");
            ui::readLine("Destination", dest);
            try
            {
                ui::loading("Enregistrement du colis");
                systeme.ajouterColis(Colis(id, p, {l, w, h}, static_cast<TypeColis>(t), d, dest));
                rapporteur.logEvenement("Colis ID " + std::to_string(id) + " cree.");
                ui::success("Colis enregistré avec succès.");
            }
            catch (const std::exception &e)
            {
                ui::error(e.what());
            }
            break;
        }
        case 2:
        {
            int id, s;
            ui::section("Mise à jour du statut");
            ui::readNumber("Identifiant colis", id);
            ui::readNumber("Statut", s, "(0:Att, 1:Tran, 2:Liv, 3:Ann)");
            try
            {
                ui::loading("Mise à jour du statut");
                systeme.mettreAJourStatut(id, static_cast<StatutColis>(s));
                ui::success("Statut mis à jour.");
            }
            catch (const std::exception &e)
            {
                ui::error(e.what());
            }
            break;
        }
        case 3:
        {
            int id;
            ui::section("Attribution d'un transporteur");
            ui::readNumber("Identifiant colis", id);
            bool trouve = false;
            for (const Colis &c : systeme.getColis())
            {
                if (c.getId() == id)
                {
                    ui::loading("Recherche du transporteur optimal");
                    systeme.assignerTransporteur(c);
                    ui::success("Transporteur assigné.");
                    trouve = true;
                    break;
                }
            }
            if (!trouve)
                ui::warning("Colis introuvable.");
            break;
        }
        case 4:
            ui::section("Stock des colis");
            if (systeme.getColis().empty())
            {
                ui::warning("Aucun colis enregistré.");
            }
            else
            {
                afficherTableStock(systeme.getColis());
            }
            ui::pause();
            break;
        case 8:
            ui::loading("Export du rapport");
            rapporteur.exporterFichier("rapport_final.txt");
            ui::success("Rapport exporté vers rapport_final.txt");
            break;
        case 9:
            themeClair = !themeClair;
            ui::setTheme(themeClair);
            ui::success("Thème activé : " + (themeClair ? std::string("Clair") : std::string("Sombre")));
            break;
        case 0:
            ui::success("Fermeture du simulateur. À bientôt !");
            break;
        default:
            ui::warning("Option non reconnue.");
            break;
        }
    } while (choix != 0);
    return 0;
}
