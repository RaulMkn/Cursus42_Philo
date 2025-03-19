/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_rutine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 19:29:14 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/19 19:26:43 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_thinking(t_philo *philo, int *flag)
{
	pthread_mutex_lock(&philo->data->print_lock);
	printf(THINK, (get_timestamp_ms() - philo->data->rutine_start), philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	if (philo->data->num_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		printf(FORK, (get_timestamp_ms() - philo->data->rutine_start),
			philo->id);
		usleep(1000 * philo->data->time_to_die);
		printf(DEATH, (get_timestamp_ms() - philo->data->rutine_start),
			philo->id);
		pthread_mutex_unlock(philo->left_fork);
		philo->dead = 1;
		*flag = 1;
	}
}

static void	philo_eating(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		printf(FORK, (print_time(philo)), philo->id);
		usleep(1000);
		pthread_mutex_lock(philo->left_fork);
		printf(FORK, (print_time(philo)), philo->id);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		printf(FORK, (print_time(philo)), philo->id);
		usleep(1000);
		pthread_mutex_lock(philo->right_fork);
		printf(FORK, (print_time(philo)), philo->id);
	}
	pthread_mutex_lock(&philo->data->print_lock);
	printf(EAT, (print_time(philo)), philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	usleep(1000 * philo->data->time_to_eat);
	philo->last_meal_time = get_timestamp_ms();
	philo->meals_eaten++;
}

static void	philo_sleeping(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&philo->data->print_lock);
	printf(SLEEP, (print_time(philo)), philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	usleep(1000 * philo->data->time_to_sleep);
}

static int	check_death(t_philo *philo)
{
	if ((get_timestamp_ms()
			- philo->last_meal_time) >= philo->data->time_to_die)
	{
		pthread_mutex_lock(&philo->data->print_lock);
		printf(DEATH, (print_time(philo)), philo->id);
		pthread_mutex_unlock(&philo->data->print_lock);
		pthread_mutex_lock(&philo->check_lock);
		philo->dead = 1;
		pthread_mutex_unlock(&philo->check_lock);
		return (1);
	}
	return (0);
}

void	*philo_routine(void *philos)
{
	t_philo	*philo;
	int		flag;

	philo = (t_philo *)philos;
	if (!philo || !philo->left_fork || !philo->right_fork)
		return (NULL);
	while (1)
	{
		pthread_mutex_lock(&philo->data->over_lock);
		if (philo->data->over == 1)
			return (pthread_mutex_unlock(&philo->data->over_lock), NULL);
		pthread_mutex_unlock(&philo->data->over_lock);
		philo_thinking(philo, &flag);
		if (flag == 1)
			break ;
		philo_eating(philo);
		philo_sleeping(philo);
		if (check_death(philo))
			break ;
	}
	return (NULL);
}
