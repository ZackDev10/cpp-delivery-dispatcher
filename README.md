<div align="center">

<img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++17"/>
<img src="https://img.shields.io/badge/Qt-Framework-41CD52?style=for-the-badge&logo=qt&logoColor=white" alt="Qt"/>
<img src="https://img.shields.io/badge/CMake-3.16%2B-064F8C?style=for-the-badge&logo=cmake&logoColor=white" alt="CMake"/>
<img src="https://img.shields.io/badge/JSON-Persistance-000000?style=for-the-badge&logo=json&logoColor=white" alt="JSON"/>
<img src="https://img.shields.io/badge/Interface-Dark%20Mode-1a1a2e?style=for-the-badge&logo=windowsterminal&logoColor=white" alt="Dark Mode"/>

<br/><br/>

# 🚚 Simulateur de Livraison Logistique

### *Qt GUI Edition*

<p align="center">
  <em>Une application desktop de simulation logistique avancée, construite sur des fondations d'architecture orientée objet rigoureuse.<br/>
  Assignation intelligente de colis à une flotte de véhicules polymorphiques, pilotée par contraintes de poids, distance, coût et vitesse.</em>
</p>

<br/>

> **Projet universitaire — Programmation Orientée Objet Avancée**
> Évolution d'un outil CLI vers une application Desktop complète avec interface graphique en mode sombre.

</div>

---

## Table des Matières

