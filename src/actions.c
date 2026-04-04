/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:51:29 by asato             #+#    #+#             */
/*   Updated: 2026/04/04 16:47:15 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

void	print_log(t_philo *philo, char *msg)
{
	if (is_stopped(philo) == 0)
	{
		pthread_mutex_lock(&philo->data->print_lock);
		printf("%lld %d %s", get_timestamp(philo->data), philo->id, msg);
		pthread_mutex_unlock(&philo->data->print_lock);
	}
}

int	single_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_log(philo, TAKE_FORK);
	ft_sleep(philo, philo->data->time_to_eat_ms);
	pthread_mutex_unlock(philo->left_fork);
	return (1);
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

int	eat(t_philo *philo)
{
	if (philo->data->philo_count == 1)
		return (single_philo_routine(philo), 1);
	take_forks(philo);
	print_log(philo, EAT);
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time_ms = get_current_time();
	philo->meals_eaten ++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	if (ft_sleep(philo, philo->data->time_to_eat_ms) == 1)
		return (release_forks(philo), 1);
	release_forks(philo);
	return (0);
}

int	rest(t_philo *philo)
{
	print_log(philo, SLEEP);
	if (ft_sleep(philo, philo->data->time_to_sleep_ms) == 1)
		return (1);
	return (0);
}

void	think(t_philo *philo)
{
	print_log(philo, THINK);
}
