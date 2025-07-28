/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:12:07 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/28 20:14:33 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	init_semaphores(t_data *data)
{
	sem_unlink(SEM_NAME);
	sem_unlink(SEM_STOP);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_MEAL);
	sem_unlink(SEM_ROOM);
	data->forks = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, data->n_philos);
	data->sem_stop = sem_open(SEM_STOP, O_CREAT | O_EXCL, 0644, 1);
	data->sem_print = sem_open(SEM_PRINT, O_CREAT | O_EXCL, 0644, 1);
	data->sem_meal = sem_open(SEM_MEAL, O_CREAT | O_EXCL, 0644, 1);
	data->state = sem_open(SEM_ROOM, O_CREAT | O_EXCL, 0644, 1);
	if (data->forks == SEM_FAILED || data->sem_print == SEM_FAILED
		|| data->sem_meal == SEM_FAILED || data->state == SEM_FAILED
		|| data->sem_stop == SEM_FAILED)
		print_error("open semaphores fails\n", data);
}

static void	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		data->philos[i].philo_id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = timestamp_in_ms();
		data->philos[i].data = data;
		i++;
	}
}

void	init_data(t_data *data, int ac, char **av)
{
	data->n_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->max_meals = -1;
	if (ac > 5)
	{
		data->max_meals = ft_atoi(av[5]);
		if (data->max_meals <= 0 || data->max_meals == INT_MAX - 1)
			return (printf(RED "Error : An argument is unacceptable\n"),
				exit(EXIT_FAILURE));
	}
	if (data->n_philos <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0)
		return (printf(RED "Error : An argument is unacceptable\n"), exit(1));
	init_semaphores(data);
	data->philos = (t_philo *)malloc(sizeof(t_philo) * data->n_philos);
	if (!data->philos)
		return (clear_data(data), printf(RED "Error : While Allocating\n"),
			exit(EXIT_FAILURE));
	init_philos(data);
}
