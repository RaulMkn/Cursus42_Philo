/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_rutine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 19:29:14 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/23 17:17:36 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_thinking(t_philo *philo, int *flag)
{
	pthread_mutex_lock(&philo->data->print_lock);
	if (philo->data->over == 0)
		printf(THINK, (get_timestamp_ms() - philo->data->rutine_start),
			philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	if (philo->data->num_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		if (philo->data->over == 0)
			printf(FORK, (get_timestamp_ms() - philo->data->rutine_start),
				philo->id);
		usleep(1000 * philo->data->time_to_die);
		if (philo->data->over == 0)
			printf(DEATH, (get_timestamp_ms() - philo->data->rutine_start),
				philo->id);
		pthread_mutex_unlock(philo->left_fork);
		philo->dead = 1;
		*flag = 1;
	}
}

static void	philo_eating(t_philo *philo)
{
	if (!philo->right_fork || !philo->left_fork)
		return ;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->data->over_lock);
		if (philo->data->over == 0)
			printf(FORK, (print_time(philo)), philo->id);
		pthread_mutex_unlock(&philo->data->over_lock);
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->data->over_lock);
		if (philo->data->over == 0)
			printf(FORK, (print_time(philo)), philo->id);
		pthread_mutex_unlock(&philo->data->over_lock);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->data->over_lock);
		if (philo->data->over == 0)
			printf(FORK, (print_time(philo)), philo->id);
		pthread_mutex_unlock(&philo->data->over_lock);
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->data->over_lock);
		if (philo->data->over == 0)
			printf(FORK, (print_time(philo)), philo->id);
		pthread_mutex_unlock(&philo->data->over_lock);
	}
	pthread_mutex_lock(&philo->data->print_lock);
	if (philo->data->over == 0)
		printf(EAT, (print_time(philo)), philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	usleep(1000 * philo->data->time_to_eat);
	philo->last_meal_time = get_timestamp_ms();
	philo->meals_eaten++;
	pthread_mutex_lock(&philo->data->full_lock);
	if (philo->meals_eaten >= philo->data->must_eat_count && philo->full == 0)
	{
		philo->full = 1;
		philo->data->philo_full++;
	}
	pthread_mutex_unlock(&philo->data->full_lock);
	pthread_mutex_lock(&philo->data->full_lock);
	if (philo->data->philo_full == philo->data->num_philos)
	{
		pthread_mutex_lock(&philo->data->over_lock);
		philo->data->over = 1;
		pthread_mutex_unlock(&philo->data->over_lock);
	}
	pthread_mutex_unlock(&philo->data->full_lock);
}

static void	philo_sleeping(t_philo *philo)
{
	long long	start_time;

	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&philo->data->print_lock);
	pthread_mutex_lock(&philo->data->over_lock);
	if (philo->data->over == 0)
		printf(SLEEP, (print_time(philo)), philo->id);
	pthread_mutex_unlock(&philo->data->over_lock);
	pthread_mutex_unlock(&philo->data->print_lock);
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

static int	check_death(t_philo *philo)
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

void	*philo_routine(void *philos)
{
	t_philo	*philo;
	int		flag;

	flag = 0;
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
		pthread_mutex_lock(&philo->data->over_lock);
		if (philo->data->over == 1)
			return (pthread_mutex_unlock(&philo->data->over_lock), NULL);
		pthread_mutex_unlock(&philo->data->over_lock);
		philo_eating(philo);
		pthread_mutex_lock(&philo->data->over_lock);
		if (philo->data->over == 1)
			return (pthread_mutex_unlock(&philo->data->over_lock), NULL);
		pthread_mutex_unlock(&philo->data->over_lock);
		philo_sleeping(philo);
		if (check_death(philo))
			break ;
	}
	return (NULL);
}
