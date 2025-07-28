/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:10:31 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/28 20:19:35 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	ft_atoi(char *str)
{
	int			mul;
	int			i;
	long long	result;

	mul = 1;
	i = 0;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		mul = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i++] - '0');
		if ((mul == 1 && result > INT_MAX) || (mul == -1 && - result < INT_MIN))
			return (INT_MAX - 1);
	}
	return ((int)(result * mul));
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

long	timestamp_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	clear_data(t_data *data)
{
	if (!data)
		return ;
	if (data->forks)
		sem_close(data->forks);
	sem_unlink(SEM_NAME);
	if (data->sem_print)
		sem_close(data->sem_print);
	sem_unlink(SEM_PRINT);
	if (data->sem_meal)
		sem_close(data->sem_meal);
	sem_unlink(SEM_MEAL);
	if (data->state)
		sem_close(data->state);
	sem_unlink(SEM_ROOM);
	if (data->sem_stop)
		sem_close(data->sem_stop);
	sem_unlink(SEM_STOP);
	if (data->philos)
		free(data->philos);
}

void	print_error(char *msg, t_data *data)
{
	write(2, msg, ft_strlen(msg));
	clear_data(data);
	exit(EXIT_FAILURE);
}
