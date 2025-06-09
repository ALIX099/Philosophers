/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 01:34:18 by abouknan          #+#    #+#             */
/*   Updated: 2025/06/09 07:32:15 by abouknan         ###   ########.fr       */
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

int	init_data(t_data *data, int ac, char **av)
{
	data->n_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->max_meals = -1;
	if (ac > 5)
		data->max_meals = ft_atoi(av[5]);
	if (data->n_philos == 0)
		return (0);
	data->someone_died = 0;
	data->forks = init_forks(data);
	if (!data->forks)
		return (0);
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (destroy_forks(data->forks, data->n_philos), 0);
	data->philos = malloc(sizeof(t_philo) * data->n_philos);
	if (!data->philos)
		return (destroy_forks(data->forks, data->n_philos),
			pthread_mutex_destroy(&data->print_mutex), 0);
	return (1);
}

void	assign_forks(t_philo *philos)
{
	pthread_mutex_t	*tmp;

	philos->left_fork = &philos->data->forks[philos->philo_id
		% philos->data->n_philos];
	philos->right_fork = &philos->data->forks[(philos->philo_id + 1)
		% philos->data->n_philos];
	if (philos->philo_id % 2)
	{
		tmp = philos->left_fork;
		philos->left_fork = philos->right_fork;
		philos->right_fork = tmp;
	}
}

void	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		data->philos[i].philo_id = i;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = 0;
		data->philos[i].data = data;
		assign_forks(&data->philos[i]);
		i++;
	}
}
