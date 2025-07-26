/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 08:19:21 by abouknan          #+#    #+#             */
/*   Updated: 2025/07/23 23:47:16 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
#include <semaphore.h>
#include <signal.h>

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[0;34m"
# define RESET "\033[0m"

typedef struct s_philo
{
    int             philo_id;
    pid_t           pid;
    int				meals_eaten;
	long long		last_meal_time;
    struct s_data	*data;
}t_philo;

typedef struct s_data
{
    int             n_philos;
    long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				max_meals;
	int				someone_died;
	long long		start_time;
    sem_t           *forks;
    sem_t           *print;
    
    t_philo         *philos;
}t_data;


int         is_valid_arg(int ac, char **av);
void        init_data(t_data *data, int ac, char **av);
int         ft_atoi(char *str);
void        ft_cleanup(t_data *data);
long long	timestamp_in_ms(void);
#endif