#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generer_entier.c"


int lunch_food(int stock) {
	int reponse_valide = 0;
	char reponse[100];
	int bouffe_demande;

	while (reponse_valide == 0) {
		// On récupère la réponse de l'utilisateur
		printf("Vous voulez donner combien de bouffe à la poly-vache ?\n");
		printf("(entre 0 exclu et %d inclus)\n -> ", stock);
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
		



int main() {
	int fitness = 5;
	int stock = 5;
	long digestion = -1 * generer_entier(4);
	/* int bite = lunch_food(stock); */
}
