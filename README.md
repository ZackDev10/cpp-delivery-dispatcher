# Module Lead — Moteur Principal du Simulateur

**Auteur :** Zakaria 
**Rôle dans le projet :** Conception et implémentation du moteur central d'attribution des transporteurs, point d'entrée de l'application, et intégration finale.

---

## Contexte et objectif

Dans le cadre du projet *Simulateur de Système de Livraison*, le travail a été réparti entre quatre membres de l'équipe avec des périmètres strictement délimités. Ce module constitue **le cœur du système** : la logique qui décide quel transporteur prend en charge quel colis.

L'objectif principal est de démontrer une application stricte du **principe Ouvert/Fermé (OCP)** via le **polymorphisme pur en C++17**. En termes simples : le moteur peut fonctionner avec n'importe quel nouveau type de transporteur sans qu'on touche à une seule ligne de son code.

> **Note pour les coéquipiers :** Ce module compile et fonctionne de manière totalement autonome grâce à des classes de substitution (*stubs*). Vous n'avez pas besoin d'intégrer votre code pour tester le vôtre. Le plan d'intégration est détaillé en bas de ce fichier.

---

## Structure des fichiers du module

```
LIVRAISON-SIMULATEUR-Z/
│
├── include/                        # Tous les fichiers d'en-tête (.h)
│   ├── SystemeLivraison.h          # Déclaration du moteur principal
│   ├── Transporteur.h              # Interface abstraite + classes de test (Fake*)
│   └── Colis.h                     # Entité colis (version provisoire en attendant M2)
│
├── src/                            # Implémentations (.cpp)
│   └── SystemeLivraison.cpp        # Logique du moteur : attribution, statuts, mémoire
│
├── main.cpp                        # Menu CLI interactif + scénarios de validation
└── CMakeLists.txt                  # Configuration de compilation (C++17)
```

Chaque fichier a un rôle unique et bien délimité — il n'y a aucun chevauchement avec les fichiers des autres membres de l'équipe.

---

## Comment compiler et lancer

### Méthode 1 — CMake (recommandée)

```bash
# Depuis la racine du projet
cmake -B build
cmake --build build
./build/simulateur
```

### Méthode 2 — Compilation directe avec g++

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude \
    src/SystemeLivraison.cpp main.cpp \
    -o simulateur

./simulateur
```

Le programme affiche un menu interactif dans le terminal. Aucune interface graphique n'est nécessaire.

---

## Ce que fait ce module

### 1. Le moteur d'attribution — `SystemeLivraison`

C'est la classe centrale du projet. Elle gère deux collections :

- un stock de colis (`std::vector<Colis>`)
- une flotte de transporteurs (`std::vector<Transporteur*>`)

Sa méthode la plus importante est `assignerTransporteur()`. Voici comment elle fonctionne :

```
Pour chaque transporteur de la flotte :
  → Peut-il livrer ce colis ?   (canDeliver)
  → Est-il disponible ?         (isAvailable)
  → Si oui aux deux : calculer le coût  (computeCost)

