CC = cc

CFLAGS = -Wall -Wextra -Werror -g3

SRC = $(wildcard *.c) \
      $(wildcard parsing/*.c) \
	  $(wildcard helpers/*.c)

OBJS = $(SRC:.c=.o)

NAME = philo

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
