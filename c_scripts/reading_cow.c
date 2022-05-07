#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// macros utiles (KEEP IT SIMPLE STUPID!!!!)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define CLAMP(x, min, max) MAX(min, MIN(x, max))
#define UPDATE printf("\033[H\033[J")
#define GOTOXY(x, y) printf("\033[%d;%dH", y, x)

// États de l'automate qui parse les arguments de la ligne de commande
#define LIRE_OPTION 0
#define YEUX 1
#define LANGUE 2
#define QUEUE 3
#define MESSAGE 4
#define BOX_WIDTH 5
#define TYPE_ALLIGNEMENT 6

// États de l'allignement vertical
#define ALLIGNE_GAUCHE 0
#define TEXTE_CENTRE   1
#define ALLIGNE_DROITE 2


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


void affiche_vache(char* yeux, char* langue, int longueur_queue, int pos_x, int pos_y) {
	char* queue = genere_queue(longueur_queue);
	char* tableau_vache[11] = {
		"\\   ^__^\n",
        " \\  (",
		yeux,
		")\\_______\n",
        "    (__)\\       )",
		queue,
		"\n",
        "     ",
		langue,
		" ||----w |\n",
        "        ||     ||\n"
	};

	GOTOXY(pos_x, pos_y);
	for (int i=0; i<11; i++) {
		printf("%s", tableau_vache[i]);
		if (tableau_vache[i][strlen(tableau_vache[i] + 1)] == '\n') {
			pos_y++;
			GOTOXY(pos_x, pos_y);
		}
	}

	free(queue);
}


int mot_plus_long(char* texte) {
	int max_longueur = 0;
	int longueur_mot = 0;

	for (int i=0; texte[i] != '\0'; i++) {
		if (texte[i] == ' ') {
			if (longueur_mot > max_longueur) {
				max_longueur = longueur_mot;
			}
			longueur_mot = 0;
		} else {
			longueur_mot++;
		}
	}
	return max_longueur;
}


int nb_lignes_boite(char* texte, int max_longueur_ligne) {
	int nb_lignes = 1;
	int longueur_ligne = 0;
	char* mot = malloc(sizeof(char) * max_longueur_ligne);

	/* printf("==>>————<>————-——-—--· ·\n"); */
	while (strlen(mot) <= strlen(texte)) {
		sscanf(texte, "%s", mot);
		if (longueur_ligne + strlen(mot) > max_longueur_ligne) {
			nb_lignes++;
			longueur_ligne = strlen(mot);
		} else {
			longueur_ligne += strlen(mot) + 1;
		}
		texte += sizeof(char) * (strlen(mot) + 1);  // +1 pour l'espace
	}
	/* printf("==>>————<>————-——-—--· ·\n"); */

	free(mot);
	return nb_lignes;
}


char* extraire_ligne(char* texte, int max_longueur_ligne) {
	char* rv_ligne = malloc(sizeof(char) * max_longueur_ligne);
	char* mot = malloc(sizeof(char) * max_longueur_ligne);

	sscanf(texte, "%s", mot);  // Récupérer le premier mot du texte
	strcat(rv_ligne, mot);     // len du mot <= max_longueur_ligne donc ça va
	texte += (sizeof(char) * (strlen(mot) + 1));  // On regarde le mot suivant
	sscanf(texte, "%s", mot);  // Récupérer le premier mot du texte

	// Pas dépasser la taille max de ligne et s'arrêter à la fin du texte
	while (strlen(rv_ligne) + strlen(mot) < max_longueur_ligne &&
			strlen(mot) <= strlen(texte)) {
		strcat(rv_ligne, " ");
		strcat(rv_ligne, mot);
		texte += (sizeof(char) * (strlen(mot) + 1));  // On regarde le mot suivant
		sscanf(texte, "%s", mot);
	}

	// Free ce pointeur créé avec malloc = crash, je sais pas pourquoi
	/* free(mot); */
	return rv_ligne;
}


char** texte_formate(char* texte, int max_longueur_ligne, int nb_lignes, int allignement) {
	char** rv_texte = malloc(sizeof(char*) * nb_lignes);
	char* espaces = malloc(sizeof(char) * max_longueur_ligne);
	char* tmp = malloc(sizeof(char) * max_longueur_ligne);

	for (int i=0; i<nb_lignes; i++) {
		char* ligne = extraire_ligne(texte, max_longueur_ligne);
		int nb_caracteres_ligne = strlen(ligne);
		int nb_espaces = max_longueur_ligne - nb_caracteres_ligne;

		for (int j=0; j<max_longueur_ligne; j++) {
			espaces[j] = '\0';
			tmp[j] = '\0';
		}
		
		switch (allignement) {
			case ALLIGNE_GAUCHE:
				for (int j=0; j<nb_espaces; j++) {
					espaces[j] = ' ';
				}
				rv_texte[i] = strcat(ligne, espaces);
				break;

			case TEXTE_CENTRE:
				for (int j=0; j<(nb_espaces / 2); j++) {
					espaces[j] = ' ';
				}
				// Échanger les pointeurs pour que strcat agisse sur le bon pointeur
				// oui je sais c'est moche mais je savais pas faire autrement.
				strcpy(tmp, espaces);
				strcpy(espaces, ligne);
				strcpy(ligne, tmp);
				rv_texte[i] = strcat(ligne, espaces);

				for (int j=0; j<max_longueur_ligne; j++) {
					espaces[j] = '\0';
				}

				for (int j=0; j<((nb_espaces / 2) + (nb_espaces % 2)); j++) {
					espaces[j] = ' ';
				}
				strcat(rv_texte[i], espaces);
				break;

			case ALLIGNE_DROITE:
				for (int j=0; j<nb_espaces; j++) {
					espaces[j] = ' ';
				}
				// Échanger les pointeurs pour que strcat agisse sur le bon pointeur
				// Oui c'est toujours aussi moche.
				strcpy(tmp, espaces);
				strcpy(espaces, ligne);
				strcpy(ligne, tmp);
				rv_texte[i] = strcat(ligne, espaces);
				break;
		}

		texte += (sizeof(char) * (nb_caracteres_ligne + 1));
	}

	// Free ce pointeur créé avec malloc = crash, je sais pas pourquoi
	/* free(tmp); */
	free(espaces);
	return rv_texte;
}


