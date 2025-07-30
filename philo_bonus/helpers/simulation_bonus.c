/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:21:18 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/30 05:10:13 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	assign_death_flag(t_data *data)
{
	sem_wait(data->died_sem);
	data->someone_died = 1;
	sem_post(data->died_sem);
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
			return (sem_post(philo->data->meal_sem),
				assign_death_flag(philo->data), (void *)1);
		if (philo->meals_eaten >= philo->data->max_meals
			&& philo->data->max_meals >= 0)
			return (sem_post(philo->data->meal_sem),
				assign_death_flag(philo->data), (void *)0);
		sem_post(philo->data->meal_sem);
		usleep(500);
	}
	return (NULL);
}

void	simulation(t_philo *philo)
{
	pthread_t	thread;
	void		*death_detect;

	if (philo->data->n_philos == 1)
	{
		sem_wait(philo->data->forks);
		safe_print(philo, "has taken a fork");
		ft_usleep(philo->data, philo->data->time_to_die);
		sem_post(philo->data->forks);
		exit(EXIT_FAILURE);
	}
	if (pthread_create(&thread, NULL, cycle, philo))
		return (ft_cleanup(philo->data), exit(-1));
	while (!philo->data->someone_died)
	{
		eating(philo);
		safe_print(philo, "is sleeping");
		ft_usleep(philo->data, philo->data->time_to_sleep);
		safe_print(philo, "is thinking");
	}
	pthread_join(thread, &death_detect);
	if (death_detect == (void *)1)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
