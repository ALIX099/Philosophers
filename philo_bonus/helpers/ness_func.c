/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ness_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 20:56:17 by macbookpro        #+#    #+#             */
/*   Updated: 2025/07/28 09:47:18 by macbookpro       ###   ########.fr       */
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
	unlink("/print");
	unlink("/forks");
	unlink("/state");
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

	start_time = timestamp_in_ms();
	while (!philo->data->someone_died && (timestamp_in_ms() - start_time <= time_to_sleep))
		usleep(100);
}

void	safe_print(t_philo *philo, const char *msg)
{
	if (philo->data->someone_died)
		return ;
	sem_wait(philo->data->print);
	printf(msg, timestamp_in_ms() - philo->data->start_time, philo->philo_id);
	sem_post(philo->data->print);
}

void	exit_proc(t_data *data)
{
	int i;

	i = 0;
	while (i < data->n_philos)
	{
		kill(data->philos[i].pid, SIGKILL);
		i++;
	}
	sem_close(data->forks);
	sem_close(data->state);
	sem_close(data->print);
	if (data->philos)
		free(data->philos);
	if (data->someone_died)
		exit(1);
	else if (!data->someone_died)
		exit(0);
}
