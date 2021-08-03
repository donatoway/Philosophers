# ifndef PHILO_BONUS_H
#define PHILO_BONUS_H

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <signal.h>

struct              s_philo;

typedef struct      s_struct_philo
{  
    pid_t           pid_id;
    int             id;
    int             nb_ate;
    int             left_fork;
    int             right_fork;
    long long       last_meal;
    struct s_philo  *t_philo;
     pthread_t      thread_id; 
    
}                   struct_philo;

typedef struct      s_philo
{
    int             error;
    int             n_philo;
    int             time_die;
    int             time_eat;
    int             time_sleep;
    int             must_eat;
    int             died;
    int             all_ate;
    long long       start_time;
    struct_philo    philosophers[250];
    sem_t           *forks;
    sem_t           *display;
    sem_t           *check_meal;

}                   t_philo;

typedef enum 
{
    ARG_NUM =       100,
    ARG_NOT_NUM =   102,
    MUST_EAT =      103,
    PHILO_NUM =     104,
    MUTEX =         105,
    LAUNCH =        106,
    INIT =          107
}                   err;

int			        ft_atoi(const char *str);
void                display_out(t_philo *philo, int id, char *str);
long long	        timestamp(void);
void                ft_error(t_philo *philo);
long long	        timestamp(void);
long long           time_diff(long long past, long long pres);
int                 get_arg(t_philo *philo, int ac, char **av);
int                 launch_program(t_philo *philo);

#endif

