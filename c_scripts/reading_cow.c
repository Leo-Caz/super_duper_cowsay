#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */
#include <time.h>

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
#define VITESSE_ANIMATION 7
#define BOX_STYLE 8

// États de l'allignement vertical
#define ALLIGNE_GAUCHE 0
#define TEXTE_CENTRE   1
#define ALLIGNE_DROITE 2

// Styles de boites possibles
#define COMPACT 1

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


void wait(int duree) {  // duree en milisecondes
	int temps_ecoule = 0;
	clock_t temps_precedent = clock();
	while (temps_ecoule < duree) {
		clock_t delta_time = clock() - temps_precedent;
		temps_ecoule = delta_time * 1000 / CLOCKS_PER_SEC;
	}
}


char* genere_queue(int longueur) {
	char* queue = (char *) malloc((sizeof (char)) * longueur);
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
		// Si le dernier caractère est un '\n'
		if (tableau_vache[i][strlen(tableau_vache[i] + 1)] == '\n') {
			pos_y++;
			GOTOXY(pos_x, pos_y);
		}
	}
	printf("\n");

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


int longueur_mot_suivant(char* texte) {
	int longueur = 0;
	for (int i=0; texte[i] != ' ' && texte[i] != '\0'; i++) {
		longueur++;
	}
	return longueur;
}


int nb_lignes_boite(char* texte, int max_longueur_ligne) {
	int nb_lignes = 1;
	int longueur_ligne = 0;
	int mot_suivant;
	while (strlen(texte) > 0) {
		mot_suivant = longueur_mot_suivant(texte);
		if (longueur_ligne + mot_suivant >= max_longueur_ligne && strlen(texte + sizeof(char) * mot_suivant) > 1) {
			nb_lignes++;
			longueur_ligne = mot_suivant;
		} else {
			longueur_ligne += mot_suivant + 1;  // +1 pour l'espace
		}

		texte += sizeof(char) * (mot_suivant + 1);  // +1 pour l'espace
	}
	return nb_lignes;
}


int ligne_plus_longue(char* texte, int max_longueur_ligne) {
	int longueur_max = 0;
	int longueur_ligne = 0;
	int mot_suivant;
	while (strlen(texte) > 0) {
		mot_suivant = longueur_mot_suivant(texte);
		if (longueur_ligne + mot_suivant >= max_longueur_ligne && strlen(texte + sizeof(char) * mot_suivant) > 1) {
			if (longueur_ligne > longueur_max) {
				longueur_max = longueur_ligne;
			}
			longueur_ligne = mot_suivant;
		} else {
			longueur_ligne += mot_suivant + 1;  // +1 pour l'espace
		}

		texte += sizeof(char) * (mot_suivant + 1);  // +1 pour l'espace
	}
	return longueur_max - 1;
}


char* extraire_ligne(char* texte, int max_longueur_ligne) {
	char* rv_ligne = malloc(sizeof(char) * (max_longueur_ligne + 1));
	int longueur_ligne = 0;
	int mot_suivant = longueur_mot_suivant(texte);
	while (longueur_ligne + mot_suivant <= max_longueur_ligne) {
		longueur_ligne += mot_suivant + 1;  // +1 pour l'espace
		mot_suivant = longueur_mot_suivant(texte + (sizeof(char) * longueur_ligne));
	}

	// -1 pour virer le dernier espace de la ligne
	for (int i=0; i<longueur_ligne - 1; i++) {
		rv_ligne[i] = texte[i];

		// finir "proprement" la dernière ligne
		if (strlen(texte + (sizeof(char) * i)) == 1) {
			rv_ligne[i] = '\0';
			break;
		}
	}

	rv_ligne[longueur_ligne - 1] = '\0';
	return rv_ligne;
}


