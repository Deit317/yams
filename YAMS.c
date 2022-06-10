/**
 * @file YAMS.c
 * @author LAMBERT Chloé
 * @brief Ce programme permet de jouer une partie de YAMS à deux
 * @version 0.1
 * @date 2021-11-28
 * 
 * @copyright Copyright (c) 2021
 *
 * Ce programme permet de jouer une partie de YAMS. Le principe est simple,
 * les joueurs disposent de 5 dés qu'ils vont lancer pour réaliser différentes combinaisons
 * qui correspondent chacune à un score.
 * A chaque tour, les joueurs lancent lancent les dés, ils ont le droit de relancer jusqu'à
 * 3 fois, un ou tous les dés et choisir une combinaisons à valider et marquer le score associé à cette combinaison
 * dans une feuille de score. Si aucune combinaison n'est disponible, le joueur a quand même l'obligation de choisir
 * une combinaison à barer, même si ça ne lui apporte aucun points
 * La fin du jeu s'arrête lorsque toutes les combinaisons de la grilles ont été validées.
 * Celui qui à le plus de points remporte la partie.
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/**
 * @brief Raccourci associé au nombre de dés présents dans le jeu
 * 
 */
#define NOMBRE_DES 5

/**
 * @brief Raccourci associé aux nombre de joueurs pour une partie
 * 
 */
#define NOMBRE_JOUEURS 2

/**
 * @brief Représente les dés que le joueur va lancer
 * 
 */
typedef int t_des[NOMBRE_DES];

/**
 * @brief Représente le tableau de score
 * 
 * Ce tableau va permettre le stockage des scores marqués par les joueurs, à savoir les 13 combinaisons possibles
 * 
 */
typedef int t_score[13];

/**
 * @brief Représente les 13 combinaisons possibles du jeu
 * 
 * Ce tableau permet de stocker les 13 combinaisons jouables associées à leur noms
 * 
 */
const char NOMS_COMBINAISONS[13][13] = {
    "Total de 1", "Total de 2", "Total de 3", 
    "Total de 4", "Total de 5", "Total de 6", 
    "Brelan", "Carré", "Full House", 
    "Petite suite", "Grande suite", "Yams", 
    "Chance"
};

/*********************************
 *    FONCTIONS ET PROcÉDURES    *
 *********************************/

/**
 * @fn void compter_des(t_des des, int occurences[6])
 * 
 * @brief Cette procédure va compter les occurences des faces des dés pour déterminer les combinaisons
 * 
 * @param des Les 5 dés lancés par le joueur
 * @param occurences Les 6 faces des dés
 * 
 * Consiste à compter les occurences des faces des dés de chaques lancer pour déterminer quelles combinaisons ont été réalisées
 */
void compter_des(t_des des, int occurences[6]) {
    for (int i = 0; i < 6; i++) {
        occurences[i] = 0;
    }

    for (int i = 0; i < NOMBRE_DES; i++) {
        occurences[des[i] -1] = occurences[des[i] - 1] + 1;
    }
}

/**
 * @fn void trier_des(t_des des, t_des resultat)
 * 
 * @brief Cette procédure trie les dés
 * 
 * @param des Les 5 dés lancés par le joueur
 * @param resultat Le résultat, les dés triés
 * 
 * 
 */
void trier_des(t_des des, t_des resultat) {
    int temp;

    for(int i = 0; i < NOMBRE_DES; i++) {
        resultat[i] = des[i];
    }

    for (int i = 0; i < NOMBRE_DES; i++) {
        for (int j = 1; j < NOMBRE_DES; j++) {
            if (resultat[j - 1] > resultat[j]) {
                temp = resultat[j - 1];
                resultat[j - 1] = resultat[j];
                resultat[j] = temp;
            }
        }
    }
}

/**
 * @fn bool full_house(t_des des)
 * 
 * @brief Cette fonction détermine si la combinaison effectuée par le joueur est un Full House
 * 
 * @param des Les 5 dés lancés par le joueur
 * @return true si la combinaison est un Full House
 * @return false dans les autres cas
 * 
 * Consiste à déterminer si la combinaison réalisée après le lancer du joueur est un Full House
 * C'est à dire une Paire + un Brelan
 */
