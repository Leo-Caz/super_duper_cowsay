#! /bin/env bash

fichierDevinettes="devinettes.txt"
nbLignes=$(wc -l $fichierDevinettes | cut -d ' ' -f 1)
indiceDevinette=$(( RANDOM% (nbLignes / 3) ))

devinette=$(sed -n $(( (indiceDevinette * 3) + 1))p $fichierDevinettes)
solution=$(sed -n $(( (indiceDevinette * 3) + 2))p $fichierDevinettes)

cowsay "Tiens, j'ai une devinette pour toi : $devinette"

essaisRestants=5
bonneReponse=0
reponse=""

while [ "$bonneReponse" -eq 0 ] && [ "$essaisRestants" -gt 0 ]
do
	printf "\nQuelle est ta proposition ? "
	read -r reponse

	if [ "$reponse" = "$solution" ]
	then
		cowsay -e "^^" -T "U" "Bravo ! Tu as trouvé la bonne réponse !!!!"
		bonneReponse=1
	else
		essaisRestants=$((essaisRestants - 1))
		if [ "$essaisRestants" -eq 0 ]
		then
			cowsay -e "xx" "Tu as perdu..."
			echo "La réponse était : $solution"
		else
			cowsay -e "--" "Non c'était pas ça, il te reste $essaisRestants essais"
		fi
	fi
done
