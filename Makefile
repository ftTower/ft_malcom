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

fclean: clean

re: fclean all

run: $(NAME)
	sudo ./$(NAME) "10.10.10.10" "64:64:64:64:64:64" "10.10.10.10" "64:64:64:64:64:64"
	rm -f $(NAME)

requirements:
	sudo apt-get update
	sudo apt-get install -y vim net-tools iproute2 arping iputils-ping tcpdump