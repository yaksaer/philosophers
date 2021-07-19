#include "philosophers.h"

int 	taking_forks(t_main *main, t_philo *diogen)
{
	pthread_mutex_lock(&diogen->fork);
	if (!print_message(main, diogen, 1))
		return (0);
	pthread_mutex_lock(&diogen->next->fork);
	if (!print_message(main, diogen, 2))
		return (0);
	return (1);
}

int 	eating(t_main *main, t_philo *diogen)
{
//	pthread_mutex_lock(&main->queue);
	if (!taking_forks(main, diogen))
		return (0);
//	pthread_mutex_unlock(&main->queue);
	pthread_mutex_lock(&diogen->check);
	diogen->last_eat = calculate_time(main);
	if (!print_message(main, diogen, 3))
		return (0);
	usleep(main->eat_tm * 1000);
	diogen->amount_eat++;
	pthread_mutex_unlock(&diogen->check);
	pthread_mutex_unlock(&diogen->fork);
	pthread_mutex_unlock(&diogen->next->fork);
	return (1);
}

void 	*checker(void *args)
{
	t_main		*main;
	t_philo		*diogen;

	main = (t_main *)args;
	diogen = ft_dlist_get_n(main->philo_list, main->iter - 1);
	while (1)
	{
		pthread_mutex_lock(&diogen->check);
		pthread_mutex_lock(&main->queue);
		if (main->death_of_diogen == 1)
			return ((void*)0);
		else if ((calculate_time(main) - diogen->last_eat > main->die_tm + 5))
		{
//			if (main->death_of_diogen == 0)
//				print_message(main, diogen, 6);
			pthread_mutex_lock(&main->print);
			if (main->death_of_diogen == 0)
				printf("%s%ums %d is die%s\n", RED, calculate_time(main),
					   diogen->num, RESET);
			pthread_mutex_unlock(&main->print);
			main->death_of_diogen = 1;
			pthread_mutex_unlock(&main->queue);
			pthread_mutex_unlock(&main->death);
			return ((void*)1);
		}
		else if(main->n == main->num_philo * main->num_eat)
			return ((void*)0);
		pthread_mutex_unlock(&main->queue);
		pthread_mutex_unlock(&diogen->check);
		usleep(500);
	}
}

void 	*eat_checker(void *args)
{
	t_main		*main;
	t_philo		*tmp;
	int 		i;

	main = (t_main *)args;
	while (1)
	{
		i = -1;
		main->n = 0;
		tmp = main->philo_list->head;
		while (++i < main->num_philo)
		{
			if (tmp->amount_eat >= main->num_eat)
				main->n++;
			if (main->n == main->num_philo)
			{
				print_message(main, tmp, 7);
				main->death_of_diogen = 1;
				pthread_mutex_unlock(&main->death);
				return ((void*)1);
			}
			tmp = tmp->next;
		}
		usleep(500);
	}
}

void 	*start_thread(void *args)
{
	t_main		*main;
	t_philo		*diogen;
	pthread_t	diogen_check;

	main = (t_main *)args;
	diogen = ft_dlist_get_n(main->philo_list, main->iter);
	if (pthread_create(&diogen_check, NULL, checker, (void *)main))
		return ((void*)1);
	pthread_detach(diogen_check);
	while (main->death_of_diogen == 0)
	{
		if (!eating(main, diogen))
			return ((void *)0);
		if (main->death_of_diogen == 0)
			print_message(main, diogen, 4);
		usleep(main->sleep_tm * 1000);
		if (main->death_of_diogen == 0)
			print_message(main, diogen, 5);
		usleep(100);
	}
	return (NULL);
}
