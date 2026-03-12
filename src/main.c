/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 13:29:01 by asato             #+#    #+#             */
/*   Updated: 2026/03/12 18:13:26 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct s_philo
{
	int	number_of_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_eat;
	int	number_of_fork;
}				t_philo;

int cnt = 0;
pthread_mutex_t	mutex;

void *routine(void *p)
{
	(void)*p;
	int	eat_count;

	eat_count = 0;
	for (int i = 0; i < 1000; i++)
	{
		pthread_mutex_lock(&mutex);	//lockして同時アクセスを防ぐ
		eat_count++;
		cnt++;
		pthread_mutex_unlock(&mutex);
	}
	return (NULL);
}
bool	validate_argc(int ac)
{
	if  (ac == 5 || ac ==  6)
		return (true);
	return (false);
}

bool	init_philo(t_philo *philo, int ac, char **av)
{
	int	i;
	int	*argv;

	i = 1;
	argv = calloc(ac, sizeof(int));
	if (!argv)
		return (true);
	if (ac == 6)
	{
		while (av[i])
		{
			if (atoi(av[i]) == 0)
				return (free(argv), false);
			argv[i - 1] = atoi(av[i]);
			i++;
		}
		philo->number_of_philos = argv[0];
		philo->time_to_die = argv[1];
		philo->time_to_eat = argv[2];
		philo->time_to_sleep= argv[3];
		philo->number_of_eat = argv[4];
	}
	else
		philo->number_of_eat = 0;
	philo->number_of_fork = philo->number_of_philos;

	return (free(argv), true);
}

int	main(int ac, char **av)
{
	t_philo	philo;


	if (validate_argc(ac) == false)
		return (2);
	if (init_philo(&philo, ac, av) == false)
		return (3);
	pthread_t th[philo.number_of_philos];
	pthread_mutex_init(&mutex, NULL);
	int i = 1;
	while (i < philo.number_of_philos + 1)
	{
		if (pthread_create(th + i, NULL, &routine, NULL) != 0)
		{
			perror("thread");
			return (1);
		}
		printf("Thread %d has started\n", i);
		i++;
	}
	i = 1;
	while (i < philo.number + 1)
	{
		if (pthread_join(th[i], NULL) != 0)
		{
			perror("thread");
			return (1);
		}
		printf("Thread %d has finished\n", i);
		i++;
	}
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
