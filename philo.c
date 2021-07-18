#include "philosophers.h"

int 	error_n_exit(t_main *main)
{
	printf("%sWrong params!%s\n", RED, RESET);
	return (1);
}

//void	ft_usleep(unsigned int time)
//{
//	unsigned int n;
//
//	n = time;
//	while (n > 0)
//	{
//		usleep(100);
//		n -= 10;
//	}
//}

unsigned int	calculate_time(t_main *main)
{
	unsigned int	curr_time;

	gettimeofday(&main->tv, NULL);
	curr_time = ((main->tv.tv_sec * 1000) + (main->tv.tv_usec / 1000)) -
				main->start_time;
	return (curr_time);
}

void 	print_message(t_main *main, t_philo *diogen, int flag)
{
	pthread_mutex_lock(&main->print);
	if (flag == 1)
		printf("%s%ums %d take right fork%s\n", GREEN, calculate_time(main),
			   diogen->num, RESET);
	else if (flag == 2)
		printf("%s%ums %d take left fork%s\n", GREEN, calculate_time(main),
			   diogen->num, RESET);
	else if (flag == 3)
		printf("%s%ums %d is eating%s\n", CYAN, calculate_time(main),
			   diogen->num,
			   RESET);
	else if (flag == 4)
		printf("%s%ums %d is sleeping%s\n", BLUE, calculate_time(main),
			   diogen->num, RESET);
	else if (flag == 5)
		printf("%s%ums %d is thinking%s\n", MAGENTA, calculate_time(main),
		   diogen->num, RESET);
	else if (flag == 6)
		printf("%s%ums %d is die%s\n", RED, calculate_time(main), diogen->num,
			   RESET);
	else if (flag == 7)
		printf("%s%ums The philosophers are full%s\n", CYAN,
			   calculate_time(main), RESET);
	pthread_mutex_unlock(&main->print);
}

int 	init_struct(t_main *main)
{
	main->num_philo = -1;
	main->die_tm = -1;
	main->eat_tm = -1;
	main->sleep_tm = -1;
	main->num_eat = -1;
	main->death_of_diogen = 0;
	return (1);
}

int 	pars_params(t_main *main, int argc, char **argv)
{
	main->num_philo = ft_atoi(argv[1]);
	if (main->num_philo == 0 && ft_strlen(argv[1]) != 1 && argv[1][0] != '0')
		return (0);
	main->die_tm = ft_atoi(argv[2]);
	if (main->die_tm == 0 && ft_strlen(argv[2]) != 1 && argv[2][0] != '0')
		return (0);
	main->eat_tm = ft_atoi(argv[3]);
	if (main->eat_tm == 0 && ft_strlen(argv[3]) != 1 && argv[3][0] != '0')
		return (0);
	main->sleep_tm = ft_atoi(argv[4]);
	if (main->sleep_tm == 0 && ft_strlen(argv[4]) != 1 && argv[4][0] != '0')
		return (0);
	if (argc == 6)
	{
		main->num_eat = ft_atoi(argv[5]);
		if ((main->num_eat == 0 && ft_strlen(argv[5]) != 1
		&& argv[5][0] != '0') || main->num_eat < 0 || main->num_eat == 0)
			return (0);
	}
	if (main->num_philo < 0 || main->die_tm < 0 || main->eat_tm < 0
		|| main->sleep_tm < 0)
		return (0);
	return (1);
}

int 	check_death(int argc, t_main *main, t_philo *diogen)
{
	if (argc == 6 && diogen->amount_eat == main->num_eat)
		return (0);
	if (calculate_time(main) - diogen->last_eat > main->die_tm)
	{
		printf("%s%dms %d is die%s\n", RED, calculate_time(main), diogen->num,
			   RESET);
//		diogen->alive = 0;
//		main->who_die += 1;
		return (0);
	}
	return (1);
}

int 	philo_init(t_main *main)
{
	int 	i;

	i = 1;
	pthread_mutex_init(&main->queue, NULL);
	pthread_mutex_init(&main->print, NULL);
	pthread_mutex_init(&main->death, NULL);
	pthread_mutex_lock(&main->death);
	while (i <= main->num_philo)
	{
		ft_dlist_push_back(main->philo_list, i);
		if (main->num_eat > 0)
			main->philo_list->tail->amount_eat = 0;
		main->philo_list->tail->alive = 1;
		main->philo_list->tail->last_eat = 0;
		pthread_mutex_init(&main->philo_list->tail->fork, NULL);
		pthread_mutex_init(&main->philo_list->tail->check, NULL);
		i++;
	}
	return (1);
}

