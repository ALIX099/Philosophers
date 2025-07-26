/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 03:07:28 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/12 01:05:24 by macbookpro       ###   ########.fr       */
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

static void	pars_function(int ac, char **av)
{
	char	**splited_arr;
	char	*joined_str;

	joined_str = join_function(ac, av);
	if (!joined_str)
		return (printf(RED "Error: while allocating\n" RESET), exit(1));
	splited_arr = ft_split(joined_str);
	if (!splited_arr)
		return (free(joined_str), printf(RED "Error: while allocating\n" RESET),
			exit(1));
	free(joined_str);
	if (!valid_arguments(ac, splited_arr))
		return (free_split(splited_arr),
			printf(RED "Error: invalid argument(s).\n" RESET), exit(1));
	free_split(splited_arr);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac > 6 || ac < 5)
		return (printf(RED "Error: invalid argument(s).\n" RESET), 1);
	pars_function(ac, av);
	if (!init_data(&data, ac, av))
		return (printf(RED "Error: Initialization failed.\n" RESET), 1);
	init_philosophers(&data);
	if (!philo_simulation(&data))
		return (printf(RED "Error: Simulation failed to execute.\n" RESET), 1);
	cleanup(&data);
}
