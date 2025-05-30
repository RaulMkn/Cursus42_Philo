/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 19:27:42 by rmakende          #+#    #+#             */
/*   Updated: 2025/04/22 13:58:30 by rmakende         ###   ########.fr       */
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
# define ERROR "Usage example:\n./philo 7 300 200 200 5*\n*optional argument\n"

typedef struct s_fork
{
	int				taken;
	pthread_mutex_t	fork_lock;
}					t_fork;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				over;
	int				philo_full;
	long long		rutine_start;
	pthread_mutex_t	full_lock;
	pthread_mutex_t	over_lock;
	pthread_mutex_t	*forks_mutex;
	int				*forks_arr;
}					t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	int				dead;
	int				full;
	long long		last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	check_lock;
	pthread_mutex_t	*l_fork_m;
	pthread_mutex_t	*r_fork_m;
	int				*l_fork_b;
	int				*r_fork_b;
	t_data			*data;
}					t_philo;

int					ft_atoi(const char *str);
void				*philo_routine(void *philo);
long long			get_timestamp_ms(void);
void				*monitor_routine(void *philos);
long long			print_time(t_philo *philo);
int					check_death(t_philo *philo);
void				update_philo_state(t_philo *philo);
int					check_if_over(t_philo *philo);
int					take_forks(t_philo *philo);
void				release_forks(t_philo *philo);
int					philo_eats(t_philo *philo);

#endif