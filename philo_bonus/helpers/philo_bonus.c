/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:21:18 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/28 20:15:52 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../philo_bonus.h"

void take_forks(t_philo *philo)
{
	sem_wait(philo->data->state);
	sem_wait(philo->data->forks);
	print_status(philo, "has taken a fork");
	sem_wait(philo->data->forks);
	print_status(philo, "has taken a fork");
	sem_post(philo->data->state);
}

void	set_stop(t_data *data)
{
	sem_wait(data->sem_stop);
	if (!data->stop)
		data->stop = 1;
	sem_post(data->sem_stop);
}

int	check_state(t_data *data)
{
	int	res;

	sem_wait(data->sem_stop);
	res = data->stop;
	sem_post(data->sem_stop);
	return (res);
}

void	*monitor(void *arg)
{
	t_philo	*philo;
	long	now;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->data->sem_meal);
		now = timestamp_in_ms();
		if (now - philo->last_meal_time >= philo->data->time_to_die)
		{
			set_stop(philo->data);
			sem_post(philo->data->sem_meal);
			return ((void *)1);
		}
		if (philo->meals_eaten >= philo->data->max_meals
			&& philo->data->max_meals > -1)
		{
			set_stop(philo->data);
			sem_post(philo->data->sem_meal);
			return (NULL);
		}
		sem_post(philo->data->sem_meal);
		usleep(500);
	}
	return (NULL);
}

void	philo_routine(t_philo *philo)
{
	pthread_t	th;
	void		*res;

	th = call_monitor(philo, monitor);
	while (!check_state(philo->data))
	{
		take_forks(philo);
		sem_wait(philo->data->sem_meal);
		philo->last_meal_time = timestamp_in_ms();
		sem_post(philo->data->sem_meal);
		print_status(philo, "is eating");
		ft_usleep(philo->data, philo->data->time_to_eat);
		sem_wait(philo->data->sem_meal);
		philo->meals_eaten++;
		sem_post(philo->data->sem_meal);
		sem_post(philo->data->forks);
		sem_post(philo->data->forks);
		print_status(philo, "is sleeping");
		ft_usleep(philo->data, philo->data->time_to_sleep);
		print_status(philo, "is thinking");
	}
	pthread_join(th, &res);
	if (res)
		clean_exit(philo, EXIT_FAILURE);
	clean_exit(philo, EXIT_SUCCESS);
}
