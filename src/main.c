/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ayumi <Ayumi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 13:29:01 by asato             #+#    #+#             */
/*   Updated: 2026/03/22 12:50:18 by Ayumi            ###   ########.fr       */
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
	pthread_mutex_t	right_fork;
	pthread_mutex_t	left_fork;
}				t_data;

typedef struct s_philo
{
	int	id;
	t_data	*data;
	int	last_time_to_eat;
	int	count_meal;
	struct s_philo	*next;
}				t_philo;

// pthread_mutex_t	mutex;

void *routine(void *p)
{
	// struct timeval start_time;
	struct timeval end_time;
	struct timeval now;
	t_philo *philo = (t_philo *)p;
	
	printf("Thread %d has started\n", philo->id);

	pthread_mutex_init(&philo->data->right_fork, NULL);
	pthread_mutex_init(&philo->data->left_fork, NULL);
	
	if (philo->data->number_of_fork > 1)
	{
		
		// gettimeofday(&start_time, NULL);
		pthread_mutex_lock(&philo->data->right_fork);
		philo->data->number_of_fork -= 1;
		// printf("%d %d has taken a right fork\n", philo->data->current_time, philo->id);
		pthread_mutex_lock(&philo->data->left_fork);
		philo->data->number_of_fork -= 1;
		printf("%d %d has taken a fork\n", philo->data->current_time, philo->id);
		printf("%d %d is eating\n", philo->data->current_time, philo->id);
		usleep(philo->data->time_to_eat * 1000);
		philo->data->current_time += philo->data->time_to_eat;
		pthread_mutex_unlock(&philo->data->right_fork);
		pthread_mutex_unlock(&philo->data->left_fork);
		gettimeofday(&end_time, NULL);
	}

	printf("%d %d is sleeping\n", philo->data->current_time, philo->id);
	philo->data->current_time += philo->data->time_to_sleep;
	usleep(philo->data->time_to_sleep * 1000);
	
	gettimeofday(&now, NULL);
	int time_past_after_eat = (now.tv_sec - end_time.tv_sec) * 1000 + (now.tv_usec - end_time.tv_usec) / 1000; //millisec
	if (time_past_after_eat > philo->data->time_to_die)
		return (printf("%d %d died\n", philo->data->current_time, philo->id), 
		pthread_mutex_destroy(&philo->data->right_fork), pthread_mutex_destroy(&philo->data->left_fork), NULL);
		
	printf("%d %d is thinking\n", philo->data->current_time, philo->id);
	
	gettimeofday(&now, NULL);
	time_past_after_eat = (now.tv_sec - end_time.tv_sec) * 1000 + (now.tv_usec - end_time.tv_usec) / 1000; //millisec
	if (time_past_after_eat > philo->data->time_to_die)
		return (printf("%d %d died\n", philo->data->current_time, philo->id), 
		pthread_mutex_destroy(&philo->data->right_fork), pthread_mutex_destroy(&philo->data->left_fork), NULL);
		
	pthread_mutex_destroy(&philo->data->right_fork);
	pthread_mutex_destroy(&philo->data->left_fork);
	return (NULL);
}
// void *routine(void *p)
// {
// 	// (void)p;
// 	struct timeval start_time;
// 	struct timeval end_time;
// 	t_data philo = *(t_data *)p;
	
// 	pthread_mutex_init(&mutex, NULL);
	
// 	gettimeofday(&start_time, NULL);
// 	printf("0 1 has taken a fork\n");
// 	pthread_mutex_lock(&mutex);	//lockして同時アクセスを防ぐ
// 	printf("0 1 is eating\n");
// 	usleep(philo.time_to_eat);
// 	pthread_mutex_unlock(&mutex);
// 	gettimeofday(&end_time, NULL);
// 	int diff = (end_time.tv_usec - start_time.tv_usec);
// 	printf("%d 1 is sleeping\n", philo.time_to_eat);
// 	printf("%d 1 is thinking\n", philo.time_to_eat + philo.time_to_sleep);
// 	if (diff > philo.time_to_die)
// 		printf("%d 1 dead\n", philo.time_to_eat + philo.time_to_sleep);
// 	pthread_mutex_destroy(&mutex);
// 	return (NULL);
// }
bool	validate_argc(int ac)
{
	if  (ac == 5 || ac ==  6)
		return (true);
	return (false);
}

