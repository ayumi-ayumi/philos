/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:44:22 by asato             #+#    #+#             */
/*   Updated: 2026/04/03 17:55:36 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"


void *routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 != 0)
		usleep(500);
	while (is_stopped(philo) == 0)
	{
		if (eat(philo) == 1)
			break ;
		if (rest(philo) == 1)
			break ;
		think(philo);
		usleep(500);
	}
	return (NULL);
}


bool	start_threads(t_data *data)
{
	pthread_t	monitor;
	int			i;

	if (pthread_create(&monitor, NULL, &monitor_loop, data) != 0)
		return (false);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_create(&data->philos[i].thread, NULL, &routine, &data->philos[i]) != 0)
			return (false);
		i++;
	}
	if (pthread_join(monitor, NULL) != 0)
		return (false);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_join(data->philos[i++].thread, NULL) != 0)
			return (false);
	}
	return (true);
}
