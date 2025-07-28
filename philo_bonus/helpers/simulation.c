/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 23:26:47 by macbookpro        #+#    #+#             */
/*   Updated: 2025/07/28 13:51:58 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	one_philo(t_data *data)
{
	t_philo	*philo;

	philo = &data->philos[0];
	sem_wait(data->forks);
	safe_print(philo, "%lld %d has taken a fork\n");
	ft_usleep(data->time_to_die, philo);
	sem_wait(data->print);
	printf("%lld %d died\n", timestamp_in_ms() - data->start_time,
		philo->philo_id);
	sem_post(data->print);
	sem_post(data->forks);
	exit(EXIT_FAILURE);
}

void eating(t_philo *philo)
{
	t_data *data = philo->data;
	
	sem_wait(data->forks);
	safe_print(philo, "%lld %d has taken a fork\n");
	sem_wait(data->forks);
	safe_print(philo, "%lld %d has taken a fork\n");
	safe_print(philo, "%lld %d is eating\n");
	
	sem_wait(data->state);
	philo->last_meal_time = timestamp_in_ms();
	philo->meals_eaten++;
	int meals_completed = (data->max_meals > 0 && 
		philo->meals_eaten >= data->max_meals);
	sem_post(data->state);
	
	ft_usleep(data->time_to_eat, philo);
	sem_post(data->forks);
	sem_post(data->forks);
	
	if (meals_completed)
		exit(EXIT_SUCCESS);
}

void *monitor(void *args)
{
	t_philo *philo = (t_philo *)args;
	
	while (1)
	{
		ft_usleep(1, philo);
		sem_wait(philo->data->state);
		if (philo->data->someone_died || 
			(timestamp_in_ms() - philo->last_meal_time >= philo->data->time_to_die))
		{
			if (!philo->data->someone_died)
				philo->data->someone_died = 1;
			sem_post(philo->data->state);
			break;
		}
		sem_post(philo->data->state);
	}
	return (NULL);
}

void	ft_simulation(t_philo *philo)
{
	pthread_t thread;
	t_data	*data = philo->data;
	
	if (pthread_create(&thread, NULL, monitor, philo))
	{
		printf(RED"Error: Fail in Thread Creating!\n");
		exit(EXIT_FAILURE);
	}
	pthread_detach(thread);
	if (philo->philo_id % 2)
		ft_usleep(1, philo);
	while (1)
	{
		sem_wait(data->state);
		if (data->someone_died)
		{
			sem_post(data->state);
			exit_proc(data, "died", philo->philo_id);
		}
		sem_post(data->state);
		eating(philo);
		safe_print(philo, "%lld %d is sleeping\n");
		ft_usleep(data->time_to_sleep, philo);
		safe_print(philo, "%lld %d is thinking\n");
	}
}
