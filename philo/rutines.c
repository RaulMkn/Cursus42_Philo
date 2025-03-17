/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rutines.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 19:29:14 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/17 19:32:28 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_routine(void *philos)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)philos;
	while (1)
	{
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
		usleep(1);
	}
	return (NULL);
}

void	*philo_routine(void *philos)
{
	t_philo	*philo;

	philo = (t_philo *)philos;
	if (!philo || !philo->left_fork || !philo->right_fork)
		return (NULL);
	while (1)
	{
		pthread_mutex_lock(&philo->data->over_lock);
		if (philo->data->over == 1)
		{
			pthread_mutex_unlock(&philo->data->over_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->data->over_lock);
		philo_thinking(philo);
		if (philo->data->num_philos == 1)
		{
			pthread_mutex_lock(philo->left_fork);
			printf(YELLOW "%lld %d has taken a fork\n" RESET,
				(get_timestamp_ms() - philo->data->rutine_start), philo->id);
			usleep(1000 * philo->data->time_to_die);
			printf(RED "%lld %d died\n" RESET, (get_timestamp_ms()
					- philo->data->rutine_start), philo->id);
			pthread_mutex_unlock(philo->left_fork);
			philo->dead = 1;
			return (NULL);
		}
		philo_eating(philo);
		philo_sleeping(philo);
		if (check_death(philo))
			break ;
	}
	return (NULL);
}
