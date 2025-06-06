#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 3
#define TAILLE (N*N)

typedef struct {
    int valeur;
    int candidats[TAILLE];
    int nbCandidats;
} tCase1;

typedef tCase1 tGrille[TAILLE][TAILLE];

void ajouterCandidat(tCase1 *laCase, int val);
void retirerCandidat(tCase1 *laCase, int val, int *nbelim);
bool EstCandidat(tCase1 laCase, int val);
int nbCandidats(tCase1 laCase);
void initialiserCandidats(tGrille g, int *nbCand);
void chargerGrille(tGrille g);
void afficherGrille(tGrille g);
bool possible(tGrille grille1, int lig, int col, int candidats);

//bool backtracking(tGrille grille, int numeroCase);

int main(){
    tGrille g;
    bool progression;
    int nbCasesVides, nbremplie, nbelim, nbCand, videinit;
    videinit=0;
    nbCand=0;
    nbelim=0;
    nbCasesVides=0;
    nbremplie=0;


    chargerGrille(g);
    for(int i = 0; i < TAILLE; i++){
        for(int j = 0; j < TAILLE; j++){
            if(g[i][j].valeur == 0){
                // si la case est vide
                nbCasesVides++; // à la fin de la boucle on a le nombre de cases vides au total
                //for(int k = 1; k < TAILLE + 1; k++){
                //    EstCandidat(g,i,j,k,&nbCasesVides);
                //}
            }
        }
    }
    initialiserCandidats(g, &nbCand);
    afficherGrille(g);
    progression = true;
    videinit = nbCasesVides;

    while (progression && nbCasesVides != 0)
    {
        progression = false;

        for (int i = 0; i < TAILLE; i++)
        {
            printf("\nligne %d: ", i);
            for (int j = 0; j < TAILLE; j++)
            {
                printf("%d avec %dcand      ", g[i][j].valeur, g[i][j].nbCandidats);
                if (g[i][j].valeur==0 && nbCandidats(g[i][j])==1 && possible(g, i, j, g[i][j].candidats[1])==true)
                {
                    g[i][j].valeur=g[i][j].candidats[1];
                    nbremplie++;
                    for (int x = 0; x < TAILLE; x++)
                    {
                        printf("\nvaleur sup: %d en %d %d\n", g[i][j].valeur, x, j);
                        retirerCandidat(&g[x][j], g[i][j].valeur, &nbelim);
                        retirerCandidat(&g[i][x], g[i][j].valeur, &nbelim);
                    }
                    nbCasesVides = nbCasesVides-1;
                    progression = true;
                }
            }
        }
        printf("\n\n%d          %d\n", progression, nbCasesVides);
    }
    afficherGrille(g);
    printf("\n\n- Nombre de cases remplies: %d sur %d\n", nbremplie, videinit);
    printf("\n- Taux de remplissage: %.2f%%\n", (nbremplie/(videinit*1.0))*100);
    printf("\n- Nombre de candidats elimines: %d sur %d\n", nbelim, nbCand);
    printf("\n- Pourcentage d'elimination: %.2f%%\n", (nbelim/(nbCand*1.0))*100);
    //tGrille grille;
    //int numeroCase = 0;
    //chargerGrille(grille);
    //afficherGrille(grille);
    //clock_t begin = clock();
    //backtracking(grille, numeroCase);
    //clock_t end = clock();
    //double tmpsCPU = (end - begin)*1.0 / CLOCKS_PER_SEC;
    //afficherGrille(grille);
    //printf( "Temps CPU = %.3f secondes\n",tmpsCPU);
}

void ajouterCandidat(tCase1 *laCase, int val){
    if (laCase->nbCandidats<TAILLE)
    {
        laCase->nbCandidats++;
        laCase->candidats[laCase->nbCandidats]=val;
    } else
    {
        printf("erreur\n");
    }
}

void retirerCandidat(tCase1 *laCase, int val, int *nbelim){
    //int i, x;
    //if (laCase->nbCandidats>=0)
    //{
    //    for (i = 0; i < TAILLE; i++)
    //    {
    //        if (laCase->candidats[i]==val)
    //        {
    //            laCase->nbCandidats=laCase->nbCandidats-1;
    //            for (x = i ; x < laCase->nbCandidats; x++)
    //            {
    //                laCase->candidats[x]=laCase->candidats[x+1];
    //            }
    //            (*nbelim)++;
    //        }
    //    }
    //} else {
    //    printf("vide\n");
    //}
    for (int i = 0; i < laCase->nbCandidats; i++)
    {
        if (laCase->candidats[i]==val)
        {
            for (int y = i; y < laCase->nbCandidats; y++)
            {
                laCase->candidats[y]=laCase->candidats[y+1];
            }
            laCase->nbCandidats--;
            (*nbelim)++;
        }
    }
}

bool EstCandidat(tCase1 laCase, int val){
    bool x;
    x=false;
    for (int i = 0; i < TAILLE; i++)
    {
        if (laCase.candidats[i]==val)
        {
            x=true;
        }
    }
    return x;
}

int nbCandidats(tCase1 laCase){
    return laCase.nbCandidats;
}

void initialiserCandidats(tGrille g, int *nbCand){
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            g[i][j].nbCandidats=0;
            for (int x = 0; x <= TAILLE; x++)
            {
                g[i][j].candidats[x]=0;
            }
        }
    }
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            for (int x = 1; x <= TAILLE; x++)
            {
                if (g[i][j].valeur==0 && possible(g, i, j, x)== true)
                {
                    ajouterCandidat(&g[i][j], x);
                    (*nbCand)++;
                    if (i==5 && j==7)
                    {
                        printf("\ncand = %d\n", x);
                    }
                    
                }
            }
        }
    }
}

void chargerGrille(tGrille g) {
    //char nomFichier[30];
    ///FILE *f;

    char nomFichier[30] = "GrilleA.sud";
    // char entre;
    FILE *f;
    f = fopen(nomFichier, "rb");
    //printf("Nom du fichier ? ");
    //scanf("%s", nomFichier);
    //printf("Fichier : %s\n", nomFichier);

    while (f == NULL) {
        printf("\nERREUR sur le fichier %s\n", nomFichier);
        printf("Nom du fichier ? ");
        scanf("%s", nomFichier);
        printf("Fichier : %s\n", nomFichier);
        f = fopen(nomFichier, "rb");
    }

    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            fread(&g[i][j].valeur, sizeof(int), 1, f);
        }
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
            if (grille[l][c].valeur != 0) {
                printf("%2d ", grille[l][c].valeur);
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
    int debut_ligne_bloc = N*(lig / N);
    int debut_colonne_bloc = N*(col / N);
    for(l = 0; l < TAILLE; l++){
        if(grille1[l][col].valeur == candidats){
            verif = false;
        }
    }
    for(c=0; c<TAILLE; c++){
        if(grille1[lig][c].valeur == candidats){
            verif = false;
        }
    }
    for (int i = debut_ligne_bloc; i < debut_ligne_bloc + (N); i++) {
        for (int j = debut_colonne_bloc; j < debut_colonne_bloc + (N); j++) {
            if (grille1[i][j].valeur == candidats) {
                verif = false;
            }
        }
    }
    return verif;
}
/*
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
}*/