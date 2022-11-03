#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct philosophers
{
	long			death_time;
	long			eat_time;
	long			sleep_time;
	int				ph_id;
	int				ph_total;
	int				*philo_died;
	time_t			time_of_start;
	time_t			last_time_eaten;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*lock;
}t_philo;

typedef struct variables
{
    int				death_time;
    int				eat_time;
	int				sleep_time;
	int				ph_total;
	int				philo_died;
	pthread_t		*threads;
	pthread_mutex_t	lock;
	pthread_mutex_t	*forks;
	t_philo			*philo;
}t_arg;

long int	get_time(void)
{
		struct timeval	tv;
	long			res;

	gettimeofday(&tv, NULL);
	res = 1000 * (size_t)tv.tv_sec + (size_t)tv.tv_usec / 1000;
	return (res);
}

void	ft_usleep(long int time_in_ms)
{
	long int	start_time;

	start_time = 0;
	start_time = get_time();
	while ((get_time() - start_time) < time_in_ms)
	{
		usleep(time_in_ms / 10);
	}
}




void	pick_forks_two(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(philo->lock);
	if (*philo->philo_died == 0)
	{
		printf("%ld %d has taken a fork\n", get_time() -
		philo->time_of_start, philo->ph_id + 1);
	}
	pthread_mutex_unlock(philo->lock);
}

void	pick_up_forks(t_philo *philo)
{
	if ((philo->ph_id) % 2 == 0 && philo->ph_id + 1 != philo->ph_total)
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->right_fork);
		if (*philo->philo_died == 0)
		{
			pthread_mutex_lock(philo->lock);
			printf("%ld %d has taken a fork\n", get_time() -
			philo->time_of_start, philo->ph_id + 1);
			pthread_mutex_unlock(philo->lock);
		}
		else
			pick_forks_two(philo);
	}
}

void	print_status(char *str, t_philo *philo)
{
	pthread_mutex_lock(philo->lock);
	printf("%ld %d %s",get_time() - philo->time_of_start, philo->ph_id + 1, str);
	pthread_mutex_unlock(philo->lock);
}

void	eat_sleep_think(t_philo *philo)
{
	print_status("is eating\n", philo);
	philo->last_time_eaten = get_time();
	ft_usleep(philo->eat_time);
	if (*philo->philo_died != 0)
		return ;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	print_status("is sleeping\n", philo);
	ft_usleep(philo->sleep_time);
	if (*philo->philo_died != 0)
		return ;
	print_status("is thinking\n", philo);
}


void	*philosopher(void *param)
{
	t_philo *philo;

	philo = (t_philo *) param;
	philo->last_time_eaten = get_time();
	philo->time_of_start = get_time();
	// if (philo->ph_id % 2 == 0)
	// 	ft_usleep(philo->eat_time / 10);
	while(*philo->philo_died == 0)
	{
		pick_up_forks(philo);
		eat_sleep_think(philo);
	}
	return (NULL);
}

void	*watcher(void *param)
{
	t_philo	*philo;
	long	now;
	int		i;

	philo = (t_philo *)param;
	i = 0;
	while (*philo->philo_died == 0)
	{
		i = -1;
		while (++i < philo->ph_total)
		{
			now = get_time();
			if (now - philo[i].last_time_eaten > philo[i].death_time)
			{
				*philo->philo_died = 1;
				pthread_mutex_lock(philo->lock);
				printf("%ld %d died\n", get_time() - philo->last_time_eaten, philo[i].ph_id + 1);
				pthread_mutex_unlock(philo->lock);
				return (NULL);
			}
		}
	}
	return (NULL);
}



int	digit_check(char *str)
{
	while (*str)
	{
		if ('0' <= *str && *str <= '9')
			str++;
		else
			return (1);
	}
	return (0);
}
int	get_args(int argc, char **argv, t_arg *ph)
{
	int	i;

	i = 1;
	if (argc != 5)
		return (1);
	while (argv[i])
	{
		if (digit_check(argv[i]))
			return (1);
		i++;
	}
	ph->ph_total = atoi(argv[1]);
	ph->death_time = atoi(argv[2]);
	ph->eat_time = atoi(argv[3]);
	ph->sleep_time = atoi(argv[4]);
	ph->philo_died = 0;
	if (argc == 5 && ph->ph_total < 1)
		return (1);
	return (0);
}

void	threads_init(t_arg *ph)
{
	pthread_t	the_watcher;
	int			i;

	ph->threads = malloc(sizeof(pthread_t) * ph->ph_total);
	i = -1;
	while (++i < ph->ph_total)
	{
		pthread_create(&ph->threads[i], NULL, philosopher, &ph->philo[i]);
	}
	pthread_create(&the_watcher, NULL, watcher, ph->philo);
	pthread_join(the_watcher, NULL);
}

void	mutex_and_philos(t_arg *ph)
{
	int	i;

	ph->forks = malloc(sizeof(pthread_mutex_t) * ph->ph_total);
	ph->philo = malloc(sizeof(t_philo) * ph->ph_total);
	i = -1;
	while (++i < ph->ph_total)
		pthread_mutex_init(&ph->forks[i], NULL);
	pthread_mutex_init(&ph->lock, NULL);
	i = -1;
	while (++i < ph->ph_total)
	{
		ph->philo[i].ph_id = i;
		ph->philo[i].philo_died = &ph->philo_died;
		ph->philo[i].ph_total = ph->ph_total;
		ph->philo[i].eat_time = ph->eat_time;
		ph->philo[i].death_time = ph->death_time;
		ph->philo[i].sleep_time = ph->sleep_time;
		ph->philo[i].last_time_eaten = get_time();
		ph->philo[i].lock = &ph->lock;
		ph->philo[i].left_fork = &ph->forks[i];
		ph->philo[i].right_fork = &ph->forks[(i + 1) % ph->ph_total];
	}
}

void	terminate(t_arg *ph)
{
	int	i;

	i = 0;
	while (i < ph->ph_total)
	{
		pthread_join(ph->threads[i], NULL);
		i++;
	}
}

int main(int argc, char **argv)
{
    t_arg ph;

	if (get_args(argc, argv, &ph))
		return (1);
	mutex_and_philos(&ph);
	threads_init(&ph);
	terminate(&ph);
}