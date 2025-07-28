/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 18:20:30 by macbookpro        #+#    #+#             */
/*   Updated: 2025/07/28 13:59:14 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void init_semaphores(t_data *data)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/state");
	data->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, data->n_philos);
	data->print = sem_open("/print", O_CREAT | O_EXCL, 0644, 1);
	data->state = sem_open("/state", O_CREAT | O_EXCL, 0644, 1);
	if (data->forks == SEM_FAILED || data->print == SEM_FAILED || data->state == SEM_FAILED)
		return (printf(RED "Error : While Opening a Semaphore!\n"), exit(EXIT_FAILURE));
}

void	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		data->philos[i].philo_id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].meals_to_eat = data->max_meals;
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
			return (printf(RED "Error : An argument is unacceptable\n"), exit(EXIT_FAILURE));
	}
	if (data->n_philos <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0)
		return (printf(RED "Error : An argument is unacceptable\n"), exit(1));
	data->start_time = timestamp_in_ms();
	data->someone_died = 0;
	init_semaphores(data);
	data->philos = (t_philo *)malloc(sizeof(t_philo) * data->n_philos);
	if (!data->philos)
		return (ft_cleanup(data), printf(RED "Error : While Allocating\n"),
			exit(EXIT_FAILURE));
	init_philos(data);
}

void	init_processes(t_data *data)
{
	int		i;
	int		status;

	i = -1;
	while (++i < data->n_philos)
	{
		data->philos[i].pid = fork();
		if (data->philos[i].pid == -1)
			return (ft_cleanup(data), printf(RED "Unable to Create a Process\n"),
				exit(1));
		else if (data->philos[i].pid == 0)
		{
			if (data->n_philos == 1)
				one_philo(data);
			else
				ft_simulation(&data->philos[i]);
			exit(0);
		}
	}
	i = -1;
	while (++i < data->n_philos)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE)
		{
			int j = -1;
			while (++j < data->n_philos)
				kill(data->philos[j].pid, SIGKILL);
			break;
		}
	}
}
