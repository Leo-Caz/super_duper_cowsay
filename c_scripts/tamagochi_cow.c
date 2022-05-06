#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define CLAMP(x, min, max) MAX(min, MIN(x, max))

#define BYEBYELIFE 0
#define LIFESUCKS  1
#define LIFEROCKS  2


// Demande à l'utilisateur la quantité de nourriture à donner à la vache
int lunch_food(int stock) {
	int reponse_valide = 0;
	char reponse[100];
	int bouffe_demande;

	while (reponse_valide == 0) {
		// On récupère la réponse de l'utilisateur
		printf("Vous voulez donner combien de bouffe à la poly-vache ?\n -> ");
		fscanf(stdin, "%s", reponse);

		// On récupère les nombres dans la chaîne de caractères
		sscanf(reponse, "%d", &bouffe_demande);

		if (bouffe_demande > 0 && bouffe_demande <= stock) {
			reponse_valide = 1;
		} else {
			printf("Quantité de nourriture non valide\n\n");
		}
	}

	return bouffe_demande;
}


void tmp_affiche_vache(int etat_vache) {
	switch (etat_vache) {
		case LIFEROCKS:
			printf("Yooooooo, comment chuis grave bien làààààà.....\n");
			break;

		case LIFESUCKS:
			printf("Chuis pas venu ici pour souffrir OKAAAY!!!\n");
			break;

		case BYEBYELIFE:
			printf("Fuck le capitalisme\n");
			break;
	}
}


int stock_update(int stock, int nourriture_donne) {
	// crop compris entre -3 et 3
	int crop = (rand() % 6) - 3;
	printf("crop: %d\n", crop);
	int nouveau_stock = stock + crop - nourriture_donne;
	return CLAMP(nouveau_stock, 0, 10);
}


int fitness_update(int fitness, int nourriture_donne) {
	// digestion compris entre 0 et 3
	int digestion = rand() % 3;
	printf("digestion: %d\n", digestion);
	int nouvelle_fitness = fitness + nourriture_donne - digestion;
	return CLAMP(nouvelle_fitness, 0, 10);
}
		

int main() {
	int stock = 5;
	int fitness = 5;
	int etat_vache = LIFEROCKS;
	int nourriture_donnee;
	unsigned int duree_de_vie = 0;

	tmp_affiche_vache(etat_vache);

	while (etat_vache != BYEBYELIFE) {
		if (stock == 0) {
			printf("Vous n'avez plus de nourriture dans le stock, le tour est passé\n");
			nourriture_donnee = 0;
		} else {
			printf("Il vous reste %d nourritures dans le stock\n", stock);
			nourriture_donnee = lunch_food(stock);
		}
		stock = stock_update(stock, nourriture_donnee);
		fitness = fitness_update(fitness, nourriture_donnee);
		duree_de_vie++;

		if (fitness >= 4 && fitness <= 6) {
			etat_vache = LIFEROCKS;
		} else if (fitness >= 1 && fitness <= 9) {
			etat_vache = LIFESUCKS;
		} else {
			etat_vache = BYEBYELIFE;
		}

		// Affichage de l'état de la vache
		printf("______________________\n");
		tmp_affiche_vache(etat_vache);
	}

	printf("Votre vache à vécu pendant %d tours\n", duree_de_vie);
}
