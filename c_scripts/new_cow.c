// valgrind : vérifier qu'on a pas fait de fuite de mémoire
// gdb : débuggeur cool
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */
#include <time.h>
#include <sys/ioctl.h>

// macros utiles (KEEP IT SIMPLE STUPID!!!!)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define CLAMP(x, min, max) MAX(min, MIN(x, max))
#define UPDATE printf("\033[H\033[J")
#define GOTOXY(x, y) printf("\033[%d;%dH", y, x)

// états de l'automate qui parse les arguments
#define LIRE_OPTION 0
#define YEUX 1
#define LANGUE 2
#define QUEUE 3
#define ANIMATION 4

// animations possibles
#define COINCOIN 0
#define RACING_COW 1
#define SKATEBOARD 2
#define TUX 3
#define EATING_COW 4

/*
 * Petit disclaimer: les annimations qui dépassent l'écran
 * à gauche ne marchent pas dans le term de neovim et
 * potentiellement d'autres. Ça marche dans xfce4-term.
 *
 */


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
 

void affiche_vache(char** tableau_vache, int taille_tableau_vache, int pos_x, int pos_y, int nb_colonnes) {
	GOTOXY(MAX(pos_x, 0), pos_y);
	int u = 0;  // u et uu c'est pour couper la vache au bon endroit et pas dépasser l'écran
	for (int i=0; i<taille_tableau_vache; i++) {
		int uu = u;
		for (int j=0; j<nb_colonnes - pos_x - uu && tableau_vache[i][j] != '\0'; j++) {
			u++;
			if (pos_x + u > 0) {
				printf("%c", tableau_vache[i][j]);
			} else {
				GOTOXY(0, pos_y);
			}
		}
		// Si le dernier caractère est un '\n'
		if (tableau_vache[i][strlen(tableau_vache[i] + 1)] == '\n') {
			pos_y++;
			u=0;
			GOTOXY(pos_x, pos_y);
		}
	}
	printf("\n");

}


void racing_cow(char* yeux) {
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	int nb_colonnes = w.ws_col;

	char* tableau_vache[9] = {
		"                ^__^\n",
		"                (",
		yeux,
		")  \n",
		"            /|  (__) \n",
		"  _o\\______/_|\\__|__\\_/_|______|\n",
		" /_________   \\   00          /== -* * -\n",
		"[_____/^^\\_____\\_____/^^\\_____]     *- * -\n",
		"      \\__/           \\__/\n"
	};

	/* char* roue = " __ \n/  \\\n\\__/\n"; */
	char* roue[3] = {
		" __\n",
		"/  \\\n",
		"\\__/\n"
	};

	int coord_x_roue[17] = {1, 7, 13, 19, 25, 28, 31, 33, 35, 37, 39, 40, 41, 42, 43, 44, 45};
	int coord_y_roue[17] = {6, 5,  5,  6,  7,  8,  7,  6,  7,  8,  7,  7,  8,  7,  8,  8,  8};

	UPDATE;
	for (int i=nb_colonnes; i>-43; i--) {
		UPDATE;
		affiche_vache(tableau_vache, 9, i, 5, nb_colonnes);
		wait(20);
	}

	wait(300);

	for (int i=0; i<17; i++) {
		UPDATE;
		/* GOTOXY(coord_x_roue[i], coord_y_roue[i]); */
		/* printf("%s\n", roue); */
		affiche_vache(roue, 3, coord_x_roue[i], coord_y_roue[i], nb_colonnes);
		wait(100);
	}
}


void coincoin() {
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	int nb_colonnes = w.ws_col;

	char* tableau_coincoin[3] = {
		 "  _\n",
		 "=(.)__\n",
		 " (___/\n"
	};

	int pos_y_coincoins[3] = {2, 2, 2};

	char* eau = "      /\\___/\\__/\\___    ___/\\                        _____/\\__/\\__    _/\\\n\
				~~     ~~~  __/\\___     _/\\\\                            ~~~       ~~~       ~~  ~\n\
				    ~~~~                                   _____/\\______/\\____/\\  \n";

	while (1) {
		for (int i=0; i<3; i++) {
			UPDATE;
			GOTOXY(1, 2);
			printf("%s\n", eau);

			affiche_vache(tableau_coincoin, 3, 20, pos_y_coincoins[0], nb_colonnes);
			affiche_vache(tableau_coincoin, 3, 40, pos_y_coincoins[1], nb_colonnes);
			affiche_vache(tableau_coincoin, 3, 60, pos_y_coincoins[2], nb_colonnes);

			if (pos_y_coincoins[i] == 2) {
				pos_y_coincoins[i] = 1;
			} else {
				pos_y_coincoins[i] = 2;
			}
			wait(300);
		}
	}
}