Retenir le moins cher.
Si deux transporteurs ont exactement le même coût → choisir le plus rapide (getVitesse).
```

Ce qui est important pour le professeur : **aucune condition sur le type** n'apparaît dans ce code. On n'écrit jamais `if (type == "Drone")` ou `dynamic_cast<Camion*>`. Tout est délégué aux méthodes virtuelles — c'est la définition du polymorphisme appliqué au monde réel.

```cpp
// Extrait de SystemeLivraison.cpp — boucle d'attribution
for (Transporteur* t : transporteurs_)
{
    if (!t->canDeliver(colis) || !t->isAvailable())
        continue;

    const double cout    = t->computeCost(colis);
    const double vitesse = t->getVitesse();

    if (cout < meilleurCout)                              // Règle 1 : moins cher
    { meilleur = t; meilleurCout = cout; meilleurVit = vitesse; }
    else if (cout == meilleurCout && vitesse > meilleurVit) // Règle 2 : départage
    { meilleur = t; meilleurVit = vitesse; }
}
```

Ajouter un cinquième transporteur demain (par exemple un `Sous-marin`) ne nécessite **aucune modification** de cette méthode. C'est exactement ce que demande le challenge avancé du cahier des charges.

---

### 2. La gestion mémoire — Destructeur de `SystemeLivraison`

Les transporteurs sont alloués dynamiquement (`new`) par l'appelant et confiés au système. Le destructeur de `SystemeLivraison` est responsable de les libérer :

```cpp
SystemeLivraison::~SystemeLivraison()
{
    for (Transporteur* t : transporteurs_)
        delete t;
}
```

Cela garantit qu'il n'y a aucune fuite mémoire, quelle que soit la façon dont le programme se termine. Le destructeur de `Transporteur` doit être `virtual` (responsabilité du Membre 3) pour que le bon destructeur dérivé soit appelé.

---

### 3. L'interface `Transporteur` — Le contrat de l'équipe

Définie dans `include/Transporteur.h`, cette interface est le **contrat que tous les membres doivent respecter**. Toute classe concrète (`Velo`, `Drone`, `Camion`, `Avion`) doit implémenter ces six méthodes :

| Méthode | Description |
|---|---|
| `canDeliver(const Colis&)` | Le transporteur est-il physiquement capable de livrer ce colis ? |
| `isAvailable()` | Est-il libre (pas déjà en mission) ? |
| `computeCost(const Colis&)` | Quel est son tarif pour ce colis (en €) ? |
| `getVitesse()` | Quelle est sa vitesse (en km/h) ? Utilisé pour départager à coût égal. |
| `getNom()` | Son nom lisible (ex : "Drone Alpha"). |
| `afficher()` | Affichage console de ses caractéristiques. |

Le destructeur virtuel est **obligatoire** :

```cpp
virtual ~Transporteur() = default;
```

Sans lui, appeler `delete` sur un pointeur de base ne déclencherait pas le destructeur de la classe dérivée — fuite mémoire garantie.

---

### 4. Les classes de test (Stubs)

En attendant les vraies implémentations des membres 3 et 4, trois classes fictives sont définies dans `Transporteur.h` uniquement pour les tests :

| Classe | Comportement | But du test |
|---|---|---|
| `FakePasCher` | Accepte colis ≤ 30 kg, tarif bas (3.5 € pour 5 kg), lent (40 km/h) | Candidat économique |
| `FakeSamePrice` | Même tarif que `FakePasCher`, mais plus rapide (70 km/h) | Valide le départage par vitesse |
| `FakeRapide` | Accepte tout, tarif élevé (110 € pour 35 kg), très rapide (200 km/h) | Seul candidat sur colis lourd |

Ces classes seront **supprimées lors de l'intégration finale** et remplacées par les implémentations réelles.

---

### 5. Le menu CLI et les scénarios de validation

Le programme propose un menu avec 6 options. L'option `9` lance quatre tests automatiques qui s'exécutent sans intervention manuelle :

| Test | Ce qui est vérifié | Résultat attendu |
|---|---|---|
| Test 1 | Colis 5 kg, deux candidats au même prix | `FakeSamePrice` gagne (plus rapide) |
| Test 2 | Colis 35 kg, seul `FakeRapide` peut livrer | `FakeRapide` sélectionné |
| Test 3 | Mise à jour statut sur un colis existant | Succès silencieux |
| Test 4 | Mise à jour statut sur un ID inexistant | `std::out_of_range` levée et capturée |

Tous les tests affichent `✔ PASS` ou `✘ FAIL` dans le terminal.

---

## Périmètre de ce module

Ce module se limite strictement à son domaine pour ne pas empiéter sur le travail des coéquipiers :

- Pas de classes `Velo`, `Drone`, `Camion`, `Avion` → **Membres 3 et 4**
- Pas de `RapportManager` (export `.txt` via `std::fstream`) → **Membre 2**
- Pas d'exceptions personnalisées (`ColisInvalideException`, etc.) → **Membre 4**
- Pas d'implémentation complète de `Colis` → **Membre 2** (le stub suffit pour les tests du moteur)

---

## Plan d'intégration (pour les coéquipiers)

Quand vous avez terminé votre partie, voici la procédure pour intégrer votre code dans le projet :

**Étape 1 — Pousser votre travail sur le dépôt**

```bash
git add .
git commit -m "feat(Velo): implémentation canDeliver et computeCost"
git push origin feature/votre-module
```

**Étape 2 — Ouvrir une Pull Request vers `dev`**
Sur GitHub, créez une PR de votre module vers `dev`. Zack effectuera la revue.

**Étape 3 — Checklist de revue (vérifiée par Zack)**

Avant toute intégration, les points suivants sont vérifiés :

- [ ] Le destructeur de `Transporteur` est bien `virtual`
- [ ] Toutes les méthodes de l'interface sont implémentées
- [ ] Aucun `cout` de debug oublié
- [ ] Compilation propre sans warnings (`-Wall -Wextra`)
- [ ] L'option `9` du menu affiche encore `✔ PASS` sur les 4 tests après intégration

**Étape 4 — Décommenter dans `main.cpp`**

```cpp
// Décommenter votre include et votre ligne ajouterTransporteur(new ...)
#include "Velo.h"
systeme.ajouterTransporteur(new Velo("Vélo Express", 15.0));
```

---

## Conventions de code du projet

Pour garantir une base cohérente lors de l'intégration, toute l'équipe respecte ces règles :

```cpp
#pragma once                          // Toujours, jamais de include guards manuels

class MaClasse                        // PascalCase pour les classes
{
    void maMethode();                 // camelCase pour les méthodes
    int  membre_;                     // underscore final pour les membres privés

    int  getValeur() const;           // Tout getter est const
    void traiter(const Colis& c);     // Paramètre non modifié → const&
};

// Jamais de code métier dans un .h   → uniquement dans le .cpp correspondant
```

---

*Module maintenu par Zack — toute question sur le moteur ou le workflow Git peut lui être adressée directement.*