void allignement_vertical(char* ligne, int max_longueur_ligne, int allignement) {
	char* espaces = malloc(sizeof(char) * (max_longueur_ligne + 1));
	int nb_caracteres_ligne = strlen(ligne);
	int nb_espaces = max_longueur_ligne - nb_caracteres_ligne;

	// Why?!?!?!?!?!
	for (int i=0; i<max_longueur_ligne+1; i++) {
		espaces[i] = '\0';
	}

	switch (allignement) {
		case ALLIGNE_GAUCHE:
			for (int i=nb_caracteres_ligne; i<max_longueur_ligne; i++) {
				ligne[i] = ' ';
			}
			break;

		case TEXTE_CENTRE:
			for (int i=0; i<(nb_espaces / 2); i++) {
				espaces[i] = ' ';
			}
			strcat(espaces, ligne);
			for (int i=nb_caracteres_ligne + (nb_espaces / 2); i<max_longueur_ligne; i++) {
				espaces[i] = ' ';
			}
			strcpy(ligne, espaces);
			break;

		case ALLIGNE_DROITE:
			for (int i=0; i<nb_espaces; i++) {
				espaces[i] = ' ';
			}
			strcat(espaces, ligne);
			strcpy(ligne, espaces);
			break;
	}

	free(espaces);
}


char** texte_dans_boite(char* texte, int nb_lignes, int max_longueur_lignes, int allignement) {
	char** rv_texte_formatte = malloc(sizeof(char*) * nb_lignes);
	for (int i=0; i<nb_lignes; i++) {
		rv_texte_formatte[i] = extraire_ligne(texte, max_longueur_lignes);
		texte += sizeof(char) * (strlen(rv_texte_formatte[i]) + 1);  // +1 pour virer l'espace
		allignement_vertical(rv_texte_formatte[i], max_longueur_lignes, allignement);
	}
	return rv_texte_formatte;
}


int calcule_largeur_boite(char* texte, int longueur_modifie, int longueur_defaut) {
	int longueur_texte = strlen(texte);
	int mot_long_max = mot_plus_long(texte);
	int largeur_boite_finale;
	if (longueur_modifie == 1) {
		largeur_boite_finale = MAX(longueur_defaut, mot_long_max);
	} else {
		if (nb_lignes_boite(texte, longueur_defaut) > 1) {
			int longueur_ligne_max = ligne_plus_longue(texte, longueur_defaut);
			largeur_boite_finale = longueur_ligne_max;
		} else {
			largeur_boite_finale = CLAMP(longueur_texte, mot_long_max, longueur_defaut);
		}
	}

	return largeur_boite_finale;
}


typedef struct {
	char* debut_boite;
	char*** bordures;
	char* fin_boite;
} ascii_art_boite;


void genere_boite(ascii_art_boite* boite, int style_boite, int nb_lignes, int longueur_lignes) {
	switch (style_boite) {
		case COMPACT:
			boite->debut_boite = malloc(sizeof(char) * (longueur_lignes + 3));
			boite->debut_boite[0] = '+';
			for (int i=1; i<longueur_lignes+1; i++) {
				boite->debut_boite[i] = '-';
			}
			boite->debut_boite[longueur_lignes+1] = '+';
			boite->debut_boite[longueur_lignes+2] = '\0';

			boite->bordures = malloc(sizeof(char**) * nb_lignes);
			for (int i=0; i<nb_lignes; i++) {
				boite->bordures[i] = malloc(sizeof(char*) * 2);  // bordure à gauche et droite
				for (int j=0; j<2; j++) {
					boite->bordures[i][j] = malloc(sizeof(char) * 1);
					strcpy(boite->bordures[i][j], "|");
				}
			}

			boite->fin_boite = malloc(sizeof(char) * (longueur_lignes + 3));
			boite->fin_boite[0] = '+';
			for (int i=1; i<longueur_lignes+1; i++) {
				boite->fin_boite[i] = '-';
			}
			boite->fin_boite[longueur_lignes+1] = '+';
			boite->fin_boite[longueur_lignes+2] = '\0';
			break;
	}
}


