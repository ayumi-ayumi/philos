/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 13:29:01 by asato             #+#    #+#             */
/*   Updated: 2026/04/04 17:01:15 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

static bool	validate_numeric_args(char **av)
{
	int	i;
	int	j;

	i = 1;
	while (av[i])
	{
		j = 0;
		if (ft_atoi(av[i]) == 0)
			return (false);
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

static void	print_error(char *msg)
{
	size_t	len;

	len = ft_strlen(msg);
	write(2, msg, len);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		return (print_error(ERR_ARGS), 1);
	data = (t_data){0};
	if (validate_numeric_args(av) == false)
		return (print_error(ERR_INPUT), 1);
	if (init_data(&data, &av[1]) == false)
		return (print_error(ERR_INIT_DATA), 1);
	if (init_philos(&data) == false)
		return (print_error(ERR_INIT_PHILO), cleanup(&data), 1);
	if (start_threads(&data) == false)
		return (print_error(ERR_THREAD), cleanup(&data), 1);
	cleanup(&data);
	return (0);
}
