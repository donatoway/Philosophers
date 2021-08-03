#include "includes/philo_bonus.h"

int			ft_atoi(const char *str)
{
	long int	n;
	int			sign;

	n = 0;
	sign = 1;
	while ((*str <= 13 && *str >= 9) || *str == 32)
		str++;
	if (*str == '-')
		return (-1);
	else if (*str == '+')
		str++;
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			n = n * 10 + (*str++ - '0');
		else
			return (-1);
	}
	return ((int)(n * sign));
}

void    display_out(t_philo *philo, int id, char *str)
{
    sem_wait(philo->display);
	if (philo->died == 0)
	{
		printf("%lli ", timestamp() - philo->start_time);
		printf("%i ", id + 1);
		printf("%s\n", str);
	}
	sem_post(philo->display);
    return ;
}
