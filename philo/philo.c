/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:53:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/13 22:36:03 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_data	data;
	t_philo	*philos;
	pthread_t monitor_thread;
	int		i;

	if (argc < 5 || argc > 6)
		return (printf("Error: Invalid number of arguments\n"), 1);
	data.num_philos = ft_atoi(argv[1]);
	data.time_to_die = ft_atoi(argv[2]);
	data.time_to_eat = ft_atoi(argv[3]);
	data.time_to_sleep = ft_atoi(argv[4]);
	data.rutine_start = get_timestamp_ms();
	if (data.num_philos <= 0)
		return (printf("Error: Invalid number of philosophers\n"), 1);
	philos = malloc(data.num_philos * sizeof(t_philo));
	if (!philos)
		return (printf("Error: Memory allocation failed\n"), 1);
	data.forks = malloc(data.num_philos * sizeof(pthread_mutex_t));
	if (!data.forks)
		return (printf("Error: Memory allocation failed\n"), free(philos), 1);
	pthread_mutex_init(&data.print_lock, NULL);
	i = 0;
	while (i < data.num_philos)
	{
		pthread_mutex_init(&data.forks[i], NULL);
		philos[i].id = i;
		philos[i].left_fork = &data.forks[i];
		philos[i].right_fork = &data.forks[(i + 1) % data.num_philos];
		philos->dead = 0;
		philos[i].data = &data; // ✅ Asignación correcta de `data`
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor_routine, philos) != 0) {
        printf("Error: Monitor thread creation failed\n");
        return 1;
    }
	i = 0;
	while (i < data.num_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_routine,
				&philos[i]) != 0)
			return (printf("Error: Thread creation failed\n"), 1);
		i++;
	}
	i = 0;
	while (i < data.num_philos)
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
			return (printf("Error joining thread\n"), 1);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < data.num_philos)
	{
		pthread_mutex_destroy(&data.forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data.print_lock);
	free(data.forks);
	free(philos);
	return (0);
}
