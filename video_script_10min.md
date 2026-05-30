# Script vidéo explicatif — 10 minutes

## Introduction générale (remerciements)

Bonjour Professeurs,

Avant de commencer, nous tenons à vous remercier pour votre accompagnement et vos conseils tout au long du projet. Grâce à vos retours, nous avons pu améliorer la qualité du travail et la clarté du système.

Aujourd’hui, nous vous présentons un **système complet de gestion logistique** qui simule la création, le suivi et l’assignation des colis dans une plateforme de livraison moderne.

---

**Description courte du code source (5 lignes)**
- `main_gui.cpp` lance l’application Qt et ouvre la fenêtre principale.
- `MainWindow` gère l’interface, les boutons, et les actions utilisateur.
- `SystemeLivraison` contient la logique métier (colis, statuts, transporteurs).
- `Colis` représente les données d’un colis (poids, dimensions, destination).
- `RapportManager` et le JSON assurent l’export et la persistance des données.
- `CMakeLists.txt` définit la configuration du projet et automatise la compilation (sources, options, et dépendances Qt).

> **But :** présenter clairement l’application, la logique métier, l’interface GUI et la persistance JSON.

---

## 0:00 – 0:30 | Introduction

Bonjour Professeur, je vous présente notre application **Simulateur Logistique**.  
C’est une application en **C++** avec **interface graphique Qt**, qui simule la gestion d’une plateforme de livraison : création de colis, suivi des statuts, attribution automatique des transporteurs, et sauvegarde des données.

---

## 0:30 – 1:30 | Vue générale de l’interface

L’interface est organisée en **quatre sections** :

1. **Dashboard** : une vue globale des statistiques.
2. **Opérations** : création et mise à jour des colis.
3. **Stock** : liste complète des colis.
4. **Rapports** : export des logs.

L’objectif est d’avoir une interface claire, professionnelle, et intuitive.

---

## 1:30 – 2:30 | Dashboard (statistiques en temps réel)

Sur le **Dashboard**, on voit les **cartes de statistiques** :
- Total des colis
- En attente
- En transit
- Livrés
- Annulés

Ces données se mettent à jour automatiquement quand on ajoute ou modifie un colis.

On a aussi une zone de **répartition des statuts**, qui permet de voir rapidement l’état global des livraisons.

---

## 2:30 – 4:30 | Création d’un colis (Opérations)

Je vais créer un colis en direct :

- Je saisis un **ID**
- Le **poids** en kg
- La **distance** en km
- Les **dimensions** (longueur, largeur, hauteur)
- Le **type** du colis
- La **destination**

Je clique sur **“Créer le colis”**.

Résultat :
- Un message de succès s’affiche
- Les champs se vident automatiquement
- Les statistiques se mettent à jour

Cela prouve que la création est bien liée à la logique métier.

---

## 4:30 – 5:30 | Mise à jour du statut

Ensuite, je peux mettre à jour l’état d’un colis :

- Je saisis l’ID
- Je choisis le nouveau statut (En attente, En transit, Livré, Annulé)
- Je clique sur **Mettre à jour**

Le statut du colis est modifié, et on voit directement le changement sur le Dashboard.

---

## 5:30 – 6:30 | Attribution automatique d’un transporteur

Le système assigne automatiquement le **meilleur transporteur** selon :

- La capacité
- Le coût
- La vitesse

C’est géré par le **polymorphisme**, sans `if` ou `switch` sur les types.  
Chaque transporteur a ses propres règles, et le moteur choisit le meilleur.

---

## 6:30 – 7:30 | Stock (liste complète)

Dans l’onglet **Stock**, on retrouve tous les colis enregistrés avec :

- ID
- Type
- Statut
- Poids
- Distance
- Volume

Cela permet un suivi global et rapide.

---

## 7:30 – 8:30 | Export des logs

Dans **Rapports**, on peut exporter l’historique dans un fichier texte.  
C’est utile pour garder une trace des opérations, par exemple pour une analyse ou un rapport.

---

## 8:30 – 9:30 | Persistance JSON

Toutes les données sont **sauvegardées automatiquement** dans un fichier JSON :

`data/colis.json`

Quand on relance l’application, les colis sont rechargés automatiquement.  
Cela permet de ne jamais perdre les données.

---

## 9:30 – 10:00 | Conclusion

En résumé, cette application simule un **système logistique complet** :

✅ création et suivi des colis  
✅ attribution intelligente des transporteurs  
✅ interface graphique professionnelle  
✅ persistance des données en JSON  

Merci pour votre attention.
