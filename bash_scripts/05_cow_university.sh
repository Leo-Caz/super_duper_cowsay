#! /bin/env bash

# cow_university

# Vérifie qu'un nombre entier est premier
estPremier() {
	diviseurs=0
	for i in $(seq "$1")
	do
		if [ $(("$1" % i)) -eq 0 ]
		then
			diviseurs=$((diviseurs + 1))
		fi
	done

	if [ $diviseurs -le 2 ]
	then
		echo 0
	else
		echo 1
	fi
}

for i in $(seq "$1")
do
	if [ "$(estPremier "$i")" -eq 0 ]
	then
		cowsay "$i"
		sleep 1
	fi
done
