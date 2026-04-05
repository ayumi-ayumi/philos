/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:44:22 by asato             #+#    #+#             */
/*   Updated: 2026/04/05 18:39:40 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (!philo || !philo->data)
		return (NULL);
	if (philo->id % 2 != 0)
		usleep(500);
	while (is_stopped(philo) == false)
	{
		if (eat(philo) == STOP)
			break ;
		if (rest(philo) == STOP)
			break ;
		think(philo);
		usleep(700);
	}
	return (NULL);
}

static bool	join_threads(t_data *data, pthread_t *monitor)
{
	int	i;

	if (pthread_join(*monitor, NULL) != 0)
	{
		pthread_mutex_lock(&data->stop_mutex);
		data->stop_flag = true;
		pthread_mutex_unlock(&data->stop_mutex);
		i = 0;
		while (i < data->philo_count)
			pthread_join(data->philos[i++].thread, NULL);
		return (false);
	}
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_join(data->philos[i++].thread, NULL) != 0)
			return (false);
	}
	return (true);
}

bool	start_threads(t_data *data)
{
	pthread_t	monitor;
	int			i;

	if (!data || !data->philos)
		return (false);
	if (pthread_create(&monitor, NULL, &monitor_loop, data) != 0)
		return (false);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_create(&data->philos[i].thread, NULL, &routine,
				&data->philos[i]) != 0)
		{
			pthread_mutex_lock(&data->stop_mutex);
			data->stop_flag = true;
			pthread_mutex_unlock(&data->stop_mutex);
			while (i > 0)
				pthread_join(data->philos[--i].thread, NULL);
			pthread_join(monitor, NULL);
			return (false);
		}
		i++;
	}
	return (join_threads(data, &monitor));
}
