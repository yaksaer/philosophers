#include "philosophers.h"

static void	*check_thread(void *data)
{
	t_main	*main;
	t_philo	*tmp;

	main = (t_main *)data;
	tmp = main->philo_list->head;
	while (tmp && main->death_of_diogen == 0)
	{
		pthread_mutex_lock(&tmp->check);
		if (calc_time(main) - tmp->last_eat >= (unsigned int)main->die_tm)
		{
			pthread_mutex_lock(&main->print);
			if (main->death_of_diogen == 0)
				printf("%s%ums %d died%s\n", RED, calc_time(main), tmp->num,
					RESET);
			main->death_of_diogen = 1;
			pthread_mutex_unlock(&main->print);
			pthread_mutex_unlock(&tmp->check);
			return ((void *)0);
		}
		pthread_mutex_unlock(&tmp->check);
		tmp = tmp->next;
		usleep(50);
	}
	return ((void *)1);
}

int	check_philo(t_main *main)
{
	pthread_t	check1;

	if (pthread_create(&check1, NULL, check_thread, (void *)main))
		return (0);
	pthread_join(check1, NULL);
	return (1);
}

void	ft_usleep(unsigned int n)
{
	struct timeval	start;
	struct timeval	tmp;

	gettimeofday(&start, NULL);
	while (1)
	{
		usleep(50);
		gettimeofday(&tmp, NULL);
		if ((unsigned int)((unsigned int)(tmp.tv_sec - start.tv_sec)
			*1000000 + ((unsigned int)(tmp.tv_usec - start.tv_usec))) > n)
			break ;
	}
}

int	clean_n_exit(t_main *main, int flag)
{
	t_philo	*tmp;

	if (main->philo_list)
		tmp = main->philo_list->head;
	if (flag > 0)
	{
		while (tmp)
		{
			pthread_mutex_destroy(&tmp->fork);
			pthread_mutex_destroy(&tmp->check);
			if (tmp->num == main->num_philo)
				break ;
			tmp = tmp->next;
		}
		pthread_mutex_destroy(&main->print);
	}
	if (main->philo_list)
		ft_dlist_del(&main->philo_list, main->num_philo);
	if (flag == 1 || flag == 0)
		printf("%sError!%s\n", RED, RESET);
	return (1);
}
