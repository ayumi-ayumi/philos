/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:53:52 by asato             #+#    #+#             */
/*   Updated: 2026/04/03 17:54:35 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"


void	cleanup(t_data *data)
{
	free(data->philos);
	destroy_mutex(data);
}

void	destroy_mutex(t_data *data)
{
	int	i;

	i = 0;
	if (data->fork_mutex)
	{
		while (i < data->fork_count)
			pthread_mutex_destroy(&data->fork_mutex[i++]);
	}
	free(data->fork_mutex);
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->meal_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
}
