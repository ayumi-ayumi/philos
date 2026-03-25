/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 13:29:01 by asato             #+#    #+#             */
/*   Updated: 2026/03/25 17:24:56 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_data
{
	int	number_of_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_eat;
	int	number_of_fork;
	int	current_time;
	bool		stop_program;
	pthread_mutex_t	*forks;
}				t_data;

typedef struct s_philo
{
	int	id;
	t_data	*data;
	int	last_time_to_eat;
	int	count_meal;
	bool	is_dead;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	struct s_philo	*next;
}				t_philo;

// typedef struct s_top
// {
// 	bool		stop_program;
// 	t_philo		*philos;
// }				t_top;




void *monitoring(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	t_data	*data = philo->data;
	t_philo	*current = philo;
	while (1)
	{
		current = philo;
		while (current != NULL)
		{
			if (current->is_dead == true)
			{
				data->stop_program = true;
				return (data->stop_program = true, NULL) ;
			}
			current = current->next;
		}
	}
	return (NULL) ;
}

void *routine(void *arg)
{
	struct timeval start_time;
	struct timeval end_time;
	struct timeval now;
	t_philo *philo = (t_philo *)arg;
	int diff;

	printf("Thread %d has started\n", philo->id);

	while (philo->is_dead == false)
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->right_fork);
			pthread_mutex_lock(philo->left_fork);
			printf("%d %d has taken a fork\n", philo->data->current_time, philo->id);
		}
		else
		{
			usleep(1000);
			pthread_mutex_lock(philo->left_fork);
			pthread_mutex_lock(philo->right_fork);
			printf("%d %d has taken a fork\n", philo->data->current_time, philo->id);
		}

		printf("%d %d is eating\n", philo->data->current_time, philo->id);
		philo->count_meal += 1;
		usleep(philo->data->time_to_eat * 1000);
		philo->data->current_time += philo->data->time_to_eat;
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		gettimeofday(&end_time, NULL);

		gettimeofday(&now, NULL);
		diff = (now.tv_sec - end_time.tv_sec) * 1000 + (now.tv_usec - end_time.tv_usec) / 1000; //millisec
		if (diff > philo->data->time_to_die)
		return (printf("%d %d died\n", philo->data->current_time, philo->id), philo->is_dead = true, philo->data->stop_program = true, NULL);

		gettimeofday(&now, NULL);
		diff = (now.tv_sec - end_time.tv_sec) * 1000 + (now.tv_usec - end_time.tv_usec) / 1000; //millisec
		if (diff > philo->data->time_to_die)
		return (printf("%d %d died\n", philo->data->current_time, philo->id), philo->is_dead = true, philo->data->stop_program = true, NULL);

		printf("%d %d is sleeping\n", philo->data->current_time, philo->id);
		usleep(philo->data->time_to_sleep * 1000);
		philo->data->current_time += philo->data->time_to_sleep;

		gettimeofday(&now, NULL);
		diff = (now.tv_sec - end_time.tv_sec) * 1000 + (now.tv_usec - end_time.tv_usec) / 1000; //millisec
		if (diff > philo->data->time_to_die)
		return (printf("%d %d died\n", philo->data->current_time, philo->id), philo->is_dead = true, philo->data->stop_program = true, NULL);

		gettimeofday(&start_time, NULL);
		printf("%d %d is thinking\n", philo->data->current_time, philo->id);
		// usleep(5000);
		gettimeofday(&end_time, NULL);
		diff = (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_usec - start_time.tv_usec) / 1000; //millisec
		philo->data->current_time += diff;

		if (philo->count_meal == philo->data->number_of_eat)
		{
			philo->data->stop_program = true;
			return (NULL);
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
	int	i;
	int	*argv;

	argv = calloc(ac, sizeof(int));
	if (!argv)
		return (free(data), false);
	i = 0;
	while (av[i])
	{
		if (atoi(av[i]) == 0)
			return (free(argv), false);
		argv[i] = atoi(av[i]);
		i++;
	}
	data->number_of_philos = argv[0];
	data->time_to_die = argv[1];
	data->time_to_eat = argv[2];
	data->time_to_sleep = argv[3];
	if (ac == 6)
		data->number_of_eat = argv[4];
	else
		data->number_of_eat = 0;
	data->number_of_fork = data->number_of_philos;
	data->forks = (pthread_mutex_t *)malloc(data->number_of_fork * sizeof(pthread_mutex_t));
	if (!data->forks)
		return (free(argv), false);
	data->current_time = 0;
	data->stop_program = false;
	return (free(argv), true);
}

t_philo	*create_philo_node(t_data *data)
{
	t_philo	*node;

	node = (t_philo *)malloc(1 * sizeof(t_philo));
	if (!node)
		return (NULL);
	node->data = data;
	node->last_time_to_eat = 0;
	node->count_meal = 0;
	node->is_dead = false;
	node->next = NULL;
	return (node);
}
bool	init_philos(t_data *data, t_philo **philos)
{
	t_philo	*head;
	t_philo	*new_node;
	t_philo	*current;

	head = create_philo_node(data);
	if (!head)
		return (false);
	head->id = 1;
	current = head;
	int i = 1;
	while (i < data->number_of_philos)
	{
		new_node = create_philo_node(data);
		if (!new_node)
			return (free(head), false);
		new_node->id = i + 1;
		current->next = new_node;
		current = new_node;
		i++;
	}
	i = 0;
	t_philo	*temp;
	temp = head;
	while (temp != NULL)
	{
		temp->left_fork = (pthread_mutex_t *)malloc(1 * sizeof(pthread_mutex_t));
		temp->right_fork = (pthread_mutex_t *)malloc(1 * sizeof(pthread_mutex_t));
		temp->left_fork = &(data->forks[i]);
		if (temp->next == NULL)
			temp->right_fork = &(data->forks[0]);
		else
			temp->right_fork = &(data->forks[i + 1]);
		i++;
		temp = temp->next;
	}
	*philos = head;
	return (true);
}

void	ft_tokdelone(t_philo *philo)
{
	if (!philo)
		return ;
	free(philo->right_fork);
	free(philo->left_fork);
	// ft_free((void **)&philo->text);
	free(philo);
}

void	ft_tokclear(t_philo **philos)
{
	t_philo	*next;

	if (!philos || !(*philos))
		return ;
	while ((*philos))
	{
		next = (*philos)->next;
		ft_tokdelone(*philos);
		*philos = next;
	}
	*philos = NULL;
	philos = NULL;
}

int	main(int ac, char **av)
{
	t_data	data;
	t_philo	*philos;
	t_philo	*current;

	int	i;
	philos = NULL;

	if (validate_argc(ac) == false)
		return (2);
	if (init_data(&data, ac, &av[1]) == false)
		return (3);
	for (int i = 0; i < data.number_of_philos; i++)
		pthread_mutex_init(&data.forks[i], NULL);
	if (init_philos(&data, &philos) == false)
		return (3);
	// th = (pthread_t *)malloc(sizeof(pthread_t) * data.number_of_philos);
	// if (!th)
	// 	perror("th");
	pthread_t	th[data.number_of_philos];
	pthread_t	monitor;
	if (pthread_create(&monitor, NULL, &monitoring, philos) != 0)
	{
		perror("thread1");
		return (1);
	}
	i = 0;
	current = philos;
	while (current)
	{
		pthread_mutex_init(current->right_fork, NULL);
		pthread_mutex_init(current->left_fork, NULL);
		if (pthread_create(th + i, NULL, &routine, current) != 0)
		{
			perror("thread2");
			return (1);
		}
		current = current->next;
		i++;
	}
	i = 0;
	if (philos->data->stop_program == true)
	{
		while (i < data.number_of_philos)
		{
			if (pthread_join(th[i], NULL) != 0)
			{
				perror("thread3");
				return (1);
			}
			printf("Thread %d has finished\n", i + 1);
			i++;
		}
	}
	if (pthread_join(monitor, NULL) != 0)
	{
		perror("thread4");
		return (1);
	}
	while (th[i])
	{
		free(&th[i]);
		i++;
	}
	current = philos;
	// ft_tokclear(&philos);
	while (current)
	{
		// pthread_mutex_destroy(current->right_fork);
		// pthread_mutex_destroy(current->left_fork);
		ft_tokdelone(current);
		current = current->next;
	}
	free(philos);
	for (int i = 0; i < data.number_of_philos; i++)
		pthread_mutex_destroy(&data.forks[i]);
	return (0);
}
