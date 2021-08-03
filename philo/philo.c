#include "includes/philo.h"

void    check_if_death(t_philo *philo, struct_philo *phi)
{
    int i;

    while (!(philo->all_ate))
    {
        i = -1;
		while (++i < philo->n_philo && !(philo->died))
		{
			pthread_mutex_lock(&(philo->mutex_meal));
			if (time_diff(phi[i].last_meal, timestamp()) > philo->time_die)
			{
				display_out(philo, i, "died");
				philo->died = 1;
			}
			pthread_mutex_unlock(&(philo->mutex_meal));
			usleep(100);
		}
		if (philo->died)
			break ;
		i = 0;
		while (philo->must_eat != -1 && i < philo->n_philo && phi[i].nb_ate >= philo->must_eat)
			i++;
		if (i == philo->n_philo)
			philo->all_ate = 1;
    }
}

void    join_destroy(t_philo *philo, struct_philo *phi)
{
    int i;

	i = -1;
	while (++i < philo->n_philo)
		pthread_join(phi[i].thread_id, NULL);
	i = -1;
	while (++i < philo->n_philo)
		pthread_mutex_destroy(&(philo->forks[i]));
	pthread_mutex_destroy(&(philo->display));
}

int     launch_program(t_philo *philo)
{
    int     i;
    struct_philo *phi;

    i = 0;
    phi = philo->philosophers;
    philo->start_time = timestamp();
    while (i < philo->n_philo)
	{
		if (pthread_create(&(phi[i].thread_id), NULL, life, &(phi[i])))
        {
            philo->error = LAUNCH;
			return (1); 
        }
		phi[i].last_meal = timestamp();
		i++;
	}
    check_if_death(philo, philo->philosophers);
    join_destroy(philo, phi);
    return (0);
}

int ft_alone(t_philo *philo)
{
    if (philo->n_philo == 1)
    {
        printf("0 1 has taken a fork\n");
        usleep(philo->time_die * 1000);
        printf("%d 1 is dead\n", philo->time_die);
        return (1);
    }
	return (0);
}

int main(int ac, char **av)
{
    t_philo philo;

    if (get_arg(&philo, ac, av) != 0)
    {
        ft_error(&philo);
        return (1);
    }
    if (init_mutex(&philo) != 0)
    {
        ft_error(&philo);
        return (1);
    }
    if (ft_alone(&philo))
        return (1);
    get_philosophers(&philo);
    if (launch_program(&philo))
    {
        ft_error(&philo);
        return (1);
    }
    return (0);
}