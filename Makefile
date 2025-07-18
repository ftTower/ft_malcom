NAME = ft_malcolm
SRC = srcs/main.c srcs/tools.c srcs/constructor.c srcs/boolean.c srcs/display.c
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3

.PHONY: all clean fclean re valgrind

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	rm -f $(NAME)

clear:
	clear

fclean: clean

re: fclean all

run: re clear $(NAME)
	sudo ./$(NAME) "10.0.2.1" "08:00:27:d5:38:22" "10.0.2.5" "08:00:27:bc:f7:31"
# 	rm -f $(NAME)
