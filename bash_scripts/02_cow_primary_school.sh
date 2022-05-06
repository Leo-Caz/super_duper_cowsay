#! /bin/env bash

#cow_primaryschool
for i in $(seq "$1") 
do
   cowsay "$i"
   sleep 1
done

cowsay -T U "j'ai fini"
