/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:18:51 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/29 12:31:00 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_usleep(t_data *data, long time)
{
	long	start;

	start = timestamp_in_ms();
	while ((timestamp_in_ms() - start < time) && !data->someone_died)
		usleep(500);
}

int	get_philo_id(t_philo *philos, pid_t pid)
{
	int	i;

	i = 0;
	while (i < philos->data->n_philos)
	{
		if (pid == philos[i].pid)
			return (philos[i].philo_id);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac > 6 || ac < 5)
		return (printf(RED "Error : The arguments number not valid!\n" RESET),
			1);
	if (!is_valid_arg(ac, av))
		return (printf(RED "Error : An or More Arguments not valid!\n" RESET),
			1);
	memset(&data, 0, sizeof(t_data));
	init_data(&data, ac, av);
	data.start_time = timestamp_in_ms();
	init_proc(&data);
	ft_cleanup(&data);
	return (0);
}
