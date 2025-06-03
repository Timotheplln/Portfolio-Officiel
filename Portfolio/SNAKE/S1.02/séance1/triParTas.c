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
void affiche_tableau(t_tableau T);
void triParTas(t_tableau t);
void tamiser(t_tableau t, int noeud, int n);
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
    triParTas(T);
    clock_t end = clock();
    affiche_tableau(T);
    double tmpsCPU = ((end - begin)*1.0) / CLOCKS_PER_SEC;
    printf("Temps CPU = %.3f secondes\n",tmpsCPU);
    
    return EXIT_SUCCESS;
}

void tamiser(t_tableau t, int noeud, int n){
    int fils,permuter;
    fils=2*noeud+1;
    if((fils<n)&&(t[fils+1]>t[fils])){
        fils=fils+1;
    }
    if((fils<=n)&&(t[noeud]<t[fils])){
        permuter=t[noeud];
        t[noeud]=t[fils];
        t[fils]=permuter;
        tamiser(t,fils,n);
    }
}

void triParTas(t_tableau t){
    int i,permuter;
    for(i=TAILLE/2-1;i>=0;i--){
        tamiser(t,i,TAILLE-1);
    }

    for(i=TAILLE-1;i>=0;i--){
        permuter=t[0];
        t[0]=t[i];
        t[i]=permuter;
        tamiser(t,0,i-1);
    }
}

void affiche_tableau(t_tableau T){
    int i;
    for(i=1;i<TAILLE;i++){
        printf("%d\t", T[i]);
    }
    printf("\n");
}