/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_rutine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 19:29:14 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/25 21:22:59 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_if_over(t_data *data)
{
	pthread_mutex_lock(&data->over_lock);
	if (data->over == 1)
	{
		pthread_mutex_unlock(&data->over_lock);
		return (1);
	}
	pthread_mutex_unlock(&data->over_lock);
	return (0);
}

int	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(philo->left_fork);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->right_fork);
	}
	pthread_mutex_lock(&philo->data->print_lock);
	pthread_mutex_lock(&philo->data->over_lock);
	if (philo->data->over == 0)
		printf(FORK, print_time(philo), philo->id);
	pthread_mutex_unlock(&philo->data->over_lock);
	pthread_mutex_unlock(&philo->data->print_lock);
	return (0);
}

void	philo_eats(t_philo *philo)
{
	long	start;

	pthread_mutex_lock(&philo->data->print_lock);
	if (philo->data->over == 0)
		printf(EAT, print_time(philo), philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	start = get_timestamp_ms();
	while (get_timestamp_ms() - start < philo->data->time_to_eat)
		usleep(500);
}

void	update_philo_state(t_philo *philo)
{
	philo->last_meal_time = get_timestamp_ms();
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
				pthread_mutex_lock(&philo->data->print_lock);
				printf(DEATH, (print_time(philo)), philo->id);
				pthread_mutex_unlock(&philo->data->print_lock);
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
