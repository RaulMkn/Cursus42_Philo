/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:32:46 by rmakende          #+#    #+#             */
/*   Updated: 2025/04/22 14:01:25 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	lock_fork(t_philo *philo, pthread_mutex_t *mutex, int *bool)
{
	while (1)
	{
		pthread_mutex_lock(mutex);
		if (*bool == 0)
		{
			*bool = 1;
			pthread_mutex_unlock(mutex);
			pthread_mutex_lock(&philo->data->over_lock);
			if (philo->data->over == 0)
				printf(FORK, print_time(philo), philo->id);
			pthread_mutex_unlock(&philo->data->over_lock);
			return (0);
		}
		pthread_mutex_unlock(mutex);
		if (check_if_over(philo) || check_death(philo))
			return (1);
		usleep(50);
	}
	return (0);
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork_m);
	*philo->r_fork_b = 0;
	pthread_mutex_unlock(philo->r_fork_m);
	pthread_mutex_lock(philo->l_fork_m);
	*philo->l_fork_b = 0;
	pthread_mutex_unlock(philo->l_fork_m);
}

int	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		if (lock_fork(philo, philo->r_fork_m, philo->r_fork_b))
			return (1);
		if (lock_fork(philo, philo->l_fork_m, philo->l_fork_b))
			return (1);
	}
	else
	{
		if (lock_fork(philo, philo->l_fork_m, philo->l_fork_b))
			return (1);
		if (lock_fork(philo, philo->r_fork_m, philo->r_fork_b))
			return (1);
	}
	return (0);
}
