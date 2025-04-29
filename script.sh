#!/bin/bash

NAME=ft_malcom

clear

echo "FT MALCOM" && echo  

gcc -Wall -Wextra -Werror srcs/main.c srcs/tools.c -g3 -o ft_malcom || echo "Failed"

./$NAME

rm $NAME