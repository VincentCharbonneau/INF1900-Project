# Polytechnique Montréal

Département de génie informatique et génie logiciel

INF1900: Projet initial de système embarqué

# Grille de correction des programmes:

Identification:
+ Travail    : Machines à états finis logicielles
+ Section #  : 3
+ Équipe #   : 57
+ Correcteur : Charles De Lafontaine

# LISIBILITÉ
## En-têtes en début de programme   

| Pénalité par erreur                          | -1.5       |
| -------------------------------------------- | ---------- |
| Noms des auteurs                             |            |
| Description du programme                     |            |
| Identifications matérielles (Broches I/O)    |            |
| Table des états ou autres s'il y a lieu      |            |
| __Résultat partiel__                         | __(1.5/1.5)__ |

## Identificateurs significatifs (Autodocumentation)

| Points importants                            | Poids      |
| -------------------------------------------- | ---------- |
| Absence de *chiffres magiques*               | (/0.5)     |
| Noms de variables décrivent l'intention      | (0.2/0.5)     |
| Noms de fonctions décrivent l'intention      | (/0.5)     |
| __Résultat partiel__                         | __(1.2/1.5)__ |

## Commentaires pertinents

Bonnes raisons d'un commentaire
 + Explication d'un algorithme 
 + Procédure peu évidente (ou *hack*)
 + Référence d'extraits de code copiés d'Internet
 + Détail du POURQUOI d'un extrait de code

| Pénalité par erreur                          | -0.5       |
| -------------------------------------------- | ---------- |
| __Résultat partiel__                         | __(1/1.0)__ |


## Indentation   

| Points importants                            | Poids      |
| -------------------------------------------- | ---------- |
| Indentation structurée                       | (/0.5)     |
| Indentation uniforme dans le programme       | (/0.5)     |
| __Résultat partiel__                         | __(1/1.0)__ |


# MODULARITÉ ET COMPRÉHENSION
## Fonctions bien définies

| Pénalité par erreur                          | -0.5       |
| -------------------------------------------- | ---------- |
| Responsabilité unique d'une fonction         |            |
| Maximum de 4 paramètres                      |            |
| Absence de code dupliqué                     |            |
| etc                                          |            |
| __Résultat partiel__                         | __(1/1.0)__ |


## Bonne application des concepts de programmation et systèmes embarqués

| Pénalité par erreur                          | -1.0       |
| -------------------------------------------- | ---------- |
| Utilisation appropriée des registres         |            |
| Machine à états codée clairement             |            |
| Délais et minuteries utilisés judicieusement |            |
| PWM bien établi                              |            |
| Scrutation et interruptions bien utilisées   |            |
| etc                                          |            |
| __Résultat partiel__                         | __(4/4.0)__ |

# FONCTIONNALITÉS
## Git

| Points importants                            | Poids      |
| -------------------------------------------- | ---------- |
| Fichiers aux bons endroits                   | (/1.5)     |
| Absence de fichiers inutiles                 | (/1.5)     |
| __Résultat partiel__                         | __(3/3.0)__ |


## Compilation    

| Points importants                            | Poids      |
| -------------------------------------------- | ---------- |
| Absence de messages d'erreurs de compilation | (/1.5)     |
| Absence de messages d'attention (*warning*)  | (/0.5)     |
| __Résultat partiel__                         | __(2/2.0)__ |
   

## Évaluation boîte noire  

| Points importants                            | Poids      |
| -------------------------------------------- | ---------- |
| Cas normal de fonctionnement                 | (/2.5)     |
| Cas limites observables                      | (/2.5)     |
| __Résultat partiel__                         | __(5/5.0)__ |

# Résultat

__Total des points: 19.7/20__

# Commentaires du correcteur:

- Attention à la norme des noms de vos *enum classes*; même pour des acronymes (`LED`), ceux-ci devraient être en *camelCase*. Exemple: `LEDColors` -> `ledColors`.
- Attention à l'espacement de vos noms d'états (exemple ligne 33 pb1 vs lignes 35 & 37). Également, il est préférable d'espacer chaque nom d'état avec un retour de ligne. Exemple:
```
enum class Counter { 
    ZERO_PRESS, 
    ONE_PRESS, 
    TWO_PRESS 
};
```
- Votre *enum class* `ButtonReleased` inclut `PRESSED` alors que vous définissez son nom de `Released`, ambiguïtés.
- Vous n'avez pas besoin de définir un cas par défaut si vous comptez implémenter tous vos états (exemple de `Counter` lignes 57-61 pb1).
- Vos *oneliners* sont préférables d'être écrits en une seule ligne. Exemple: lignes 79-81 pb1. Les accolades peuvent être retirées pour une meilleure lisibilité.
- Vous auriez pu découpler votre `main` en sous-fonction pour votre `while (true)`.
- Code mort: `return 0` (étant donné votre boucle while).
- Votre code est très propre, bravo !
- Attention cela dit à vos noms de fichiers. Non pénalisé, car le *grader* a pu compiler vos programmes, mais les noms respectifs devraient être tout de même `pb1` et `pb2`, et non `main`.

# Basé sur le commit suivant
```
commit a64a445d234bd9fbafac6d0bcc9bcdb1b39249f1
Author: Camil Bisson <camilbisson2004@gmail.com>
Date:   Mon Jan 29 15:36:39 2024 -0500

    TP4: Work in progress,
```

# Fichiers indésirables pertinents
Aucun

# Tous les fichiers indésirables
Aucun

# Sorties de `make` dans les sous-répertoires

## Sortie de `make` dans `tp/tp2/pb1`
```
make : on entre dans le répertoire « /usagers1/chdelh/Bureau/inf1900-grader/correction_tp2/57/tp/tp2/pb1 »
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall --param=min-pagesize=0 -fno-exceptions -c main.cpp
avr-gcc -Wl,-Map,TP1_2.elf.map -mmcu=atmega324pa -o TP1_2.elf  main.o \
-lm 
avr-objcopy -j .text -j .data -O ihex TP1_2.elf TP1_2.hex
make : on quitte le répertoire « /usagers1/chdelh/Bureau/inf1900-grader/correction_tp2/57/tp/tp2/pb1 »

```

## Sortie de `make` dans `tp/tp2/pb2`
```
make : on entre dans le répertoire « /usagers1/chdelh/Bureau/inf1900-grader/correction_tp2/57/tp/tp2/pb2 »
avr-gcc -I. -I/usr/include/simavr -MMD  -g -mmcu=atmega324pa -Os -std=c++14 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall --param=min-pagesize=0 -fno-exceptions -c main.cpp
avr-gcc -Wl,-Map,TP1_2.elf.map -mmcu=atmega324pa -o TP1_2.elf  main.o \
-lm 
avr-objcopy -j .text -j .data -O ihex TP1_2.elf TP1_2.hex
make : on quitte le répertoire « /usagers1/chdelh/Bureau/inf1900-grader/correction_tp2/57/tp/tp2/pb2 »

```
