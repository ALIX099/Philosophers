/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:11:14 by abouknan          #+#    #+#             */
/*   Updated: 2025/06/14 20:29:25 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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
	eating(philo);
	safe_print(philo, "%ld %d is sleeping\n");
	safe_print(philo, "%ld %d is thinking\n");
	return (NULL);
}

int	philo_simulation(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philosophers,
				&data->philos[i]) != 0)
			return (cleanup(data), 0);
	}
	i = -1;
	while (++i < data->n_philos)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			return (cleanup(data), 0);
	}
	return (1);
}