bool	init_data(t_data *data, int ac, char **av)
{
	int	i;
	int	*argv;

	// philo = calloc(1, sizeof(t_data));
	// if (!philo)
	// 	return (false);
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
	if (ac == 6)
	{
		data->number_of_philos = argv[0];
		data->time_to_die = argv[1];
		data->time_to_eat = argv[2];
		data->time_to_sleep= argv[3];
		data->number_of_eat = argv[4];
	}
	else
	{
		data->number_of_philos = argv[0];
		data->time_to_die = argv[1];
		data->time_to_eat = argv[2];
		data->time_to_sleep= argv[3];
		data->number_of_eat = 0;
	}
	data->number_of_fork = data->number_of_philos;
	data->current_time = 0;
	return (free(argv), true);
}

t_philo	*create_philo_node(t_data *data)
{
	t_philo *node;

	node = calloc(1, sizeof(t_philo));
	if (!node)
		return (NULL);
	node->data = data;
	return (node);
}
bool	init_philos(t_data *data, t_philo **philos)
{
	t_philo *head;
	t_philo *new_node;
	t_philo *current;

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
	*philos = head;
	return (true);
}

int	main(int ac, char **av)
{
	t_data	data;
	t_philo *philos = NULL;
	t_philo *current;
	int	i;

	if (validate_argc(ac) == false)
		return (2);
	if (init_data(&data, ac, &av[1]) == false)
		return (3);
	if (init_philos(&data, &philos) == false)
		return (3);
	pthread_t th[data.number_of_philos];
	i = 0;
	current = philos;
	while (current)
	{
		if (pthread_create(th + i, NULL, &routine, current) != 0)
		{
			perror("thread");
			return (1);
		}
		current = current->next;
		i++;
	}
	
	i = 0;
	while (i < data.number_of_philos)
	{
		if (pthread_join(th[i], NULL) != 0)
		{
			perror("thread");
			return (1);
		}
		printf("Thread %d has finished\n", i + 1);
		i++;
	}
	// pthread_create(&th[0], NULL, routine, &data);
	// printf("Thread 1 has started\n");
	// pthread_create(&th[1], NULL, routine, &data);
	// printf("Thread 2 has started\n");
	// pthread_join(th[0], NULL);
	// pthread_join(th[1], NULL);
	return (0);
	// pthread_t th[philo->number_of_philos];
	// int i = 0;
	// while (i < philo->number_of_philos)
	// {
	// 	if (pthread_create(th + i, NULL, &routine, &philo) != 0)
	// 	{
	// 		perror("thread");
	// 		return (1);
	// 	}
	// 	printf("Thread %d has started\n", i);
	// 	i++;
	// }
	// i = 0;
	// while (i < philo->number_of_philos)
	// {
	// 	if (pthread_join(th[i], NULL) != 0)
	// 	{
	// 		perror("thread");
	// 		return (1);
	// 	}
	// 	printf("Thread %d has finished\n", i);
	// 	i++;
	// }
}

// int main(void)
// {
// 	struct timeval time1;
//     struct timeval time2;
//     int diff_time;

//     gettimeofday(&time1, NULL);
// //    printf("sec: %d, usec: %d\n", (int)time1.tv_sec, (int)time1.tv_usec);

//     /** Describe Function for Measurement of time **/


	// pthread_t th[1];
	// pthread_mutex_init(&mutex, NULL);
	// int i = 0;
	// while (i < 4)
	// {
	// 	if (pthread_create(th + i, NULL, &routine, NULL) != 0)
	// 	{
	// 		perror("thread");
	// 		return (1);
	// 	}
	// 	printf("Thread %d has started\n", i);
	// 	i++;
	// }
	// i = 0;
	// while (i < 4)
	// {
	// 	if (pthread_join(th[i], NULL) != 0)
	// 	{
	// 		perror("thread");
	// 		return (1);
	// 	}
	// 	printf("Thread %d has finished\n", i);
	// 	i++;
	// }
// 	gettimeofday(&time2, NULL);
// 	printf("sec: %d, usec: %d\n", (int)time1.tv_usec, (int)time2.tv_usec);

// 	diff_time = time2.tv_usec - time1.tv_usec +  (int)(time2.tv_usec - time1.tv_usec) / 1000000;
// 	printf("diff: %d[s]\n", diff_time);
// 	pthread_mutex_destroy(&mutex);
// 	printf("cnt -> %d\n", cnt);
// }
