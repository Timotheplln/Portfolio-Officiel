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
void retirerCandidat(tCase1 *laCase, int val, int nbelim);
bool EstCandidat(tCase1 laCase, int val);
int nbCandidats(tCase1 laCase);
void initialiserCandidats(tGrille g, int *nbCand);
void chargerGrille(tGrille g);
void afficherGrille(tGrille g);

int main(){
    tGrille g;
    bool progression;
    int nbCasesVides, nbremplie, nbelim, nbCand, videinit;
    videinit=0;
    nbCand=0;
    nbelim=0;
    nbCasesVides=0;


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
    int debut_ligne_bloc;
    int debut_colonne_bloc;
    progression = true;
    videinit = nbCasesVides;

    while (progression && nbCasesVides != 0)
    {
        progression = false;

        for (int i = 0; i < TAILLE; i++)
        {
            for (int j = 0; j < TAILLE; j++)
            {
                if (g[i][j].valeur==0 && nbCandidats(g[i][j])==1)
                {
                    g[i][j].valeur=g[i][j].candidats[0];
                    for (int x = 0; x < TAILLE; x++)
                    {
                        retirerCandidat(&g[x][j], g[i][j].valeur, nbelim);
                        retirerCandidat(&g[i][x], g[i][j].valeur, nbelim);
                    }
                    nbCasesVides = nbCasesVides-1;
                    progression = true;
                }
                if (g[i][j].valeur==0 && nbCandidats(g[i][j])==1)
                {
                    g[j][i].valeur=g[j][i].candidats[0];
                    for (int x = 0; x < TAILLE; x++)
                    {
                        retirerCandidat(&g[x][i], g[j][i].valeur, nbelim);
                        retirerCandidat(&g[j][x], g[j][i].valeur, nbelim);
                    }
                    nbCasesVides = nbCasesVides-1;
                    progression = true;
                }
                for (int y = 0; y < N; y++)
                {
                    if(i < N*y+1){
                        debut_ligne_bloc = N*y;
                        for (int x = 0; x < N; x++)
                        {
                            if(j <N*x+1){
                                debut_colonne_bloc = N*x;
                            }
                        }
                    }
                }
                for (int z = 0; z < TAILLE; z++){
                    for (int x = debut_ligne_bloc; x < debut_ligne_bloc + TAILLE; x++) {
                        for (int y = debut_colonne_bloc; y < debut_colonne_bloc + TAILLE; y++) {
                            {
                                if (g[x][y].valeur == z) {
                                    retirerCandidat(&g[x][y], z, nbelim);
                                }
                            }
                        }
                    }
                }
                if (g[i][j].nbCandidats == 1 )
                {
                    g[i][j].valeur = g[i][j].candidats[1];
                    nbremplie = nbremplie + 1;
                    progression = true;
                }
            }
        }
    }
    afficherGrille(g);
    printf("\n\n- Nombre de cases remplies: %d sur %f\n", nbremplie, videinit);
    printf("\n- Taux de remplissage: %f%%\n", (nbremplie/videinit)*100);
    printf("\n- Nombre de candidats elimines: %d sur %f\n", nbelim, nbCand);
    printf("\n- Pourcentage d'elimination: %f%%", (nbelim/(nbCand*1,0))*100);
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

void retirerCandidat(tCase1 *laCase, int val, int nbelim){
    int i, x;
    for (i = 0; i < TAILLE; i++)
    {
        if (laCase->candidats[i]==val)
        {
            laCase->nbCandidats=laCase->nbCandidats-1;
            for (x = 1 ; x < laCase->nbCandidats; x++)
            {
                laCase->candidats[x]=laCase->candidats[x+1];
            }
            nbelim++;
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
            for (int x = 1; x < TAILLE+1; x++)
            {
                if (g[i][j].valeur==0 && possible(g[i][j], x)== true)
                {
                    ajouterCandidat(&g[i][j], x);
                    (*nbCand)++;
                }
            }
        }
    }
}

void chargerGrille(tGrille g) {
    char nomFichier[30];
    FILE *f;

    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    printf("Fichier : %s\n", nomFichier);

    f = fopen(nomFichier, "rb");
    while (f == NULL) {
        printf("\nERREUR sur le fichier %s\n", nomFichier);
        printf("Nom du fichier ? ");
        scanf("%s", nomFichier);
        printf("Fichier : %s\n", nomFichier);
        f = fopen(nomFichier, "rb");
    }

    int i, j;
    for (i = 0; i < TAILLE; i++) {
        for (j = 0; j < TAILLE; j++) {
            fread(&g[i][j].valeur, sizeof(int), 1, f);
            fclose(f);
            return;
        }
    }

    fclose(f);
}

void afficherGrille(tGrille g) {
    int l, c, i, j;

    printf("    ");
    for (j = 1; j <= N * N; j++) { /**affiche la ligne pour avoir les cohordonnées des colonnes */
        printf(" %d ", j);
        if (j % N == 0) {
            printf(" ");
        }
    }
    printf("\n");

    printf("   +");
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
        printf("%d  |", l + 1);

        for (c = 0; c < TAILLE; c++) {
            if (g[l][c].valeur != 0) {
                printf(" %d ", g[l][c].valeur); /**affiche les valeur des case grace a la grille choisie*/
            } else {
                printf(" . ");
            }

            if ((c + 1) % N == 0) {
                printf("|");
            }
        }

        printf("\n");

        if ((l + 1) % N == 0) {
            printf("   +");
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