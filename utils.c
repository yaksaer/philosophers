#include "philosophers.h"

void	*ft_bzero(void *s, size_t n)
{
	unsigned char	*b;
	unsigned int	i;

	b = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		b[i] = 0;
		i++;
	}
	return (b);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*mem;

	mem = (unsigned char *)malloc(nmemb * size);
	if (!mem)
		return (NULL);
	mem = ft_bzero(mem, nmemb * size);
	return (mem);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}

int	ft_isspace(const char *buff, int j)
{
	while (buff[j] == ' ' || buff[j] == '\f' || buff[j] == '\n'
		   || buff[j] == '\r' || buff[j] == '\t' || buff[j] == '\v')
	{
		j++;
	}
	return (j);
}

int	ft_atoi(const char *nptr)
{
	size_t			i;
	unsigned long	num;
	unsigned long	max;
	int				zn;

	num = 0;
	zn = 1;
	max = (unsigned long)(INT32_MAX / 10);
	i = ft_isspace(nptr, 0);
	if (nptr[i] == '-')
		zn = -1;
	if (nptr[i] == '-' || nptr[i] == '+')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if ((num > max || (num == max && (nptr[i] - '0') > 7))
			&& zn == 1)
			return (0);
		else if ((num > max || (num == max && (nptr[i] - '0') > 8))
			&& zn == -1)
			return (0);
		num = (10 * num) + (nptr[i] - '0');
		i++;
	}
	return ((int)(num * zn));
}
