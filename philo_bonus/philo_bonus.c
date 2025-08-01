/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:18:51 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/02 16:15:44 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_usleep(t_data *data, long time_in_ms)
{
	long	start;

	if (time_in_ms <= 0)
		return ;
	start = timestamp_in_ms();
	while ((timestamp_in_ms() - start < time_in_ms) && !check_death(data))
		usleep(100);
}

void	kill_all(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philos)
		kill(data->philos[i].pid, SIGKILL);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac > 6 || ac < 5)
		return (printf(RED "Error : The arguments number not valid!\n" RESET),
			1);
	if (!is_valid_arg(ac, av))
		return (printf(RED "Error : An or More Arguments not valid\n" RESET),
			1);
	memset(&data, 0, sizeof(t_data));
	init_data(&data, ac, av);
	init_proc(&data);
	ft_cleanup(&data);
	return (0);
}
