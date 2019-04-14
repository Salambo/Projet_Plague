## Information pratiques
- Salomé Ménard
- Matthieu Bell-Montembault

## Compilation
`$ make` à partir de la racine du projet permet de générer les deux exécutables \
Les exécutables générés sont ensuites disponibles dans les répertoires `/server` et `/journalist`, ils portent tous les deux le nom `main`

## Nettoyage
`$ make reset` permet de supprimer l'ensembl des fichiers générés, dont les exécutables

## Fonctionnement
Les résultats, en plus d'être affichés en temps réel, sont enregistrés dans le fichier `évolution.txt` dans `/server`\
Une fois la simulation lancée, pour avancer dans les jours, il suffit de d'appuyer sur `entrer`, affichant le déroulement actuel de la simulation avec le nombre de ctoyens contaminés, etc...