void affiche_boite(char** texte_par_lignes, int nb_lignes, int style_boite) {
	int longueur_lignes = strlen(texte_par_lignes[0]);
	ascii_art_boite boite;
	genere_boite(&boite, style_boite, nb_lignes, longueur_lignes);

	printf("%s\n", boite.debut_boite);

	for (int i=0; i<nb_lignes; i++) {
		printf("%s%s%s\n", boite.bordures[i][0], texte_par_lignes[i], boite.bordures[i][1]);
	}

	printf("%s\n", boite.fin_boite);

	free(boite.debut_boite);
	free(boite.fin_boite);
	for (int i=0; i<nb_lignes; i++) {
		for (int j=0; j<2; j++) {
			free(boite.bordures[i][j]);
		}
		free(boite.bordures[i]);
	}
	free(boite.bordures);
}


typedef struct {
	char* message;
	char yeux[3];
	char langue[3];
	int langue_modifie;
	int longueur_queue;
	int largeur_boite;
	int largeur_boite_modifie;
	int allignement;
	int vitesse_animation;
	int box_style;
} animation_parameters;


void parse_arguments_ligne_commande(animation_parameters* options_anim, int argc, char** argv) {
	// On définit la taille de la chaine de caractère du message pour
	// être sûr de pas faire un buffer-overflow
	int longueur_message_max = 0;
	for (int i=1; i<argc; i++) {
		longueur_message_max += strlen(argv[i]) + 1;  // +1 pour l'espace
	}

	options_anim->message = malloc(sizeof(char) * longueur_message_max);
	for (int i=0; i<longueur_message_max; i++) {
		options_anim->message[i] = '\0';
	}

	// Valeurs par défaut
	strcpy(options_anim->yeux, "00\0");
	strcpy(options_anim->langue, "  \0");
	options_anim->langue_modifie = 0;
	options_anim->longueur_queue = 3;
	options_anim->largeur_boite = 40;
	options_anim->largeur_boite_modifie = 0;
	options_anim->allignement = ALLIGNE_GAUCHE;
	options_anim->vitesse_animation = 50;
	options_anim->box_style = COMPACT;

	int etat_courant = LIRE_OPTION;
	int etat_suivant;

	// Parser les arguments de la ligne de commande
	for (int i=1; i < argc; i++) {
		switch (etat_courant) {
			case LIRE_OPTION:
				if (strcmp(argv[i], "-e") == 0 ||
						strcmp(argv[i], "--eyes") == 0) {
					etat_suivant = YEUX;
				} else if ((strcmp(argv[i], "-T") == 0) ||
						(strcmp(argv[i], "--toungue") == 0)) {
					etat_suivant = LANGUE;
				} else if ((strcmp(argv[i], "-t") == 0) || 
						(strcmp(argv[i], "--tail") == 0)) {
					etat_suivant = QUEUE;
				} else if ((strcmp(argv[i], "-w") == 0) ||
						(strcmp(argv[i], "--width") == 0)) {
					etat_suivant = BOX_WIDTH;
				} else if (strcmp(argv[i], "-a") == 0 ||
						(strcmp(argv[i], "--allign") == 0)) {
					etat_suivant = TYPE_ALLIGNEMENT;
				} else if (strcmp(argv[i], "-v") == 0 ||
						(strcmp(argv[i], "--vitesse") == 0)) {
					etat_suivant = VITESSE_ANIMATION;
				} else if (strcmp(argv[i], "-b") == 0 ||
						(strcmp(argv[i], "--box-style") == 0)) {
					etat_suivant = BOX_STYLE;
				} else {  // aucune option trouvé => message à afficher
					etat_suivant = MESSAGE;
					strcpy(options_anim->message, argv[i]);
					strcat(options_anim->message, " ");
				}
				break;

			case YEUX:
				strcpy(options_anim->yeux, argv[i]);
				etat_suivant = LIRE_OPTION;
				break;

			case LANGUE:
				strcpy(options_anim->langue, argv[i]);
				options_anim->langue_modifie = 1;
				etat_suivant = LIRE_OPTION;
				break;

			case QUEUE:
				sscanf(argv[i], "%i", &options_anim->longueur_queue);
				etat_suivant = LIRE_OPTION;
				break;

			case BOX_WIDTH:
				sscanf(argv[i], "%i", &options_anim->largeur_boite);
				options_anim->largeur_boite_modifie = 1;
				etat_suivant = LIRE_OPTION;
				break;

			case TYPE_ALLIGNEMENT:
				if (strcmp(argv[i], "gauche") == 0) {
					options_anim->allignement = ALLIGNE_GAUCHE;
				} else if (strcmp(argv[i], "droite") == 0) {
					options_anim->allignement = ALLIGNE_DROITE;
				} else if (strcmp(argv[i], "centre") == 0) {
					options_anim->allignement = TEXTE_CENTRE;
				} else {
					printf("Allignement vertical non valide.\n");
					exit(1);
				}
				etat_suivant = LIRE_OPTION;
				break;

			case BOX_STYLE:
				if (strcmp(argv[i], "compact") == 0) {
					options_anim->box_style = COMPACT;
					etat_suivant = LIRE_OPTION;
				} else {
					printf("Style de boite non valide.\n");
					exit(1);
				}
				break;

			case VITESSE_ANIMATION:
				sscanf(argv[i], "%i", &options_anim->vitesse_animation);
				etat_suivant = LIRE_OPTION;
				break;

			// On ne peut pas mettre d'autres arguments après avoir commencé le message
			case MESSAGE:
				strcat(options_anim->message, argv[i]);
				strcat(options_anim->message, " ");
				break;
		}
		etat_courant = etat_suivant;
	}
}


