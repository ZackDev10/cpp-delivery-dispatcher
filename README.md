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
