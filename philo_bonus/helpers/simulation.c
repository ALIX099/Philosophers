/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 23:26:47 by macbookpro        #+#    #+#             */
/*   Updated: 2025/07/26 22:01:50 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	one_philo(t_data *data)
{
	sem_wait(data->forks);
	safe_print(&data->philos[0], "%lld %d has taken a fork\n");
	ft_usleep(data->time_to_die, data);
	sem_wait(data->state);
	data->someone_died = 1;
	sem_post(data->state);
	sem_wait(data->print);
	printf("%lld %d died\n", timestamp_in_ms() - data->start_time,
		data->philos[0].philo_id);
	sem_post(data->print);
	sem_post(data->forks);
}

void	ft_simulation(t_data *data, int num)
{
	if (num == 1)
		one_philo(data);
	// else
	// 	normal_simulation(data);
}