/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:53:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/04/24 21:03:22 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	validate_arguments(int argc, char *argv[], t_data *data)
{
	if (argc < 5 || argc > 6)
		return (printf("Error: Invalid number of arguments\n"), 1);
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		data->must_eat_count = ft_atoi(argv[5]);
		if (data->must_eat_count <= 0)
			return (printf("Invalid number of meals\n" ERROR), 1);
	}
	else
		data->must_eat_count = 2147483647;
	data->rutine_start = get_timestamp_ms();
	data->over = 0;
	data->philo_full = 0;
	pthread_mutex_init(&data->over_lock, NULL);
	if (data->num_philos <= 0)
		return (printf("Error: Invalid number of philosophers\n" ERROR), 1);
	else if (data->num_philos < 0 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (printf("Invalid arguments\n" ERROR), 1);
	return (0);
}

static int	initialize_philosophers(t_data *data, t_philo **philos)
{
	int	i;

	pthread_mutex_init(&data->full_lock, NULL);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_init(&data->forks_mutex[i], NULL);
		data->forks_arr[i] = 0;
		(*philos)[i].id = i;
		(*philos)[i].meals_eaten = 0;
		(*philos)[i].l_fork_m = &data->forks_mutex[i];
		(*philos)[i].r_fork_m = &data->forks_mutex[(i + 1) % data->num_philos];
		(*philos)[i].l_fork_b = &data->forks_arr[i];
		(*philos)[i].r_fork_b = &data->forks_arr[(i + 1) % data->num_philos];
		(*philos)[i].dead = 0;
		(*philos)[i].full = 0;
		(*philos)[i].data = data;
		pthread_mutex_init(&(*philos)[i].check_lock, NULL);
		i++;
	}
	return (0);
}

static int	create_philosophers_threads(t_data *data, t_philo *philos)
{
	int			i;
	pthread_t	monitor_thread;

	if (data->num_philos == 1)
	{
		printf(FORK, (print_time(philos)), philos->id);
		usleep(data->time_to_die * 1000);
		printf(DEATH, (print_time(philos)), philos->id);
		return (0);
	}
	if (pthread_create(&monitor_thread, NULL, monitor_routine, philos) != 0)
		return (printf("Error: Monitor thread creation failed\n"), 1);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_routine,
				&philos[i]) != 0)
			return (printf("Error: Thread creation failed\n"), 1);
		i++;
	}
	i = -1;
	while (++i < data->num_philos)
		pthread_join(philos[i].thread, NULL);
	return (pthread_join(monitor_thread, NULL), 0);
}

static void	cleanup(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks_mutex[i]);
		pthread_mutex_destroy(&philos[i].check_lock);
		i++;
	}
	pthread_mutex_destroy(&data->over_lock);
	free(data->forks_mutex);
	free(data->forks_arr);
	free(philos);
}

int	main(int argc, char *argv[])
{
	t_data	data;
	t_philo	*philos;

	if (validate_arguments(argc, argv, &data))
		return (1);
	data.forks_arr = malloc(data.num_philos * sizeof(int));
	if (!data.forks_arr)
		return (printf("Error: Memory allocation failed\n"), 1);
	philos = malloc(data.num_philos * sizeof(t_philo));
	if (!philos)
		return (printf("Error: Memory allocation failed\n"), 1);
	data.forks_mutex = malloc(data.num_philos * sizeof(pthread_mutex_t));
	if (!data.forks_mutex)
		return (printf("Error: Memory allocation failed\n"), free(philos), 1);
	if (initialize_philosophers(&data, &philos))
		return (1);
	if (create_philosophers_threads(&data, philos))
		return (1);
	cleanup(&data, philos);
	return (0);
}
