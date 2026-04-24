# 🚚✈️ README — Module Transporteurs (Delivery System Simulator)

Ce document explique clairement la partie **Transporteurs** du projet :
- ce que fait chaque classe,
- la logique métier utilisée,
- comment ce module s’intègre dans le système,
- comment l’équipe peut l’étendre proprement.

> Objectif principal : gérer la livraison des colis avec un design **OOP polymorphique**, sans `if(type == "...")`.

---

## 1) Contexte de la partie

Dans la simulation de livraison, un colis (`Colis`) doit être attribué au transporteur le plus adapté.

Chaque transporteur doit répondre à 3 questions :
1. **Peut-il livrer ce colis ?** (`canDeliver`)
2. **Combien cela coûte ?** (`computeCost`)
3. **Quel est son nom métier ?** (`getName`)

Cette logique est centralisée via une interface abstraite : `Transporteur`.

---

## 2) Architecture technique

### Interface commune
- `include/Transporteur.h`

Contient le contrat polymorphique :
- `virtual bool canDeliver(const Colis& c) const = 0;`
- `virtual double computeCost(const Colis& c, double distance) const = 0;`
- `virtual std::string getName() const = 0;`
- destructeur virtuel.

### Implémentations actuelles
- `include/Camion.h`
- `src/Camion.cpp`
- `include/Avion.h`
- `src/Avion.cpp`

### Modèle de données utilisé
- `include/Colis.h`

Expose les infos nécessaires :
- `getPoids()`
- `getType()`

---

## 3) Logique métier par classe

## `Camion`

### Capacité
- Peut livrer si le poids est `<= 1000 kg`.

### Coût
Formule actuelle :

\[
\text{cost} = 25.0 + (0.90 \times \text{distance}) + (0.002 \times \text{poids} \times \text{distance})
\]

### Interprétation métier
- `25.0` = frais fixes de prise en charge
- `0.90 × distance` = carburant/péages/usure distance
- `0.002 × poids × distance` = surcharge charge lourde

---

## `Avion`

### Capacité
- Peut livrer tous les colis (`canDeliver` retourne `true`).

### Coût
Formule actuelle :

\[
\text{cost} = 180.0 + (2.75 \times \text{distance}) + (0.12 \times \text{poids}) + \max(0, \text{poids} - 200) \times 0.35
\]

### Interprétation métier
- `180.0` = coûts fixes aérien (opérations)
- `2.75 × distance` = coût important au km
- `0.12 × poids` = manutention
- surcharge au-delà de `200 kg` = fret lourd

---

## 4) Règles de robustesse implémentées

Dans `Camion` et `Avion` :
- poids négatif → ramené à `0`
- distance négative → ramenée à `0`

But : éviter des coûts incohérents si les données amont sont invalides.

---

## 5) Comment le module fonctionne dans le projet

Flux standard du dispatcher :
1. Recevoir `Colis` + `distance`.
2. Parcourir une liste de `Transporteur` (`Camion`, `Avion`, ...).
3. Pour chaque transporteur :
   - appeler `canDeliver(colis)`
   - si `true`, appeler `computeCost(colis, distance)`
4. Choisir la meilleure option métier (souvent le coût minimal parmi les admissibles).

✅ Aucun test explicite sur le type n’est nécessaire.

---

## 6) Pourquoi la séparation `.h` / `.cpp` est importante

- `*.h` : contrat/API (déclarations)
- `*.cpp` : implémentation (logique)

Avantages :
- meilleure lisibilité,
- maintenance plus simple,
- travail d’équipe plus fluide,
- architecture scalable.

---

## 7) Comment ajouter un nouveau transporteur

Exemple `Drone` :
1. Créer `include/Drone.h` qui hérite de `Transporteur`.
2. Créer `src/Drone.cpp`.
3. Implémenter les 3 méthodes :
   - `canDeliver`
   - `computeCost`
   - `getName`
4. L’ajouter à la collection des transporteurs dans le dispatcher.

👉 Aucun besoin de modifier `Camion`/`Avion`.

---

## 8) Tests de cette partie

Fichier de test actuel :
- `tests/TransporteursTests.cpp`

Le test vérifie notamment :
- noms des classes,
- règles d’acceptation (`canDeliver`),
- exactitude des formules de coût,
- cas limites (valeurs négatives).

Critère de succès :
- résumé avec `Failed: 0`
- code de sortie process = `0`

---

## 9) Bonnes pratiques pour les membres de l’équipe

- Ne pas utiliser `if(type == ...)` pour choisir un transporteur.
- Garder les constantes tarifaires visibles dans les headers.
- Écrire un test pour chaque nouvelle règle métier.
- Préserver `const`, `override`, et la lisibilité des noms.

---

## 10) Résumé

Cette partie fournit un module transporteurs :
- **propre** (interfaces nettes),
- **robuste** (validations défensives),
- **extensible** (nouveaux transporteurs sans casser l’existant),
- **adapté au travail d’équipe** (tests + structure claire).

Si tu veux, on peut ajouter ensuite une version **diagramme d’architecture + séquence dispatcher** dans ce même README.
