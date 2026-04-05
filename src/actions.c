/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asato <asato@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:51:29 by asato             #+#    #+#             */
/*   Updated: 2026/04/05 18:51:42 by asato            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_log(t_philo *philo, char *msg)
{
	if (is_stopped(philo) == CONTINUE)
	{
		pthread_mutex_lock(&philo->data->print_lock);
		printf("%lld %d %s", get_timestamp(philo->data), philo->id, msg);
		pthread_mutex_unlock(&philo->data->print_lock);
	}
}

t_routine_loop	eat(t_philo *philo)
{
	if (philo->data->philo_count == 1)
		return (single_philo_routine(philo), STOP);
	take_forks(philo);
	print_log(philo, EAT);
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time_ms = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	if (ft_sleep(philo, philo->data->time_to_eat_ms) == STOP)
		return (release_forks(philo), STOP);
	release_forks(philo);
	return (CONTINUE);
}

t_routine_loop	rest(t_philo *philo)
{
	print_log(philo, SLEEP);
	if (ft_sleep(philo, philo->data->time_to_sleep_ms) == STOP)
		return (STOP);
	return (CONTINUE);
}

void	think(t_philo *philo)
{
	print_log(philo, THINK);
}
