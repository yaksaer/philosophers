#include "philosophers.h"

int 	clean_n_exit(t_main *main, int flag)
{
	if (main->philo_list)
		ft_dlist_del(&main->philo_list, main->num_philo);
	if (flag > 0)
	{
		pthread_mutex_destroy(&main->death);
		pthread_mutex_destroy(&main->print);
		pthread_mutex_destroy(&main->queue);
	}
	else if (flag == 1 || flag == 0)
		printf("%sError!%s\n", RED, RESET);
	return (1);
}
