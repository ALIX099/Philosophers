/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ness_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 20:56:17 by macbookpro        #+#    #+#             */
/*   Updated: 2025/07/27 03:03:57 by abouknan         ###   ########.fr       */
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
		if ((mul == 1 && result > INT_MAX) || (mul == -1 && -result < INT_MIN))
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

void	ft_usleep(long time_to_sleep, t_philo *philo)
{
	long long	start_time;

	start_time = timestamp_in_ms();
	while (timestamp_in_ms() - start_time < time_to_sleep)
	{
		if (timestamp_in_ms()
			- philo->last_meal_time >= philo->data->time_to_die)
		{
			sem_wait(philo->data->print);
			printf("%lld %d died\n", timestamp_in_ms()
				- philo->data->start_time, philo->philo_id);
			exit(1);
		}
		usleep(100);
	}
}

void	safe_print(t_philo *philo, const char *msg)
{
	sem_wait(philo->data->print);
	printf(msg, timestamp_in_ms() - philo->data->start_time, philo->philo_id);
	sem_post(philo->data->print);
}