int largeur_boite(char* texte, int longueur_modifie, int longueur_defaut) {
	int longueur_texte = strlen(texte);
	int mot_long_max = mot_plus_long(texte);
	int largeur_boite_finale;
	if (longueur_modifie == 1) {
		largeur_boite_finale = MAX(longueur_defaut, mot_long_max);
	} else {
		largeur_boite_finale = CLAMP(longueur_texte, mot_long_max, longueur_defaut);
	}

	return largeur_boite_finale;
}


void affiche_boite(char* texte, int largeur_par_defaut, int allignement) {
	int longueur_max_mot = mot_plus_long(texte);
	int longueur_lignes = MAX(largeur_par_defaut, longueur_max_mot);
	int nb_lignes = nb_lignes_boite(texte, longueur_lignes);
	char** texte_par_lignes = texte_formate(texte, longueur_lignes, nb_lignes, allignement);

	printf("+");
	for (int i=0; i<largeur_par_defaut; i++) printf("-");
	printf("+\n");

	for (int i=0; i<nb_lignes; i++) {
		printf("¦%s¦\n", texte_par_lignes[i]);
		free(texte_par_lignes[i]);
		/* printf("==>>————<>————-——-—--· ·\n"); */
	}

	printf("+");
	for (int i=0; i<largeur_par_defaut; i++) printf("-");
	printf("+\n");

	free(texte_par_lignes);
}


int main(int argc, char* argv[]) {
	char message[250];
	char yeux[3] = "OO\0";
	char langue[3] = "  \0";
	int longueur_queue = 3;
	int largeur_boite_defaut = 40;
	int largeur_boite_modifie = 0;
	int allignement = ALLIGNE_GAUCHE;

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
				} else if ((strcmp(argv[i], "-w") == 0) ||
						(strcmp(argv[i], "--width") == 0)) {
							etat_suivant = BOX_WIDTH;
				} else if (strcmp(argv[i], "-a") == 0 ||
						(strcmp(argv[i], "--allign") == 0)) {
					etat_suivant = TYPE_ALLIGNEMENT;
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
				/* langue = *argv[i]; */
				strcpy(langue, argv[i]);
				etat_suivant = LIRE_OPTION;
				break;

			case QUEUE:
				sscanf(argv[i], "%i", &longueur_queue);
				etat_suivant = LIRE_OPTION;
				break;

			case BOX_WIDTH:
				sscanf(argv[i], "%i", &largeur_boite_defaut);
				largeur_boite_modifie = 1;
				etat_suivant = LIRE_OPTION;
				break;

			case TYPE_ALLIGNEMENT:
				if (strcmp(argv[i], "gauche") == 0) {
					allignement = ALLIGNE_GAUCHE;
				} else if (strcmp(argv[i], "droite") == 0) {
					allignement = ALLIGNE_DROITE;
				} else if (strcmp(argv[i], "centre") == 0) {
					allignement = TEXTE_CENTRE;
				} else {
					printf("Allignement vertical non valide.\n");
					return 1;
				}
				etat_suivant = LIRE_OPTION;
				break;

			// On ne peut pas mettre d'autres arguments après avoir commencé le message
			case MESSAGE:
				strcat(message, argv[i]);
				strcat(message, " ");
				break;
		}
		etat_courant = etat_suivant;
	}

	int largeur_boite_finale = largeur_boite(message, largeur_boite_modifie, largeur_boite_defaut);
	int pos_vache = nb_lignes_boite(message, largeur_boite_finale) + 3;

	int largeur_boite_anime;
	message[strlen(message) - 1] = '\0';
	char* message_anime = malloc(sizeof(char) * strlen(message));
	for (int i=0; message[i] != '\0'; i++) {
		UPDATE;
		sleep(1);
		message_anime[i] = message[i];

		largeur_boite_anime = largeur_boite(message_anime, largeur_boite_modifie, largeur_boite_defaut);
		affiche_boite(message_anime, largeur_boite_anime, allignement);
		affiche_vache(yeux, langue, longueur_queue, 0, pos_vache);
	}

	return 0;
}
