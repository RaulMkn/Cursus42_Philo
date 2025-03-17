/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:53:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/17 19:38:04 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initialize_philosophers(t_data *data, t_philo **philos)
{
	int	i;

	*philos = malloc(data->num_philos * sizeof(t_philo));
	if (!(*philos))
		return (printf("Error: Memory allocation failed\n"), 1);
	data->forks = malloc(data->num_philos * sizeof(pthread_mutex_t));
	if (!data->forks)
		return (printf("Error: Memory allocation failed\n"), free(*philos), 1);
	pthread_mutex_init(&data->print_lock, NULL);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		(*philos)[i].id = i;
		(*philos)[i].left_fork = &data->forks[i];
		(*philos)[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		(*philos)[i].dead = 0;
		(*philos)[i].data = data;
		pthread_mutex_init(&(*philos)[i].check_lock, NULL);
		i++;
	}
	return (0);
}

int	create_philosophers_threads(t_data *data, t_philo *philos)
{
	int			i;
	pthread_t	monitor_thread;

	if (pthread_create(&monitor_thread, NULL, monitor_routine, philos) != 0)
	{
		printf("Error: Monitor thread creation failed\n");
		return (1);
	}
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_routine,
				&philos[i]) != 0)
			return (printf("Error: Thread creation failed\n"), 1);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	return (0);
}

int	join_philosophers_threads(t_philo *philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
			return (printf("Error joining thread\n"), 1);
		i++;
	}
	return (0);
}

void	cleanup(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&philos[i].check_lock);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->over_lock);
	free(data->forks);
	free(philos);
}

int	main(int argc, char *argv[])
{
	t_data	data;
	t_philo	*philos;
	int		result;

	result = validate_arguments(argc, argv, &data);
	if (result != 0)
		return (result);
	result = initialize_philosophers(&data, &philos);
	if (result != 0)
		return (result);
	result = create_philosophers_threads(&data, philos);
	if (result != 0)
		return (result);
	result = join_philosophers_threads(philos, &data);
	if (result != 0)
		return (result);
	cleanup(&data, philos);
	return (0);
}
