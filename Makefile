NAME = minishell
CC = cc -g
LIBFT = lib/libft/libft.a
MFLAGS = -lreadline

# CFLAGS = -Wall -Wextra -Werror
SRCS = $(wildcard src/*.c src/executor/*.c src/utils/*.c src/parser/*.c src/builtins/*.c)

OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(MFLAGS) $(GNL) lib/libft/libft.a

$(LIBFT):
		${MAKE} -C ./lib/libft

clean:
	$(MAKE) -C lib/libft clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C lib/libft fclean
	rm -f $(NAME)

re: fclean all

push:
	git add .
	git status
	git commit -m "minishell"
	git push

pull:
	git pull

.PHONY: all clean fclean re
