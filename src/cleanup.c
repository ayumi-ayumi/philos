/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ayumi <Ayumi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:53:52 by asato             #+#    #+#             */
/*   Updated: 2026/04/06 11:50:51 by Ayumi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup(t_data *data)
{
	if (!data || !data->philos)
		return ;
	destroy_mutex(data);
	free(data->philos);
	data->philos = NULL;
}

void	destroy_mutex(t_data *data)
{
	int	i;

	if (!data || !data->fork_mutex)
		return ;
	i = 0;
	if (data->fork_mutex)
	{
		while (i < data->fork_count)
			pthread_mutex_destroy(&data->fork_mutex[i++]);
		free(data->fork_mutex);
		data->fork_mutex = NULL;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->meal_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
}
