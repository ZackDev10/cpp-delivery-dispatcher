# 🚚 ✈️ README — Module Transporteurs Légers (Drone & Velo)

Ce document explique clairement la partie **Transporteurs Légers** du projet, dont je suis responsable (Hamza) :

* **Ce que fait chaque classe**
* **La logique métier et les formules utilisées**
* **Les corrections architecturales pour la robustesse du système**

---

### 1) Contexte de la partie
Dans cette simulation, je gère les options de livraison pour les colis légers et urbains. Ma mission était de garantir que les classes `Velo` et `Drone` s'intègrent parfaitement dans le dispatcher via l'interface `Transporteur`.

---

### 2) Architecture Technique & Corrections (Crucial)

Pour éviter les erreurs de compilation, j'ai implémenté les standards C++17 suivants :

* **Const Correctness** : Ajout du qualificatif `const` aux méthodes `canDeliver`, `computeCost` et `getDelay` pour correspondre au contrat de l'interface parente.
* **Constructeurs Dynamiques** : Les noms ne sont plus "en dur". Chaque transporteur reçoit son nom via son constructeur : `Velo(const std::string& nom)`.
* **Gestion des Exceptions** : Refactorisation de la classe `ColisNotFoundException` (PascalCase) et correction des messages d'erreur.

---

### 3) Logique Métier par Classe

#### **Velo** (Transport Urbain)
* **Capacité** : Poids ≤ 5.0 kg et Distance ≤ 30.0 km.
* **Acceptation** : Uniquement les colis de type `STANDARD`.
* **Coût** : `(coutParKm * distance) + (poids * 0.1)`.
* **Délai** : `(distance / vitesse) + 5 minutes`.

#### **Drone** (Livraison Rapide)
* **Capacité** : Poids ≤ 3.0 kg et Distance ≤ 50.0 km.
* **Acceptation** : Colis `STANDARD` et `FRAGILE`.
* **Logique Fragile** : Majoration du coût de **10%** et du délai de **20%**.

---

### 4) Organisation des Fichiers
* `include/Velo.h`, `include/Drone.h` : Signatures des classes.
* `src/Velo.cpp`, `src/Drone.cpp` : Implémentation de la logique.
* `include/Exceptions.h` : Gestion des erreurs.
