// valgrind : vérifier qu'on a pas fait de fuite de mémoire
// gdb : débuggeur cool
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LIRE_OPTION 0
#define YEUX 1
#define LANGUE 2
#define QUEUE 3


int est_pair(int n) {  // comment ça % existe ?
	if (n == 0) {
		return 0;
	} else if (n == 1) {
		return 1;
	} else if (n < 0) {
		return (est_pair(-n));
	} else {
		return (est_pair(n - 2));
	}
}


void update() {
	printf("\033[H\033[J");
}


void gotoxy(int x, int y) {
	printf("\033[%d;%dH", y, x);
}


char* genere_queue(int longueur) {
	char* queue = NULL;
	queue = (char *) malloc((sizeof (char)) * longueur);
	for (int i=0; i < longueur; i++) {
		if (est_pair(i)) {
			queue[i] = '/';
		} else {
			queue[i] = '\\';
		}
	}
	return queue;
}
 

void affiche_vache(char* yeux, char langue, int longueur_queue) {
	char* queue = genere_queue(longueur_queue);
    printf("        \\   ^__^\n\
     \\  (%s)\\_______\n\
        (__)\\        )%s\n\
         %c  ||----w |\n\
        ||     ||\n", yeux, queue, langue);
	free(queue);
}


void sliding_cow(char* yeux, char langue, int longueur_queue) {
	char* queue = genere_queue(longueur_queue);
	int position_depart = 5;
	for (int i = position_depart; i >= 0; i--) {
		update();
		gotoxy(i, 1);
		printf("    \\   ^__^\n");

		gotoxy(i, 2);
		printf("     \\  (%s)\\_______\n", yeux);

		gotoxy(i, 3);
		printf("        (__)\\        )%s\n", queue);

		gotoxy(i, 4);
		printf("          %c  ||----w |\n", langue);

		gotoxy(i, 5);
		printf("             ||     ||\n");

		gotoxy(i, 6);
		printf("          \\-------------/\n");

		gotoxy(i, 7);
		printf("             O      O\n");
		sleep(1);
    }
    free(queue);
}


int main(int argc, char* argv[]) {
	char message[250];
	char yeux[2] = "OO";
	char langue = ' ';
	int longueur_queue = 3;


	int etat_courant = LIRE_OPTION;
	int etat_suivant;

	// Parser les arguments de la ligne de commande
	for (int i=1; i < argc; i++) {
		switch (etat_courant) {
			case LIRE_OPTION:
				if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--eyes") == 0) {
					etat_suivant = YEUX;
				} else if (strcmp(argv[i], "-T") == 0) {
					etat_suivant = LANGUE;
				} else if (strcmp(argv[i], "--tail") == 0) {
					etat_suivant = QUEUE;
				} else {  // aucune option trouvé => message à afficher
					etat_suivant = LIRE_OPTION;
					strcpy(message, argv[i]);
				}
				break;

			case YEUX:
				strcpy(yeux, argv[i]);
				etat_suivant = LIRE_OPTION;
				break;

			case LANGUE:
				langue = *argv[i];
				etat_suivant = LIRE_OPTION;
				break;

			case QUEUE:
				sscanf(argv[i], "%i", &longueur_queue);
				etat_suivant = LIRE_OPTION;
				break;
		}
		etat_courant = etat_suivant;
	}

	sliding_cow(yeux, langue, longueur_queue);
}
