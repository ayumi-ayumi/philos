/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 13:29:01 by asato             #+#    #+#             */
/*   Updated: 2026/03/30 18:39:01 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

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
	pthread_mutex_t		*dead_mutex;
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
void print_timestamp(t_philo	*philo, char *msg)
{
	pthread_mutex_lock(philo->data->print_status);
	printf("%lld %d %s", get_timestamp(*philo->data), philo->id, msg);
	pthread_mutex_unlock(philo->data->print_status);
}



int	ft_usleep_and_check_dead(t_philo *philo, int time)
{
	long long start = get_current_time();
	long long now;

	if (philo->last_time_to_eat == 0)
		philo->last_time_to_eat = get_current_time();
	while(get_current_time() - start < (long long)time * 1000LL)
	{
		if (check_death(philo) == 1)
			return (0);
		now = get_current_time();
		if (now - philo->last_time_to_eat > philo->data->time_to_die_ms * 1000LL)
		{
			pthread_mutex_lock(philo->data->dead_mutex);
			if (philo->data->dead_flag == 0)
			{
				philo->data->dead_flag = 1;
				print_timestamp(philo, "died\n");
				pthread_mutex_unlock(philo->data->dead_mutex);
				return (0);
			}
			pthread_mutex_unlock(philo->data->dead_mutex);
			return (0);
		}
		usleep(500);
	}
	return (1);
}

int	monitor(t_philo *philo)
{
	if (philo->meal_count == philo->data->number_of_eat || philo->data->dead_flag == 1)
		return (0);
	return (1);
}




int	eat(t_philo *philo)
{
	int val;

	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_timestamp(philo, "has taken a fork\n");
		pthread_mutex_lock(philo->right_fork);
		print_timestamp(philo, "has taken a fork\n");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_timestamp(philo, "has taken a fork\n");
		pthread_mutex_lock(philo->left_fork);
		print_timestamp(philo, "has taken a fork\n");
	}
	print_timestamp(philo, "is eating\n");
	val = ft_usleep_and_check_dead(philo, philo->data->time_to_eat_ms);
	philo->last_time_to_eat = get_current_time();
	philo->meal_count += 1;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	return (val);
}

int act_sleep(t_philo *philo)
{
	int val;

	print_timestamp(philo, "is sleeping\n");
	val = ft_usleep_and_check_dead(philo, philo->data->time_to_sleep_ms);
	return (val);
}




int think(t_philo *philo)
{
	if (check_death(philo) == 1)
		return (0);
	print_timestamp(philo, "is thinking\n");
	if (check_death(philo) == 1)
		return (0);
	return (1);
}

void *routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	// philo->last_time_to_eat = get_current_time();
	// while (monitor(philo) != 0)
	// while (1)

	while (check_death(philo) == 0)
	// while (philo->data->dead_flag == 0)
	// while (philo->data->stop_program == false && philo->is_dead == false)
	{
		// if (check_death(philo) == 1)
		// 	return (printf("here\n"), NULL);
		if (philo->id % 2 == 0)
			usleep(500);
		if (check_death(philo) == 1)
			return (printf("here\n"), NULL);
		if (eat(philo) == 0)
		{
			return (NULL);
		}

		if (act_sleep(philo) == 0)
		{
			return (NULL);
		}

		// if (check_death(philo) == 1)
		// 	return (NULL);
		// print_timestamp(philo, "is thinking\n");


		if (think(philo) == 0)
		{
			return (NULL);
		}
		if (philo->meal_count == philo->data->number_of_eat)
		{
			// philo->data->stop_program = true;
			return (NULL) ;
			// break;
		}
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
	data->dead_flag = 0;
	data->current_time = 0;
	data->start_time = 0;
	data->dead_flag = 0;
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
	t_data	data;

	int	i;
	if (validate_argc(ac) == false)
		return (2);
	if (init_data(&data, ac, &av[1]) == false)
		return (3);
	for (int i = 0; i < data.number_of_philos; i++)
		pthread_mutex_init(&data.forks[i], NULL);
	if (init_philos(&data) == false)
		return (3);
	i = 0;
	data.start_time = get_current_time();

	while (i < data.number_of_philos)
	{
		if (pthread_create(&data.philos[i].thread, NULL, &routine, &(data.philos[i])) != 0)
		{
			perror("thread1");
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < data.number_of_philos)
	{
		if (pthread_join(data.philos[i].thread, NULL) != 0)
		{
			perror("thread3");
			return (1);
		}
		// printf("Thread %d has finished\n", i + 1);
		i++;
	}
	// }
	// if (pthread_join(monitor, NULL) != 0)
	// {
	// 	perror("thread4");
	// 	return (1);
	// }
	// while (th[i])
	// {
	// 	free(&th[i]);
	// 	i++;
	// }
	// current = philos;
	// // ft_tokclear(&philos);
	// while (current)
	// {
	// 	// pthread_mutex_destroy(current->right_fork);
	// 	// pthread_mutex_destroy(current->left_fork);
	// 	ft_tokdelone(current);
	// 	current = current->next;
	// }
	// free(philos);
	// for (int i = 0; i < data.number_of_philos; i++)
	// 	pthread_mutex_destroy(&data.forks[i]);
	return (0);
}
