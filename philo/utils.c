/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:57:06 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/21 02:28:41 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sum;
	int	mult;

	i = 0;
	sum = 0;
	mult = 1;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			mult -= 2;
		i++;
	}
	if (mult < -1)
		return (0);
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		sum = sum * 10 + (str[i] - '0');
		i++;
	}
	return (sum * mult);
}

long long	get_timestamp_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long long	print_time(t_philo *philo)
{
	return (get_timestamp_ms() - philo->data->rutine_start);
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
		usleep(100);
	}
	return (NULL);
}
