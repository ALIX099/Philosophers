/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ness_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:10:31 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/30 05:08:08 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	ft_atoi(const char *str)
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
			return (-2);
	}
	return ((int)(result * mul));
}

long	timestamp_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_cleanup(t_data *data)
{
	if (data->forks)
		sem_close(data->forks);
	sem_unlink("/forks");
	if (data->sem_print)
		sem_close(data->sem_print);
	sem_unlink("/print");
	if (data->meal_sem)
		sem_close(data->meal_sem);
	sem_unlink("/meal_sem");
	if (data->state)
		sem_close(data->state);
	sem_unlink("/state");
	if (data->died_sem)
		sem_close(data->died_sem);
	sem_unlink("/died_sem");
	if (data->philos)
		free(data->philos);
}

void	safe_print(t_philo *philo, const char *msg)
{
	t_data	*data;

	data = philo->data;
	sem_wait(data->sem_print);
	if (data->someone_died)
	{
		sem_post(data->sem_print);
		return ;
	}
	printf("%ld %d %s\n", timestamp_in_ms() - data->start_time, philo->philo_id,
		msg);
	sem_post(data->sem_print);
}

int	philo_id(t_philo *philos, pid_t pid)
{
	int	i;
	int	n;

	i = -1;
	n = philos->data->n_philos;
	while (++i < n)
		if (philos[i].pid == pid)
			return (philos[i].philo_id);
	return (0);
}
