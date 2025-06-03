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
void tri_insertion(t_tableau T, int *comp);
void affiche_tableau(t_tableau T);
int main(){
    srand(time(NULL));
    // Déclaration des variables
    int i,comp=0;
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
    tri_insertion(T,&comp);
    clock_t end = clock();
    affiche_tableau(T);
    double tmpsCPU = ((end - begin)*1.0) / CLOCKS_PER_SEC;
    printf("Comparaisons = %d\n",comp);
    printf("Temps CPU = %.3f secondes\n",tmpsCPU);
    
    return EXIT_SUCCESS;
}

void tri_insertion(t_tableau T,int *comp){
    int i,j,x;
    for(i=1;i<TAILLE;i++){
        // x représente l'élément à placer au bon endroit
        x=T[i];

        // décaler les éléments T[0]..T[i-1] qui sont plus grands
        // que x, en partant de T[i-1]
        j=i;
        while((j>0)&&(T[j-1]>x)){
            T[j]=T[j-1];
            j=j-1;
            *comp=*comp+1;
        }
        // placer x dans le "trou" laissé par le décalage
        T[j]=x;
    }
}

void affiche_tableau(t_tableau T){
    int i;
    for(i=1;i<TAILLE;i++){
        printf("%d\t", T[i]);
    }
    printf("\n");
}