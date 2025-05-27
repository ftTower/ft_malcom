#!/bin/bash

NAME=ft_malcolm

clear

echo "FT MALCOLM" && echo  

rm $NAME 

gcc -Wall -Wextra -Werror srcs/main.c srcs/tools.c srcs/constructor.c srcs/boolean.c srcs/display.c -g3 -o ft_malcolm || echo "Failed"

sudo valgrind ./$NAME "10.10.10.10" "64:64:64:64:64:64" "10.10.10.10" "64:64:64:64:64:64"

rm $NAME