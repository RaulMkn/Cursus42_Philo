/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 19:27:42 by rmakende          #+#    #+#             */
/*   Updated: 2025/03/19 19:30:02 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define THINK "\033[34m%lld %d is thinking\n\033[0m"
# define FORK "\033[33m%lld %d has taken a fork\n\033[0m"
# define DEATH "\033[31m%lld %d died\n\033[0m"
# define EAT "\033[32m%lld %d is eating\n\033[0m"
# define SLEEP "\033[35m%lld %d is sleeping\n\033[0m"

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	long long		rutine_start;
	int				over;
	pthread_mutex_t	over_lock;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;

}					t_data;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	long long		last_meal_time;
	int				meals_eaten;
	int				dead;
	pthread_mutex_t	check_lock;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;

}					t_philo;

int					ft_atoi(const char *str);
void				*philo_routine(void *philo);
long long			get_timestamp_ms(void);
void				*monitor_routine(void *philos);
int					validate_arguments(int argc, char *argv[], t_data *data);
long long			print_time(t_philo *philo);

#endif