void tux() {
	UPDATE;
	printf("\
              .88888888:.\n\
            88888888.88888.\n\
           .8888888888888888.\n\
           888888888888888888\n\
           88' _`88'_  `88888\n\
           88 88 88 88  88888\n\
           88_88_::_88_:88888\n\
           88:::,::,:::::8888\n\
           88`:::::::::'`8888\n\
          .88  `::::'    8:88.\n\
         8888            `8:888.\n\
       .8888'             `888888.\n\
      .8888:..  .::.  ...:'8888888:.\n\
     .8888.'     :'     `'::`88:88888\n\
    .8888        '         `.888:8888.\n\
   888:8         .           888:88888\n\
 .888:88        .:           888:88888:\n\
 8888888.       ::           88:888888\n\
 `.::.888.      ::          .88888888\n\
 .::::::.888.    ::         :::`8888'.:.\n\
::::::::::.888   '         .::::::::::::\n\
::::::::::::.8    '      .:8::::::::::::.\n\
.::::::::::::::.        .:888:::::::::::::\n\
:::::::::::::::88:.__..:88888:::::::::::'\n\
 `'.:::::::::::88888888888.88:::::::::'\n\
miK     `':::_:' -- '' -'-' `':_::::'`\n ");
	wait(5000);
	printf("Y'a rien de particulier, juste j'aime bien le pinguin\n");
}

void eating_cow(char* yeux, char* langue, int longueur_queue) {
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	int nb_colonnes = w.ws_col;

	char* queue = genere_queue(longueur_queue);
	char* tableau_vache[11] = {
		"    ^__^\n",
        "    (",
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

	char* tableau_vache_broute[9] = {
		"     __ _______\n",
		"^__^/ /\\       )",
		queue,
		"\n",
		"(",
		yeux,
		") / ||----w |\n",
		"(__)   ||     ||\n",
		",,,,   ||     ||\n"
	};

	UPDATE;
	affiche_vache(tableau_vache, 11, 9, 1, nb_colonnes);
	GOTOXY(9, 5);
	printf(",,,,\n");
	wait(2000);

	UPDATE;
	affiche_vache(tableau_vache_broute, 9, 9, 1, nb_colonnes);
	wait(1000);

	UPDATE;
	affiche_vache(tableau_vache, 11, 9, 1, nb_colonnes);
}
													 


void skating_cow(char* yeux, char* langue, int longueur_queue) {
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	int nb_colonnes = w.ws_col;

	char* queue = genere_queue(longueur_queue);
	char* tableau_vache[11] = {
		"    ^__^\n",
        "    (",
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

	char* skate[2] = {
		"\\-------------/\n",
		"   O      O\n"
	};

	for (int i=nb_colonnes; i>-20-longueur_queue; i--) {
		UPDATE;
		if (i >= 43 && i <= 55) {
			affiche_vache(tableau_vache, 11, i, 1, nb_colonnes);
			affiche_vache(skate, 2, i + 5, 6, nb_colonnes);
		} else if (i >= 38 && i <= 62) {
			affiche_vache(tableau_vache, 11, i, 2, nb_colonnes);
			affiche_vache(skate, 2, i + 5, 7, nb_colonnes);
		} else if (i > 9 && i < 13) {
			affiche_vache(tableau_vache, 11, i, 4, nb_colonnes);
			affiche_vache(skate, 2, i + 5, 8, nb_colonnes);
		} else if (i <= 9) {
			affiche_vache(tableau_vache, 11, 9, 5, nb_colonnes);
			affiche_vache(skate, 2, i + 5, 8, nb_colonnes);
		} else {
			affiche_vache(tableau_vache, 11, i, 3, nb_colonnes);
			affiche_vache(skate, 2, i + 5, 8, nb_colonnes);
		}

		wait(50);
	}

	GOTOXY(1, 2);
	printf(" _____________\n< skateboard. >\n -------------\n");

	GOTOXY(9, 5);
	printf("\\");
	GOTOXY(10, 6);
	printf("\\");
	GOTOXY(1, 10);
	free(queue);
}

int main(int argc, char* argv[]) {
	char yeux[3] = "OO\0";
	char langue[3] = "  \0";
	int longueur_queue = 3;
	int animation = EATING_COW;

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
				} else if (strcmp(argv[i], "-f") == 0) {
					etat_suivant = ANIMATION;
				} else {  // aucune option trouvé => message à afficher
					printf("Option invalide\n");
					exit(1);
				}
				break;

			case YEUX:
				strcpy(yeux, argv[i]);
				etat_suivant = LIRE_OPTION;
				break;

			case LANGUE:
				langue[0] = *argv[i];
				etat_suivant = LIRE_OPTION;
				break;

			case QUEUE:
				sscanf(argv[i], "%i", &longueur_queue);
				etat_suivant = LIRE_OPTION;
				break;

			case ANIMATION:
				if (strcmp(argv[i], "skateboard") == 0) {
					animation = SKATEBOARD;
				} else if (strcmp(argv[i], "eat") == 0) {
					animation = EATING_COW;
				} else if (strcmp(argv[i], "coincoin") == 0) {
					animation = COINCOIN;
				} else if (strcmp(argv[i], "race") == 0) {
					animation = RACING_COW;
				} else if (strcmp(argv[i], "tux") == 0) {
					animation = TUX;
				}
				break;
		}
		etat_courant = etat_suivant;
	}

	switch (animation) {
		case SKATEBOARD:
			skating_cow(yeux, langue, longueur_queue);
			break;

		case EATING_COW:
			eating_cow(yeux, langue, longueur_queue);
			break;

		case COINCOIN:
			coincoin();
			break;

		case TUX:
			tux();
			break;

		case RACING_COW:
			racing_cow(yeux);
			break;
	}
}