bool full_house(t_des des) {
    bool pair;
    bool brelan;
    int occurences[6];

    pair = false;
    brelan = false;

    compter_des(des, occurences);
    for (int i = 0; i < 6; i++) {

        if (occurences[i] == 3) {
            brelan = true;
        }
        else if (occurences[i] == 2) {
            pair = true;
        }
    }

    return pair && brelan;
}

/**
 * @fn bool brelan(t_des des)
 * 
 * @brief Cette fonction détermine si la combinaison effectuée par le joueur est un Brelan
 * 
 * @param des Les 5 dés lancés par le joueur
 * @return true si la combinaison est un Brelan
 * @return false dans les autres cas
 * 
 * Consiste à déterminer si la combinaison réalisée après le lancer du joueur est un Brelan
 * C'est à dire 3 dés identiques
 */
bool brelan(t_des des) {
    bool resultat;
    int occurences[6];

    resultat = false;

    compter_des(des, occurences);
    for (int i = 0; i < 6; i++) {
        if (occurences[i] >= 3) {
            resultat = true;
        }
    }

    return resultat;
}

/**
 * @fn bool carre(t_des des)
 * 
 * @brief Cette fonction détermine si la combinaison effectuée par le joueur est un Carré
 * 
 * @param des Les 5 dés lancés par le joueur
 * @return true si la combinaison est un Carré
 * @return false dans les autres cas
 * 
 * Consiste à déterminer si la combinaison réalisée après le lancer du joueur est un Carré
 * C'est à dire 4 dés identiques
 */
bool carre(t_des des) {
    bool resultat;
    int occurences[6];

    resultat = false;

    compter_des(des, occurences);
    for (int i = 0; i < 6; i++) {
        if (occurences[i] >= 4) {
            resultat = true;
        }
    }

    return resultat;
}

/**
 * @fn bool petite_suite(t_des des)
 * 
 * @brief Cette fonction détermine si la combinaison effectuée par le joueur est une Petite Suite
 * 
 * @param des Les 5 dés lancés par le joueur
 * @return true si la combinaison est une Petite Suite
 * @return false dans les autres cas
 * 
 * Consiste à déterminer si la combinaison réalisée après le lancer du joueur est une Petite Suite
 * C'est à dire 4 dés dont la valeur se suit
 */
bool petite_suite(t_des des) {
    bool resultat;
    int occurences[6];

    compter_des(des, occurences);

    for (int i = 0; i < 3; i++) {
        resultat = true;
        for (int j = i; j < i + 4; j++) {
            if (occurences[j] == 0) {
                resultat = false;
            }
        }

        if (resultat) {
            break;
        }
    }

    return resultat;
}

/**
 * @fn bool grande_suite(t_des des)
 * 
 * @brief Cette fonction détermine si la combinaison effectuée par le joueur est une Grande Suite
 * 
 * @param des Les 5 dés lancés par le joueur
 * @return true si la combinaison est une Grande Suite
 * @return false dans les autres cas
 * 
 * Consiste à déterminer si la combinaison réalisée après le lancer du joueur est une Grande Suite
 * C'est à dire 5 dés dont la valeur se suit
 */
bool grande_suite(t_des des) {
    bool resultat;
    t_des des_tries;

    trier_des(des, des_tries);
    resultat = true;

    for (int j = 0; j < NOMBRE_DES -1; j++) {
        if (des_tries[j] != des_tries[j + 1] -1) {
            resultat = false;
        }
    }

    return resultat;
}

/**
 * @fn bool yams(t_des des)
 * 
 * @brief Cette fonction détermine si la combinaison effectuée par le joueur est un Yams
 * 
 * @param des Les 5 dés lancés par le joueur
 * @return true si la combinaison est un Yams
 * @return false dans les autres cas
 * 
 * Consiste à déterminer si la combinaison réalisée après le lancer du joueur est un Yams
 * C'est à dire 5 dés de la même valeur
 */
