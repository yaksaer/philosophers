SRCS =  ./philo/philo.c ./philo/second_utils.c ./philo/thread_process.c \
 		./philo/utils.c ./philo/end.c

BONUS_SRCS = ./philo_bonus/philo.c ./philo_bonus/second_utils.c ./philo_bonus/fork_process.c \
              		./philo_bonus/utils.c ./philo_bonus/end_n_helpers.c

NAME = ./philo/philo

BONUS_NAME = ./philo_bonus/philo_bonus

OBJ = ${SRCS:.c=.o}

BONUS_OBJ = $(BONUS_SRCS:.c=.o)

HEAD = ./philo/philosophers.h

BONUS_HEAD = ./philo_bonus/philosophers.h

FLAGS = -Wall -Wextra -Werror

INCS = -I $(HEAD)

RM = rm -f

$(NAME): $(OBJ) $(HEAD)
		gcc ${FLAGS} $(OBJ) $(INCS) -o $(NAME)

all: $(NAME)

$(BONUS_NAME): $(BONUS_OBJ) $(BONUS_HEAD)
		gcc $(FLAGS) $(BONUS_OBJ) $(INCS) -o $(BONUS_NAME)

bonus: $(BONUS_NAME)

%.o: %.c ${HEAD}
	gcc ${FLAGS} ${INCS} -c $< -o $@

clean:
	$(RM) $(OBJ) $(OBJ_BO)
	$(RM) $(OBJ) $(BONUS_OBJ)

fclean: clean
	$(RM) $(NAME) $(BONUS_NAME)

re: fclean all