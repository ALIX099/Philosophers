/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 01:34:18 by abouknan          #+#    #+#             */
/*   Updated: 2025/06/08 04:53:45 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static pthread_mutex_t	*init_forks(t_data *data)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * data->n_philos);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (NULL);
		i++;
	}
	return (forks);
}

static void	assign_forks(t_philo *philo)
{
	pthread_mutex_t	*temp;

	philo->left_fork = &philo->data->forks[philo->philo_id
		% philo->data->n_philos];
	philo->right_fork = &philo->data->forks[(philo->philo_id + 1)
		% philo->data->n_philos];
	if (philo->philo_id % 2)
	{
		temp = philo->left_fork;
		philo->left_fork = philo->right_fork;
		philo->right_fork = temp;
	}
}

int	init_data(t_data *data, int argc, char **argv)
{
	data->n_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->max_meals = -1;
	if (argc > 5)
		data->max_meals = ft_atoi(argv[5]);
	data->someone_died = 0;
	if (data->n_philos == 0)
		return (0);
	data->forks = init_forks(data);
	if (!data->forks)
		return (0);
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (0);
	data->philos = malloc(sizeof(t_philo) * data->n_philos);
	if (!data->philos)
		return (0);
	return (1);
}

int	init_philos(t_data *data)
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
	return (1);
}
