/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:11:14 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/02 17:33:49 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	one_philosopher(t_data *data)
{
	safe_print(&data->philos[0], "%ld %d has taken a fork\n");
	ft_usleep(data->time_to_die);
	pthread_mutex_lock(&data->mutex);
	data->someone_died = 1;
	printf("%lld %d died\n", timestamp_in_ms() - data->start_time,
		data->philos[0].philo_id);
	pthread_mutex_unlock(&data->mutex);
}

void	*check_meal_death(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < data->n_philos)
		{
			if (death_check(data, i))
				return (NULL);
		}
		if (meals_check(data))
			return (NULL);
		ft_usleep(100);
	}
	return (NULL);
}

void	eating(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	safe_print(philo, "%ld %d has taken a fork\n");
	pthread_mutex_lock(philo->right_fork);
	safe_print(philo, "%ld %d has taken a fork\n");
	pthread_mutex_lock(&philo->data->mutex);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->mutex);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return ;
	}
	pthread_mutex_unlock(&philo->data->mutex);
	safe_print(philo, "%ld %d is eating\n");
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = timestamp_in_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philosophers(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->philo_id % 2)
		ft_usleep(philo->data->time_to_eat / 2);
	while (1)
	{
		pthread_mutex_lock(&philo->data->mutex);
		if (philo->data->someone_died)
		{
			pthread_mutex_unlock(&philo->data->mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->mutex);
		eating(philo);
		safe_print(philo, "%ld %d is sleeping\n");
		ft_usleep(philo->data->time_to_sleep);
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
	if (pthread_create(&data->death_thread, NULL, check_meal_death, data) != 0)
		return (cleanup(data), 0);
	i = -1;
	while (++i < data->n_philos)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			return (cleanup(data), 0);
	}
	ft_usleep(50);
	if (pthread_join(data->death_thread, NULL) != 0)
		return (cleanup(data), 0);
	return (1);
}
