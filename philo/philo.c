/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:53:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/08 17:29:57 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_data	data;
	t_philo	*philos;
	int		i;

	if (argc < 5 || argc > 6)
		return (printf("Error: Invalid number of arguments\n"), 1);
	data.num_philos = ft_atoi(argv[1]);
	data.time_to_die = ft_atoi(argv[2]);
	data.time_to_eat = ft_atoi(argv[3]);
	data.time_to_sleep = ft_atoi(argv[4]);
	philos = malloc(data.num_philos * sizeof(t_philo));
	if (!philos)
		return (printf("Error: Memory allocation failed\n"), 1);
	data.forks = malloc(data.num_philos * sizeof(pthread_mutex_t));
	if (!data.forks)
		return (printf("Error: Memory allocation failed\n"), free(philos), 1);
	i = 0;
	while (i++ < data.num_philos)
	{
		pthread_mutex_init(&data.forks[i], NULL);
		philos[i].id = i;
		philos[i].left_fork = &data.forks[i];
		philos[i].right_fork = &data.forks[(i + 1) % data.num_philos];
		i++;
	}
	i = 0;
	while (i++ < data.num_philos)
		pthread_mutex_destroy(&data.forks[i]);
	free(data.forks);
	free(philos);
	return (0);
}
