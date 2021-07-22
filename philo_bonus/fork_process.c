#include "philosophers.h"

static int 	taking_forks(t_main *main, t_philo *diogen)
{
	sem_wait(main->forks);
	if (!print_message(main, diogen, 1))
		return (0);
	sem_wait(main->forks);
	if (!print_message(main, diogen, 2))
		return (0);
	return (1);
}

static int 	eating(t_main *main, t_philo *diogen)
{
	if (!taking_forks(main, diogen))
		return (0);
	sem_wait(diogen->check);
	diogen->last_eat = calc_time(main);
	if (!print_message(main, diogen, 3))
		return (0);
	usleep(main->eat_tm * 1000);
	diogen->eat_time++;
	if (diogen->eat_time == main->num_eat)
		sem_post(main->eat_time);
	sem_post(diogen->check);
	sem_post(main->forks);
	sem_post(main->forks);
	return (1);
}

static void 	*checker(void *args)
{
	t_main		*main;
	t_philo		*diogen;

	main = (t_main *)args;
	diogen = ft_dlist_get_n(main->philo_list, main->iter);
	while (diogen->alive == 1)
	{
		sem_wait(diogen->check);
		sem_wait(main->queue);
		if (((int)calc_time(main) - (int)diogen->last_eat > main->die_tm))
		{
			sem_wait(main->print);
			if (diogen->alive == 1)
				printf("%ums %d is die\n", calc_time(main), diogen->num);
			diogen->alive = 0;
			sem_post(main->print);
			sem_post(main->queue);
			sem_post(main->death);
			return ((void *)0);
		}
		sem_post(main->queue);
		sem_post(diogen->check);
	}
	return ((void *)1);
}

void 	*eat_checker(void *args)
{
	t_main		*main;
	t_philo		*tmp;

	main = (t_main *)args;
	tmp = main->philo_list->head;
	main->n = 0;
	while (1)
	{
		while (main->n < main->num_eat)
		{
			sem_wait(main->eat_time);
			main->n++;
			if (main->n == main->num_philo)
				break ;
		}
		if (!print_message(main, tmp, 7))
			return ((void *)0);
		tmp->alive = 0;
		sem_post(main->death);
		return ((void *)1);
	}
}

int	start_fork(t_main *main, t_philo *diogen)
{
	pthread_t	diogen_check;

	if (pthread_create(&diogen_check, NULL, checker, (void *)main))
		return (0);
	pthread_detach(diogen_check);
	while (diogen->alive == 1)
	{
		if (!eating(main, diogen))
			break ;
		if (diogen->alive == 1)
			if (!print_message(main, diogen, 4))
				break ;
		usleep(main->sleep_tm * 1000);
		if (diogen->alive == 1)
			if (!print_message(main, diogen, 5))
				break ;
		usleep(100);
	}
	return (1);
}
