/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ness_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 05:40:44 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/29 03:18:42 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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
			return (-2);
	}
	return ((int)(result * mul));
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	if (data->forks)
	{
		while (i < data->n_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	if (data->print_mutex_init != 0)
		pthread_mutex_destroy(&data->mutex);
	if (data->philos)
	{
		pthread_mutex_destroy(&data->meal_mutex);
		free(data->philos);
	}
}

void	safe_print(t_philo *philo, const char *msg)
{
	pthread_mutex_lock(&philo->data->mutex);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->mutex);
		return ;
	}
	printf(msg, timestamp_in_ms() - philo->data->start_time, philo->philo_id);
	pthread_mutex_unlock(&philo->data->mutex);
}

int	death_check(t_data *data, int i)
{
	long long	now;
	long long	last_meal;

	pthread_mutex_lock(&data->meal_mutex);
	now = timestamp_in_ms();
	last_meal = data->philos[i].last_meal_time;
	if (now - last_meal > data->time_to_die)
	{
		pthread_mutex_unlock(&data->meal_mutex);
		pthread_mutex_lock(&data->mutex);
		if (!data->someone_died)
		{
			data->someone_died = 1;
			printf("%lld %d died\n", now - data->start_time,
				data->philos[i].philo_id);
		}
		pthread_mutex_unlock(&data->mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->meal_mutex);
	return (0);
}

int	meals_check(t_data *data)
{
	int	i;
	int	full_philos;

	if (data->max_meals == -1)
		return (0);
	full_philos = 0;
	i = -1;
	while (++i < data->n_philos)
	{
		pthread_mutex_lock(&data->meal_mutex);
		if (data->philos[i].meals_eaten >= data->max_meals)
			full_philos++;
		pthread_mutex_unlock(&data->meal_mutex);
	}
	if (full_philos == data->n_philos)
	{
		pthread_mutex_lock(&data->mutex);
		data->someone_died = 1;
		pthread_mutex_unlock(&data->mutex);
		return (1);
	}
	return (0);
}
