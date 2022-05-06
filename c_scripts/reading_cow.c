#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define LIRE_OPTION 0
#define YEUX 1
#define LANGUE 2
#define QUEUE 3
#define MESSAGE 4

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
 

void affiche_vache(char* yeux, char langue, int longueur_queue) {
	char* queue = genere_queue(longueur_queue);
    printf("        \\   ^__^\n\
         \\  (%s)\\_______\n\
            (__)\\       )%s\n\
             %c  ||----w |\n\
                ||     ||\n", yeux, queue, langue);
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
	int longueur_mot = 0;
	
	for (int i=0; texte[i] != '\0'; i++) {
		if (texte[i] == ' ') {
			if (longueur_ligne + longueur_mot >= max_longueur_ligne) {
				nb_lignes++;
				longueur_ligne = longueur_mot;
			} else {
				longueur_ligne += longueur_mot + 1;  // +1 pour l'espace
			}
			longueur_mot = 0;
		} else {
			longueur_mot++;
		}
	}

	return nb_lignes;
}


char* extraire_ligne(char* texte, int max_longueur_ligne) {
	char* rv_ligne = malloc(sizeof(char) * max_longueur_ligne);
	char* mot = malloc(sizeof(char) * max_longueur_ligne);

	sscanf(texte, "%s", mot);  // Récupérer le premier mot du texte
	strcat(rv_ligne, mot);     // len du mot <= max_longueur_ligne donc ça va
	texte += (sizeof(char) * (strlen(mot) + 1));  // On regarde le mot suivant
	sscanf(texte, "%s", mot);  // Récupérer le premier mot du texte

	while (strlen(rv_ligne) + strlen(mot) < max_longueur_ligne) {
		sscanf(texte, "%s", mot);
		strcat(rv_ligne, " ");
		strcat(rv_ligne, mot);
		texte += (sizeof(char) * (strlen(mot) + 1));  // On regarde le mot suivant
	}

	free(mot);
	return rv_ligne;
}


char** texte_formate(char* texte, int max_longueur_ligne, int nb_lignes, int allignement) {
	// TODO: rajouter des options sur l'alignement vertical
	char** rv_texte = malloc(sizeof(char*) * nb_lignes);
	char* ligne = malloc(sizeof(char) * max_longueur_ligne);
	char* espaces = malloc(sizeof(char) * max_longueur_ligne);
	/* int nb_espaces; */

	for (int i=0; i<nb_lignes; i++) {
		ligne = extraire_ligne(texte, max_longueur_ligne);
		int nb_espaces = max_longueur_ligne - strlen(ligne);
		
		switch (allignement) {
			case ALLIGNE_GAUCHE:
				for (int j=0; j<nb_espaces; j++) espaces[j] = ' ';
				rv_texte[i] = strcat(ligne, espaces);
				break;

			case TEXTE_CENTRE:
				for (int j=0; j<(nb_espaces / 2); j++) espaces[j] = ' ';
				rv_texte[i] = strcat(espaces, ligne);
				strcpy(espaces, "");
				for (int j=0; j<((nb_espaces / 2) + (nb_espaces % 2)); j++) espaces[j] = ' ';
				strcat(rv_texte[i], espaces);

			case ALLIGNE_DROITE:
				for (int j=0; j<nb_espaces; j++) espaces[j] = ' ';
				rv_texte[i] = strcat(espaces, ligne);
				break;
		}

		strcpy(espaces, "");
		texte += (sizeof(char) * (strlen(rv_texte[i]) + 1));
	}

	free(ligne);
	free(espaces);
	return rv_texte;
}


void affiche_boite(char* texte, int largeur_par_defaut, int allignement) {
	int longueur_max_mot = mot_plus_long(texte);
	int longueur_lignes = MAX(largeur_par_defaut, longueur_max_mot);
	int nb_lignes = nb_lignes_boite(texte, longueur_lignes);
	char** texte_par_lignes = texte_formate(texte, longueur_lignes, nb_lignes, allignement);

	for (int i=0; i<nb_lignes; i++) {
		printf("%s\n", texte_par_lignes[i]);
	}

	for (int i=0; i<nb_lignes; i++) {
		free(texte_par_lignes[i]);
	}
	free(texte_par_lignes);
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

	/* printf("%d\n", nb_lignes_boite(message, 8)); */
	/* printf("%s\n", message); */
	/* printf("%lu\n", strlen(message)); */
	/* char* suite_message = message + ((sizeof(char)) * 5); */
	/* printf("%s\n", suite_message); */
	/* printf("%lu\n", strlen(suite_message)); */

	/* /1* char* test1 = "u"; *1/ */
	/* char* test1 = malloc((sizeof(char)) * 4); */
	/* char* test2 = malloc((sizeof(char)) * 4); */
	/* char* test3 = "iu"; */
	/* strcat(test1, test2); */
	/* strcat(test1, test3); */
	/* printf("%s\n", test1); */

	/* int test_lignes = nb_lignes_boite(message, 8); */
	/* printf("%d\n", test_lignes); */
	/* char** tableau_texte = texte_formate(message, 8, test_lignes); */
	/* for (int i=0; i<test_lignes; i++) { */
	/* 	printf("%s\n", tableau_texte[i]); */
	/* } */

	affiche_boite(message, 8, ALLIGNE_DROITE);

	affiche_vache(yeux, langue, longueur_queue);

	/* for (int i=0; i<test_lignes; i++) { */
	/* 	free(tableau_texte[i]); */
	/* } */
	/* free(tableau_texte); */

	return 0;
}
