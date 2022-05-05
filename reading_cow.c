#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LIRE_OPTION 0
#define YEUX 1
#define LANGUE 2
#define QUEUE 3
#define MESSAGE 4

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


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
            (__)\\       )%s\n\
             %c  ||----w |\n\
                ||     ||\n", yeux, queue, langue);
	free(queue);
}


typedef struct {
	int longueur_lignes;
	int nb_lignes;
} dimensions_boite;

dimensions_boite recup_dimensions_boite(char* texte, int longueur_max_defaut) {
	dimensions_boite dimensions;
	int longueur_mot = 0;
	int max_longueur = longueur_max_defaut;
	int lignes = 1;

	for (int i=0; texte[i] != '\0'; i++) {
		if (texte[i] != ' ') {
			longueur_mot++;
		} else {
			if (longueur_mot > max_longueur) {
				max_longueur = longueur_mot;
				lignes++;
			}
			longueur_mot = 0;
		}
	}

	dimensions.longueur_lignes = MAX(max_longueur, max_longueur);
	dimensions.nb_lignes = lignes;
	return dimensions;
}


void affiche_boite(dimensions_boite dimensions, char* texte) {
	int pos_debut_mot = 0;
	int longueur_mot = 0;
	int longueur_ligne = 0;
	/* int ligne = 0; */

	printf("+");
	for (int i=0; i<dimensions.longueur_lignes + 2; i++) {
		printf("—");
	}
	printf("+\n");

	printf("|");
	for (int i=0; texte[i] != '\0'; i++) {
		if (texte[i] != ' ') {
			longueur_mot++;
		} else {
			if (longueur_ligne + longueur_mot <= dimensions.longueur_lignes) {
				for (int j=0; j<longueur_mot; j++) {
					printf("%c", texte[pos_debut_mot + j]);
				}
				longueur_ligne += longueur_mot + 1;
			} else {
				for (int j=longueur_ligne; j<dimensions.longueur_lignes + 2; j++) {
					printf(" ");
				}
				printf("|\n|");
				for (int j=0; j<longueur_mot; j++) {
					printf("%c", texte[pos_debut_mot + j]);
				}
				longueur_ligne = longueur_mot + 1;
		    }

			printf(" ");
			pos_debut_mot += longueur_mot + 1;
			longueur_mot = 0;
		}
	}
	for (int j=longueur_ligne; j<dimensions.longueur_lignes + 2; j++) {
		printf(" ");
	}
	printf("|\n");

	printf("+");
	for (int i=0; i<dimensions.longueur_lignes + 2; i++) printf("—");
	printf("+\n");
}


int main(int argc, char* argv[]) {
	char message[250];
	char yeux[] = "OO";
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
					etat_suivant = MESSAGE;
					strcpy(message, argv[i]);
					strcat(message, " ");
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

			case MESSAGE:
				strcat(message, argv[i]);
				strcat(message, " ");
				break;
		}
		etat_courant = etat_suivant;
	}

	/* printf("%s\n", message); */
	/* printf("%d\n", largeur_texte_boite); */
	dimensions_boite dimensions = recup_dimensions_boite(message, 30);
	affiche_boite(dimensions, message);
	affiche_vache(yeux, langue, longueur_queue);
	return 0;
}
