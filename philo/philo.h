/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 03:07:34 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/26 16:52:54 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define RED "\033[0;31m"
# define RESET "\033[0m"

typedef struct s_philo
{
	int				philo_id;
	pthread_t		thread;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				n_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				max_meals;
	int				someone_died;
	long long		start_time;
	int				print_mutex_init;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	mutex;
	pthread_mutex_t	*forks;
	pthread_t		death_thread;
	t_philo			*philos;
}					t_data;

int					valid_arguments(int ac, char **av);
char				*join_function(int ac, char **av);
char				**ft_split(char *str);
void				free_split(char **split);
int					ft_strlen(char *str);
char				*ft_strdup(char *s1);
int					ft_atoi(char *str);
int					init_data(t_data *data, int ac, char **av);
void				init_philosophers(t_data *data);
void				safe_print(t_philo *philo, const char *msg);
int					philo_simulation(t_data *data);
long long			timestamp_in_ms(void);
void				cleanup(t_data *data);
int					death_check(t_data *data, int i);
int					meals_check(t_data *data);
void				ft_usleep(long time_in_ms, t_data *data);

#endif