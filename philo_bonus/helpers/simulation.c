/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 23:26:47 by macbookpro        #+#    #+#             */
/*   Updated: 2025/07/28 09:56:20 by macbookpro       ###   ########.fr       */
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
void eating(t_philo *philo)
{
	t_data *data;

	data = philo->data;
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
	exit(EXIT_SUCCESS);
}

void *cycle(void *args)
{
	t_philo *philo;
	
	philo = (t_philo *)args;
	while (1)
	{
		if (timestamp_in_ms()
			- philo->last_meal_time >= philo->data->time_to_die)
		{
			sem_wait(philo->data->print);
			sem_wait(philo->data->state);
			philo->data->someone_died = 1;
			sem_post(philo->data->state);
			printf("%lld %d died\n", timestamp_in_ms()
				- philo->data->start_time, philo->philo_id);
			sem_post(philo->data->print);
			break;
		}
		if (philo->data->max_meals <= 0)
			break;
	}
	return (NULL);
}

void	ft_simulation(t_philo *philo)
{
	pthread_t thread;
	t_data	*data;
	
	data = philo->data;
	if (pthread_create(&thread, NULL, cycle, philo))
		return (ft_cleanup(data), printf (RED"Error : Fail in Thread Creating!\n"), exit(EXIT_FAILURE));
	if (philo->philo_id % 2)
		ft_usleep(1, philo);
	while (1)
	{
		if (data->someone_died)
			exit_proc(data);
		eating(philo);
		safe_print(philo, "%lld %d is sleeping\n");
		ft_usleep(data->time_to_sleep, philo);
		safe_print(philo, "%lld %d is thinking\n");
	}
	pthread_join(thread, NULL);
}
