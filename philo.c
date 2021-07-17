#include "philosophers.h"

int 	error_n_exit(t_main *main)
{
	printf("%sWrong params!%s\n", RED, RESET);
	return (1);
}

int 	init_struct(t_main *main)
{
	main->num_philo = -1;
	main->die_tm = -1;
	main->eat_tm = -1;
	main->sleep_tm = -1;
	main->num_eat = -1;
	main->who_die = 0;
	return (1);
}

unsigned int	calculate_time(t_main *main)
{
	gettimeofday(&main->tv, NULL);
	main->curr_time = ((main->tv.tv_sec * 1000) + (main->tv.tv_usec / 1000)) -
					   main->start_time;
	return (main->curr_time);
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
		&& argv[5][0] != '0') || main->num_eat < 0)
			return (0);
	}
	if (main->num_philo < 0 || main->die_tm < 0 || main->eat_tm < 0
		|| main->sleep_tm < 0)
		return (0);
	return (1);
}

int 	check_death(t_main *main, t_philo *diogen)
{
	if (diogen->amount_eat == main->num_eat)
		return (0);
	if (calculate_time(main) - diogen->last_eat > main->die_tm)
	{
		printf("%s%dms %d is die%s\n", RED, calculate_time(main), diogen->num,
			   RESET);
		diogen->alive = 0;
		main->who_die += 1;
		return (0);
	}
	return (1);
}

int 	philo_init(t_main *main)
{
	int 	i;

	i = 1;
	while (i <= main->num_philo)
	{
		ft_dlist_push_back(main->philo_list, i);
		if (main->num_eat > 0)
			main->philo_list->tail->amount_eat = 0;
		main->philo_list->tail->alive = 1;
		main->philo_list->tail->last_eat = 0;
		pthread_mutex_init(&main->philo_list->tail->fork, NULL);
		i++;
	}
	return (1);
}

void 	eating(t_main *main, t_philo *diogen)
{
	if (diogen->num % 2 != 0)
		usleep(170 * 1000);
	pthread_mutex_lock(&diogen->fork);
	printf("%s%dms %d take right fork%s\n", GREEN, calculate_time(main),
		   diogen->num, RESET);
	if (main->num_philo == 1)
	{
		usleep((main->die_tm + 1) * 1000);
		return;
	}
	pthread_mutex_lock(&diogen->next->fork);
	printf("%s%dms %d take left fork%s\n", GREEN, calculate_time(main),
		   diogen->num, RESET);
	diogen->last_eat = calculate_time(main);
	printf("%s%dms %d is eating%s\n", CYAN, calculate_time(main), diogen->num,
		   RESET);
	usleep(main->eat_tm * 1000);
	diogen->amount_eat++;
	pthread_mutex_unlock(&diogen->fork);
	printf("%s%dms %d put right fork%s\n", BLUE, calculate_time(main),
		   diogen->num, RESET);
	pthread_mutex_unlock(&diogen->next->fork);
	printf("%s%dms %d put left fork%s\n", BLUE, calculate_time(main),
		   diogen->num, RESET);
}

void 	*start_thread(void *args)
{
	t_main main;
	t_philo	*diogen;

	main = *(t_main *)args;
	diogen = ft_dlist_get_n(main.philo_list, main.iter - 1);
	while (check_death(&main, diogen))
	{
		//pthread_join(diogen->next->thread, NULL);
		if (!check_death(&main, diogen))
			break ;
		eating(&main, diogen);
		if (!check_death(&main, diogen))
			break ;
		printf("%s%dms %d is sleeping%s\n", CYAN, calculate_time(&main),
			   diogen->num, RESET);
		usleep(main.sleep_tm * 1000);
		printf("%s%dms %d is thinking%s\n", MAGENTA, calculate_time(&main),
			   diogen->num, RESET);
		while (calculate_time(&main) - diogen->last_eat > main.die_tm + main.eat_tm)
			usleep(1000);
	}
	return (NULL);
}

int 	check_amount_eat(t_main *main)
{
	t_philo *tmp;

	main->iter = 0;
	tmp = main->philo_list->head;
	while (tmp->num <= main->num_philo)
	{
		if (main->num_eat == tmp->amount_eat)
			main->iter++;
		if (tmp->num == main->num_philo)
			break ;
		tmp = tmp->next;
	}
	if (main->iter == main->num_philo)
		return (1);
	return (0);
}

int 	philo_start(t_main *main)
{
	t_philo		*tmp;

	tmp = main->philo_list->head;
	main->iter = 1;
	gettimeofday(&main->tv, NULL);
	main->start_time = (main->tv.tv_sec * 1000) + (main->tv.tv_usec / 1000);
	while (tmp->num <= main->num_philo)
	{
		pthread_create(&tmp->thread, NULL, start_thread, (void*)main);
		pthread_detach(tmp->thread);
		if (tmp->num == main->num_philo)
			break ;
		tmp = tmp->next;
		main->iter++;
	}
	tmp = main->philo_list->head;
	while (tmp->alive == 1)
	{
		if (check_amount_eat(main))
			break ;
		tmp = tmp->next;
	}
//	tmp = main->philo_list->head;
//	while (tmp->num <= main->num_philo)
//	{
//		free(tmp->thread);
//		tmp = tmp->next;
//	}
	return (0);
}

int 	main(int argc, char **argv)
{
	t_main	main;

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
	philo_start(&main);
	return (0);
}