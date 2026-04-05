/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:55:02 by asato             #+#    #+#             */
/*   Updated: 2026/04/05 18:39:40 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

static t_routine_loop	all_philos_ate_enough(t_data *data)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (data->must_eat_count != -1)
	{
		while (i < data->philo_count)
		{
			pthread_mutex_lock(&data->meal_mutex);
			if (data->philos[i].meals_eaten >= data->must_eat_count)
				count++;
			pthread_mutex_unlock(&data->meal_mutex);
			i++;
		}
		if (count == data->philo_count)
		{
			pthread_mutex_lock(&data->stop_mutex);
			if (data->stop_flag == false)
				data->stop_flag = true;
			pthread_mutex_unlock(&data->stop_mutex);
			return (STOP);
		}
	}
	return (CONTINUE);
}

static t_routine_loop	has_philo_died(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&data->meal_mutex);
		if (get_current_time()
			- data->philos[i++].last_meal_time_ms >= data->time_to_die_ms)
		{
			pthread_mutex_unlock(&data->meal_mutex);
			pthread_mutex_lock(&data->stop_mutex);
			pthread_mutex_lock(&data->print_lock);
			if (data->stop_flag == false)
			{
				data->stop_flag = true;
				printf("%lld %d %s", get_timestamp(data), data->philos[i].id,
					DIED);
			}
			pthread_mutex_unlock(&data->print_lock);
			pthread_mutex_unlock(&data->stop_mutex);
			return (STOP);
		}
		pthread_mutex_unlock(&data->meal_mutex);
	}
	return (CONTINUE);
}

void	*monitor_loop(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	if (!data || !data->philos)
		return (NULL);
	while (1)
	{
		if (has_philo_died(data) || all_philos_ate_enough(data))
			break ;
		usleep(500);
	}
	return (NULL);
}