bool yams(t_des des) {
    bool resultat;
    int occurences[6];

    resultat = false;

    compter_des(des, occurences);
    for (int i = 0; i < 6; i++) {
        if (occurences[i] == 5) {
            resultat = true;
        }
    }

    return resultat;
}

/**
 * @fn int entrer_combinaison(t_score scores)
 * 
 * @brief Cette fonction permet d'entrer la combinaison que le joueur a réalisé
 * 
 * @param scores La feuille de score qui contient les 13 combinaisons possibles
 * @return int l'indice de la combinaison dans le tableau des scores
 * 
 * Consiste à entrer le nom de combinaison que le joueur à rélisé et qu'il souhaite valider
 */
int entrer_combinaison(t_score scores) {
    char entree[13];
    int indice;

    indice = -1;

    printf("Entrez le nom de la combinaison que vous souhaitez valider / barrer : \n");
    scanf(" %[^\n]s", entree);

    while (indice == -1) {
        for (int i = 0; i < 13; i++) {
            if (strcmp(entree, NOMS_COMBINAISONS[i]) == 0) {
                indice = i;
            }
        }

        if (indice == -1) {
            printf("Saisie invalide, recommencez\n");
            scanf(" %[^\n]s", entree);
        }
        else if (scores[indice] != -1) {
            printf("Cette combinaison est déjà validée / barrée, recommencez\n");
            indice = -1;
            scanf(" %[^\n]s", entree);
        }
    }

    return indice;
}

/**
 * @fn int compter_points(int combinaison, t_des des)
 * 
 * @brief Cette fonction sert à compter les points que le joueur à fait avec son lancer
 * 
 * @param combinaison Le numéro, ou indice de la combinaison dans le tableau
 * @param des Les 5 dés lancés par le joueur
 * @return int le score obtenu
 * 
 * Consiste à compter les points marqués par le joueur avec son lance en fonction des combinaisons réalisées s'il y en a 
 * ou sinon par les points indiqués sur les dés
 */
int compter_points(int combinaison, t_des des) {
    int occurences[6];
    int resultat;

    compter_des(des, occurences);
    resultat = 0;

    switch (combinaison) {
    case 0 :
    case 1 :
    case 2 :
    case 3 :
    case 4 :
    case 5 :
        resultat = occurences[combinaison] * (combinaison + 1);
        break;

    case 6 :
        if (brelan(des)) {
            for (int i = 0; i < NOMBRE_DES; i++) {
                if(occurences[des[i] - 1] >= 3) {
                    resultat = resultat + des[i];
                }
            }
        }
        break;

    case 7 :
        if (carre(des)) {
            for (int i = 0; i < NOMBRE_DES; i++) {
                if(occurences[des[i] - 1] >= 4) {
                    resultat = resultat + des[i];
                }
            }
        }
        
        break;

    case 8 :
        if (full_house(des)) {
            resultat = 25;
        }
        break;
    
    case 9 :
        if (petite_suite(des)) {
            resultat = 30;
        }
        break;

    case 10 :
        if (grande_suite(des)) {
            resultat = 40;
        }
        break;

    case 11 :
        if (yams(des)) {
            resultat = 50;
        }
        break;

    case 12 :
        for (int i = 0; i < NOMBRE_DES; i++) {
            resultat = resultat + des[i];
        }
        break;

    default:
        break;
    }

    printf("points: %d\n", resultat);
    return resultat;
}

/**
 * @fn int trouver_vainqueurs(t_score scores[NOMBRE_JOUEURS])
 * 
 * @brief Cette fonction détermine quel joueur a remporté la partie
 * 
 * @param scores La feuille de score
 * @return int l'indice du vainqueur, si c'est le joueur 1 ou 2
 * 
 * Consiste à dire lequel des deux joueurs à gagner la partie, celui qui à le score le plus élevé
 */
int trouver_vainqueurs(t_score scores[NOMBRE_JOUEURS]) {
    int indice_vainqueurs;
    int total_max;
    int total_temp;

    indice_vainqueurs = -1;

    for(int i = 0; i < NOMBRE_JOUEURS; i++) {
        total_temp = 0;
        for (int j = 0; j < 13; j++) {
            total_temp = total_temp + scores[i][j];
        }
        if (total_temp > total_max || indice_vainqueurs == -1) {
            indice_vainqueurs = i;
            total_max = total_temp;
        }
    }

    return indice_vainqueurs;
}