int main(int argc, char* argv[]) {
	animation_parameters options_anim;
	parse_arguments_ligne_commande(&options_anim, argc, argv);
	
	int total_lignes_message = calcule_largeur_boite(options_anim.message, options_anim.largeur_boite_modifie, options_anim.largeur_boite);
	int pos_y_vache_finale = nb_lignes_boite(options_anim.message, total_lignes_message) + 3;
	if (options_anim.vitesse_animation > 0) {
		char* message_anime = malloc(sizeof(char) * strlen(options_anim.message));
		for (int i=0; i<strlen(options_anim.message); i++) {
			UPDATE;
			message_anime[i] = options_anim.message[i];

			int longueur_lignes = calcule_largeur_boite(message_anime, options_anim.largeur_boite_modifie, options_anim.largeur_boite);
			int nb_lignes = nb_lignes_boite(message_anime, longueur_lignes);

			char** texte_formate = texte_dans_boite(message_anime, nb_lignes, longueur_lignes, options_anim.allignement);
			affiche_boite(texte_formate, nb_lignes, options_anim.box_style);

			if (options_anim.langue_modifie == 0) {
				options_anim.langue[0] = options_anim.message[i];
			}
			affiche_vache(options_anim.yeux, options_anim.langue, options_anim.longueur_queue, 9, pos_y_vache_finale);

			for (int j=0; j<nb_lignes; j++) {
				free(texte_formate[j]);
			}
			free(texte_formate);

			wait(options_anim.vitesse_animation);
		}
	} else {  // vitesse <= 0: affiche direct la vache
		UPDATE;
		int longueur_lignes = calcule_largeur_boite(options_anim.message, options_anim.largeur_boite_modifie, options_anim.largeur_boite);
		int nb_lignes = nb_lignes_boite(options_anim.message, longueur_lignes);
		char** texte_formate = texte_dans_boite(options_anim.message, nb_lignes, longueur_lignes, options_anim.allignement);
		affiche_boite(texte_formate, nb_lignes, options_anim.box_style);
		affiche_vache(options_anim.yeux, options_anim.langue, options_anim.longueur_queue, 9, pos_y_vache_finale);
		for (int j=0; j<nb_lignes; j++) {
			free(texte_formate[j]);
		}
		free(texte_formate);
	}

	free(options_anim.message);
	return 0;
}
