#include "philosophers.h"

void	ft_usleep(unsigned int time)
{
	unsigned int 	n;
	int 			i;

	i = -1;
	n = time / 10;
	while (++i < 10)
		usleep(n * 1000);
}

int 	print_message(t_main *main, t_philo *diogen, int flag)
{
	pthread_mutex_lock(&main->print);
	if (main->death_of_diogen == 1)
		return (0);
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
	else if (flag == 7)
		printf("%s%ums The philosophers are full%s\n", CYAN,
			   calculate_time(main), RESET);
//	else if (flag == 6)
//		printf("%s%ums %d is die%s\n", RED, calculate_time(main), diogen->num,
//			   RESET);
	pthread_mutex_unlock(&main->print);
	return (1);
}

unsigned int	calculate_time(t_main *main)
{
	unsigned int	curr_time;

	gettimeofday(&main->tv, NULL);
	curr_time = ((main->tv.tv_sec * 1000) + (main->tv.tv_usec / 1000))
		- main->start_time;
	return (curr_time);
}

void	ft_dlist_del(t_philo_list **list, int size)
{
	t_philo	*tmp;
	t_philo	*next;
	int 	i;

	tmp = (*list)->head;
	next = NULL;
	i = -1;
	while (++i < size)
	{
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	free(*list);
	(*list) = NULL;
}

t_philo	*ft_dlist_get_n(t_philo_list *list, size_t index)
{
	t_philo	*tmp;
	size_t	i;

	tmp = NULL;
	if (index < list->size / 2)
	{
		i = 0;
		tmp = list->head;
		while (tmp && i < index)
		{
			tmp = tmp->next;
			i++;
		}
	}
	else
	{
		i = list->size - 1;
		tmp = list->tail;
		while (tmp && i > index)
		{
			tmp = tmp->prev;
			i--;
		}
	}
	return (tmp);
}

void	ft_dlist_push_back(t_philo_list *list, int num)
{
	t_philo	*tmp;

	tmp = (t_philo *)malloc(sizeof(t_philo));
	if (tmp == NULL)
		return ;
	tmp->num = num;
	if (list->head == NULL)
		list->head = tmp;
	tmp->next = list->head;
	if (list->tail == NULL)
		list->tail = tmp;
	else if (list->tail)
	{
		list->tail->next = tmp;
		tmp->prev = list->tail;
		list->tail = tmp;
	}
	list->head->prev = list->tail;
	list->size++;
}
