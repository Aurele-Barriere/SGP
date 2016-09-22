# SGP - TP1

## Premier exercice

Cet exercice s'intéresse à la création de processus fils grâce à `fork`.
Nous récupérons dans le processus père la valeur renvoyée par le fils après qu'il se soit terminé, grâce à `wait`, puis l'affichons.

## Deuxième exercice

Cet exercice s'intéresse à la création d'un simple système producteur-consommateur, grâce à un tube Unix (`pipe`).
Nous utilisons les fonctions `read` et `write` pour écrire des données depuis le producteur, et les lire caractère par caractère dans le consommateur.

En particulier, nous nous sommes efforcés de fermer le tube dès que nécessaire, et d'intercepter le plus d'erreurs possible.

## Troisième exercice

Cet exercice est une variation du précédent, qui utilise cette fois un tube nommé.

Notre solution est très similaire à l'exemple précédent.
