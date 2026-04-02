/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ayumi <Ayumi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 13:29:01 by asato             #+#    #+#             */
/*   Updated: 2026/04/02 22:36:44 by Ayumi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

typedef struct s_data t_data;
typedef struct s_philo t_philo;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	t_data			*data;
	long long		last_time_to_eat;
	int				meal_count;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
}				t_philo;

typedef struct s_data
{
	int				number_of_philos;
	int				time_to_die_ms;
	int				time_to_eat_ms;
	int				time_to_sleep_ms;
	int				number_of_eat;
	int				number_of_fork;
	int				stop;
	long long		start_time;
	pthread_mutex_t		*dead_mutex;
	pthread_mutex_t		*meal_mutex;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print_status;
	t_philo			*philos;
}				t_data;

long long	get_current_time() //us
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000000LL + time.tv_usec);
}

long long	get_timestamp(t_data data) //ms
{
	return ((get_current_time() - data.start_time) / 1000LL);
}

int	is_stopped(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(data->dead_mutex);
	if (data->stop == 1)
		return (pthread_mutex_unlock(data->dead_mutex), 1);
	pthread_mutex_unlock(data->dead_mutex);
	return (0);
}

void print_timestamp(t_philo *philo, char *msg)
{
	if (is_stopped(philo) == 0)
	{
		pthread_mutex_lock(philo->data->print_status);
		printf("%lld %d %s", get_timestamp(*philo->data), philo->id, msg);
		pthread_mutex_unlock(philo->data->print_status);
	}
}

int	ft_usleep_and_check_dead(t_philo *philo, int time)
{
	long long	start;
	
	start = get_current_time();
	while (get_current_time() - start < (long long)time * 1000LL)
	{
		if (is_stopped(philo) == 1)
			return (1);
		usleep(500);
	}
	return (0);
}

int	all_eat_enough(t_data *data)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (data->number_of_eat > 0)
	{
		while (i < data->number_of_philos)
		{
			pthread_mutex_lock(data->meal_mutex);
			if (data->philos[i].meal_count >= data->number_of_eat)
				count++;
			pthread_mutex_unlock(data->meal_mutex);
			i++;
		}
		if (count == data->number_of_philos)
		{
			pthread_mutex_lock(data->dead_mutex);
			if (data->stop == 0)
				data->stop = 1;
			pthread_mutex_unlock(data->dead_mutex);
			return (1);
		}
	}
	return (0);
}

int	is_someone_dead(t_data *data)
{
	int i;

	i = 0;
	while (i < data->number_of_philos)
	{
		pthread_mutex_lock(data->meal_mutex);
		if (get_current_time() - data->philos[i].last_time_to_eat >= data->time_to_die_ms * 1000LL)
		{
			pthread_mutex_unlock(data->meal_mutex);
			pthread_mutex_lock(data->dead_mutex);
			if (data->stop == 0)
			{
				data->stop = 1;
				pthread_mutex_lock(data->print_status);
				printf("%lld %d %s", get_timestamp(*data), data->philos[i].id, "died\n");
				pthread_mutex_unlock(data->print_status);
			}
			pthread_mutex_unlock(data->dead_mutex);
			return (1);
		}
		pthread_mutex_unlock(data->meal_mutex);
		i++;
	}
	return (0);
}

void *monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (is_someone_dead(data) || all_eat_enough(data))
			break ;
		usleep(500);
	}
	return (NULL);
}

int	handle_one_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_timestamp(philo, "has taken a fork\n");
	ft_usleep_and_check_dead(philo, philo->data->time_to_eat_ms);
	pthread_mutex_unlock(philo->left_fork);
	return (1);
}

int	pick_forks(t_philo *philo)
{
	int	val;
	val = 0;

	if (philo->data->number_of_philos == 1)
		return (handle_one_philo(philo));
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		// if (is_stopped(philo) == 1)
		// 	return (pthread_mutex_unlock(philo->left_fork), 1);
		print_timestamp(philo, "has taken a fork\n");
		pthread_mutex_lock(philo->right_fork);
		// if (is_stopped(philo) == 1)
		// 	return (pthread_mutex_unlock(philo->right_fork), pthread_mutex_unlock(philo->left_fork), 1);
		print_timestamp(philo, "has taken a fork\n");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		// if (is_stopped(philo) == 1)
		// 	return (pthread_mutex_unlock(philo->right_fork), 1);
		print_timestamp(philo, "has taken a fork\n");
		pthread_mutex_lock(philo->left_fork);
		// if (is_stopped(philo) == 1)
			// return (pthread_mutex_unlock(philo->left_fork), pthread_mutex_unlock(philo->right_fork), 1);
		print_timestamp(philo, "has taken a fork\n");
	}
	return (val);
}

void	put_down_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

int	eat(t_philo *philo)
{
	if (pick_forks(philo) == 1)
		return (1);
	print_timestamp(philo, "is eating\n");
	pthread_mutex_lock(philo->data->meal_mutex);
	philo->last_time_to_eat = get_current_time();
	pthread_mutex_unlock(philo->data->meal_mutex);
	if (ft_usleep_and_check_dead(philo, philo->data->time_to_eat_ms) == 1)
		return (put_down_forks(philo), 1);
	put_down_forks(philo);
	pthread_mutex_lock(philo->data->meal_mutex);
	philo->meal_count ++;
	pthread_mutex_unlock(philo->data->meal_mutex);
	return (0);
}

int act_sleep(t_philo *philo)
{
	print_timestamp(philo, "is sleeping\n");
	if (ft_usleep_and_check_dead(philo, philo->data->time_to_sleep_ms) == 1)
		return (1);
	return (0);
}

void	think(t_philo *philo)
{
	print_timestamp(philo, "is thinking\n");
}


void *routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	if (philo->id % 2 != 0)
		usleep(500);
	while (is_stopped(philo) == 0)
	{
		if (eat(philo) == 1)
			break ;
		if (act_sleep(philo) == 1)
			break ;
		think(philo);
		usleep(500);
	}
	return (NULL);
}

bool	validate_argc(int ac)
{
	if (ac == 5 || ac ==  6)
		return (true);
	return (false);
}

bool	init_data(t_data *data, int ac, char **av)
{
	data->number_of_philos = atoi(av[0]);
	data->time_to_die_ms = atoi(av[1]);
	data->time_to_eat_ms = atoi(av[2]);
	data->time_to_sleep_ms = atoi(av[3]);
	if (ac == 6)
		data->number_of_eat = atoi(av[4]);
	else
		data->number_of_eat = -1;
	data->number_of_fork = data->number_of_philos;
	data->forks = malloc(data->number_of_fork * sizeof(pthread_mutex_t));
	if (!data->forks)
		return (false);
	data->print_status = malloc(sizeof(pthread_mutex_t));
	if (!data->print_status)
		return (false);
	pthread_mutex_init(data->print_status, NULL);
	
	data->dead_mutex = malloc(sizeof(pthread_mutex_t));
	if (!data->dead_mutex)
		return (false);
	pthread_mutex_init(data->dead_mutex, NULL);

	data->meal_mutex = malloc(sizeof(pthread_mutex_t));
	if (!data->meal_mutex)
		return (false);
	pthread_mutex_init(data->meal_mutex, NULL);
	
	data->start_time = get_current_time();
	data->stop = 0;
	data->philos = malloc(data->number_of_philos * sizeof(t_philo));
	if (!data->philos)
		return (false);
	return (true);
}

bool	init_philos(t_data *data)
{
	t_philo	*philos;
	int	n;
	int	i;
	
	if (!data || !data->philos)
		return (false);
	philos = data->philos;
	n = data->number_of_philos;
	i = 0;
	while (i < n)
	{
		philos[i].id = i + 1;
		philos[i].data = data;
		philos[i].last_time_to_eat = get_current_time();
		philos[i].meal_count = 0;
		philos[i].left_fork = &data->forks[i];
		philos[i].right_fork = &data->forks[(i + 1) % n];
		i++;
	}
	return (true);
}

int	main(int ac, char **av)
{
	t_data		data;
	pthread_t	monitor;
	int			i;

	if (validate_argc(ac) == false)
		return (2);
	if (init_data(&data, ac, &av[1]) == false)
		return (3);
	for (int i = 0; i < data.number_of_philos; i++)
		pthread_mutex_init(&data.forks[i], NULL);
	if (init_philos(&data) == false)
		return (3);
	if (pthread_create(&monitor, NULL, &monitor_routine, &data) != 0)
	{
		perror("thread_monitor");
		return (1);
	}
	i = 0;
	while (i < data.number_of_philos)
	{
		if (pthread_create(&data.philos[i].thread, NULL, &routine, &(data.philos[i])) != 0)
		{
			perror("thread1");
			return (1);
		}
		i++;
	}
	if (pthread_join(monitor, NULL) != 0)
	{
		perror("thread3");
		return (1);
	}
	i = 0;
	while (i < data.number_of_philos)
	{
		if (pthread_join(data.philos[i].thread, NULL) != 0)
		{
			perror("thread3");
			return (1);
		}
		i++;
	}
	for (int i = 0; i < data.number_of_philos; i++)
		pthread_mutex_destroy(&data.forks[i]);
	pthread_mutex_destroy(data.dead_mutex);
	pthread_mutex_destroy(data.meal_mutex);
	pthread_mutex_destroy(data.print_status);
	return (0);
}