int 	taking_forks(t_main *main, t_philo *diogen)
{
	//pthread_mutex_lock(&main->queue);
	pthread_mutex_lock(&diogen->fork);
	print_message(main, diogen, 1);
	if (main->num_philo == 1)
	{
		usleep((main->die_tm + 1) * 1000);
		return (0);
	}
	pthread_mutex_lock(&diogen->next->fork);
	print_message(main, diogen, 2);
	//pthread_mutex_unlock(&main->queue);
	return (1);
}

void 	eating(t_main *main, t_philo *diogen)
{
	if (!taking_forks(main, diogen))
		return ;
	pthread_mutex_lock(&diogen->check);
	diogen->last_eat = calculate_time(main);
	print_message(main, diogen, 3);
	usleep(main->eat_tm * 1000);
	diogen->amount_eat++;
	pthread_mutex_unlock(&diogen->check);
	pthread_mutex_unlock(&diogen->fork);
	pthread_mutex_unlock(&diogen->next->fork);
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
		if ((calculate_time(main) - diogen->last_eat > main->die_tm + 5)
			|| main->death_of_diogen == 1)
		{
			print_message(main, diogen, 6);
//			diogen->alive = 0;
			//pthread_mutex_unlock(&diogen->check);
			main->death_of_diogen = 1;
			pthread_mutex_unlock(&main->death);
			return ((void*)1);
		}
		else if(main->n ==main->num_philo * main->num_eat)
			return ((void*)1);
		pthread_mutex_unlock(&diogen->check);
		usleep(1000);
	}
}

void 	*start_thread(void *args)
{
	t_main		*main;
	t_philo		*diogen;
	pthread_t	diogen_check;

	main = (t_main *)args;
	diogen = ft_dlist_get_n(main->philo_list, main->iter);
	printf("Hello it's %d\n", main->iter);
	if (pthread_create(&diogen_check, NULL, checker, (void *)main))
		return ((void*)1);
	pthread_detach(diogen_check);
	while (diogen->alive != 0)
	{
		eating(main, diogen);
		print_message(main, diogen, 4);
		usleep(main->sleep_tm * 1000);
		print_message(main, diogen, 5);
	}
	return (NULL);
}

//int 	check_amount_eat(t_main *main)
//{
//	t_philo *tmp;
//
//	main->iter = 0;
//	tmp = main->philo_list->head;
//	while (tmp->num <= main->num_philo)
//	{
//		if (main->num_eat == tmp->amount_eat)
//			main->iter++;
//		if (tmp->num == main->num_philo)
//			break ;
//		tmp = tmp->next;
//	}
//	if (main->iter == main->num_philo)
//		return (1);
//	return (0);
//}

void 	*eat_checker(void *args)
{
	t_main		*main;
	t_philo		*tmp;
	int 		i;

	main = (t_main *)args;
	tmp = main->philo_list->head;
	while (1)
	{
		i = -1;
		main->n = 0;
		while (++i < main->num_philo)
		{
			main->n += tmp->amount_eat;
			if (main->n == main->num_philo * main->num_eat)
			{
				print_message(main, tmp, 7);
				pthread_mutex_unlock(&main->death);
				return ((void*)1);
			}
			tmp = tmp->next;
		}
		usleep(500);
	}
}

int 	philo_start(int argc, t_main *main)
{
	t_philo		*tmp;
	pthread_t	eat_check;

	tmp = main->philo_list->head;
	main->iter = 0;
	if (argc == 6)
		if (pthread_create(&eat_check, NULL, eat_checker, (void*)main))
			return (0);
	gettimeofday(&main->tv, NULL);
	main->start_time = (main->tv.tv_sec * 1000) + (main->tv.tv_usec / 1000);
	while (main->iter < main->num_philo)
	{
		if (pthread_create(&tmp->thread, NULL, start_thread, (void*)main))
			return (0);
		pthread_detach(tmp->thread);
		usleep(100);
		tmp = tmp->next;
		main->iter++;
	}
	return (0);
}

int 	main(int argc, char **argv)
{
	t_main	main;
	t_philo	*tmp;

	if (argc < 5 || argc > 6)
		return (1);
	init_struct(&main);
	if (!pars_params(&main, argc, argv))
		return (error_n_exit(&main));
//	main.philo = malloc((sizeof(t_philo) * main.num_philo) + 1);
//	if (!main.philo)
//		return(error_n_exit(&main));
	main.philo_list = ft_calloc(1, sizeof(t_philo_list));
	if (!main.philo_list)
		return(error_n_exit(&main));
	philo_init(&main);
	philo_start(argc, &main);
	pthread_mutex_lock(&main.death);
	pthread_mutex_unlock(&main.death);
	tmp = main.philo_list->head;
	while (tmp)
	{
		pthread_join(tmp->thread, NULL);
		if (tmp->num == main.num_philo)
			break ;
		tmp = tmp->next;
	}
	return (0);
}