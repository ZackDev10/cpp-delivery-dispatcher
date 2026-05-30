# 📦 Simulateur de Système de Livraison (C++)

![C++17](https://img.shields.io/badge/C++-17-blue.svg)
![Architecture](https://img.shields.io/badge/Architecture-POO%20%7C%20SOLID-success.svg)

Une application console robuste orientée objet simulant la gestion complète d'une plateforme logistique (gestion des colis, attribution des transporteurs, calcul des coûts et suivis).

Ce projet met en œuvre un moteur de routage purement **polymorphique**, garantissant l'attribution automatique du meilleur transporteur sans utiliser de logique conditionnelle explicite sur les types, respectant ainsi strictement le principe d'Ouverture/Fermeture (Open/Closed Principle de SOLID).

## 🚀 Fonctionnalités Clés

* **Attribution Polymorphique :** Sélection automatique du transporteur (Vélo, Drone, Camion, Avion) le plus rentable et apte, sans `if` ou `switch` sur le type.
* **Gestion du Cycle de Vie :** Suivi strict des états des colis (`EN_ATTENTE` ➔ `EN_TRANSIT` ➔ `LIVRÉ`).
* **Calcul Dynamique :** Formules de calcul des coûts et délais intégrées dynamiquement selon les contraintes de chaque véhicule.
* **Génération de Rapports :** Exportation automatisée des historiques et statistiques financières via flux de fichiers (`std::fstream`).
* **Robustesse :** Gestion complète des erreurs avec des exceptions C++ personnalisées.

## 📂 Structure du Projet

```text
livraison-simulateur/
│
├── include/          # Déclarations des classes et contrats (.h)
├── src/              # Implémentations de la logique métier (.cpp)
├── tests/            # Tests unitaires et de robustesse
├── rapports/         # Dossier de sortie pour les historiques générés
├── data/             # Fichiers de persistance (si implémenté)
├── CMakeLists.txt    # Configuration de build
└── README.md         # Documentation du projet
---

## Table des matières

1. [Présentation](#présentation)
2. [Architecture du projet](#architecture-du-projet)
3. [Répartition de l'équipe](#répartition-de-léquipe)
4. [Prérequis](#prérequis)
5. [Compilation & Exécution](#compilation--exécution)
6. [Fonctionnalités](#fonctionnalités)
7. [Principe clé : Attribution polymorphique](#principe-clé--attribution-polymorphique)
8. [Conventions de code](#conventions-de-code)
9. [Workflow Git](#workflow-git)
10. [Statut d'avancement](#statut-davancement)

---

## Présentation

Ce projet modélise une entreprise de livraison gérant différents types de colis et de transporteurs. Il implémente :

- La **création et le suivi de colis** (poids, dimensions, type fragile/standard)
- La **gestion d'une flotte** de transporteurs (Vélo, Drone, Camion, Avion)
- Le **calcul automatique** du coût et du délai de livraison
- Le **suivi des statuts** (EN_ATTENTE → EN_TRANSIT → LIVRE)
- La **génération de rapports** exportés en `.txt`

### Challenge avancé implémenté

L'attribution du transporteur optimal repose **exclusivement sur le polymorphisme** via les méthodes virtuelles pures `canDeliver()`, `computeCost()` et `getVitesse()`. Aucun `if (type == "...")`, `switch` ou `dynamic_cast` n'est présent dans le moteur d'attribution.

---

## Architecture du projet

```

SimulateurLivraison/
├── include/                    # Headers (.h) — interfaces et déclarations
│   ├── Transporteur.h          # Interface abstraite (Membre 3)
│   ├── Colis.h                 # Entité colis (Membre 2)
│   ├── SystemeLivraison.h      # Moteur principal (Zack)
│   ├── Camion.h                # Transporteur lourd (Membre 3)
│   ├── Avion.h                 # Transporteur lourd (Membre 3)
│   ├── Velo.h                  # Transporteur léger (Membre 4)
│   ├── Drone.h                 # Transporteur léger (Membre 4)
│   ├── Exceptions.h            # Exceptions personnalisées (Membre 4)
│   └── RapportManager.h        # Export .txt (Membre 2)
│
├── src/                        # Implémentations (.cpp)
│   ├── SystemeLivraison.cpp    # Moteur + boucle polymorphique (Zack)
│   ├── Colis.cpp               # Logique colis (Membre 2)
│   ├── RapportManager.cpp      # fstream / export (Membre 2)
│   ├── Camion.cpp              # (Membre 3)
│   ├── Avion.cpp               # (Membre 3)
│   ├── Velo.cpp                # (Membre 4)
│   └── Drone.cpp               # (Membre 4)
│
├── main.cpp                    # Point d'entrée CLI (Zack)
├── CMakeLists.txt              # Build system
├── .gitignore                  # Exclusions C++ / IDE
└── README.md                   # Ce fichier

```

---

## Répartition de l'équipe

| Membre | Rôle | Fichiers |
|--------|------|----------|
| **Zack** (Lead) | Moteur principal, intégration, `main.cpp` | `SystemeLivraison.h/.cpp`, `main.cpp` |
| **Membre 2** | Entité `Colis`, `RapportManager` | `Colis.h/.cpp`, `RapportManager.h/.cpp` |
| **Membre 3** | Interface `Transporteur`, `Camion`, `Avion` | `Transporteur.h`, `Camion.h/.cpp`, `Avion.h/.cpp` |
| **Membre 4** | `Velo`, `Drone`, exceptions | `Velo.h/.cpp`, `Drone.h/.cpp`, `Exceptions.h` |

---

## Prérequis

| Outil | Version minimale |
|-------|-----------------|
| Compilateur C++ | GCC 9+ / Clang 10+ / MSVC 2019+ |
| Standard | C++17 (`-std=c++17`) |
| CMake | 3.16+ |

---

## Compilation & Exécution

### Avec CMake (recommandé)

```bash
# Cloner le dépôt
git clone https://github.com/ZackDev10/cpp-delivery-dispatcher.git
cd SimulateurLivraison

# Configurer et compiler
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Exécuter
./build/SimulateurLivraison
```

### Interface graphique (Qt Widgets)

> Prérequis : Qt 6 (module Widgets) installé et visible par CMake.

```bash
# Configurer avec l'option GUI activée (par défaut)
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_QT_GUI=ON
cmake --build build

# Exécuter l'application graphique
./build/simulateur_gui
```

### Compilation manuelle (g++)

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude \
    src/*.cpp main.cpp \
    -o simulateur

./simulateur
```

---

## Fonctionnalités

### Menu principal (CLI)

```
╔══════════════════════════════════════════╗
║    SIMULATEUR DE SYSTÈME DE LIVRAISON    ║
╠══════════════════════════════════════════╣
║  1. Créer un colis                       ║
║  2. Mettre à jour le statut d'un colis   ║
║  3. Assigner un transporteur             ║
║  4. Afficher tous les colis              ║
║  5. Afficher la flotte                   ║
║  0. Quitter                              ║
╚══════════════════════════════════════════╝
```

### Statuts d'un colis

```
EN_ATTENTE  ──►  EN_TRANSIT  ──►  LIVRE
```

---

## Conventions de code

* **Standard :** C++17 strict
* **Nommage :** `camelCase` pour les méthodes, `PascalCase` pour les classes, `snake_case_` avec underscore final pour les membres privés
* **Const-correctness :** tout getter doit être `const`, tout paramètre non modifié passé par `const&`
* **Mémoire :** pas de `new` sans `delete` correspondant — le destructeur de `SystemeLivraison` libère toute la flotte
* **Headers :** toujours utiliser `#pragma once`
* **Séparation :** déclarations dans `.h`, implémentations dans `.cpp` — jamais de code métier dans un header

---

## Workflow Git

### Branches

```
main          ← production stable (merge après validation)
dev           ← branche d'intégration commune
feature/zack-moteur
feature/m2-colis
feature/m3-transporteurs
feature/m4-vehicules-legers
```

### Règles

1. **Ne jamais pusher directement sur `main`**
2. Chaque membre travaille sur sa branche `feature/xxx`
3. Pull Request → revue par Zack → merge dans `dev`
4. Merge `dev` → `main` uniquement quand le build est propre

### Commandes utiles

```bash
# Récupérer les dernières modifications de dev
git checkout dev && git pull origin dev

# Créer sa branche de feature
git checkout -b feature/m2-colis

# Après votre travail
git add include/Colis.h src/Colis.cpp
git commit -m "feat(Colis): ajout getters et setStatut()"
git push origin feature/m2-colis
# → ouvrir une Pull Request vers dev
```

---
