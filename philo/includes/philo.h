# ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>

struct s_philo;

typedef struct s_struct_philo
{  
    int         id;
    int         nb_ate;
    int         left_fork;
    int         right_fork;
    long long   last_meal;
    struct s_philo *t_philo;
    pthread_t    thread_id;  
    
}               struct_philo;

typedef enum 
{
    ARG_NUM = 100,
    ARG_NOT_NUM = 102,
    MUST_EAT = 103,
    PHILO_NUM = 104,
    MUTEX = 105,
    LAUNCH = 106
}           err;

typedef struct  s_philo
{
    int         error;
    int         n_philo;
    int         time_die;
    int         time_eat;
    int         time_sleep;
    int         must_eat;
    int         died;
    int         all_ate;
    long long   start_time;
    pthread_mutex_t mutex_meal;
    pthread_mutex_t forks[250];
    pthread_mutex_t display;
    struct_philo philosophers[250];

}               t_philo;

int			ft_atoi(const char *str);
void    get_philosophers(t_philo *philo);
int    init_mutex(t_philo *philo);
int     get_arg(t_philo *philo, int ac, char **av);
void    display_out(t_philo *philo, int id, char *str);
void    ft_error(t_philo *philo);
long long time_diff(long long past, long long pres);
long long	timestamp(void);
void    *life(void  *philosopher);


# endif