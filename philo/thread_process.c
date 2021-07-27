#include "philosophers.h"

static int	taking_forks(t_main *main, t_philo *diogen)
{
	if (diogen->num == main->num_philo)
	{
		pthread_mutex_lock(&diogen->next->fork);
		if (!print_message(main, diogen, 2))
			return (0);
		if (main->philo_list->size == 1)
			return (usleep(main->die_tm * 1000));
		pthread_mutex_lock(&diogen->fork);
		if (!print_message(main, diogen, 1))
			return (0);
	}
	else
	{
		pthread_mutex_lock(&diogen->fork);
		if (!print_message(main, diogen, 1))
			return (0);
		pthread_mutex_lock(&diogen->next->fork);
		if (!print_message(main, diogen, 2))
			return (0);
	}
	return (1);
}

static int	eating(t_main *main, t_philo *diogen)
{
	if (!taking_forks(main, diogen))
		return (0);
	pthread_mutex_lock(&diogen->check);
	diogen->last_eat = calc_time(main);
	pthread_mutex_unlock(&diogen->check);
	if (!print_message(main, diogen, 3))
		return (0);
	ft_usleep(main->eat_tm * 1000);
	diogen->amount_eat++;
	if (diogen->amount_eat == main->num_eat)
		main->n++;
	pthread_mutex_unlock(&diogen->fork);
	pthread_mutex_unlock(&diogen->next->fork);
	return (1);
}

void	*eat_checker(void *args)
{
	t_main		*main;

	main = (t_main *)args;
	main->n = 0;
	while (1)
	{
		if (main->n == main->num_philo)
		{
			pthread_mutex_lock(&main->print);
			main->death_of_diogen = 1;
			printf("%s%ums The philosophers are full%s\n", CYAN,
				   calc_time(main), RESET);
			pthread_mutex_unlock(&main->print);
			return ((void *)1);
		}
	}
}

void	*start_thread(void *args)
{
	t_main		*main;
	t_philo		*diogen;

	main = (t_main *)args;
	diogen = ft_dlist_get_n(main->philo_list, main->iter);
	main->iter++;
	while (main->death_of_diogen == 0)
	{
		if (!eating(main, diogen))
			break ;
		if (main->death_of_diogen == 1)
			break ;
		print_message(main, diogen, 4);
		ft_usleep(main->sleep_tm * 1000);
		if (main->death_of_diogen == 1)
			break ;
		print_message(main, diogen, 5);
		ft_usleep(100);
	}
	return ((void *) 1);
}
