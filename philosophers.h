#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

# define RESET   "\033[0m"
# define RED     "\033[31;31m"
# define GREEN  "\033[1;32m"
# define BLUE   "\033[1;34m"
# define MAGENTA   "\033[1;35m"
# define CYAN   "\033[1;36m"

//typedef struct	s_philo {
//	int 			number;
//	unsigned int	last_eat;
//	int 			amount_eat;
//	pthread_mutex_t	prev;
//	pthread_mutex_t	next;
//	pthread_t		thread;
//	int 			alive;
//}				t_philo;

typedef struct s_philo {
	int				num;
	unsigned int	last_eat;
	int				amount_eat;
	pthread_mutex_t	fork;
	pthread_t		thread;
	pthread_mutex_t	check;
	int				alive;
	struct s_philo	*next;
	struct s_philo	*prev;
}				t_philo;

typedef struct s_philo_list {
	size_t		size;
	t_philo		*head;
	t_philo		*tail;
}				t_philo_list;

typedef struct s_main {
	int					num_philo;
	int					die_tm;
	int					eat_tm;
	int					sleep_tm;
	int					num_eat;
	unsigned long long	start_time;
	int					iter;
	int					death_of_diogen;
	int					n;
	pthread_mutex_t		queue;
	pthread_mutex_t		print;
	pthread_mutex_t		death;
	t_philo_list		*philo_list;
	struct timeval		tv;
}				t_main;
int 	main(int argc, char **argv);
/* Thread */
void			*start_thread(void *args);
void			*checker(void *args);
int				eating(t_main *main, t_philo *diogen);
int				taking_forks(t_main *main, t_philo *diogen);
void			*eat_checker(void *args);
/* Utils */
int				print_message(t_main *main, t_philo *diogen, int flag);
unsigned int	calculate_time(t_main *main);
void			ft_usleep(unsigned int time);
/* Lib_utils */
int				ft_atoi(const char *nptr);
size_t			ft_strlen(const char *s);
void			ft_dlist_push_back(t_philo_list *list, int num);
t_philo			*ft_dlist_get_n(t_philo_list *list, size_t index);
void			ft_dlist_del(t_philo_list **list, int size);
void			*ft_calloc(size_t nmemb, size_t size);
void			*ft_bzero(void *s, size_t n);

#endif
