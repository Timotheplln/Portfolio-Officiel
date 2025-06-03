#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define N 4
#define TAILLE (N*N)

typedef int tGrille[TAILLE][TAILLE];

bool backtracking(tGrille grille, int numeroCase);
void chargerGrille(tGrille g);
void afficherGrille(tGrille grille);
bool possible(tGrille grille1, int lig, int col, int candidats);

int main(){
    tGrille grille;
    int numeroCase = 0;
    chargerGrille(grille);
    afficherGrille(grille);
    clock_t begin = clock();
    backtracking(grille, numeroCase);
    clock_t end = clock();
    double tmpsCPU = (end - begin)*1.0 / CLOCKS_PER_SEC;
    afficherGrille(grille);
    printf( "Temps CPU = %.3f secondes\n",tmpsCPU);
}

bool backtracking(tGrille grille, int numeroCase){
    int ligne, colonne;
    bool resultat = false;

    if (numeroCase == TAILLE * TAILLE){
        resultat = true;
    } else {
        ligne = numeroCase / TAILLE;
        colonne = numeroCase % TAILLE;
        if (grille[ligne][colonne]!=0){
            resultat = backtracking(grille, numeroCase+1);
        } else {
            for (int i = 0; i < TAILLE; i++){
                if (possible(grille, ligne, colonne, i) == true){
                    grille[ligne][colonne] = i;
                    if (backtracking(grille, numeroCase+1)==true){
                        resultat = true;
                    } else {
                        grille[ligne][colonne] = 0;
                    }
                }
            }
        }
    }
    return resultat;
}

void chargerGrille(tGrille g) {
    char nomFichier[30] = "MaxiGrilleA.sud";
    FILE *f;
    /**
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);*/
    f = fopen(nomFichier, "rb");
    if (f == NULL) {
        printf("\nERREUR sur le fichier %s\n", nomFichier);
    } else {
        fread(g, sizeof(int), TAILLE * TAILLE, f);
    }
    fclose(f);
}

void afficherGrille(tGrille grille) {
    int l, c, i, j;

    printf("     ");
    for (j = 1; j <= N * N; j++) {
        printf("%2d ", j);
        if (j % N == 0) {
            printf(" ");
        }
    }
    printf("\n");

    printf("    +");
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++) {
            printf("---");
            if ((j + 1) % N == 0) {
                printf("+");
            }
        }
    }
    printf("\n");

    for (l = 0; l < TAILLE; l++) {
        printf("%2d  |", l + 1);

        for (c = 0; c < TAILLE; c++) {
            if (grille[l][c] != 0) {
                printf("%2d ", grille[l][c]);
            } else {
                printf(" . ");
            }

            if ((c + 1) % N == 0) {
                printf("|");
            }
        }

        printf("\n");

        if ((l + 1) % N == 0) {
            printf("    +");
            for (i = 0; i < N; i++)
            {
                for (j = 0; j < N; j++) {
                    printf("---");
                    if ((j + 1) % N == 0) {
                        printf("+");
                    }
                }
            }
            printf("\n");
        }
    }
}

bool possible(tGrille grille1, int lig, int col, int candidats){
    bool verif = true;
    int l,c;
    int debut_ligne_bloc = TAILLE*(lig / TAILLE);
    int debut_colonne_bloc = TAILLE*(col / TAILLE);

    for(l = 0; l < TAILLE*TAILLE; l++){
        if(grille1[l][col] == candidats){
            verif = false;
        }
    }
    for(c=0; c<TAILLE * TAILLE; c++){
        if(grille1[lig][c] == candidats){
            verif = false;
        }
        
    }
    for (int i = debut_ligne_bloc; i < debut_ligne_bloc + (TAILLE); i++) {
        for (int j = debut_colonne_bloc; j < debut_colonne_bloc + (TAILLE); j++) {
            if (grille1[i][j] == candidats) {
                verif = false;
            }
        }
    }
    
    return verif;
}