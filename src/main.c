/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 13:29:01 by asato             #+#    #+#             */
/*   Updated: 2026/04/03 18:00:26 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

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


bool	validate_arg_count(int ac)
{
	if (ac == 5 || ac ==  6)
		return (true);
	return (false);
}

bool	validate_numeric_args(char **av)
{
	int	i;
	int	j;

	i = 1;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (false);
			j++;
		}
		i++;
	}
	return (true);
}



int	main(int ac, char **av)
{
	t_data	data;

	data = (t_data){0};
	if (validate_arg_count(ac) == false)
		return (print_error(ERR_ARGS), 1);
	if (validate_numeric_args(av) == false)
		return (print_error(ERR_INPUT), 1);
	if (init_data(&data, ac, &av[1]) == false)
		return (print_error(ERR_INIT_DATA), cleanup(&data),1);
	if (init_philos(&data) == false)
		return (print_error(ERR_INIT_PHILO), cleanup(&data), 1);
	if (start_threads(&data) == false)
		return (print_error(ERR_THREAD), cleanup(&data), 1);
	cleanup(&data);
	return (0);
}
