/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:12:07 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/29 02:58:01 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	init_semaphores(t_data *data)
{
	sem_unlink("/forks");
	sem_unlink("/died_sem");
	sem_unlink("/print");
	sem_unlink("/meal_sem");
	sem_unlink("/state");
	data->forks = sem_open("/forks", O_CREAT, 0644, data->n_philos);
	data->died_sem = sem_open("/died_sem", O_CREAT, 0644, 1);
	data->sem_print = sem_open("/print", O_CREAT, 0644, 1);
	data->sem_meal = sem_open("/meal_sem", O_CREAT, 0644, 1);
	data->state = sem_open("/state", O_CREAT, 0644, 1);
	if (data->forks == SEM_FAILED || data->sem_print == SEM_FAILED
		|| data->sem_meal == SEM_FAILED || data->state == SEM_FAILED
		|| data->died_sem == SEM_FAILED)
		return (ft_cleanup(data),
			printf(RED "Error : While Openning Sem!\n" RESET),
			exit(EXIT_FAILURE));
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
			return (printf(RED "Error : An argument is unacceptable\n" RESET),
				exit(EXIT_FAILURE));
	}
	if (data->n_philos <= 0 || data->n_philos == INT_MAX - 1
		|| data->time_to_die <= 0 || data->time_to_die == INT_MAX - 1
		|| data->time_to_eat <= 0 || data->time_to_eat == INT_MAX - 1
		|| data->time_to_sleep <= 0 || data->time_to_sleep == INT_MAX - 1)
		return (printf(RED "Error : An argument is unacceptable\n" RESET),
			exit(1));
	init_semaphores(data);
	data->philos = (t_philo *)malloc(sizeof(t_philo) * data->n_philos);
	if (!data->philos)
		return (ft_cleanup(data),
			printf(RED "Error : While Allocating\n" RESET), exit(EXIT_FAILURE));
	init_philos(data);
}

void	wait_for_children(t_data *data)
{
	int		status;
	pid_t	pid;
	long	died_time;
	
	pid = 1;
	while (pid > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE)
		{
			died_time = timestamp_in_ms() - data->start_time;
			assign_death_flag(data);
			kill_all(data);
			sem_wait(data->sem_print);
			printf("%ld %d died\n", died_time, get_philo_id(data->philos, pid));
			sem_post(data->sem_print);
			break ;
		}
	}
	while (waitpid(-1, NULL, 0) > 0)
	{
	}
}

void	init_proc(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philos)
	{
		data->philos[i].pid = fork();
		if (data->philos[i].pid == -1)
			return (ft_cleanup(data),
				printf(RED "Error : Creating child process\n" RESET),
				exit(EXIT_FAILURE));
		if (data->philos[i].pid == 0)
			simulation(&data->philos[i]);
	}
	wait_for_children(data);
}
