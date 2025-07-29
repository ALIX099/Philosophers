/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 03:07:28 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/29 04:12:13 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_usleep(long time_in_ms, t_data *data)
{
	long long	start;
	long long	current;
	long long	elapsed;

	if (time_in_ms <= 0)
		return ;
	start = timestamp_in_ms();
	while (1)
	{
		pthread_mutex_lock(&data->mutex);
		current = timestamp_in_ms();
		elapsed = current - start;
		if (elapsed >= time_in_ms || data->someone_died)
		{
			pthread_mutex_unlock(&data->mutex);
			break ;
		}
		pthread_mutex_unlock(&data->mutex);
		usleep(100);
	}
}

int	main(int ac, char **av)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (ac > 6 || ac < 5)
		return (printf(RED "Error: invalid argument(s).\n" RESET), 1);
	if (!is_valid_arg(ac, av))
		return (printf(RED "Error : An or More Arguments not valid!\n" RESET),
			1);
	if (!init_data(&data, ac, av))
		return (printf(RED "Error: Initialization failed.\n" RESET), 1);
	init_philosophers(&data);
	if (!philo_simulation(&data))
		return (printf(RED "Error: Simulation failed to execute.\n" RESET), 1);
	cleanup(&data);
}
