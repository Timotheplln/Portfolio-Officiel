#include <stdio.h>

// Taille de la carte
#define LARGEUR 10
#define HAUTEUR 10

// Les symboles utilisés pour représenter la carte
#define MUR '#'
#define JOUEUR '@'
#define VIDE ' '

// La carte du jeu
char carte[LARGEUR][HAUTEUR] = {
    "##########",
    "#        #",
    "#        #",
    "#        #",
    "#   @    #",
    "#        #",
    "#        #",
    "#        #",
    "#        #",
    "##########"
};

// Position du joueur
int posX = 4;
int posY = 4;

// Afficher la carte
void afficherCarte() {
    for (int y = 0; y < HAUTEUR; y++) {
        for (int x = 0; x < LARGEUR; x++) {
            if (x == posX && y == posY) {
                printf("%c", JOUEUR);
            } else {
                printf("%c", carte[x][y]);
            }
        }
        printf("\n");
    }
}

int main() {
    int touche;
    do {
        system("cls"); // Effacer la console
        afficherCarte(); // Afficher la carte

        // Lire la touche pressée par le joueur
        touche = getch();
        switch (touche) {
            case 'w':
                if (carte[posX][posY - 1] != MUR) {
                    posY--;
                }
                break;
            case 's':
                if (carte[posX][posY + 1] != MUR) {
                    posY++;
                }
                break;
            case 'a':
                if (carte[posX - 1][posY] != MUR) {
                    posX--;
                }
                break;
            case 'd':
                if (carte[posX + 1][posY] != MUR) {
                    posX++;
                }
                break;
        }
    } while (touche != 'q'); // Quitter le jeu si 'q' est pressé

    return 0;
}