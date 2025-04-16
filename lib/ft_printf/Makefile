NAME = ft_printf.a

FLAGS = -Wall -Wextra -Werror

SRC = ft_printf.c ft_putnbr.c ft_puthexadecimal.c ft_u_putnbr.c ft_putstr.c

OBJ = ${SRC:.c=.o}

all : ${NAME}

${NAME} : ${OBJ}
	ar crs ${NAME} ${OBJ}

%.o : %.c
	cc ${FLAGS} -c $< -o $@

clean :
	rm -rf ${OBJ}

fclean : clean
	rm -rf ${NAME}

re : fclean all

.PHONY : all clean fclean re
