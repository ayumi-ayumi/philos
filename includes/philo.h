/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ayumi <Ayumi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 13:26:44 by asato             #+#    #+#             */
/*   Updated: 2026/04/05 09:32:34 by Ayumi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
# include <pthread.h>

typedef struct s_data t_data;
typedef struct s_philo t_philo;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	t_data			*data;
	long long		last_meal_time_ms;
	int				meals_eaten;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
}				t_philo;

typedef struct s_data
{
	int					philo_count;
	int					time_to_die_ms;
	int					time_to_eat_ms;
	int					time_to_sleep_ms;
	int					must_eat_count;
	int					fork_count;
	long long			start_time_ms;
	int					stop_flag;
	pthread_mutex_t		stop_mutex;
	pthread_mutex_t		meal_mutex;
	pthread_mutex_t		print_lock;
	pthread_mutex_t		*fork_mutex;
	t_philo				*philos;
}				t_data;

# define ERR_INPUT "Invalid input\n"
# define ERR_ARGS "Usage: ./philo <number_of_philosophers> <time_to_die> \
<time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n"
# define ERR_INIT_DATA "Data structure initialization failed\n"
# define ERR_INIT_PHILO "Philosopher structure initialization failed\n"
# define ERR_THREAD "Failed threading"

# define TAKE_FORK "has taken a fork\n"
# define EAT "is eating\n"
# define SLEEP "is sleeping\n"
# define THINK "is thinking\n"
# define DIED "died\n"

/* Init */
bool		init_data(t_data *data, char **av);
bool		init_mutex(t_data *data);
bool		init_philos(t_data *data);

/* Threads */
bool		start_threads(t_data *data);
void		*routine(void *arg);

/* Actions */
int			eat(t_philo *philo);
void		take_forks(t_philo *philo);
void		release_forks(t_philo *philo);
int			rest(t_philo *philo);
void		think(t_philo *philo);

/* Monitor */
void		*monitor_loop(void *arg);
int			is_stopped(t_philo *philo);
int			all_philos_ate_enough(t_data *data);
int			has_philo_died(t_data *data);

/* libft */
size_t		ft_strlen(const char *s);
int			ft_isdigit(int c);
int			ft_sleep(t_philo *philo, long long milliseconds);
int			ft_atoi(char *str);

/* Utils*/
long long	get_current_time(void);
int			is_stopped(t_philo *philo);
long long	get_timestamp(t_data *data);

/* Clean Up*/
void		cleanup(t_data *data);
void		destroy_mutex(t_data *data);

#endif
