#include "includes/philo_bonus.h"


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
        if (philo->died == 1)
            break;
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


    i = 0;
	while (i < philo->n_philo)
	{
		waitpid(-1, &ret, 0);
		if (ret != 0)
		{
			i = -1;
			while (++i < philo->n_philo)
				kill(philo->philosophers[i].pid_id, SIGTERM);
			break ;
		}
		i++;
	}
    close_program(philo);
    return (0);
}

int     init_all(t_philo *philo)
{
    int i;

    i = philo->n_philo;
    while (--i >= 0)
    {
        philo->philosophers[i].nb_ate = 0;
		philo->philosophers[i].last_meal = 0;
		philo->philosophers[i].t_philo = philo;
        philo->philosophers[i].id = i;
    }
    
    sem_unlink("/forks");
    sem_unlink("/display");
    sem_unlink("/check_meal");
    philo->forks = sem_open("/forks", O_CREAT, S_IRWXU, philo->n_philo);
    philo->display = sem_open("/display", O_CREAT, S_IRWXU, 1);
    philo->check_meal = sem_open("/check_meal", O_CREAT, S_IRWXU, 1);
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
    if (init_all(&philo))
    {
        ft_error(&philo);
        return (1);
    }
    if (launch_program(&philo))
    {
        philo.error = LAUNCH;
        ft_error(&philo);
        return (1);
    }
    return (0);
}