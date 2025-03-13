/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:57:06 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/13 22:30:19 by rmakende         ###   ########.fr       */
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
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void	*monitor_routine(void *philos)
{
	t_philo *philo = (t_philo *)philos;
	int i;
	int temp;

	i = 0;
	temp = 1;
	while (temp)
	{
		while (i < philo->data->num_philos)
		{
			pthread_mutex_lock(&philo[i].check_lock);

			if (philo[i].dead == 1)
			{
				temp = 0;
				pthread_mutex_unlock(&philo[i].check_lock);
				return NULL;
			}
			
			i++;
		}
		pthread_mutex_unlock(&philo[i].check_lock);
		usleep(1000);
		
	}
	return NULL;
	
	
}

void	*philo_routine(void *philos)
{
	t_philo			*philo;

	philo = (t_philo *)philos;
	if (!philo || !philo->left_fork || !philo->right_fork)
		return (NULL);
	while (!philo->dead)
	{	 
		// Pensando
		pthread_mutex_lock(&philo->data->print_lock);
		printf(CYAN "%lld %d is thinking\n" RESET, (get_timestamp_ms() - philo->data->rutine_start), philo->id);
		pthread_mutex_unlock(&philo->data->print_lock);

		if (philo->data->num_philos == 1) {
			printf(YELLOW "%lld %d has taken a fork\n" RESET, (get_timestamp_ms() - philo->data->rutine_start), philo->id);
			usleep(1000 * philo->data->time_to_die);
			printf(RED "%lld %d died\n" RESET, (get_timestamp_ms() - philo->data->rutine_start), philo->id);
			return NULL;
		}
		
		
		// Toma de tenedores (evita deadlock)
		if (philo->id % 2 == 0)
		{
			printf(YELLOW "%lld %d has taken a fork\n" RESET, (get_timestamp_ms() - philo->data->rutine_start), philo->id);
			pthread_mutex_lock(philo->right_fork);
			printf(YELLOW "%lld %d has taken a fork\n" RESET, (get_timestamp_ms() - philo->data->rutine_start), philo->id);
			pthread_mutex_lock(philo->left_fork);
		}
		else
		{
			printf(YELLOW "%lld %d has taken a fork\n" RESET, (get_timestamp_ms() - philo->data->rutine_start), philo->id);
			pthread_mutex_lock(philo->left_fork);
			printf(YELLOW "%lld %d has taken a fork\n" RESET, (get_timestamp_ms() - philo->data->rutine_start), philo->id);
			pthread_mutex_lock(philo->right_fork);
		}
		// Comiendo
		pthread_mutex_lock(&philo->data->print_lock);
		printf(GREEN "%lld %d is eating\n" RESET, (get_timestamp_ms() - philo->data->rutine_start), philo->id);
		pthread_mutex_unlock(&philo->data->print_lock);
		usleep(1000 * philo->data->time_to_eat);
		philo->last_meal_time = get_timestamp_ms();
		
		// Soltar tenedores
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		
		// Durmiendo
		pthread_mutex_lock(&philo->data->print_lock);
		printf(MAGENTA "%lld %d is sleeping\n"RESET, (get_timestamp_ms() - philo->data->rutine_start), philo->id);
		pthread_mutex_unlock(&philo->data->print_lock);
		usleep(1000 * philo->data->time_to_sleep);
		
		if ((get_timestamp_ms() - philo->last_meal_time) >= philo->data->time_to_die)
		{
			pthread_mutex_lock(&philo->data->print_lock);
			printf(RED "%lld %d died\n" RESET, (get_timestamp_ms() - philo->data->rutine_start), philo->id);
			pthread_mutex_unlock(&philo->data->print_lock);
			pthread_mutex_lock(&philo->check_lock);
            philo->dead = 1;
            pthread_mutex_unlock(&philo->check_lock);
			break;
		}
	}
	return (NULL);
}
