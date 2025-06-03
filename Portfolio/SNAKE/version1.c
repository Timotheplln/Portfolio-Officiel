/**
* @file version1.c
* @brief Programme affichant un snake qui vas vers la droite
* @author Timothé pellen
* @version 1.0
* @date 25/10/2024
*
* programme qui permet de jouer au jeu 'snake' en langage C
* le snake doit se déplacer vers la droite tant que l’utilisateur n’a pas appuyé sur la touche ‘a’
*/

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define CORP_S 'X' /**corps du snake*/
#define TETE_S 'O' /**tete du snake*/
#define TAILLE 10 /**taille du snake*/
#define COORDONNE 20

void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int lesX[], int lesY[]);
void progresserdroite(int lesX[], int lesY[]);
void progressergauche(int lesX[], int lesY[]);
void progresserbas(int lesX[], int lesY[]);
void progresserhaut(int lesX[], int lesY[]);
void gotoXY(int x, int y);
int kbhit();

int main() {
	int lesX[TAILLE];
	int lesY[TAILLE];
	int indice;
	int x = COORDONNE*2,y = COORDONNE;
	/*
	printf("quel x?");
	scanf("%d", &x);
	if (x>40 || x<=0){
		while (x>40 || x<=0)
		{
			printf("erreur x n'est pas valide\n");
			printf("quel x?");
			scanf("%d", &x);
		}
	}
	printf("quel y?");
	scanf("%d", &y);
	if (y>40 || y<=0){
		while (y>40 || y<=0)
		{
			printf("erreur y n'est pas valide\n");
			printf("quel y?");
			scanf("%d", &y);
		}
	} */
	system("clear");
	for (indice=0 ; indice<TAILLE ; indice++){
		lesX[indice]=x-indice;
		lesX[indice+1]=-1;
		lesY[indice]=y;
	}
	dessinerSerpent(lesX,lesY);
	lesX[0]--;
	while (1)
	{
		if (kbhit())
		{
			char touche = getchar();
			switch (touche)// Lire la touche pressée
			{
			case "z":
				progresserhaut(lesX,lesY);
				break;
			case "s":
				progresserbas(lesX,lesY);
				break;
			case "d":
				progresserdroite(lesX,lesY);
				break;
			case "q":
				progressergauche(lesX,lesY);
				break;
			case "a":
				break;
			
			default:
				progresserdroite(lesX,lesY);
				break;
			}
			if (touche == 'a') { // Si la touche 'a' est pressée
				break; // Sortir de la boucle de jeu
			}
		} else
		{
			progresserdroite(lesX,lesY);
		}
	}
}

void dessinerSerpent(int lesX[], int lesY[]){ /** affiche le snake a partir des cohordonnées stockée dans lesX et celles dans lesY */
	int i=0;
	afficher(lesX[i],lesY[i], TETE_S);
	i++;
	while (i<TAILLE)
	{
		afficher(lesX[i],lesY[i], CORP_S);
		i++;
	}
	afficher(lesX[0],lesY[0], TETE_S);
	fflush(stdout);
}

void afficher(int x, int y, char c){ /** affiche le caractère c en cohordonnée x y */
	gotoXY(x,y);
	printf("%s", &c);
}

void effacer(int x, int y){ /** efface le caractère positionner en x y */
	afficher(x,y,' ');
}

void gotoXY(int x, int y) { 
    printf("\033[%d;%df", y, x);
}

void progresserdroite(int lesX[], int lesY[]){ /**fait avancer le snake vers la droite */
	int i=TAILLE;
	int j=0;
	lesX[0]++;
	while (j<TAILLE)
	{
		effacer(lesX[i],lesY[i]);
		j++;
	}
	
	
	while (i>0)
	{
		lesX[i]=lesX[i-1];
		lesY[i]=lesY[i-1];
		i--;
		
		dessinerSerpent(lesX,lesY);
		usleep(10000);
	}
	effacer(lesX[TAILLE],lesY[TAILLE]);
}

void progressergauche(int lesX[], int lesY[]){ /**fait avancer le snake vers la droite */
	int i=TAILLE;
	int j=0;
	lesX[0]--;
	while (j<TAILLE)
	{
		effacer(lesX[i],lesY[i]);
		j++;
	}
	
	
	while (i>0)
	{
		lesX[i]=lesX[i-1];
		lesY[i]=lesY[i-1];
		i--;
		
		dessinerSerpent(lesX,lesY);
		usleep(10000);
	}
	effacer(lesX[TAILLE],lesY[TAILLE]);
}

void progresserhaut(int lesX[], int lesY[]){ /**fait avancer le snake vers la droite */
	int i=TAILLE;
	int j=0;
	lesY[0]--;
	while (j<TAILLE)
	{
		effacer(lesX[i],lesY[i]);
		j++;
	}
	
	
	while (i>0)
	{
		lesX[i]=lesX[i-1];
		lesY[i]=lesY[i-1];
		i--;
		
		dessinerSerpent(lesX,lesY);
		usleep(10000);
	}
	effacer(lesX[TAILLE],lesY[TAILLE]);
}

void progresserbas(int lesX[], int lesY[]){ /**fait avancer le snake vers la droite */
	int i=TAILLE;
	int j=0;
	lesY[0]++;
	while (j<TAILLE)
	{
		effacer(lesX[i],lesY[i]);
		j++;
	}
	
	
	while (i>0)
	{
		lesX[i]=lesX[i-1];
		lesY[i]=lesY[i-1];
		i--;
		
		dessinerSerpent(lesX,lesY);
		usleep(10000);
	}
	effacer(lesX[TAILLE],lesY[TAILLE]);
}

/*
void avance(int lesX[], int lesY[]){ *permer de faire avancer le corps du snake derière sa tête 
	int i=TAILLE;
	while (i>0)
	{
		effacer(lesX[i],lesY[i]);
		lesX[i]=lesX[i-1];
		lesY[i]=lesY[i-1];
		i--;
	}
}*/

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
