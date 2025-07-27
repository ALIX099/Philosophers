/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 23:26:47 by macbookpro        #+#    #+#             */
/*   Updated: 2025/07/27 03:04:28 by abouknan         ###   ########.fr       */
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
}

void	ft_simulation(t_data *data)
{
	t_philo	*philo;

	philo = &data->philos[0];
	while (1)
	{
		if ((data->max_meals == 0) || (timestamp_in_ms()
				- philo->last_meal_time >= data->time_to_die))
			exit(0);
		if (data->n_philos % 2 == 0)
			ft_usleep(data->time_to_sleep / 2, philo);
		sem_wait(data->forks);
		safe_print(philo, "%lld %d has taken a fork\n");
		sem_wait(data->forks);
		safe_print(philo, "%lld %d has taken a fork\n");
		safe_print(philo, "%lld %d is eating\n");
		sem_wait(data->state);
		philo->last_meal_time = timestamp_in_ms();
		if (data->max_meals > 0)
			data->max_meals--;
		sem_post(data->state);
		ft_usleep(data->time_to_eat, philo);
		sem_post(data->forks);
		sem_post(data->forks);
		safe_print(philo, "%lld %d is sleeping\n");
		ft_usleep(data->time_to_sleep, philo);
		safe_print(philo, "%lld %d is thinking\n");
	}
}
