#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				n_philos;
	int 			time_to_die;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	t_philo			*philos;
}					t_data;

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
		if ((mul == 1 && result > INT_MAX) || (mul == -1 && -result < INT_MIN))
			return (-1);
	}
	return ((int)(result * mul));
}

void	safe_print(t_data *data, const char *msg, int id)
{
	pthread_mutex_lock(&data->print_mutex);
	printf(msg, id);
	pthread_mutex_unlock(&data->print_mutex);
}

void	*philosophers(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->left_fork);
	usleep(100);
	pthread_mutex_lock(philo->right_fork);
	safe_print(philo->data, "Philosopher %d is eating 🍝\n", philo->id);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	safe_print(philo->data, "Philosopher %d is thinking 💭\n", philo->id);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_data data;

	if (ac != 3)
		return (1);
	data.n_philos = ft_atoi(av[1]);
	data.time_to_die = ft_atoi(av[2]);
	data.philos = malloc(sizeof(t_philo) * data.n_philos);
	data.forks = malloc(sizeof(pthread_mutex_t) * data.n_philos);
	for (int i = 0; i < data.n_philos; i++)
		pthread_mutex_init(&data.forks[i], NULL);
	pthread_mutex_init(&data.print_mutex, NULL);
	for (int i = 0; i < data.n_philos; i++)
	{
		data.philos[i].id = i;
		data.philos[i].left_fork = &data.forks[i];
		data.philos[i].right_fork = &data.forks[(i + 1) % data.n_philos];
		if (data.philos[i].id % 2)
		{
			pthread_mutex_t *tmp;
			tmp = data.philos[i].left_fork;
			data.philos[i].left_fork = data.philos[i].right_fork;
			data.philos[i].right_fork = tmp;
		}
		data.philos[i].data = &data;
	}
	for (int i = 0; i < data.n_philos; i++)
		pthread_create(&data.philos[i].thread, NULL, philosophers,
			&data.philos[i]);
	for (int i = 0; i < data.n_philos; i++)
		pthread_join(data.philos[i].thread, NULL);
	for (int i = 0; i < data.n_philos; i++)
		pthread_mutex_destroy(&data.forks[i]);
	pthread_mutex_destroy(&data.print_mutex);
	free(data.forks);
	free(data.philos);
}
