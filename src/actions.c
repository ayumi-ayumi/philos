/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:51:29 by asato             #+#    #+#             */
/*   Updated: 2026/04/03 17:57:48 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"


void	print_log(t_philo *philo, char *msg)
{
	if (is_stopped(philo) == 0)
	{
		pthread_mutex_lock(&philo->data->print_lock);
		printf("%lld %d %s", get_timestamp(*philo->data), philo->id, msg);
		pthread_mutex_unlock(&philo->data->print_lock);
	}
}

int	single_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_log(philo, "has taken a fork\n");
	ft_sleep(philo, philo->data->time_to_eat_ms);
	pthread_mutex_unlock(philo->left_fork);
	return (1);
}

int	take_forks(t_philo *philo)
{
	int	val;

	val = 0;
	if (philo->data->philo_count == 1)
		return (single_philo_routine(philo));
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_log(philo, "has taken a fork\n");
		pthread_mutex_lock(philo->right_fork);
		print_log(philo, "has taken a fork\n");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_log(philo, "has taken a fork\n");
		pthread_mutex_lock(philo->left_fork);
		print_log(philo, "has taken a fork\n");
	}
	return (val);
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
	if (take_forks(philo) == 1)
		return (1);
	print_log(philo, "is eating\n");
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time_ms = get_current_time();
	pthread_mutex_unlock(&philo->data->meal_mutex);
	if (ft_sleep(philo, philo->data->time_to_eat_ms) == 1)
		return (release_forks(philo), 1);
	release_forks(philo);
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->meals_eaten ++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	return (0);
}

int	rest(t_philo *philo)
{
	print_log(philo, "is sleeping\n");
	if (ft_sleep(philo, philo->data->time_to_sleep_ms) == 1)
		return (1);
	return (0);
}

void	think(t_philo *philo)
{
	print_log(philo, "is thinking\n");
}
