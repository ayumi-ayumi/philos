/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:51:29 by asato             #+#    #+#             */
/*   Updated: 2026/04/05 18:39:40 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	release_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_log(philo, TAKE_FORK);
		pthread_mutex_lock(philo->right_fork);
		print_log(philo, TAKE_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_log(philo, TAKE_FORK);
		pthread_mutex_lock(philo->left_fork);
		print_log(philo, TAKE_FORK);
	}
}

void	single_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_log(philo, TAKE_FORK);
	ft_sleep(philo, philo->data->time_to_eat_ms);
	pthread_mutex_unlock(philo->left_fork);
}
