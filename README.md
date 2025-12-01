Exercice : Test de Compétences en Algorithme, Manipulation de Fichiers, Programmation Orientée Objet, et pertinence des questions.

Objectif : Développer un programme Python qui lit un fichier de configuration, 
modifier une entrée dans un fichier JSON représentant un annuaire de personnes, 
et envoiyer l'annuaire complet par UDP en cas de modification.

Consignes :
1)Lecture du fichier de configuration

Créez un fichier de configuration (config.txt) au format suivant :
--------------------------------------------
# Fichier de sortie
directory_file=annuaire.json
# Configuration de l'envoi UDP
destination_ip=127.0.0.1
destination_port=5005
interval=1  # Intervalle de vérification des modifications en secondes
--------------------------------------------

Le programme doit lire ce fichier pour obtenir les valeurs des paramètres nécessaires.

2) Structure de l'annuaire
L'annuaire sera un fichier JSON (annuaire.json) contenant une liste de personnes. Chaque personne est définie par :
[
  {"nom": "Dupont", "prenom": "Jean", "taille": 180},
  {"nom": "Durand", "prenom": "Marie", "taille": 165}
]

Créez une classe Personne avec les attributs nom, prenom et taille, 
et une méthode to_dict qui convertit une instance en dictionnaire pour faciliter la manipulation de l'annuaire.

3) Modification d'un paramètre dans l'annuaire
Le programme doit permettre de modifier dynamiquement un des attributs d'une personne de l'annuaire, par exemple, changer la taille de "Jean Dupont".
La modification peut être simulée pour cet exercice ou demandée à l'utilisateur.


3) Envoi de l'annuaire en UDP si une modification est effectuée
Si une modification est détectée dans l'annuaire, l'ensemble du fichier JSON mis à jour doit être envoyé en tant que payload via UDP vers l'adresse et le port spécifiés.
La vérification de modification s'effectue toutes les secondes en fonction de l'intervalle défini dans le fichier de configuration.

4) Ajouter la possibilité d'ajouter une personne  via la console (par un Ctrl+N par exemple)

5) Contraintes supplémentaires
langage a utiliser : python (version 3.8) ou c++ (via mingw)

Évaluation :
Maîtrise de l'architecture logicielle et de la programmation orientée objet.
Manipulation des fichiers JSON.
Gestion des modifications et envoi conditionnel de données via UDP.
Compréhension des boucles et de la temporisation en Python.
Gestion des erreurs
Pertinence des questions

Remarque :
le fichier de config est donné à titre d'exemple, et peut être modifié, mais doit garder la possibilité d'ecrire des commentaires (ici, précédés par un #)

