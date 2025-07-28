/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ness_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 20:56:17 by macbookpro        #+#    #+#             */
/*   Updated: 2025/07/28 13:41:42 by macbookpro       ###   ########.fr       */
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
		if ((mul == 1 && result > INT_MAX) || (mul == -1 && -result < INT_MIN))
			return (INT_MAX - 1);
	}
	return ((int)(result * mul));
}

void	ft_cleanup(t_data *data)
{
	if (data->philos)
		free(data->philos);
	sem_unlink("/print");
	sem_unlink("/forks");
	sem_unlink("/state");
	if (data->print)
		sem_close(data->print);
	if (data->forks)
		sem_close(data->forks);
	if (data->state)
		sem_close(data->state);
}

void	ft_usleep(long time_to_sleep, t_philo *philo)
{
	long long	start_time;
	long long	elapsed;

	start_time = timestamp_in_ms();
	while (1)
	{
		elapsed = timestamp_in_ms() - start_time;
		if (elapsed >= time_to_sleep)
			break;
		usleep(100);
		sem_wait(philo->data->state);
		if (philo->data->someone_died)
		{
			sem_post(philo->data->state);
			break;
		}
		sem_post(philo->data->state);
	}
}

void	safe_print(t_philo *philo, const char *msg)
{
	sem_wait(philo->data->print);
	if (philo->data->someone_died)
	{
		sem_post(philo->data->print);
		return ;
	}
	printf(msg, timestamp_in_ms() - philo->data->start_time, philo->philo_id);
	sem_post(philo->data->print);
}

void	exit_proc(t_data *data, const char *status, int philo_id)
{
	sem_wait(data->print);
	if (!ft_strcmp(status, "died"))
	{
		printf("%lld %d died\n", timestamp_in_ms() - data->start_time,
				philo_id);
	}
	sem_post(data->print);
	sem_close(data->forks);
	sem_close(data->state);
	sem_close(data->print);
	sem_unlink("/forks");
	sem_unlink("/state");
	sem_unlink("/print");
	if (data->philos)
		free(data->philos);
	exit(EXIT_FAILURE);
}
