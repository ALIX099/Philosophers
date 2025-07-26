/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 01:34:18 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/25 00:01:01 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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
	if (data->n_philos <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0)
		return (0);
	data->someone_died = 0;
	data->forks = init_forks(data);
	if (!data->forks)
		return (0);
	data->print_mutex_init = 0;
	if (pthread_mutex_init(&data->mutex, NULL) != 0)
		return (cleanup(data), 0);
	data->print_mutex_init = 1;
	if (pthread_mutex_init(&data->meal_mutex, NULL))
		return (cleanup(data), 0);
	data->philos = malloc(sizeof(t_philo) * data->n_philos);
	if (!data->philos)
		return (cleanup(data), 0);
	return (1);
}

void	assign_forks(t_philo *philos)
{
	int	id;
	int	n;

	id = philos->philo_id;
	n = philos->data->n_philos;
	philos->left_fork = &philos->data->forks[id - 1];
	if (id == n)
		philos->right_fork = &philos->data->forks[0];
	else
		philos->right_fork = &philos->data->forks[id];
}

void	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		data->philos[i].philo_id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = timestamp_in_ms();
		data->philos[i].data = data;
		assign_forks(&data->philos[i]);
		i++;
	}
}
