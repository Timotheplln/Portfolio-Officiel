/**
* @author Armel KERMANAC'H 1C1
* @version 3.0
* @date 18/11/2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Déclaration des constantes
#define TAILLE 300000
// Déclaration des tableaux
typedef int t_tableau[TAILLE];
// Déclaration des procédures & fonctions
void triRapide(t_tableau T, int debut, int fin);
void affiche_tableau(t_tableau T);
int partition(t_tableau t, int debut, int fin, int pivot);
int main(){
    srand(time(NULL));
    // Déclaration des variables
    int i;
    t_tableau T;
    // Initialisation des données
    for(i=0;i<TAILLE;i++){
        T[i]=rand();
    }
    // Saisie des données

    // Traitement des données
    
    // Affichage du résultat
    affiche_tableau(T);
    clock_t begin = clock();
    triRapide(T,0,TAILLE);
    clock_t end = clock();
    affiche_tableau(T);
    double tmpsCPU = ((end - begin)*1.0) / CLOCKS_PER_SEC;
    printf("Temps CPU = %.3f secondes\n",tmpsCPU);
    
    return EXIT_SUCCESS;
}

int partition(t_tableau t, int debut, int fin, int pivot){
    int i,j,permuter;
    permuter=t[pivot];
    t[pivot]=t[fin];
    t[fin]=permuter;
    j=debut;
    for(i=debut;i<fin;i++){
        if(t[i]<=t[fin]){
            permuter=t[i];
            t[i]=t[j];
            t[j]=permuter;
            j=j+1;
        }
    }
    permuter=t[fin];
    t[fin]=t[j];
    t[j]=permuter;

    return j;
}

void triRapide(t_tableau t, int debut, int fin){
    int pivot;
    if(debut<fin){
        pivot=(debut+fin)/2;
        pivot=partition(t, debut, fin, pivot);
        triRapide(t, debut, pivot-1);
        triRapide(t, pivot+1, fin);
    }
}

void affiche_tableau(t_tableau T){
    int i;
    for(i=1;i<TAILLE;i++){
        printf("%d\t", T[i]);
    }
    printf("\n");
}