#! /bin/env bash

# smart_cow

operateur=$(echo "$1" | sed s/"[^+-/*]"//g)
if [ ${#operateur} -ne 1 ]
then
	echo "calcul non valide"
	exit 1
fi

x=$(echo "$1" | cut -d "$operateur" -f 1)
y=$(echo "$1" | cut -d "$operateur" -f 2)

# Protection anti-débile
[ ${#x} -eq 0 ] && x=0
[ ${#y} -eq 0 ] && y=0

case $operateur in
	"+") resultat=$((x + y));;
	"-") resultat=$((x - y));;
	"*") resultat=$((x * y));;
	"/")
		if [ "$y" -eq 0 ]
		then
			echo "Division par 0 impossible"
			exit 1
		fi
		resultat=$((x / y));;
esac

if [ ${#resultat} -eq 1 ]
then
	cowsay -e "0$resultat" "$1"
elif [ ${#resultat} -gt 2 ]
then
	echo "Attention: Résultat trop long pour rentrer dans les yeux"
	cowsay -e "$resultat" "$1"
else
	cowsay -e "$resultat" "$1"
fi
