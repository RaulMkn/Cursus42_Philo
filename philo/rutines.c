/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rutines.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 19:29:14 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/17 19:44:58 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_lock);
	printf(CYAN "%lld %d is thinking\n" RESET, (get_timestamp_ms()
			- philo->data->rutine_start), philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
}

void	philo_eating(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		printf(YELLOW "%lld %d has taken a fork\n" RESET, (get_timestamp_ms()
				- philo->data->rutine_start), philo->id);
		pthread_mutex_lock(philo->left_fork);
		printf(YELLOW "%lld %d has taken a fork\n" RESET, (get_timestamp_ms()
				- philo->data->rutine_start), philo->id);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		printf(YELLOW "%lld %d has taken a fork\n" RESET, (get_timestamp_ms()
				- philo->data->rutine_start), philo->id);
		pthread_mutex_lock(philo->right_fork);
		printf(YELLOW "%lld %d has taken a fork\n" RESET, (get_timestamp_ms()
				- philo->data->rutine_start), philo->id);
	}
	pthread_mutex_lock(&philo->data->print_lock);
	printf(GREEN "%lld %d is eating\n" RESET, (get_timestamp_ms()
			- philo->data->rutine_start), philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	usleep(1000 * philo->data->time_to_eat);
	philo->last_meal_time = get_timestamp_ms();
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	philo_sleeping(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_lock);
	printf(MAGENTA "%lld %d is sleeping\n" RESET, (get_timestamp_ms()
			- philo->data->rutine_start), philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	usleep(1000 * philo->data->time_to_sleep);
}

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
