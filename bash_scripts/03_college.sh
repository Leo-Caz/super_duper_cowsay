#! /bin/env bash

#college :
y=0
z=1
i=1
cowsay " 0 "
sleep 1
while [ $i -lt "$1" ]
do
	cowsay $i
	sleep 1
	i=$((y + z))
	y=$z
	z=$i
done
cowsay -T U "j'ai fini"

