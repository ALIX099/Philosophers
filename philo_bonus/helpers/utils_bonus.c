/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:21:25 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/29 01:12:59 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	clean_exit(t_philo *philo, int status)
{
	sem_close(philo->data->forks);
	sem_close(philo->data->sem_meal);
	sem_close(philo->data->died_sem);
	sem_close(philo->data->sem_print);
	sem_close(philo->data->state);
	if (philo->data->philos)
		free(philo->data->philos);
	exit(status);
}

void	safe_print(t_philo *philo, const char *msg)
{
	if (philo->data->someone_died)
		return ;
	sem_wait(philo->data->sem_print);
	printf("%ld %d %s\n", timestamp_in_ms() - philo->data->start_time,
		philo->philo_id, msg);
	sem_post(philo->data->sem_print);
}

void	kill_all(t_data *data)
{
	int i = -1;
	while (++i < data->n_philos)
		kill(data->philos[i].pid, SIGKILL);
}