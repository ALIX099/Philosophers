/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 03:07:28 by abouknan          #+#    #+#             */
/*   Updated: 2025/06/14 20:33:47 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		return (printf(RED "Error: Simulation failed to execute.\n" RESET),
			1);
}
