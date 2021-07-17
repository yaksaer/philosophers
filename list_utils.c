#include "philosophers.h"

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
//	if (list->tail)
//	{
//		list->tail->next = tmp;
//		tmp->prev = list->tail;
//	}
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
