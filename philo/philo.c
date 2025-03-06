/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:53:00 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/06 21:01:21 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_data	data;
	t_philo	*philos;
	int		i;

	i = 0;
	if (argc < 5 || argc > 6)
	{
		printf("Error: Invalid number of arguments\n");
		return (1);
	}
	else
	{
		data.num_philos = ft_atoi(argv[1]);
		data.time_to_die = ft_atoi(argv[2]);
		data.time_to_eat = ft_atoi(argv[3]);
		data.time_to_sleep = ft_atoi(argv[4]);
		while (data.num_philos > i)
		{
			philos[i].id = i;
			philos[i].left_fork = &data.forks[i];
			philos[i].right_fork = &data.forks[(i + 1) % data.num_philos];
		}
	}
	return (0);
}
