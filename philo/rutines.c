/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rutines.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 21:15:08 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/25 21:28:37 by rmakende         ###   ########.fr       */
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
	take_forks(philo);
	philo_eats(philo);
	update_philo_state(philo);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
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

void	*monitor_routine(void *philos)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)philos;
	while (1)
	{
		if (check_if_over(philo->data))
			return (NULL);
		i = 0;
		while (i < philo->data->num_philos)
		{
			pthread_mutex_lock(&philo[i].check_lock);
			if (philo[i].dead == 1)
			{
				return (pthread_mutex_unlock(&philo[i].check_lock),
					pthread_mutex_lock(&philo->data->over_lock),
					philo->data->over = 1,
					pthread_mutex_unlock(&philo->data->over_lock), NULL);
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
	int		flag;

	flag = 0;
	philo = (t_philo *)philos;
	if (!philo || !philo->left_fork || !philo->right_fork)
		return (NULL);
	while (1)
	{
		if (check_if_over(philo->data))
			return (NULL);
		philo_eating(philo);
		if (check_if_over(philo->data))
			return (NULL);
		philo_sleeping(philo);
		if (check_if_over(philo->data))
			return (NULL);
		philo_thinking(philo, &flag);
		if (flag == 1)
			break ;
		if (check_death(philo))
			break ;
	}
	return (NULL);
}
