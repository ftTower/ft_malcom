#!/bin/bash

NAME=ft_malcom

clear

echo "FT MALCOM" && echo  

gcc -Wall -Wextra -Werror srcs/main.c srcs/tools.c srcs/constructor.c srcs/boolean.c srcs/display.c -g3 -o ft_malcom || echo "Failed"

./$NAME "10.10.10.10" "64:64:64:64:64:64" "10.10.10.10" "64:64:64:64:64:64"

rm $NAME