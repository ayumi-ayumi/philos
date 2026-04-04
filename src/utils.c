/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:36:34 by asato             #+#    #+#             */
/*   Updated: 2026/04/04 15:31:44 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

long long	get_timestamp(t_data *data)
{
	return ((get_current_time() - data->start_time_ms));
}

long long	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	is_stopped(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->stop_mutex);
	if (data->stop_flag == 1)
		return (pthread_mutex_unlock(&data->stop_mutex), 1);
	pthread_mutex_unlock(&data->stop_mutex);
	return (0);
}
