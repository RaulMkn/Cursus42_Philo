/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:57:06 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/13 19:41:35 by rmakende         ###   ########.fr       */
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

long	tiempo_transcurrido(struct timeval *inicio)
{
	struct timeval	ahora;

	gettimeofday(&ahora, NULL);
	return ((ahora.tv_sec - inicio->tv_sec) + (ahora.tv_usec - inicio->tv_usec)
		/ 1000000);
}
void	*philo_routine(void *philos)
{
	t_philo			*philo;
	long			tiempo_sin_comer;

	philo = (t_philo *)philos;
	if (!philo || !philo->left_fork || !philo->right_fork)
		return (NULL);
	while (1)
	{	 
		// Pensando
		pthread_mutex_lock(&philo->data->print_lock);
		printf(CYAN "%lld %d is thinking\n" RESET, (get_timestamp_ms() - philo->data->rutine_start), philo->id);
		pthread_mutex_unlock(&philo->data->print_lock);
		usleep(6000);
		
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
		gettimeofday(&philo->last_meal_time, NULL);
		usleep(philo->data->time_to_eat);
		
		// Soltar tenedores
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		
		// Durmiendo
		pthread_mutex_lock(&philo->data->print_lock);
		printf(MAGENTA "%lld %d is sleeping\n"RESET, (get_timestamp_ms() - philo->data->rutine_start), philo->id);
		pthread_mutex_unlock(&philo->data->print_lock);
		usleep(philo->data->time_to_sleep);
		

		tiempo_sin_comer = tiempo_transcurrido(&philo->last_meal_time);
		if (tiempo_sin_comer >= philo->data->time_to_die)
		{
			pthread_mutex_lock(&philo->data->print_lock);
			printf(RED "%lld %d died\n" RESET, (get_timestamp_ms() - philo->data->rutine_start), philo->id);

			pthread_mutex_unlock(&philo->data->print_lock);
			exit(0);
		}
	}
	return (NULL);
}
