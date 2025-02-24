NAME = game
CFLAGS = -g -Wall -Wextra -Werror
SOURCES= main.c
OBJS= $(SOURCES:.c=.o)

all:$(NAME)

$(NAME):$(OBJS)
	cc $(CFLAGS) $(OBJS) -lm -L ./minilibx-linux -lmlx -lXext -lX11 -o $(NAME)

clean:
	rm -fr $(OBJS)

fclean: clean
	rm -fr $(NAME)

re:clean fclean $(NAME)