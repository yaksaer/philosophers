SRCS =  philo.c second_utils.c thread_process.c utils.c

NAME = philo

OBJ = ${SRCS:.c=.o}

HEAD = philosophers.h

EXEC = gcc -Wall -Wextra -Werror

INCS = -I $(HEAD)

RM = rm -f

$(NAME): $(OBJ) $(HEAD)
		$(EXEC) -o $(NAME) $(OBJ) $(INCS) $(MLX_FL) $(LIBFT)

all: $(NAME)

clean:
	$(RM) $(OBJ) $(OBJ_BO)

fclean: clean
	$(RM) $(NAME)

re: fclean all