#include "includes/philo.h"

void    check_if_nb(t_philo *philo, char **av)
{
    int i;
    int j;

    i = 1;
    while (av[i])
    {
        j = 0;
        while (av[i][j])
        {
            if (av[i][j] >= '0' && av[i][j] <= '9')
                j++;
            else
            {
                philo->error = ARG_NOT_NUM;
                break;
            }
        }
        i++; 
    }
}

void    init_arg(t_philo *philo, char **av)
{
    philo->n_philo = ft_atoi(av[1]);
    if (philo->n_philo > 250 || philo->n_philo < 1)
        philo->error = PHILO_NUM;
    philo->time_die = ft_atoi(av[2]);
    philo->time_eat = ft_atoi(av[3]);
    philo->time_sleep = ft_atoi(av[4]);
    if (av[5])
    {
        philo->must_eat = ft_atoi(av[5]);
        if (philo->must_eat <= 0)
            philo->error = MUST_EAT;
    }
    else
        philo->must_eat = -1;
    philo->died = 0;
    philo->all_ate = 0;
}

int     get_arg(t_philo *philo, int ac, char **av)
{
    philo->error = 0;
    if (ac != 5 && ac != 6)
        philo->error = ARG_NUM;
    check_if_nb(philo, av);
    if (philo->error != 0)
        return (1);
    else
        init_arg(philo, av);
    return (0);
}

int    init_mutex(t_philo *philo)
{
    int i;

    i = philo->n_philo;
    while (--i >= 0)
	{
		if (pthread_mutex_init(&(philo->forks[i]), NULL))
			return (1);
	}
	if (pthread_mutex_init(&(philo->display), NULL))
		return (1);
	if (pthread_mutex_init(&(philo->mutex_meal), NULL))
		return (1);
	return (0);
}

void    get_philosophers(t_philo *philo)
{
    int i;

	i = philo->n_philo;
	while (--i >= 0)
	{
		philo->philosophers[i].id = i;
		philo->philosophers[i].nb_ate = 0;
		philo->philosophers[i].left_fork = i;
		philo->philosophers[i].right_fork = (i + 1) % philo->n_philo;
		philo->philosophers[i].last_meal = 0;
		philo->philosophers[i].t_philo = philo;
	}
}