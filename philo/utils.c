/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:57:06 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/17 21:41:20 by rmakende         ###   ########.fr       */
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
			if (philo[i].dead == 1
				|| (philo[i].meals_eaten >= philo->data->must_eat_count))
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
		usleep(1000);
	}
	return (NULL);
}

int	validate_arguments(int argc, char *argv[], t_data *data)
{
	if (argc < 5 || argc > 6)
		return (printf("Error: Invalid number of arguments\n"), 1);
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->must_eat_count = ft_atoi(argv[5]);
	data->rutine_start = get_timestamp_ms();
	data->over = 0;
	pthread_mutex_init(&data->over_lock, NULL);
	if (data->num_philos <= 0)
		return (printf("Error: Invalid number of philosophers\n"), 1);
	return (0);
}
