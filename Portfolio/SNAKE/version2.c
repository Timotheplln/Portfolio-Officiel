/**
* @file version1.c
* @brief Programme affichant un snake qui vas vers la droite
* @author Timothé pellen
* @version 2.0
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
#include <string.h>

#define CORP_S 'X' /**corps du snake*/
#define TETE_S 'O' /**tete du snake*/
#define TAILLE 10 /**taille du snake*/
#define COORDONNE 20
#define MIN 0
#define MAX 40
#define HAUT 'z'
#define BAS 's'
#define DROITE 'd'
#define GAUCHE 'q'

void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int lesX[], int lesY[]);
void progresser(int lesX[], int lesY[],char direction);
void gotoXY(int x, int y);
void disableEcho();
void enableEcho();
int kbhit();

/**
 * @brief cette fonction principale permet de donner au serpent ses coordonnées d'origine (20,20) puis de l'y
 * afficher dans le terminal enfin il initie le mouvement du snake vers la droite et il continue en verifiant
 * si les input de l'utilisateur pour changer en fonction des touche z, q, s, d et a les actions du serpent.
 */
int main() {
	int lesX[TAILLE];
	int lesY[TAILLE];
	int indice;
	int x = COORDONNE,y = COORDONNE;
	/*
	printf("quel x?");
	scanf("%d", &x);
	if (x>MAX || x<=MIN){
		while (x>MAX || x<=MIN)
		{
			printf("erreur x n'est pas valide\n");
			printf("quel x?");
			scanf("%d", &x);
		}
	}
	printf("quel y?");
	scanf("%d", &y);
	if (y>MAX || y<=MIN){
		while (y>MAX || y<=MIN)
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
	char avance=DROITE;
	disableEcho();
	while (1)
	{
		if (kbhit())
		{
			char touche = getchar();
			if (touche == 'z' && avance!='s')
			{
				avance=touche;
			} else if (touche == 's' && avance!='z')
			{
				avance=touche;
			} else if (touche == 'd' && avance!='q')
			{
				avance=touche;
			} else if (touche == 'q' && avance!='d')
			{
				avance=touche;
			} else if (touche == 'a')
			{
				enableEcho();
				printf("fini");
				break;
			}
		}
		/*
		if (lesX[0]<=MIN || lesY[0]<=MIN || lesX[0]>=MAX*2 || lesY[0]>=MAX)
		{
			enableEcho();
			printf("perdu");
			break;
		}*/
		progresser(lesX,lesY,avance);
	}
}

/**
 *  @brief affiche le snake a partir des cohordonnées stockée dans lesX et celles dans lesY
*/
void dessinerSerpent(int lesX[], int lesY[]){ 
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

/** 
 * @brief affiche le caractère c en cohordonnée x y
*/
void afficher(int x, int y, char c){ 
	gotoXY(x,y);
	if (x>=MIN && y>=MIN/* && x<=MAX*2 && y<=MAX*/)
	{
		printf("%s", &c);
	}
	
}

/** 
 * @brief efface le caractère positionner en x y 
*/
void effacer(int x, int y){ 
	afficher(x,y,' ');
}

/** 
 * @brief positionne le curseur au coordonnées x y
*/
void gotoXY(int x, int y) { 
    printf("\033[%d;%df", y, x);
}

/** 
 * @brief fait avancer le snake vers la direction -> direction en verifiant celle-ci pour déplacer le snake de la bonne façon
*/
void progresser(int lesX[], int lesY[], char direction){
	int i=TAILLE;
	int j=0;
	switch (direction)
	{
	case HAUT:
		lesY[0]--;
		break;
	case BAS:
		lesY[0]++;
		break;
	case DROITE:
		lesX[0]++;
		break;
	case GAUCHE:
		lesX[0]--;
		break;
	default:
		break;
	}
	
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
		fflush(stdout);
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

/** 
 * @brief vérifier s’il y a eu frappe d’un caractère au clavier
*/
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

void disableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void enableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}
