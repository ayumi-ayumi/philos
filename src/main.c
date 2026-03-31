/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 13:29:01 by asato             #+#    #+#             */
/*   Updated: 2026/03/31 16:56:00 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>


// struct s_philo;
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
	// struct s_philo	*next;
}				t_philo;
typedef struct s_data
{
	int				number_of_philos;
	int				time_to_die_ms;
	int				time_to_eat_ms;
	int				time_to_sleep_ms;
	int				number_of_eat;
	int				number_of_fork;
	int				current_time;
	int				dead_flag;
	int				stop;
	pthread_mutex_t		*dead_mutex;
	pthread_mutex_t		*meal_mutex;
	pthread_mutex_t		*cutlary_mutex;
	long long		start_time;
	// bool			stop_program;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print_status;
	t_philo			*philos;
}				t_data;



long long	get_current_time() //us
{
	struct timeval	time;
	// long long 		current_time;

	gettimeofday(&time, NULL);
	// current_time = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (time.tv_sec * 1000000LL + time.tv_usec);
}

long long	get_timestamp(t_data data) //ms
{
	// struct timeval	time;
	// long long		timestamp;

	// gettimeofday(&time, NULL);
	// timestamp = time.tv_sec * 1000 + time.tv_usec / 1000 - data.start_time;
	return ((get_current_time() - data.start_time) / 1000LL);
}

int	check_death(t_philo *philo)
{
	int val;

	pthread_mutex_lock(philo->data->dead_mutex);
	if (philo->data->dead_flag == 1)
	{
		val = 1;
		pthread_mutex_unlock(philo->data->dead_mutex);
		// return (1);
	}
	else
	{
		val = 0;
		pthread_mutex_unlock(philo->data->dead_mutex);
	}
	return (val);

}


int	is_stopped(t_philo *philo)
{
	int	val;

	val = 0;
	pthread_mutex_lock(philo->data->dead_mutex);
	if (philo->data->stop == 1)
		val = 1;
	pthread_mutex_unlock(philo->data->dead_mutex);
	return (val);
}

void print_timestamp(t_philo *philo, char *msg)
{
		pthread_mutex_lock(philo->data->print_status);
		printf("%lld %d %s", get_timestamp(*philo->data), philo->id, msg);
		pthread_mutex_unlock(philo->data->print_status);
}



int	ft_usleep_and_check_dead(t_philo *philo, int time)
{
	(void)time;
	(void)philo;
	long long start = get_current_time();

	// pthread_mutex_lock(philo->data->meal_mutex);
	// if (philo->last_time_to_eat == 0)
	// 	philo->last_time_to_eat = get_current_time();
	// pthread_mutex_unlock(philo->data->meal_mutex);
	while (get_current_time() - start < (long long)time * 1000LL)
	{
		// printf("%lld\n", get_current_time() - start);
		// printf("stop: %d, philo.id: %d\n", is_stopped(philo), philo->id);
		if (is_stopped(philo) == 1)
			return (1) ;
		usleep(1000);
	}
	return (0) ;
}

int	all_eat_enough(t_data *data)
{
	int val;

	val = 0;
	for (int i = 0; i < data->number_of_philos; i++)
	{
		pthread_mutex_lock(data->meal_mutex);
		if (data->philos[i].meal_count == data->number_of_eat)
			val = 1;
		pthread_mutex_unlock(data->meal_mutex);
	}
	return (val);
}

void *monitor_routine(void *arg)
{
	t_data	*data;
	// long long now;

	data = (t_data *)arg;

	while (data->stop == 0)
	{
		for (int i = 0; i < data->number_of_philos; i++)
		{
			// now = ;
			pthread_mutex_lock(data->meal_mutex);
			if ((data->philos[i].last_time_to_eat != 0
				&&
				get_current_time() - data->philos[i].last_time_to_eat >= data->time_to_die_ms * 1000LL) ||
				data->philos[i].meal_count == data->number_of_eat)
			{
				pthread_mutex_lock(data->dead_mutex);
				data->stop = 1;
				print_timestamp(&data->philos[i], "died\n");
				pthread_mutex_unlock(data->dead_mutex);
				pthread_mutex_unlock(data->meal_mutex);
				break ;
			}
			pthread_mutex_unlock(data->meal_mutex);
		}
	}
	return (NULL);
}

int	pick_forks(t_philo *philo)
{
	int val = 0;

	if (philo->id % 2 == 0)
	{
		// pthread_mutex_lock(philo->data->cutlary_mutex);
		if (is_stopped(philo) == 1)
			return (1);
		pthread_mutex_lock(philo->left_fork);
		print_timestamp(philo, "has taken a fork\n");
		if (is_stopped(philo) == 1)
			return (pthread_mutex_unlock(philo->left_fork), 1);
		pthread_mutex_lock(philo->right_fork);
		print_timestamp(philo, "has taken a fork\n");
		if (is_stopped(philo) == 1)
			return (pthread_mutex_unlock(philo->left_fork), 1);
		// pthread_mutex_unlock(philo->data->cutlary_mutex);
	}
	else
	{
		// pthread_mutex_lock(philo->data->cutlary_mutex);
		if (is_stopped(philo) == 1)
			return (1);
		pthread_mutex_lock(philo->right_fork);
		print_timestamp(philo, "has taken a fork\n");
		if (is_stopped(philo) == 1)
			return (pthread_mutex_unlock(philo->right_fork), 1);
		pthread_mutex_lock(philo->left_fork);
		print_timestamp(philo, "has taken a fork\n");
		if (is_stopped(philo) == 1)
			return (pthread_mutex_unlock(philo->left_fork), 1);
		// pthread_mutex_unlock(philo->data->cutlary_mutex);
	}
	return (val);
}


int	eat(t_philo *philo)
{
	int val;

	val = 0;
	if (pick_forks(philo) == 1)
		return (1);
	// if (philo->id % 2 == 0)
	// {
	// 	// pthread_mutex_lock(philo->data->cutlary_mutex);
	// 	pthread_mutex_lock(philo->left_fork);
	// 	print_timestamp(philo, "has taken a fork\n");
	// 	pthread_mutex_lock(philo->right_fork);
	// 	print_timestamp(philo, "has taken a fork\n");
	// 	// pthread_mutex_unlock(philo->data->cutlary_mutex);
	// }
	// else
	// {
	// 	// pthread_mutex_lock(philo->data->cutlary_mutex);
	// 	pthread_mutex_lock(philo->right_fork);
	// 	print_timestamp(philo, "has taken a fork\n");
	// 	pthread_mutex_lock(philo->left_fork);
	// 	print_timestamp(philo, "has taken a fork\n");
	// 	// pthread_mutex_unlock(philo->data->cutlary_mutex);
	// }

	print_timestamp(philo, "is eating\n");
	if (ft_usleep_and_check_dead(philo, philo->data->time_to_eat_ms) == 1)
		val = 1;
	philo->meal_count ++;
	pthread_mutex_lock(philo->data->meal_mutex);
	philo->last_time_to_eat = get_current_time();
	pthread_mutex_unlock(philo->data->meal_mutex);
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
	if (is_stopped(philo) == 1)
		val = 1;
	return (val);
}

int act_sleep(t_philo *philo)
{
	// int val;

	print_timestamp(philo, "is sleeping\n");
	if (ft_usleep_and_check_dead(philo, philo->data->time_to_sleep_ms) == 1)
		return (1);
	return (0);
}




int think(t_philo *philo)
{
	// if (check_death(philo) == 1)
	// 	return (0);
	print_timestamp(philo, "is thinking\n");
	// if (check_death(philo) == 1)
	// 	return (0);
	return (1);
}


void *routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	pthread_mutex_lock(philo->data->meal_mutex);
	if (philo->last_time_to_eat == 0)
		philo->last_time_to_eat = get_current_time();
	pthread_mutex_unlock(philo->data->meal_mutex);
	while (1)
	{
		if (is_stopped(philo) == 1)
			break ;
		if (philo->id % 2 == 0)
			usleep(500);
		if (eat(philo) == 1)
			break ;
		if (is_stopped(philo) == 1)
			break ;
		if (act_sleep(philo) == 1)
			break ;
		if (is_stopped(philo) == 1)
			break ;
		print_timestamp(philo, "is thinking\n");
		// if (philo->meal_count == philo->data->number_of_eat)
		// 	break ;
			// return (NULL) ;
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
	data->cutlary_mutex = malloc(sizeof(pthread_mutex_t));
	if (!data->cutlary_mutex)
		return (false);
	pthread_mutex_init(data->cutlary_mutex, NULL);
	data->dead_flag = 0;
	data->current_time = 0;
	data->start_time = 0;
	data->dead_flag = 0;
	data->stop = 0;
	data->philos = malloc(data->number_of_philos * sizeof(t_philo));
	if (!data->philos)
		return (false);
	return (true);
}

bool	init_philos(t_data *data)
{
	int	n = data->number_of_philos;
	int	i = 0;
	while (i < n)
	{
		data->philos[i].id = i + 1;
		data->philos[i].data = data;
		data->philos[i].last_time_to_eat = 0;
		data->philos[i].meal_count = 0;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % n];
		// data->philos[i].thread = malloc(1 * sizeof(pthread_t));
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
	data.start_time = get_current_time();

	if (pthread_create(&monitor, NULL, &monitor_routine, &data) != 0)
	{
		perror("thread_motitor");
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
	pthread_mutex_destroy(data.cutlary_mutex);
	// for (int i = 0; i < data.number_of_philos; i++)
	// 	free(&(data.philos[i]));
	return (0);
}
