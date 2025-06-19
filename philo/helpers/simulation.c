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
	safe_print(data->philos, "%ld %d has taken a fork\n");
	usleep(1000);
	safe_print(data->philos, "%ld %d died\n");
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
				data->someone_died = 1;
				pthread_mutex_unlock(&data->meal_mutex);
				safe_print(&data->philos[i], "%ld %d died\n");
				return (NULL);
			}
			if (data->max_meals != -1
				&& data->philos[i].meals_eaten >= data->max_meals)
				full_philos++;
			pthread_mutex_unlock(&data->meal_mutex);
		}
		if (data->max_meals != -1 && full_philos == data->n_philos)
		{
			data->someone_died = 1;
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
	usleep(100 * philo->philo_id);
	pthread_mutex_lock(philo->right_fork);
	safe_print(philo, "%ld %d has taken a fork\n");
	safe_print(philo, "%ld %d is eating\n");
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philosophers(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		eating(philo);
		usleep(philo->data->time_to_eat);
		safe_print(philo, "%ld %d is sleeping\n");
		usleep(philo->data->time_to_sleep);
		safe_print(philo, "%ld %d is thinking\n");
	}
	return (NULL);
}

int	philo_simulation(t_data *data)
{
	int	i;

	i = -1;
	if (data->n_philos == 1)
		return (one_philosopher(data), 1);
	while (++i < data->n_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philosophers,
				&data->philos[i]) != 0)
			return (cleanup(data), 0);
	}
	if (pthread_create(&data->philos->thread, NULL, death_checker, data) != 0)
			return (cleanup(data), 0);
	i = -1;
	while (++i < data->n_philos)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			return (cleanup(data), 0);
	}
	return (1);
}