/**
 * @fn void entrer_points(t_score scores, int combinaison, int points)
 * 
 * @brief Cette procédure sert à entrer les points marqués dans la feuille de score
 * 
 * @param scores La feuille de score
 * @param combinaison L'indice de la combinaison dans le tableau
 * @param points Points marqués par le joueur
 * 
 * Consiste à entrer le score marqué par le joueur dans le tableau/feuille de score
 */
void entrer_points(t_score scores, int combinaison, int points) {
    scores[combinaison] = points;
}

/**
 * @fn void combinaisons_possibles(t_des des, int combinaisons[13], int *nombre_combinaisons)
 * 
 * @brief Cette procédure détermine quelles combinaisons sont possibles à marquer par le joueur en fonction de son lancer
 * 
 * @param des Les 5 dés lancés par le joueur
 * @param combinaisons Les 13 combinaisons possibles
 * @param nombre_combinaisons Le nombre de combinaisons possibles
 * 
 * Consiste à déterminer quelles combinaisons sont possibles à valider par le joueur, 
 * en effet le résultat d'un lancer peut correspondre à plusieurs combinaisons, ce sont ces 
 * combinaisons qui seront indiquées
 */
void combinaisons_possibles(t_des des, int combinaisons[13], int *nombre_combinaisons) {
    int occurences[6];

    *nombre_combinaisons = 0;
    combinaisons[*nombre_combinaisons] = 12;
    *nombre_combinaisons = 1;

    if(brelan(des)) {
        combinaisons[*nombre_combinaisons] = 6;
        *nombre_combinaisons = *nombre_combinaisons +1;
    }

    if (carre(des)) {
        combinaisons[*nombre_combinaisons] = 7;
        *nombre_combinaisons = *nombre_combinaisons + 1;
    }

    if (full_house(des)) {
        combinaisons[*nombre_combinaisons] = 8;
        *nombre_combinaisons = *nombre_combinaisons + 1;
    }

    if (petite_suite(des)) {
        combinaisons[*nombre_combinaisons] = 9;
        *nombre_combinaisons = *nombre_combinaisons + 1;
    }

    if (grande_suite(des)) {
        combinaisons[*nombre_combinaisons] = 10;
        *nombre_combinaisons = *nombre_combinaisons + 1;
    }

    if (yams(des)) {
        combinaisons[*nombre_combinaisons] = 11;
        *nombre_combinaisons = *nombre_combinaisons + 1;
    }

    compter_des(des, occurences);

    for(int i = 0; i < 6; i++) {
        if(occurences[i] >= 1) {
            combinaisons[*nombre_combinaisons] = i;
            *nombre_combinaisons = *nombre_combinaisons + 1;
        }
    }
}

/**
 * @fn void combinaisons_disponibles(t_des des, t_score scores)
 * 
 * @brief Cette procédure permet de déterminer quelles sont les combinaisons disponibles dans la grille
 * 
 * @param des Les dés lancés par le joueur
 * @param scores La feuille de score
 * 
 * Consiste à vérifier quelles sont les combinaisons disponibles dans la grille, celles que le joueur n'a pas encore validées
 * Par exemple, si le joueur réalise un score de 3 mais qu'il a déjà validé la case, alors celle-ci n'apparaîtra pas
 */
