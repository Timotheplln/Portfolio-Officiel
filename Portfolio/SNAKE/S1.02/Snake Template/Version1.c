/**
* @brief première version du jeu de snake
* @brief faire avancer le snake vers la droite
* @author Armel KERMANAC'H 1C1
* @version 1.2
* @date 25/10/2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
// DECLARATION DES CONSTANTES
const int TAILLE=10; // Taille du serpent

// DECLARATION DES PROCÉDURES
void gotoXY(int x, int y); // Aller aux coordonnées données
int kbhit(); // Vérifie si une touche à été pressée
void afficher(int x, int y, char c); // Affiche le car c à la position x y
void effacer(int x, int y); // Remplace le car à la position x y par un espace
void dessinerSerpent(int lesX[TAILLE], int lesY[TAILLE]); // Affiche un à un les éléments du serpent sur l'écran
void progresser(int lesX[TAILLE], int lesY[TAILLE]); // Calcule et affiche la prochaine position du serpent

int main(){
    // DECLARATION DES VARIABLES
    int i;
    int x; // Position sur les colonnes
    int y; // Position sur les lignes
    int lesY[TAILLE],lesX[TAILLE]; // Position de chaques parties du serpent
    // SAISIE DES DONNÉES
    printf("Position du snake sur les colonnes : ");
    scanf("%d", &x);
    while((x<1)||(x>40)){
        printf("ERREUR, l'entier doit être entre 0 et 40\n");
        if(x<1){
            printf("Le nombre que vous avez entré est inférieur ou égal à 0 : ");
        }
        if(x>40){
            printf("Le nombre que vous avez entré est supérieur ou égal à 40 : ");
        }
        scanf("%d", &x);
    }
    printf("Position du snake sur les lignes : ");
    scanf("%d", &y);
    while((y<1)||(y>40)){
        printf("ERREUR, l'entier doit être entre 0 et 40\n");
        if(y<1){
            printf("Le nombre que vous avez entré est inférieur ou égal à 0 : ");
        }
        if(y>40){
            printf("Le nombre que vous avez entré est supérieur ou égal à 40 : ");
        }
        scanf("%d", &y);
    }
    system("clear"); // Passer à l'affichage du snake
    // TRAITEMENT DES DONNÉES
    for(i=0;i<TAILLE;i++){
        lesY[i]=y; // Implémentation de la position du snake sur les lignes
        lesX[i]=x-i; // Implémentation de la position du snake sur les colonnes
    }
    // AFFICHAGE
    dessinerSerpent(lesX,lesY);
    do{
        progresser(lesX,lesY);
        usleep(50000); // Attendre 50000µs (0.05s)
        kbhit();
    }while((kbhit()!=1)||(getchar()!='a')); // Si le caractère appuyé est un 'a', le programme s'arrête

    return EXIT_SUCCESS;
}

void dessinerSerpent(int lesX[TAILLE], int lesY[TAILLE]){
    // DECLARATION DES VARIABLES
    int i;
    // TRAITEMENT DES DONNÉES
    for(i=0;i<TAILLE;i++){
        if(i==0){
            afficher(lesX[i],lesY[i],'O'); // Affichage de la tête
        }else if(lesX[i]>0){
            afficher(lesX[i],lesY[i],'X'); // Affichage du corps
        }
    }
}

void afficher(int x, int y, char c){
    // TRAITEMENT DES DONNÉES
    gotoXY(x,y);
    printf("%c", c);
}

void gotoXY(int x, int y) { 
    printf("\033[%d;%df", y, x);
}

void progresser(int lesX[TAILLE], int lesY[TAILLE]){
    int i;
    if(lesX[TAILLE-1]>0){
        effacer(lesX[TAILLE-1],lesY[TAILLE-1]);
    }
    for(i=0;i<TAILLE;i++){
        lesX[i]=lesX[i]+1;
    }
    dessinerSerpent(lesX,lesY);
}

void effacer(int x, int y){
    gotoXY(x,y);
    printf(" ");
}

int kbhit(){
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere present
	
	int unCaractere=0;
	struct termios oldt, newt;
	int ch;
	int oldf;

	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();

	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}
