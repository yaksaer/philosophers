SRCS =  philo.c second_utils.c thread_process.c utils.c end.c

NAME = philo

OBJ = ${SRCS:.c=.o}

HEAD = philosophers.h

FLAGS = -Wall -Wextra -Werror

INCS = -I $(HEAD)

RM = rm -f

$(NAME): $(OBJ) $(HEAD)
		gcc ${FLAGS} $(OBJ) $(INCS) -o $(NAME)

all: $(NAME)

%.o: %.c ${HEAD}
	gcc ${FLAGS} ${INCS} -c $< -o $@

clean:
	$(RM) $(OBJ) $(OBJ_BO)

fclean: clean
	$(RM) $(NAME)

re: fclean all