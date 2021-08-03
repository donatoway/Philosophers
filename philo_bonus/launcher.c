#include "includes/philo_bonus.h"

void    philo_eat(struct_philo *phi_str, t_philo *philo)
{
    long long t;
    sem_wait(philo->forks);
    display_out(philo, phi_str->id, "get left fork");
    sem_wait(philo->forks);
    display_out(philo, phi_str->id, "get right fork");
    sem_wait(philo->check_meal);
    display_out(philo, phi_str->id, "eating");
    sem_post(philo->check_meal);
    t = timestamp();
    while (philo->died == 0)
    {
        if (time_diff(t, timestamp()) >= philo->time_eat)
            break;
        usleep(50);
    }
    phi_str->nb_ate++;
    sem_post(philo->forks);
    phi_str->last_meal = timestamp();
    sem_post(philo->forks);
}

void    *check_death(void   *philosopher)
{
    struct_philo *phi_str;
    t_philo *philo;

    phi_str = (struct_philo*)philosopher;
    philo = phi_str->t_philo;
    while (1)
    {
        sem_wait(philo->check_meal);
        if (time_diff(phi_str->last_meal, timestamp()) > philo->time_die)
        {
            display_out(philo, phi_str->id, "is died");
            philo->died = 1;
            sem_wait(philo->display);
            exit(1);
        }
        sem_post(philo->check_meal);
        if (philo->must_eat != -1 && phi_str->nb_ate >= philo->must_eat)
        {
            philo->all_ate = 1;
            break;
        }
        usleep(1000);
    }
    exit(0);
    return (NULL);
}

void    philo_sleep(struct_philo *phi_str, t_philo *philo)
{
    long long t;

    t = timestamp();
    while (philo->died == 0)
    {
        if (time_diff(t, timestamp()) >= philo->time_sleep)
            break;
        usleep(50);
    }
}

void    life(void *phi_str, t_philo *philo)
{
    struct_philo *phi;

    phi = (struct_philo*)phi_str;
    phi->last_meal = timestamp();
    pthread_create(&(phi->thread_id), NULL, check_death, phi_str);
    if (phi->id % 2)
        usleep(15000);
    while (philo->died == 0 && philo->all_ate != 1)
    {
        philo_eat(phi, philo);
        if (phi->nb_ate >= philo->must_eat && philo->must_eat != -1)
            break;
        display_out(philo, phi->id, "sleeping");
        philo_sleep(phi, philo);
        display_out(philo, phi->id, "thinking");
    }
    pthread_join(phi->thread_id, NULL);
    if (philo->died)
		exit(1);
	exit(0);
}

void    close_program(t_philo *philo)
{
    int i;
    int ret;

    i = -1;
	while (++i < philo->n_philo)
	{
		waitpid(-1, &ret, 0);
		if (ret != 0)
		{
			i = -1;
			while (++i < philo->n_philo)
				kill(philo->philosophers[i].pid_id, SIGTERM);
			break ;
		}
	}
    i = -1;
    sem_close(philo->forks);
    sem_close(philo->display);
    sem_close(philo->check_meal);
    sem_unlink("/philo_forks");
	sem_unlink("/philo_write");
	sem_unlink("/philo_mealcheck");
}

int     launch_program(t_philo *philo)
{
    struct_philo *phi_str;
    int         i;

    i = -1;
    phi_str = philo->philosophers;
    philo->start_time = timestamp();
    while (++i < philo->n_philo)
    {
        phi_str[i].pid_id = fork();
        if (phi_str[i].pid_id < 0)
            return (1);
        if (phi_str[i].pid_id == 0)
            life(&(phi_str[i]), philo);
        usleep(100);
    }
    int ret;
    close_program(philo);
    return (0);
}