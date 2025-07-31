/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:21:18 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/31 06:46:12 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	assign_death_flag(t_data *data)
{
	sem_wait(data->died_sem);
	data->someone_died = 1;
	sem_post(data->died_sem);
}

void	one_philo(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	sem_wait(data->forks);
	safe_print(philo, "has taken a fork");
	ft_usleep(data, data->time_to_die);
	sem_post(data->forks);
	exit(EXIT_FAILURE);
}

void	eating(t_philo *philo)
{
	sem_wait(philo->data->state);
	sem_wait(philo->data->forks);
	safe_print(philo, "has taken a fork");
	sem_wait(philo->data->forks);
	safe_print(philo, "has taken a fork");
	sem_wait(philo->data->meal_sem);
	philo->last_meal_time = timestamp_in_ms();
	philo->meals_eaten++;
	sem_post(philo->data->state);
	sem_post(philo->data->meal_sem);
	safe_print(philo, "is eating");
	ft_usleep(philo->data, philo->data->time_to_eat);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	*cycle(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->data->meal_sem);
		if (timestamp_in_ms()
			- philo->last_meal_time > philo->data->time_to_die)
		{
			sem_post(philo->data->meal_sem);
			if (sem_wait(philo->data->sem_print) == 0)
			{
				printf("%ld %d died\n", timestamp_in_ms()
					- philo->data->start_time, philo->philo_id);
				(assign_death_flag(philo->data), exit(EXIT_FAILURE));
			}
		}
		if (philo->meals_eaten >= philo->data->max_meals
			&& philo->data->max_meals >= 0)
			return (sem_post(philo->data->meal_sem),
				assign_death_flag(philo->data), (void *)0);
		(sem_post(philo->data->meal_sem), usleep(100));
	}
	return (NULL);
}

void	simulation(t_philo *philo)
{
	pthread_t	thread;

	if (pthread_create(&thread, NULL, cycle, philo))
		return (ft_cleanup(philo->data), exit(-1));
	while (!check_death(philo->data))
	{
		eating(philo);
		usleep(100);
		if (!safe_print(philo, "is sleeping"))
			break ;
		ft_usleep(philo->data, philo->data->time_to_sleep);
		if (!safe_print(philo, "is thinking"))
			break ;
	}
	pthread_join(thread, NULL);
	exit(EXIT_SUCCESS);
}
