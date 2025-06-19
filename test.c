[file name]: philo.h
[file content begin]
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 03:07:34 by abouknan          #+#    #+#             */
/*   Updated: 2025/06/19 14:28:43 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[0;34m"
# define RESET "\033[0m"

typedef struct s_philo
{
	int				philo_id;
	pthread_t		thread;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				n_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				max_meals;
	int				someone_died;
	long long		start_time;
	pthread_mutex_t meal_mutex;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	*forks;
	pthread_t		death_thread;
	t_philo			*philos;
}					t_data;


int					valid_arguments(int ac, char **av);
char				*join_function(int ac, char **av);
char				**ft_split(char *str);
void				free_split(char **split);
int					ft_strlen(char *str);
char				*ft_strdup(char *s1);
int					ft_atoi(char *str);
int					init_data(t_data *data, int ac, char **av);
void				init_philosophers(t_data *data);
void				safe_print(t_philo *philo, const char *msg);
int					philo_simulation(t_data *data);
long long			timestamp_in_ms(void);
void				cleanup(t_data *data);

#endif

static pthread_mutex_t	*init_forks(t_data *data)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->n_philos);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (free(forks), NULL);
		i++;
	}
	return (forks);
}

long long	timestamp_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	init_data(t_data *data, int ac, char **av)
{
	data->n_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->start_time = timestamp_in_ms();
	data->max_meals = -1;
	if (ac > 5)
		data->max_meals = ft_atoi(av[5]);
	if (data->n_philos == 0 || data->time_to_die == 0 || data->time_to_eat == 0
		|| data->time_to_sleep == 0)
		return (0);
	data->someone_died = 0;
	data->forks = init_forks(data);
	if (!data->forks)
		return (0);
	if (pthread_mutex_init(&data->meal_mutex, NULL) != 0)
		return (cleanup(data), 0);
	if (pthread_mutex_init(&data->state_mutex, NULL) != 0)
		return (cleanup(data), 0);
	data->philos = malloc(sizeof(t_philo) * data->n_philos);
	if (!data->philos)
		return (cleanup(data), 0);
	return (1);
}

void	assign_forks(t_philo *philos)
{
	philos->left_fork = &philos->data->forks[philos->philo_id - 1];
	if (philos->philo_id == philos->data->n_philos)
		philos->right_fork = &philos->data->forks[0];
	else
		philos->right_fork = &philos->data->forks[philos->philo_id];
}

void	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		data->philos[i].philo_id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = data->start_time;
		data->philos[i].data = data;
		assign_forks(&data->philos[i]);
		i++;
	}
}
[file content end]

[file name]: ness_func.c
[file content begin]
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ness_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 05:40:44 by abouknan          #+#    #+#             */
/*   Updated: 2025/06/19 14:31:18 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_atoi(char *str)
{
	int		mul;
	int		i;
	long	result;

	mul = 1;
	i = 0;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		mul = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i++] - '0');
		if ((mul == 1 && result > INT_MAX) || (mul == -1 && - result < INT_MIN))
			return (-1);
	}
	return ((int)(result * mul));
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	if (data->forks)
	{
		while (i < data->n_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	pthread_mutex_destroy(&data->meal_mutex);
	pthread_mutex_destroy(&data->state_mutex);
	if (data->philos)
		free(data->philos);
}

void	safe_print(t_philo *philo, const char *msg)
{
	pthread_mutex_lock(&philo->data->state_mutex);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->state_mutex);
		return ;
	}
	printf(msg, timestamp_in_ms() - philo->data->start_time, philo->philo_id);
	pthread_mutex_unlock(&philo->data->state_mutex);
}
[file content end]

[file name]: simulation.c
[file content begin]
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:11:14 by abouknan          #+#    #+#             */
/*   Updated: 2025/06/19 14:33:48 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	one_philosopher(t_data *data)
{
	safe_print(&data->philos[0], "%ld %d has taken a fork\n");
	usleep(data->time_to_die * 1000);
	pthread_mutex_lock(&data->state_mutex);
	data->someone_died = 1;
	printf("%ld %d died\n", timestamp_in_ms() - data->start_time, data->philos[0].philo_id);
	pthread_mutex_unlock(&data->state_mutex);
}

void	*death_checker(void *arg)
{
	t_data	*data = (t_data *)arg;
	int		i;
	long	now;
	int		full_philos;

	while (1)
	{
		i = -1;
		full_philos = 0;
		while (++i < data->n_philos)
		{
			pthread_mutex_lock(&data->meal_mutex);
			now = timestamp_in_ms();
			if (now - data->philos[i].last_meal_time > data->time_to_die)
			{
				pthread_mutex_unlock(&data->meal_mutex);
				pthread_mutex_lock(&data->state_mutex);
				data->someone_died = 1;
				printf("%ld %d died\n", now - data->start_time, data->philos[i].philo_id);
				pthread_mutex_unlock(&data->state_mutex);
				return (NULL);
			}
			if (data->max_meals != -1
				&& data->philos[i].meals_eaten >= data->max_meals)
				full_philos++;
			pthread_mutex_unlock(&data->meal_mutex);
		}
		if (data->max_meals != -1 && full_philos == data->n_philos)
		{
			pthread_mutex_lock(&data->state_mutex);
			data->someone_died = 1;
			pthread_mutex_unlock(&data->state_mutex);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

void	eating(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	safe_print(philo, "%ld %d has taken a fork\n");
	pthread_mutex_lock(philo->right_fork);
	safe_print(philo, "%ld %d has taken a fork\n");
	safe_print(philo, "%ld %d is eating\n");
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = timestamp_in_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philosophers(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->philo_id % 2 == 0)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&philo->data->state_mutex);
		if (philo->data->someone_died)
		{
			pthread_mutex_unlock(&philo->data->state_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->state_mutex);
		eating(philo);
		safe_print(philo, "%ld %d is sleeping\n");
		usleep(philo->data->time_to_sleep * 1000);
		safe_print(philo, "%ld %d is thinking\n");
	}
	return (NULL);
}

int	philo_simulation(t_data *data)
{
	int	i;

	i = -1;
	if (data->n_philos == 1)
	{
		one_philosopher(data);
		return (1);
	}
	while (++i < data->n_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philosophers,
				&data->philos[i]) != 0)
			return (cleanup(data), 0);
	}
	if (pthread_create(&data->death_thread, NULL, death_checker, data) != 0)
		return (cleanup(data), 0);
	i = -1;
	while (++i < data->n_philos)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			return (cleanup(data), 0);
	}
	if (pthread_join(data->death_thread, NULL) != 0)
		return (cleanup(data), 0);
	return (1);
}
[file content end]