void combinaisons_disponibles(t_des des, t_score scores) {
    int combinaisons_effectuees[13];
    int nombre_combinaisons_effectuees;
    int combinaisons_dispos[13]; 
    int nombre_combinaisons_dispos;

    nombre_combinaisons_dispos = 0;

    combinaisons_possibles(des, combinaisons_effectuees, &nombre_combinaisons_effectuees);

    for(int i = 0; i < nombre_combinaisons_effectuees; i++) {
        for(int j = 0; j < 13; j++) {
            if (combinaisons_effectuees[i] == j) {
                if (scores[j] == -1) {
                    combinaisons_dispos[nombre_combinaisons_dispos] = j;
                    nombre_combinaisons_dispos = nombre_combinaisons_dispos +1;
                }
            }
        }
    }

    printf("Les combinaisons disponibles sont les suivantes :\n");
    for(int i = 0; i < nombre_combinaisons_dispos; i++ ) {
        printf(" - %s\n", NOMS_COMBINAISONS[combinaisons_dispos[i]]);
    }
    printf("Les combinaisons qui ne sont pas encore validées sont les suivantes :\n");
    for (int i = 0; i < 13; i++) {
        if (scores[i] == -1) {
            printf(" - %s\n", NOMS_COMBINAISONS[i]);
        }
    }
}

/**
 * @fn void entrer_chaine(char message[50], char reponse[20])
 * 
 * @brief Cette procédure sert à écrire un message à l'écran auquel le joueur pourra répondre, ici elle servira à saisir
 * le nom du joueur
 * 
 * @param message Correspond au message qui sera affiché à l'écran, un message qui demandera le nom du joueur
 * @param reponse Sera la réponse saisie au clavier, elle correspond au nom du joueur
 * 
 * Connsiste à saisir les noms des deux joueurs au début de la partie
 */
void entrer_chaine(char *message, char *reponse) {
    printf("%s", message);
    scanf(" %[^\n]", reponse);
}

/**
 * @fn void entrer_booleen(char message[50], bool *reponse)
 * 
 * @brief Cette procédure servira à demander au joueur s'il veut relancer les dés, il devra répondre soit "oui" soit "non"
 * 
 * @param message Correspond au message affiché à l'écran, si le joueur veut relancer ou non
 * @param reponse Correspond à la réponse saisie par le joueur, elle devra soit être "oui" soit "non"
 * 
 * Consiste à saisir une réponse qui sera vraie (oui) ou fausse (non) pour savoir si le joueur souhaite relancer un ou 
 * plusieurs dés
 */
void entrer_booleen(char *message, bool *reponse) {
    char entree[20];

    do {
        printf("%s", message);
        scanf(" %s", entree);
    }
    while (strcmp (entree, "oui") != 0 && strcmp(entree, "non") != 0);
    
    if (strcmp(entree, "oui") == 0) {
        *reponse = true;
    }
    else {
        *reponse = false;
    }
}

/**
 * @fn void entrer_suite(char message[50], t_des reponse)
 * 
 * @brief Cette procédure sert à demander au joueur quels dés sont à relancer dans le cas ou il aurait répondu "oui"
 * 
 * @param message Le message affiché avant la saisie, qui demandera quels dés sont à relancer
 * @param reponse La réponse, qui correspond au numéro, ou indice, du ou des dés que le joueur souhaite relancer
 * 
 * Consiste à saisir au clavier le ou les dés que le joueur souhaite relancer s'il veut relancer
 */
void entrer_suite(char *message, t_des reponse) {
    int entree;
    int compteur;

    compteur = 0;

    do {
        printf("%s", message);
        scanf("%d", &entree);

        if (entree > 0 && entree <= NOMBRE_DES) {
            reponse[compteur] = entree - 1;
            compteur = compteur +1;
        }
        else if (entree < 0 || entree > NOMBRE_DES) {
            printf("Entrée invalide, recommencez\n");
        }
    }
    while (entree != 0 && compteur < NOMBRE_DES);

    for (int i = compteur; i < NOMBRE_DES; i++) {
        reponse[i] = -1;
    }
}

/**
 * @fn void afficher_scores(t_score scores, char nom_joueur[20])
 * 
 * @brief Cette procédure sert à afficher les scores dans le tableau
 * 
 * @param scores La feuille de score
 * @param nom_joueur Correspond au nom du joueur dont c'est au tour de jouer, chaque joueur ayant sa feuille de score
 * 
 * Consiste à afficher les scores du joueur dans les cases correspondantes du tableau de la feuille de score
 */
