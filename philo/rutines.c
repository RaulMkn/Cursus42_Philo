/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rutines.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 21:15:08 by rmakende          #+#    #+#             */
/*   Updated: 2025/04/22 18:20:04 by rmakende         ###   ########.fr       */
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
	if (!philo->r_fork_m || !philo->l_fork_m)
		return (1);
	if (take_forks(philo))
		return (1);
	philo->last_meal_time = get_timestamp_ms();
	if (philo_eats(philo))
		return (1);
	release_forks(philo);
	update_philo_state(philo);
	return (0);
}

static int	philo_sleeping(t_philo *philo)
{
	long	start_time;

	pthread_mutex_lock(&philo->data->over_lock);
	if (philo->data->over == 0)
		printf(SLEEP, (print_time(philo)), philo->id);
	pthread_mutex_unlock(&philo->data->over_lock);
	start_time = get_timestamp_ms();
	while ((get_timestamp_ms() - start_time) < philo->data->time_to_sleep)
	{
		if (check_if_over(philo) || check_death(philo))
			return (1);
		usleep(50);
	}
	return (0);
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
	if (!philo || !philo->l_fork_m || !philo->r_fork_m)
		return (NULL);
	philo->last_meal_time = philo->data->rutine_start;
	while (1)
	{
		if (philo_eating(philo))
			break ;
		if (check_if_over(philo) || check_death(philo))
			break ;
		if (philo_sleeping(philo))
			break ;
		if (check_if_over(philo) || check_death(philo))
			break ;
		philo_thinking(philo);
		if (check_if_over(philo) || check_death(philo))
			break ;
	}
	return (NULL);
}
