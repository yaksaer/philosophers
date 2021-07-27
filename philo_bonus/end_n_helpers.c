#include "philosophers.h"

int	check_death(t_main *main)
{
	t_philo	*tmp;

	tmp = main->philo_list->head;
	while (tmp)
	{
		if (tmp->alive == 0)
			return (0);
		if (tmp->num == main->num_philo)
			break ;
		tmp = tmp->next;
	}
	return (1);
}

int	semaphore_init(t_main *main)
{
	sem_unlink("sem_fork");
	sem_unlink("sem_queue");
	sem_unlink("sem_print");
	sem_unlink("sem_death");
	sem_unlink("sem_eat_time");
	sem_unlink("sem_waiter");
	main->forks = sem_open("sem_fork", O_CREAT | O_EXCL, 0666, main->num_philo);
	main->queue = sem_open("sem_queue", O_CREAT | O_EXCL, 0666, 1);
	main->print = sem_open("sem_print", O_CREAT | O_EXCL, 0666, 1);
	main->death = sem_open("sem_death", O_CREAT | O_EXCL, 0666, 0);
	main->waiter = sem_open("sem_waiter", O_CREAT | O_EXCL, 0666,
			main->num_philo / 2);
	main->eat_time = sem_open("sem_eat_time", O_CREAT | O_EXCL, 0666, 0);
	if (main->forks < 0 || main->queue < 0 || main->print < 0 || main->death
		< 0 || main->eat_time < 0 || main->waiter < 0)
		return (0);
	return (1);
}

char	*sem_name(char *res, int n)
{
	res[0] = 's';
	res[1] = 'e';
	res[2] = 'm';
	res[3] = '_';
	res[4] = (char)(n + '0');
	return (res);
}

static void	clean_philo(t_main *main)
{
	t_philo	*tmp;
	char	str[10];

	if (!main->philo_list)
		return ;
	tmp = main->philo_list->head;
	while (tmp)
	{
		ft_bzero(str, 10);
		sem_name(str, tmp->num);
		sem_unlink(str);
		sem_close(tmp->check);
		if (tmp->num == main->num_philo)
			break ;
		tmp = tmp->next;
	}
	sem_close(main->forks);
	sem_close(main->queue);
	sem_close(main->print);
	sem_close(main->death);
	sem_close(main->eat_time);
	sem_close(main->waiter);
}

int	clean_n_exit(t_main *main, int flag)
{
	if (flag > 0)
	{
		clean_philo(main);
		sem_unlink("sem_fork");
		sem_unlink("sem_queue");
		sem_unlink("sem_print");
		sem_unlink("sem_death");
		sem_unlink("sem_eat_time");
		sem_unlink("sem_waiter");
	}
	if (main->philo_list)
		ft_dlist_del(&main->philo_list, main->num_philo);
	else if (flag == 1 || flag == 0)
		printf("%sError!%s\n", RED, RESET);
	return (1);
}
