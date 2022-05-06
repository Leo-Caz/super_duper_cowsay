#! /bin/env bash

#cow_kindergarten
for i in $(seq 10)
do
   cowsay "$i"
   sleep 1
done
cowsay -T U "j'ai fini"