void afficher_scores(t_score scores, char nom_joueur[20]) {
    int total_sup;
    int bonus;
    int total_inf;
    t_score affiche;

    total_sup = 0;
    bonus = 0;
    total_inf = 0;

    for (int i = 0; i < 6; i++) {
        if (scores[i] != -1) {
            total_sup = total_sup + scores[i];
            affiche[i] = scores[i];
        }
        else {
            affiche[i] = 0;
        }
    }

    if (total_sup > 62) {
        bonus = 35;
    }

    for (int i = 6; i < 13; i++) {
        if (scores[i] != -1) {
            total_inf = total_inf+ scores[i];
            affiche[i] = scores[i];
        }
        else {
            affiche[i] = 0;
        }
    }

    printf("           ┏━━━━┓            ┏━━━━┓\n"
           "           ┃%c%c%c%c┃            ┃%c%c%c%c┃\n"
           "┏━━━━━━━━━━╋━━━━┫ ┏━━━━━━━━━━╋━━━━┫\n"
           "┃Total de 1┃%3d ┃ ┃Brelan    ┃%3d ┃\n"
           "┣━━━━━━━━━━╋━━━━┫ ┣━━━━━━━━━━╋━━━━┫\n"
           "┃Total de 2┃%3d ┃ ┃Carré     ┃%3d ┃\n"
           "┣━━━━━━━━━━╋━━━━┫ ┣━━━━━━━━━━╋━━━━┫\n"
           "┃Total de 3┃%3d ┃ ┃Full House┃%3d ┃\n"
           "┣━━━━━━━━━━╋━━━━┫ ┣━━━━━━━━━━╋━━━━┫\n"
           "┃Total de 4┃%3d ┃ ┃Pte suite ┃%3d ┃\n"
           "┣━━━━━━━━━━╋━━━━┫ ┣━━━━━━━━━━╋━━━━┫\n"
           "┃Total de 5┃%3d ┃ ┃Gde suite ┃%3d ┃\n"
           "┣━━━━━━━━━━╋━━━━┫ ┣━━━━━━━━━━╋━━━━┫\n"
           "┃Total de 6┃%3d ┃ ┃Yams      ┃%3d ┃\n"
           "┣━━━━━━━━━━╋━━━━┫ ┣━━━━━━━━━━╋━━━━┫\n"
           "┃Bonus > 62┃%3d ┃ ┃Chance    ┃%3d ┃\n" 
           "┣━━━━━━━━━━╋━━━━┫ ┣━━━━━━━━━━╋━━━━┫\n"
           "┃Total sup ┃%3d ┃ ┃Total inf ┃%3d ┃\n"
           "┗━━━━━━━━━━┻━━━━┛ ┗━━━━━━━━━━┻━━━━┛\n" 
           "┏━━━━━━━━━━┳━━━━┓\n"
           "┃Total     ┃%3d ┃\n"
           "┗━━━━━━━━━━┻━━━━┛\n", 
           nom_joueur[0], nom_joueur[1], nom_joueur[2], nom_joueur[3], nom_joueur[0], nom_joueur[1], nom_joueur[2], nom_joueur[3],
           affiche[0], affiche[6],
           affiche[1], affiche[7],
           affiche[2], affiche[8],
           affiche[3], affiche[9],
           affiche[4], affiche[10],
           affiche[5], affiche[11],
           bonus, affiche[12],
           total_sup, total_inf, 
           total_sup + bonus + total_inf
        );
}

/**
 * @fn void lancer_des(int nombre, t_des des)
 * 
 * @brief Cette procédure sert à lancer les dés
 * 
 * @param nombre Le nombre de dés qui est à lancer, il peut être de 5 ou moins si le joueur décide de relancer certains dés
 * @param des Les 5 dés à lancer
 * 
 * Consiste à lancer les dés d'un joueur
 */
void lancer_des(int nombre, t_des des) {
    for (int i = 0; i < NOMBRE_DES; i++) {
        des[i] = -1;
    }
    for (int i = 0; i < nombre; i++) {
        des[i] = rand() % 6 + 1;
    }
}

/**
 * @fn void afficher_des(t_des des)
 * 
 * @brief Cette procédure sert à afficher les dés lancés
 * 
 * @param des Les 5 dés lancés
 * 
 * Consiste à afficher la valeur des dés du joueur après un lancer
 */
