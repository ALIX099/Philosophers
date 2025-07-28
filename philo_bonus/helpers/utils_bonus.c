/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:21:25 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/28 20:16:01 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	clean_exit(t_philo *philo, int status)
{
	sem_close(philo->data->forks);
	sem_close(philo->data->sem_meal);
	sem_close(philo->data->sem_stop);
	sem_close(philo->data->sem_print);
	sem_close(philo->data->state);
	if (philo->data->philos)
		free(philo->data->philos);
	exit(status);
}

pthread_t	call_monitor(t_philo *philo, void *monitor(void *))
{
	pthread_t	thread;

	if (philo->data->n_philos == 1)
	{
		print_status(philo, "has taken a fork");
		usleep(philo->data->time_to_die * 1000);
		exit(EXIT_FAILURE);
	}
	if (pthread_create(&thread, NULL, monitor, philo))
		print_error("thread creation fails\n", philo->data);
	return (thread);
}

void	print_status(t_philo *philo, char *msg)
{
	long	now;
	long	elapsed;

	if (check_state(philo->data))
		return ;
	now = timestamp_in_ms();
	elapsed = now - philo->data->start_time;
	sem_wait(philo->data->sem_print);
	if (!check_state(philo->data))
		printf("%ld %d %s\n", elapsed, philo->philo_id, msg);
	sem_post(philo->data->sem_print);
}
