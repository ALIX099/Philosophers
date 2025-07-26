/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ness_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 20:56:17 by macbookpro        #+#    #+#             */
/*   Updated: 2025/07/26 22:15:17 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	ft_atoi(char *str)
{
	int		mul;
	int		i;
	long	result;

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

void	ft_cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		kill(data->philos[i].pid, SIGKILL);
		i++;
	}
	if (data->philos)
		free(data->philos);
	unlink("/print");
	unlink("/forks");
	unlink("/state");
	sem_close(data->print);
	sem_close(data->forks);
	sem_close(data->state);
}

void safe_print(t_philo *philo, const char *msg)
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

void	ft_usleep(long time_in_ms, t_data *data)
{
	long long	start;
	long long	current;

	if (time_in_ms <= 0)
		return ;
	start = timestamp_in_ms();
	while (1)
	{
		sem_wait(data->state);
		if (data->someone_died)
		{
			sem_post(data->state);
			break ;
		}
		sem_post(data->state);
		current = timestamp_in_ms();
		if (current - start >= time_in_ms)
			break ;
		usleep(100);
	}
}
