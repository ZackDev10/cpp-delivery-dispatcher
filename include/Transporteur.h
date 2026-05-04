#pragma once

// =============================================================================
// Transporteur.h
// =============================================================================

#include "Colis.h"
#include <string>
#include <iostream>

// =============================================================================
// Interface abstraite
// =============================================================================
class Transporteur
{
public:
    virtual ~Transporteur() = default;

    /// Le transporteur peut-il livrer ce colis ? (poids, volume, type…)
    virtual bool canDeliver(const Colis& colis) const = 0;

    /// Le transporteur est-il disponible (pas déjà en mission) ?
    virtual bool isAvailable() const = 0;

    /// Coût de livraison pour ce colis (en €).
    virtual double computeCost(const Colis& colis) const = 0;

    /// Vitesse de livraison (km/h) — sert de départage à coût égal.
    virtual double getVitesse() const = 0;

    /// Nom lisible du transporteur.
    virtual std::string getNom() const = 0;

    /// Affichage console.
    virtual void afficher() const = 0;
};


// =============================================================================
// FakeRapide — transporteur fictif rapide mais cher
// =============================================================================
class FakeRapide : public Transporteur
{
public:
    bool        canDeliver(const Colis&)  const override { return true;          }
    bool        isAvailable()             const override { return true;          }
    double      computeCost(const Colis& c) const override { return 5.0 + c.getPoids() * 3.0; }
    double      getVitesse()              const override { return 200.0;         }
    std::string getNom()                  const override { return "FakeRapide";  }
    void        afficher()                const override
    {
        std::cout << "  [STUB] FakeRapide | vitesse: 200 km/h | tarif: 5 + 3×poids\n";
    }
};


// =============================================================================
// FakePasCher — transporteur fictif lent mais économique
// =============================================================================
class FakePasCher : public Transporteur
{
public:
    bool        canDeliver(const Colis& c) const override { return c.getPoids() <= 30.0; }
    bool        isAvailable()              const override { return true;           }
    double      computeCost(const Colis& c) const override { return 1.0 + c.getPoids() * 0.5; }
    double      getVitesse()               const override { return 40.0;           }
    std::string getNom()                   const override { return "FakePasCher";  }
    void        afficher()                 const override
    {
        std::cout << "  [STUB] FakePasCher | vitesse: 40 km/h | tarif: 1 + 0.5×poids\n";
    }
};


// =============================================================================
// FakeSamePrice — même coût que FakePasCher sur colis léger → teste le
// =============================================================================
class FakeSamePrice : public Transporteur
{
public:
    bool        canDeliver(const Colis& c) const override { return c.getPoids() <= 30.0; }
    bool        isAvailable()              const override { return true;             }
    double      computeCost(const Colis& c) const override { return 1.0 + c.getPoids() * 0.5; }
    double      getVitesse()               const override { return 70.0;             }
    std::string getNom()                   const override { return "FakeSamePrice";  }
    void        afficher()                 const override
    {
        std::cout << "  [STUB] FakeSamePrice | vitesse: 70 km/h | même tarif que FakePasCher\n";
    }
};
