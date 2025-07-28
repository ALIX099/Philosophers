/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 08:19:21 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/28 09:51:42 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <sys/wait.h>
// #include <asm-generic/fcntl.h>

# define RED "\033[0;31m"
# define RESET "\033[0m"

typedef struct s_philo
{
	int				philo_id;
	pid_t			pid;
	int				meals_eaten;
	long long		last_meal_time;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				n_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				max_meals;
	long long		start_time;
	int				someone_died;
	sem_t			*forks;
	sem_t			*print;
	sem_t			*state;
	t_philo			*philos;
}					t_data;

int					is_valid_arg(int ac, char **av);
void				init_data(t_data *data, int ac, char **av);
int					ft_atoi(char *str);
void				ft_cleanup(t_data *data);
long long			timestamp_in_ms(void);
void				ft_simulation(t_philo *philo);
void				init_processes(t_data *data);
void				safe_print(t_philo *philo, const char *msg);
void				ft_usleep(long time_in_ms, t_philo *philo);
void				one_philo(t_data *data);
void	exit_proc(t_data *data);

#endif