# 📦 Module : `Colis` & `RapportManager`

> **Projet** : Simulateur de Système de Livraison  
> **Membre** : Data & Persistence Layer  
> **Langage** : C++17 — POO stricte, séparation `.h` / `.cpp`

---

## 📁 Fichiers produits

| Fichier | Rôle |
|---|---|
| `Colis.h` | Déclaration de la classe `Colis`, des types `Dimensions`, `TypeColis`, `StatutColis` |
| `Colis.cpp` | Implémentation : constructeur, workflow de statut, historique horodaté |
| `RapportManager.h` | Déclaration de la classe `RapportManager` |
| `RapportManager.cpp` | Implémentation : export `std::fstream` en mode append |

---

## 🧱 Architecture des classes

### `struct Dimensions`
Regroupe les trois dimensions physiques d'un colis.
```cpp
struct Dimensions {
    float longueur; // en mètres
    float largeur;
    float hauteur;
};
```

### `enum class TypeColis`
```
STANDARD | FRAGILE | PERISSABLE | DANGEREUX
```

### `enum class StatutColis`
```
EN_ATTENTE | EN_TRANSIT | LIVRE | ANNULE
```

### `class Colis`
Entité centrale du module. Encapsulation stricte, workflow d'état validé.

| Attribut | Type | Description |
|---|---|---|
| `id_` | `int` | Identifiant unique auto-incrémenté (compteur `static`) |
| `poids_` | `float` | Poids en kg |
| `dimensions_` | `Dimensions` | Longueur × Largeur × Hauteur |
| `type_` | `TypeColis` | Catégorie du colis |
| `statut_` | `StatutColis` | Statut courant dans le workflow |
| `adresse_destination_` | `std::string` | Adresse du destinataire |
| `distance_km_` | `float` | Distance de livraison |
| `historique_statuts_` | `std::vector<std::string>` | Journal horodaté des transitions |

### `class RapportManager`
Responsable unique de l'écriture fichier. Reçoit les colis en `const Colis&`, ne les modifie jamais.

---

## 🔄 Workflow de statut

Le changement de statut est **strictement unidirectionnel** et validé par `avancerStatut()` :

```
EN_ATTENTE  ──►  EN_TRANSIT  ──►  LIVRE
     │
     └──────────────────────────►  ANNULE
```

- `LIVRE` et `ANNULE` sont des **états terminaux** : toute tentative d'avancement lève une `std::logic_error`.
- `annuler()` n'est autorisé que depuis `EN_ATTENTE`.

---

## ✅ Contraintes C++17 respectées

| Contrainte | Implémentation |
|---|---|
| `const`-correctness | Tous les getters sont `const` → compatibles `const Colis&` |
| `enum class` | Pas de conversion implicite vers `int`, pas de collision de noms |
| Copie supprimée | `Colis(const Colis&) = delete` — un id ne peut pas être dupliqué |
| RAII sur les fichiers | `std::ofstream` local dans chaque méthode, fermeture automatique |
| Mode append | `std::ios::app` — l'historique fichier est cumulatif, jamais écrasé |
| SRP (SOLID) | `RapportManager` écrit, `Colis` stocke — responsabilités séparées |

---

## 🚀 Utilisation

### Créer et faire évoluer un colis
```cpp
#include "Colis.h"

Dimensions dims = {0.5f, 0.4f, 0.3f};
Colis c(12.5f, dims, TypeColis::FRAGILE, "12 Rue de la Paix, Paris", 450.0f);

c.avancerStatut(); // EN_ATTENTE -> EN_TRANSIT
c.avancerStatut(); // EN_TRANSIT -> LIVRE

// Lève std::logic_error : état terminal
// c.avancerStatut();
```

### Exporter un rapport
```cpp
#include "RapportManager.h"

RapportManager manager("rapports/historique.txt");
manager.ecrireEnTeteSession("Session du 25/04/2026");
manager.exporterRapportColis(c);                         // un seul colis

std::vector<const Colis*> tous = {&c1, &c2, &c3};
manager.exporterRapportComplet(tous);                    // rapport de session
```

### Exemple de sortie fichier
```
************************************************************
  NOUVELLE SESSION : Session du 25/04/2026
************************************************************

------------------------------------------------------------
  RAPPORT COLIS #1
------------------------------------------------------------
  Type       :           FRAGILE
  Statut     :           LIVRE
  Poids      :           12.5 kg
  Dimensions :           0.5m x 0.4m x 0.3m
  Destination:           12 Rue de la Paix, Paris
  Distance   :           450 km

  -- Historique des statuts --
    [2026-04-25 10:30:00] Colis #1 créé. Statut initial : EN_ATTENTE.
    [2026-04-25 10:31:12] Statut changé : EN_ATTENTE -> EN_TRANSIT.
    [2026-04-25 10:32:05] Statut changé : EN_TRANSIT -> LIVRE.
```

---

## 🔗 Intégration avec le reste du projet

Ce module est conçu pour être consommé par le moteur central `SystemeLivraison` (Zack) :

```cpp
// Le système central peut lire un colis sans risque de le modifier
void SystemeLivraison::assignerTransporteur(const Colis& colis) {
    float poids = colis.getPoids();          // ✅ getter const
    float distance = colis.getDistanceKm();  // ✅ getter const
    // ... logique polymorphique de Zack
}
```

Les `Colis` sont créés et possédés par `SystemeLivraison`, qui appelle ensuite `RapportManager` pour persister les données.

---

## ⚙️ Compilation

```bash
# Compiler uniquement ce module
g++ -std=c++17 -Wall -Wextra -c Colis.cpp -o Colis.o
g++ -std=c++17 -Wall -Wextra -c RapportManager.cpp -o RapportManager.o

# Lier avec le projet complet (exemple)
g++ -std=c++17 Colis.o RapportManager.o Transporteur.o SystemeLivraison.o main.cpp -o simulateur
```

> **Requis** : GCC ≥ 7 ou Clang ≥ 5 avec le flag `-std=c++17`.
