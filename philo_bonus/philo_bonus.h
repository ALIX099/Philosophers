/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:21:46 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/30 05:09:50 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

# define RED "\033[0;31m"
# define RESET "\033[0m"

typedef struct s_philo
{
	int				philo_id;
	pid_t			pid;
	long			last_meal_time;
	int				meals_eaten;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				n_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			start_time;
	int				max_meals;
	int				someone_died;
	sem_t			*forks;
	sem_t			*died_sem;
	sem_t			*sem_print;
	sem_t			*meal_sem;
	sem_t			*state;
	t_philo			*philos;
}					t_data;

int					ft_atoi(const char *str);
long				timestamp_in_ms(void);
int					is_valid_arg(int ac, char **av);
void				ft_cleanup(t_data *data);
void				init_data(t_data *data, int ac, char **av);
void				simulation(t_philo *philo);
void				ft_usleep(t_data *data, long time_in_ms);
void				assign_death_flag(t_data *data);
void				*cycle(void *arg);
void				init_proc(t_data *data);
void				safe_print(t_philo *philo, const char *msg);
int					philo_id(t_philo *philos, pid_t pid);
void				kill_all(t_data *data);

#endif