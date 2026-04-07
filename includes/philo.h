/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 13:26:44 by asato             #+#    #+#             */
/*   Updated: 2026/04/07 09:48:10 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;
typedef struct s_philo	t_philo;

typedef struct s_philo
{
	pthread_t			thread;
	int					id;
	t_data				*data;
	long long			last_meal_time_ms;
	int					meals_eaten;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		*left_fork;
}						t_philo;

typedef struct s_data
{
	int					philo_count;
	int					time_to_die_ms;
	int					time_to_eat_ms;
	int					time_to_sleep_ms;
	int					must_eat_count;
	int					fork_count;
	long long			start_time_ms;
	bool				stop_flag;
	pthread_mutex_t		stop_mutex;
	pthread_mutex_t		meal_mutex;
	pthread_mutex_t		print_lock;
	pthread_mutex_t		*fork_mutex;
	t_philo				*philos;
}						t_data;

typedef enum e_routine_loop
{
	STOP = 1,
	CONTINUE = 0
}						t_routine_loop;
/* Error messages */
# define ERR_INPUT "Input must be positive integers\n"
# define ERR_ARGS \
	"Usage: ./philo <number_of_philosophers> <time_to_die> \
<time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n"
# define ERR_INIT_DATA "Data structure initialization failed\n"
# define ERR_INIT_PHILO "Philosopher structure initialization failed\n"
# define ERR_THREAD "Failed threading"

/* Macros for actions */
# define TAKE_FORK "has taken a fork\n"
# define EAT "is eating\n"
# define SLEEP "is sleeping\n"
# define THINK "is thinking\n"
# define DIED "died\n"

/* Init */
bool					init_data(t_data *data, char **av);
bool					init_philos(t_data *data);

/* Threads */
bool					start_threads(t_data *data);

/* Monitor */
void					*monitor_loop(void *arg);

/* Actions */
t_routine_loop			eat(t_philo *philo);
void					release_forks(t_philo *philo);
void					take_forks(t_philo *philo);
void					single_philo_routine(t_philo *philo);
t_routine_loop			rest(t_philo *philo);
void					think(t_philo *philo);
void					print_log(t_philo *philo, char *msg);

/* libft */
size_t					ft_strlen(const char *s);
bool					ft_isdigit(int c);
int						ft_atoi(char *str);
t_routine_loop			ft_sleep(t_philo *philo, long long milliseconds);

/* Utils*/
long long				get_timestamp(t_data *data);
long long				get_current_time(void);
t_routine_loop			is_stopped(t_philo *philo);

/* Clean Up*/
void					cleanup(t_data *data);
void					destroy_mutex(t_data *data);

#endif
