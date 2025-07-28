/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:18:51 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/28 20:13:11 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo_bonus.h"

void	ft_usleep(t_data *data, long time)
{
	long	start;


	start = timestamp_in_ms();
	while ((timestamp_in_ms() - start < time) && !check_state(data))
		usleep(500);
}

int	get_philo_id(t_philo *philos, pid_t pid)
{
	int	i;

	i = 0;
	while (i < philos->data->n_philos)
	{
		if (pid == philos[i].pid)
			return (philos[i].philo_id);
		i++;
	}
	return (0);
}

void	wait_for_children(t_data *data)
{
	int		i;
	int		status;
	pid_t	pid;
	long	died_time;

	pid = 1;
	i = 0;
	while (pid > 0)
	{
		pid = waitpid(-1, &status, 0);
		died_time = timestamp_in_ms() - data->start_time;
		if (WIFEXITED(status) && (WEXITSTATUS(status) == EXIT_FAILURE))
		{
			sem_wait(data->sem_print);
			printf("%ld %d %s\n", died_time, get_philo_id(data->philos, pid),
				"died");
			while (i < data->n_philos)
				kill(data->philos[i++].pid, SIGKILL);
			break ;
		}
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
}

void	fork_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		data->philos[i].pid = fork();
		if (data->philos[i].pid == -1)
			print_error("fork failed\n", data);
		else if (data->philos[i].pid == 0)
			philo_routine(&data->philos[i]);
		i++;
	}
	wait_for_children(data);
}

int	main(int ac, char *av[])
{
	t_data	data;

	if (ac > 6 || ac < 5)
		return (printf(RED "Error : The arguments number not valid!\n"), 1);
	if (!is_valid_arg(ac, av))
		return (printf(RED "Error : An or More Arguments not valid!\n"), 1);
	memset(&data, 0, sizeof(t_data));
	init_data(&data, ac, av);
	init_semaphores(&data);
	data.start_time = timestamp_in_ms();
	fork_philos(&data);
	clear_data(&data);
	return (0);
}
