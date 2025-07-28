/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:21:46 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/28 20:13:47 by abouknan         ###   ########.fr       */
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
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define RED "\033[0;31m"
# define RESET "\033[0m"

# define SEM_NAME "/philo_sem"
# define SEM_STOP "/sem_stop"
# define SEM_PRINT "/sem_print"
# define SEM_MEAL "/sem_meal"
# define SEM_ROOM "/sem_room"

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
	int				stop;
	sem_t			*forks;
	sem_t			*sem_stop;
	sem_t			*sem_print;
	sem_t			*sem_meal;
	sem_t			*state;
	t_philo			*philos;
}					t_data;

int					ft_atoi(char *str);
long				timestamp_in_ms(void);
int					is_valid_arg(int ac, char **av);
void				print_error(char *msg, t_data *data);
void				clear_data(t_data *data);
size_t				ft_strlen(const char *s);
void				init_data(t_data *data, int ac, char **av);
void				init_semaphores(t_data *data);
void				philo_routine(t_philo *philo);
void				ft_usleep(t_data *data, long duration);
void				set_stop(t_data *data);
void				*monitor(void *arg);
int					check_state(t_data *data);
void				clean_exit(t_philo *philo, int status);
void				print_status(t_philo *philo, char *msg);
pthread_t			call_monitor(t_philo *philo, void *monitor(void *));

#endif