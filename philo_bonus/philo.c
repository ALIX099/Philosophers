/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 23:41:50 by macbookpro        #+#    #+#             */
/*   Updated: 2025/07/28 13:51:22 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long long timestamp_in_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac > 6 || ac < 5)
		return (printf(RED "Error : The arguments number not valid!\n"), 1);
	if (!is_valid_arg(ac, av))
		return (printf(RED "Error : An or More Arguments not valid!\n"), 1);
	init_data(&data, ac, av);
	init_processes(&data);
	ft_cleanup(&data);
}
