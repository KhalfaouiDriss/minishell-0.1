NAME = minishell
CC = cc
LIBFT = lib/libft/libft.a
MFLAGS = -lreadline

CFLAGS = -Wall -Wextra -Werror

SRCS = src/main.c \
	src/free_all.c \
	src/parser/handle_dollar_sign.c \
	src/parser/handle_normal_word.c \
	src/parser/handle_quotes.c \
	src/parser/handle_special_token.c \
	src/parser/handle_variable_token.c \
	src/parser/parce_red.c \
	src/parser/parse_tokens.c \
	src/parser/syntax_error.c \
	src/parser/tokenized.c \
	src/builtins/built_in_utils.c \
	src/builtins/builtins.c \
	src/builtins/ft_cd.c \
	src/builtins/ft_echo.c \
	src/builtins/ft_env.c \
	src/builtins/ft_exit.c \
	src/builtins/ft_export.c \
	src/builtins/ft_pwd.c \
	src/builtins/ft_unset.c \
	src/executor/execute_cmds.c \
	src/executor/execute_util1.c \
	src/executor/execute_util2.c \
	src/executor/execute_util3.c \
	src/utils/builtins_utils_1.c \
	src/utils/builtins_utils_2.c \
	src/utils/herdoc_util.c \
	src/utils/init_env.c \
	src/utils/linkd_list_utils.c \
	src/utils/signals_hand.c \
	src/utils/utils_1.c \
	src/utils/utils_2.c \
	src/utils/utils_syntax_err.c \
	src/utils/utils_toenized.c \
	src/utils/utils_variable.c

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

.PHONY: all clean fclean re
