#include "includes/philo.h"

void    philo_eat(struct_philo *phi_str, t_philo *philo)
{
    long long i;

    pthread_mutex_lock(&(philo->forks[phi_str->left_fork]));
	display_out(philo, phi_str->id, "has taken a fork");
	pthread_mutex_lock(&(philo->forks[phi_str->right_fork]));
	display_out(philo, phi_str->id, "has taken a fork");
	pthread_mutex_lock(&(philo->mutex_meal));
	display_out(philo, phi_str->id, "is eating");
	phi_str->last_meal = timestamp();
	pthread_mutex_unlock(&(philo->mutex_meal));
    i = timestamp();
	while (!(philo->died))
    {
        if (time_diff(i, timestamp()) >= philo->time_eat)
            break;
        usleep(50);
    }
	(phi_str->nb_ate)++;
	pthread_mutex_unlock(&(philo->forks[phi_str->left_fork]));
	pthread_mutex_unlock(&(philo->forks[phi_str->right_fork]));
}

void    philo_sleep(t_philo *philo, struct_philo *p)
{
    long long i;

    display_out(philo, p->id, "sleeping");
    i = timestamp();
    while (!(philo->died))
    {
        if (time_diff(i, timestamp()) >= philo->time_sleep)
            break;
        usleep(50);
    }
}

void    *life(void  *philosopher)
{
    struct_philo    *phi_str;
    t_philo         *philo;

    phi_str = (struct_philo*)philosopher;
    philo = phi_str->t_philo;
    if (phi_str->id % 2)
        usleep(15000);
    while (!(philo->died))
    {
        philo_eat(phi_str, philo);
        if (philo->all_ate)
            break;
        philo_sleep(philo, phi_str);
        display_out(philo, phi_str->id, " eating");
    }
    return (NULL);
}