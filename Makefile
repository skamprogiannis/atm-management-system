CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = atm
SRC = src/main.c src/system.c src/auth.c
OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.c src/header.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

run: $(NAME)
	./$(NAME)

.PHONY: clean fclean re run
