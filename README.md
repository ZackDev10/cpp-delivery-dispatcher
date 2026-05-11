Simulateur de Livraison OOP - Contribution (Hamza)
Ce dépôt contient ma contribution au projet de simulateur de livraison. Je suis responsable du développement des transporteurs légers et de la gestion personnalisée des exceptions.

🛠 Corrections Techniques Appliquées
Suite aux retours du Tech Lead, j'ai procédé à une refactorisation complète pour garantir la compilation et la robustesse du système:


Const Correctness (Crucial) : Ajout du qualificatif const aux méthodes canDeliver, computeCost et getDelay dans les fichiers .h et .cpp. Cela assure la correspondance exacte avec l'interface parente Transporteur et résout les erreurs de compilation.


Constructeurs Dynamiques : Modification des constructeurs de Velo et Drone pour accepter un paramètre nom. Cela permet d'instancier plusieurs véhicules avec des noms uniques depuis le main.cpp au lieu d'avoir des noms figés.

Standardisation des Exceptions :

Renommage de la classe en ColisNotFoundException pour respecter la convention PascalCase.

Correction des fautes d'orthographe dans les messages d'erreur (ex: "Erreur" au lieu de "Erreue").

🚚 Logique Métier (Implémentée)
J'ai maintenu l'intégralité des formules mathématiques et des contraintes demandées dans le Cahier des Charges (CDC):

1. Velo :

Capacité max : 5.0 kg.

Distance max : 30.0 km.

Acceptation : Uniquement les colis de type STANDARD.

2. Drone :

Vitesse : 80.0 km/h.

Gestion FRAGILE : Majoration de 10% sur le coût et 20% sur le délai de livraison.
