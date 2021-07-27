#include "philosophers.h"

static int	init_struct(t_main *main)
{
	main->num_philo = -1;
	main->die_tm = -1;
	main->eat_tm = -1;
	main->sleep_tm = -1;
	main->num_eat = -1;
	main->philo_list = NULL;
	return (1);
}

static int	pars_params(t_main *main, int argc, char **argv)
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
		if ((main->num_eat == 0 && ft_strlen(argv[5]) != 1 && argv[5][0]
			!= '0') || main->num_eat <= 0 || main->num_eat == 0)
			return (0);
	}
	if (main->num_philo <= 0 || main->die_tm <= 0 || main->eat_tm <= 0
		|| main->sleep_tm <= 0)
		return (0);
	return (1);
}

static int	philo_init(t_main *main)
{
	int		i;
	char	str[10];

	i = 1;
	if (!semaphore_init(main))
		return (0);
	while (i <= main->num_philo)
	{
		ft_bzero(str, 10);
		ft_dlist_push_back(main->philo_list, i);
		main->philo_list->tail->last_eat = 0;
		main->philo_list->tail->eat_time = 0;
		main->philo_list->tail->alive = 1;
		sem_name(str, main->philo_list->tail->num);
		sem_unlink(str);
		main->philo_list->tail->check = sem_open(str, O_CREAT | O_EXCL,
				0666, 1);
		if (main->philo_list->tail->check < 0
			|| main->philo_list->tail->eat_time < 0)
			return (0);
		i++;
	}
	return (1);
}

static int	philo_start(int argc, t_main *main)
{
	t_philo		*tmp;
	pthread_t	eat_check;

	tmp = main->philo_list->head;
	if (argc == 6)
		if (pthread_create(&eat_check, NULL, eat_checker, (void *)main))
			return (0);
	if (argc == 6)
		pthread_detach(eat_check);
	gettimeofday(&main->tv, NULL);
	main->start_time = (main->tv.tv_sec * 1000) + (main->tv.tv_usec / 1000);
	while (++main->iter < main->num_philo)
	{
		tmp->pid = fork();
		if (tmp->pid < 0)
			return (0);
		if (tmp->pid == 0)
		{
			start_fork(main, tmp);
			exit(0);
		}
		tmp = tmp->next;
		usleep(300);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_main	main;
	t_philo	*tmp;

	if (argc < 5 || argc > 6)
		return (1);
	init_struct(&main);
	if (!pars_params(&main, argc, argv))
		return (clean_n_exit(&main, 0));
	main.philo_list = ft_calloc(1, sizeof(t_philo_list));
	if (!main.philo_list)
		return (clean_n_exit(&main, 0));
	main.iter = -1;
	if (!philo_init(&main) || !philo_start(argc, &main))
		return (clean_n_exit(&main, 1));
	sem_wait(main.death);
	tmp = main.philo_list->head;
	while (tmp)
	{
		kill(tmp->pid, SIGINT);
		if (tmp->num == main.num_philo)
			break ;
		tmp = tmp->next;
	}
	clean_n_exit(&main, 2);
	return (0);
}