void afficher_des(t_des des) {
    for (int i = 0; i < NOMBRE_DES; i++) {
        switch (des[i]) {
        case 1 :
            printf("┏━━━━━━━┓\n"
                   "┃       ┃\n"
                   "┃   ●   ┃\n"
                   "┃       ┃\n"
                   "┗━━━━━━━┛\n");
            break;
        
        case 2 :
            printf("┏━━━━━━━┓\n"
                   "┃     ● ┃\n"
                   "┃       ┃\n"
                   "┃ ●     ┃\n"
                   "┗━━━━━━━┛\n");
            break;

        case 3 :
            printf("┏━━━━━━━┓\n"
                   "┃     ● ┃\n"
                   "┃   ●   ┃\n"
                   "┃ ●     ┃\n"
                   "┗━━━━━━━┛\n");
            break;

        case 4 :
            printf("┏━━━━━━━┓\n"
                   "┃ ●   ● ┃\n"
                   "┃       ┃\n"
                   "┃ ●   ● ┃\n"
                   "┗━━━━━━━┛\n");
            break;

        case 5 :
            printf("┏━━━━━━━┓\n"
                   "┃ ●   ● ┃\n"
                   "┃   ●   ┃\n"
                   "┃ ●   ● ┃\n"
                   "┗━━━━━━━┛\n");
            break;

        case 6 :
            printf("┏━━━━━━━┓\n"
                   "┃ ●   ● ┃\n"
                   "┃ ●   ● ┃\n"
                   "┃ ●   ● ┃\n"
                   "┗━━━━━━━┛\n");
            break;
        
        default:
            printf("? ");
            break;
        }
        //printf("[%d] ",des[i]);
    }
    printf("\n");
    
}

/*********************************
 *      PROGRAMME PRINCIPAL      *
 *********************************/

int main() {
    t_score scores[NOMBRE_JOUEURS];
    t_des des;
    char joueurs[NOMBRE_JOUEURS][50];
    int compteur_essais;
    int combinaison;
    bool validation_lancer;

    srand(time(NULL));

    for (int i = 0; i < NOMBRE_JOUEURS; i++) {
        for (int j = 0; j < 13; j++) {
            scores[i][j] = -1;
        }
    }

    entrer_chaine("Entrez le nom du premier joueur\n", joueurs[0]);
    entrer_chaine("Entrez le nom du deuxième joueur\n", joueurs[1]);

    for(int tour = 0; tour < 13; tour++) {
        for (int i = 0; i < NOMBRE_JOUEURS; i++) {
            afficher_scores(scores[i], joueurs[i]);
            lancer_des(NOMBRE_DES,des);
            afficher_des(des);

            compteur_essais = 3;

            do {
                entrer_booleen("Voulez vous relancer les dés ?\n", &validation_lancer);

                if (validation_lancer) {
                    t_des des_a_relancer;
                    t_des des_relances;
                    int compteur;

                    compteur = 0;

                    entrer_suite("Entrez le numéro du dé à relancer (de 1 à 5, bornes incluses, ou 0 pour arrêter)\n", des_a_relancer);

                    for (int j = 0; j < 5; j++){
                        if (des_a_relancer[j] != -1) {
                            compteur = compteur + 1;
                        }
                    }

                    lancer_des(compteur, des_relances);

                    for (int j = 0; j < 5; j++){
                        if (des_relances[j] != -1) {
                            des[des_a_relancer[j]] = des_relances[j];
                        }
                    }

                    compteur_essais = compteur_essais -1;
                    printf("Voici les résultats du lancer de dés :\n");
                    afficher_des(des);
                }
                
            }
            while ((compteur_essais > 0) && (validation_lancer == true));

            combinaisons_disponibles(des, scores[i]);
            combinaison = entrer_combinaison(scores[i]);
            entrer_points(scores[i], combinaison, compter_points(combinaison, des));

            afficher_scores(scores[i], joueurs[i]);
        }
    }

    printf("\nLe gagnant est : %s ! Félicitations\n", joueurs[trouver_vainqueurs(scores)]);

    return EXIT_SUCCESS;
}