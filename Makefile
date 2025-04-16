NAME = minishell
CC = cc
GNL = lib/get_next_line/get_next_line.c lib/get_next_line/get_next_line_utils.c
LIBFT = lib/libft/libft.a 

MFLAGS = -lreadline

# CFLAGS = -Wall -Wextra -Werror
SRCS = $(wildcard src/*.c src/executor/*.c src/utils/*.c)

OBJS = $(SRCS:.c=.o)

all: printf $(LIBFT) $(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(MFLAGS) $(GNL) lib/libft/libft.a lib/ft_printf/ft_printf.a


$(LIBFT):
		${MAKE} -C ./lib/libft

printf:
	$(MAKE) -C lib/ft_printf

clean:
	$(MAKE) -C lib/ft_printf clean
	$(MAKE) -C lib/libft clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C lib/ft_printf fclean
	$(MAKE) -C lib/libft fclean
	rm -f $(NAME)

re: fclean all

push:
	git add .
	git status
	git commit -m"minishell"
	git push

pull:
	git pull

.PHONY: all clean fclean re printf
