/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:31:56 by asato             #+#    #+#             */
/*   Updated: 2026/04/04 15:08:18 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

bool	init_philos(t_data *data)
{
	t_philo	*philos;
	int		n;
	int		i;

	if (!data || !data->philos)
		return (false);
	philos = data->philos;
	n = data->philo_count;
	i = 0;
	while (i < n)
	{
		philos[i].id = i + 1;
		philos[i].data = data;
		philos[i].last_meal_time_ms = get_current_time();
		philos[i].meals_eaten = 0;
		philos[i].left_fork = &data->fork_mutex[i];
		philos[i].right_fork = &data->fork_mutex[(i + 1) % n];
		i++;
	}
	return (true);
}

bool	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	data->fork_mutex = malloc(data->fork_count * sizeof(pthread_mutex_t));
	if (!data->fork_mutex)
		return (false);
	while (i < data->fork_count)
		pthread_mutex_init(&data->fork_mutex[i++], NULL);
	pthread_mutex_init(&data->stop_mutex, NULL);
	pthread_mutex_init(&data->meal_mutex, NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	return (true);
}

bool	init_data(t_data *data, char **av)
{
	data->philo_count = ft_atoi(av[0]);
	data->time_to_die_ms = ft_atoi(av[1]);
	data->time_to_eat_ms = ft_atoi(av[2]);
	data->time_to_sleep_ms = ft_atoi(av[3]);
	if (av[4])
		data->must_eat_count = ft_atoi(av[4]);
	else
		data->must_eat_count = -1;
	data->fork_count = data->philo_count;
	data->start_time_ms = get_current_time();
	data->stop_flag = 0;
	if (init_mutex(data) == false)
		return (false);
	data->philos = malloc(data->philo_count * sizeof(t_philo));
	if (!data->philos)
		return (destroy_mutex(data), false);
	return (true);
}
