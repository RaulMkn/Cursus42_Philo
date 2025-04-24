/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rutine_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 19:29:14 by rmakende          #+#    #+#             */
/*   Updated: 2025/04/22 14:01:54 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_if_over(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->over_lock);
	if (philo->data->over == 1)
	{
		pthread_mutex_unlock(&philo->data->over_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->over_lock);
	return (0);
}

int	philo_eats(t_philo *philo)
{
	long long	start;

	pthread_mutex_lock(&philo->data->over_lock);
	if (philo->data->over == 0)
		printf(EAT, print_time(philo), philo->id);
	pthread_mutex_unlock(&philo->data->over_lock);
	start = get_timestamp_ms();
	while ((get_timestamp_ms() - start) < philo->data->time_to_eat)
	{
		if (check_if_over(philo) || check_death(philo))
			return (1);
		usleep(50);
	}
	return (0);
}

void	update_philo_state(t_philo *philo)
{
	philo->meals_eaten++;
	pthread_mutex_lock(&philo->data->full_lock);
	if (philo->meals_eaten >= philo->data->must_eat_count && philo->full == 0)
	{
		philo->full = 1;
		philo->data->philo_full++;
	}
	if (philo->data->philo_full == philo->data->num_philos)
	{
		pthread_mutex_lock(&philo->data->over_lock);
		philo->data->over = 1;
		pthread_mutex_unlock(&philo->data->over_lock);
	}
	pthread_mutex_unlock(&philo->data->full_lock);
}

int	check_death(t_philo *philo)
{
	if ((get_timestamp_ms()
			- philo->last_meal_time) >= philo->data->time_to_die)
	{
		pthread_mutex_lock(&philo->check_lock);
		if (philo->dead == 0)
		{
			pthread_mutex_lock(&philo->data->over_lock);
			if (philo->data->over == 0)
			{
				printf(DEATH, (print_time(philo)), philo->id);
			}
			philo->data->over = 1;
			pthread_mutex_unlock(&philo->data->over_lock);
			philo->dead = 1;
		}
		pthread_mutex_unlock(&philo->check_lock);
		return (1);
	}
	return (0);
}
