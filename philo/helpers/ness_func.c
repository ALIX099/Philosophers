/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ness_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 05:40:44 by abouknan          #+#    #+#             */
/*   Updated: 2025/06/20 02:20:36 by abouknan         ###   ########.fr       */
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
			return (-1);
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

int	check_philo_death(t_data *data, int i)
{
	long	now;

	pthread_mutex_lock(&data->meal_mutex);
	now = timestamp_in_ms();
	if (now - data->philos[i].last_meal_time > data->time_to_die)
	{
		pthread_mutex_unlock(&data->meal_mutex);
		pthread_mutex_lock(&data->mutex);
		data->someone_died = 1;
		printf("%lld %d died\n", now - data->start_time,
			data->philos[i].philo_id);
		pthread_mutex_unlock(&data->mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->meal_mutex);
	return (0);
}

int	check_meals_completion(t_data *data)
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
