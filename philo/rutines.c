/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rutines.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 21:15:08 by rmakende          #+#    #+#             */
/*   Updated: 2025/04/06 18:49:17 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->over_lock);
	if (philo->data->over == 0)
		printf(THINK, print_time(philo), philo->id);
	pthread_mutex_unlock(&philo->data->over_lock);
}

static int	philo_eating(t_philo *philo)
{
	if (!philo->right_fork || !philo->left_fork)
		return (1);
	take_forks(philo);
	philo_eats(philo);
	update_philo_state(philo);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (0);
}

static void	philo_sleeping(t_philo *philo)
{
	long long	start_time;

	pthread_mutex_lock(&philo->data->over_lock);
	if (philo->data->over == 0)
		printf(SLEEP, (print_time(philo)), philo->id);
	pthread_mutex_unlock(&philo->data->over_lock);
	start_time = get_timestamp_ms();
	while ((get_timestamp_ms() - start_time) < philo->data->time_to_sleep)
	{
		pthread_mutex_lock(&philo->data->over_lock);
		if (philo->data->over == 1)
		{
			pthread_mutex_unlock(&philo->data->over_lock);
			return ;
		}
		pthread_mutex_unlock(&philo->data->over_lock);
		usleep(1000);
	}
}

void	*monitor_routine(void *philos)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)philos;
	while (1)
	{
		if (check_if_over(philo))
			return (NULL);
		i = 0;
		while (i < philo->data->num_philos)
		{
			pthread_mutex_lock(&philo[i].check_lock);
			if (philo[i].dead == 1)
			{
				pthread_mutex_unlock(&philo[i].check_lock);
				pthread_mutex_lock(&philo->data->over_lock);
				philo->data->over = 1;
				pthread_mutex_unlock(&philo->data->over_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&philo[i].check_lock);
			i++;
		}
	}
	return (NULL);
}

void	*philo_routine(void *philos)
{
	t_philo	*philo;

	philo = (t_philo *)philos;
	if (!philo || !philo->left_fork || !philo->right_fork)
		return (NULL);
	philo->last_meal_time = philo->data->rutine_start;
	while (1)
	{
		if (check_if_over(philo))
			return (NULL);
		if (philo_eating(philo))
			return (NULL);
		if (check_if_over(philo))
			return (NULL);
		philo_sleeping(philo);
		if (check_if_over(philo))
			return (NULL);
		philo_thinking(philo);
		if (check_death(philo))
			break ;
	}
	return (NULL);
}
