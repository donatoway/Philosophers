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

long long time_diff(long long past, long long pres)
{
    return (pres - past);
}

long long	timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void    ft_error(t_philo *philo)
{
    if (philo->error == ARG_NUM)
        write(2, "ERROR: Wrong number of arg\n", 28);
    if (philo->error == ARG_NOT_NUM)
        write(2, "ERROR: arg not number\n", 23);
    if (philo->error == MUST_EAT)
        write(2, "ERROR: number to must eat wrong\n",33);
    if (philo->error == PHILO_NUM)
        write(2, "ERROR: wrong number of philosophers\n",37);
    if (philo->error == MUTEX)
        write (2, "ERROR: MUTEX\n", 14);
    if (philo->error == LAUNCH)
        write(2, "ERROR: to launch program\n", 26);
    if (philo->error == INIT)
        write(2, "ERROR: initialization\n", 23);
}


