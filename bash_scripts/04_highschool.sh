#! /bin/env bash

#highschool :
for i in $(seq "$1") 
do
   carre=$((i * i))
   cowsay $carre
   sleep 1
done

cowsay -T U "j'ai fini"