- [✨ Fonctionnalités Principales](#-fonctionnalités-principales)
- [🏗️ Architecture & Principes Développés](#️-architecture--principes-développés)
- [📁 Structure du Projet](#-structure-du-projet)
- [⚙️ Prérequis & Installation](#️-prérequis--installation)
- [👥 Équipe de Développement](#-équipe-de-développement)

---

## ✨ Fonctionnalités Principales

L'application expose **quatre vues principales** accessibles depuis une navigation latérale unifiée :

### 📊 Dashboard — Tableau de Bord en Temps Réel

- 📈 Visualisation instantanée des **statistiques globales** du système (nombre de colis, taux de livraison, flotte active)
- 🔄 Mise à jour dynamique de l'état de la simulation sans rechargement
- 🎯 Indicateurs clés de performance (KPIs) présentés sous forme de cartes métriques

### 📦 Opérations — Cœur du Moteur Logistique

- ➕ **Création de colis** avec saisie des paramètres métier (poids, destination, priorité)
- 🔁 **Mise à jour d'état via machine à états finis** : transitions contrôlées de `EN_ATTENTE` → `EN_COURS` → `LIVRÉ` / `ÉCHOUÉ`
- 🤖 **Assignation polymorphique automatique** : le moteur sélectionne le transporteur optimal (Vélo, Drone, Camion, Avion) selon les contraintes du colis

### 🗃️ Stock — Inventaire Complet

- 📋 Liste exhaustive de **tous les colis enregistrés** dans le système
- 🔍 Consultation de l'état courant et de l'historique de chaque colis
- 🚛 Vue de la **flotte de véhicules** disponibles avec leur capacité résiduelle

### 📑 Rapports & Persistance — Traçabilité & Sauvegarde

- 💾 **Auto-sauvegarde JSON** : état complet du système persisté automatiquement à chaque opération
- 📤 **Export des logs en `.txt`** : génération de rapports d'activité horodatés via `RapportManager`
- ♻️ **Rechargement à froid** : restauration intégrale de la session depuis le fichier JSON au démarrage

---

## 🏗️ Architecture & Principes Développés

Ce projet met en œuvre un ensemble de pratiques **C++ modernes et de patrons d'architecture avancés**, appliqués avec rigueur.

### 🔷 Polymorphisme Pur — Sans `dynamic_cast`

La hiérarchie de classes repose sur une interface abstraite `Transporteur`, déclinée en quatre implémentations concrètes. L'intégralité des décisions d'assignation s'effectue via **dispatch virtuel**, sans aucun recours à `dynamic_cast` ou `typeid` — garantissant une extensibilité totale et un couplage minimal.

```
Transporteur (classe abstraite)
├── Vélo        — courte distance, faible charge
├── Drone       — livraison express, charge limitée
├── Camion      — longue distance, haute capacité
└── Avion       — transport intercontinental
```

### 🔷 Sécurité Mémoire & RAII

La gestion de la mémoire dynamique applique le principe **RAII** (*Resource Acquisition Is Initialization*) de manière systématique :

- **Suppression explicite** des constructeurs de copie et opérateurs d'affectation sur toutes les classes à ressources (`= delete`)
- Utilisation de **pointeurs à durée de vie clairement définie** au sein du `SystemeLivraison`
- Aucune fuite mémoire : chaque ressource acquise est libérée dans le destructeur correspondant

### 🔷 Machine à États Finis (FSM)

Le cycle de vie d'un `Colis` est modélisé par un **automate à états finis formel** :

```
         ┌───────────────────────────────────┐
         │                                   ▼
[EN_ATTENTE] ──assign──► [EN_COURS] ──success──► [LIVRÉ]
                                   └──failure──► [ÉCHOUÉ]
```

Les transitions invalides lèvent une exception dédiée, rendant tout état incohérent **impossible par construction**.

### 🔷 Exceptions C++ Personnalisées

Le projet définit une hiérarchie d'exceptions métier héritant de `std::exception` :

| Exception | Déclencheur |
|---|---|
| `TransporteurIndisponibleException` | Aucun véhicule ne satisfait les contraintes du colis |
| `TransitionEtatInvalideException` | Tentative de transition FSM illégale |
| `ErreurPersistanceException` | Échec de lecture/écriture JSON |

---

## 📁 Structure du Projet

```
SimulateurLivraison-Qt/
├── CMakeLists.txt
├── README.md
├── data/
│   └── systeme_livraison.json      # Persistance automatique
├── src/
│   ├── core/
│   │   ├── Transporteur.h          # Interface abstraite (pure virtual)
│   │   ├── Velo.h / Velo.cpp
│   │   ├── Drone.h / Drone.cpp
│   │   ├── Camion.h / Camion.cpp
│   │   ├── Avion.h / Avion.cpp
│   │   ├── Colis.h / Colis.cpp     # FSM + couche données
│   │   └── SystemeLivraison.h / SystemeLivraison.cpp
│   ├── managers/
│   │   └── RapportManager.h / RapportManager.cpp
│   ├── exceptions/
│   │   └── Exceptions.h
│   └── gui/
│       ├── MainWindow.h / MainWindow.cpp
│       ├── DashboardView.h / .cpp
│       ├── OperationsView.h / .cpp
│       ├── StockView.h / .cpp
│       └── RapportsView.h / .cpp
└── build/                          # Généré par CMake (ignoré par git)
```

---

## ⚙️ Prérequis & Installation

### Prérequis Système

| Outil | Version minimale |
|---|---|
| Compilateur C++ | GCC 9+ / Clang 10+ / MSVC 2019+ |
| CMake | 3.16+ |
| Qt | 6.x (ou Qt 5.15 LTS) |
| Git | Toute version récente |

> **Note :** Assurez-vous que `Qt` est installé et que la variable `CMAKE_PREFIX_PATH` pointe vers votre répertoire d'installation Qt si nécessaire.

---

### 🔧 Cloner le Dépôt

```bash
git clone https://github.com/<votre-organisation>/simulateur-livraison-qt.git
cd simulateur-livraison-qt
```

### 🏗️ Configurer le Projet avec CMake

```bash
# Générer les fichiers de build dans le répertoire 'build/'
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

> Si Qt n'est pas détecté automatiquement, précisez le chemin :
>
> ```bash
> cmake -B build -DCMAKE_PREFIX_PATH="/chemin/vers/Qt/6.x.x/gcc_64"
> ```

### 🔨 Compiler le Projet

```bash
cmake --build build --config Release
```

> Pour accélérer la compilation sur les machines multi-cœurs :
>
> ```bash
> cmake --build build --config Release -- -j$(nproc)
> ```

### 🚀 Lancer l'Application

```bash
# Linux / macOS
./build/SimulateurLivraison

# Windows
.\build\Release\SimulateurLivraison.exe
```

---

## 👥 Équipe de Développement

Ce projet a été conçu et réalisé par une équipe de quatre développeurs dans le cadre d'un projet de **Programmation Orientée Objet Avancée**.

<br/>

| 👤 Membre | 🔧 Domaine de Responsabilité |
|---|---|
| **Zebbara Zakaria** | Moteur polymorphique (`SystemeLivraison`), architecture système globale, configuration CMake, gestion de la mémoire dynamique (RAII & suppression des copies) |
| **Saloua Amehsis** | Couche données (`Colis`), implémentation de la machine à états finis, export des logs (`RapportManager`), persistance et chargement des données en JSON |
| **Youssef Fahim** | Développement de l'Interface Graphique Qt (toutes les vues), conception de l'interface abstraite `Transporteur`, algorithmes des véhicules lourds (`Camion`, `Avion`) |
| **Hamza Titiou** | Logique métier des véhicules légers (`Vélo`, `Drone`), validation des contraintes de livraison, conception et hiérarchie des exceptions C++ personnalisées |

<br/>

> 🎓 **Encadrant :** Pr. **Youssef ES-SAADY**

---

<div align="center">

*Réalisé avec rigueur dans le cadre d'un projet universitaire d'ingénierie logicielle.*

</div>
