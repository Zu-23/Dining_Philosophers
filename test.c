#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<unistd.h>
typedef enum status
{
	THINKING,
	EATING,
	SLEEPING
}status;

typedef struct philo_attr
{
	long			last_eat_time;
	long			start_time;
	long			sleep_time;
	long			eat_time;
	int				ph_id;
	int				num_of_philos;
	int				*philo_died;
	pthread_t		philo_thread;
	pthread_mutex_t	fork;
	pthread_mutex_t *lock;
}t_philo_attr;

typedef struct variables
{
	int				num_of_philos;
	int				philo_died;
	int				eat_time;
	int				sleep_time;
	int				death_time;
	pthread_t		the_watcher;
	pthread_mutex_t	lock;
	t_philo_attr	*philos;

}t_arg;


void	pick_up_forks(t_philo_attr *);
long	get_time(void)
{
	long				time;
	struct timeval		current_time;

	time = 0;
	if (gettimeofday(&current_time, NULL) == -1)
		return (1);
	time = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (time);
}

void	ft_usleep(long time_in_ms)
{
	long start_time;

	start_time = 0;
	start_time = get_time();
	while ((get_time() - start_time) < time_in_ms)
		usleep(time_in_ms / 10);
}

void	ft_unlock_mutex(t_philo_attr *philo)
{
	int	ph_num;
	int	id;

	ph_num = philo->num_of_philos;
	id = philo->ph_id;
	if (id == ph_num)
	{

		ph_num = (ph_num - 1) * -1;
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo[ph_num].fork);
	}
	else
	{
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&(philo + 1)->fork);
	}
}

void	print_status(char *str, t_philo_attr *philo)
{
	pthread_mutex_lock(philo->lock);
	printf("%ld %d %s", get_time() - philo->start_time, philo->ph_id, str);
	pthread_mutex_unlock(philo->lock);
}

void	eat_sleep_think( t_philo_attr *philo)
{
	if (*philo->philo_died == 0)
	{
		print_status("is eating\n", philo);
		philo->last_eat_time = get_time();
		ft_unlock_mutex(philo);
		ft_usleep(philo->eat_time);
		if (*philo->philo_died == 1)
			return ;
		print_status("is sleeping\n", philo);
		ft_usleep(philo->sleep_time);
		if (*philo->philo_died == 1)
			return ;
		print_status("is thinking\n", philo);
	}
	else
		return ;
}

void    *philosopher(void *param)
{
	t_philo_attr *philo = (t_philo_attr *)param;

	philo->start_time = get_time();
	philo->last_eat_time = get_time();
	while (*philo->philo_died != 1)
	{
		pick_up_forks(philo);
		eat_sleep_think(philo);
	}
	return (NULL);
}

void	terminate(t_arg *ph)
{
	int i;

	i = -1;
	while (++i < ph->num_of_philos)
		pthread_mutex_destroy(&ph->philos[i].fork);
	pthread_mutex_destroy(&ph->lock);
	i = -1;
	while (++i < ph->num_of_philos)
		pthread_join(ph->philos[i].philo_thread, NULL);
}

void	pick_up_forks_two(t_philo_attr *philo)
{
	int	id;
	int	ph_num;

	id = philo->ph_id;
	ph_num = philo->num_of_philos;
	if (id == ph_num)
	{
		ph_num = (ph_num - 1) * -1;
		pthread_mutex_lock(&philo->fork);
		pthread_mutex_lock(&philo[ph_num].fork);
	}
	else
	{
		pthread_mutex_lock(&philo->fork);
		pthread_mutex_lock(&(philo + 1)->fork);
	}
	if (*philo->philo_died != 1)
	{
		print_status("has taken a fork\n",philo);
		print_status("has taken a fork\n",philo);

	}
}

void	pick_up_forks(t_philo_attr *philo)
{
	int i;
	int id;

	id = philo->ph_id;
	i = 0;
	if (philo->ph_id % 2 == 0 && philo->ph_id + 1 != philo->num_of_philos)
	{
		pthread_mutex_lock(&philo->fork);
		pthread_mutex_lock(&(philo + 1)->fork);
		if (*philo->philo_died != 1)
		{
		print_status("has taken a fork\n",philo);
		print_status("has taken a fork\n",philo);
		}
	}
	else
		pick_up_forks_two(philo);
}

void *watcher(void *param)
{
	t_arg 			*ph;
	long			current_time;
	int				i;

	i = 0;
	ph = (t_arg *) param;

	while (ph->philo_died == 0)
	{
		i = 0;
		while (i < ph->num_of_philos)
		{
			current_time = get_time();
			if (current_time - ph->philos[i].last_eat_time > ph->death_time)
			{
				ph->philo_died = 1;
				print_status("died\n", ph->philos);
				return (NULL);
			}
		i++;
		}
	}
}

void	init(t_arg *ph)
{
	int i;

	i = -1;
	while (++i < ph->num_of_philos)
	{
		ph->philos[i].ph_id = i + 1;
		ph->philos[i].num_of_philos = ph->num_of_philos;
		ph->philos[i].philo_died = &ph->philo_died;
		ph->philos[i].eat_time = ph->eat_time;
		ph->philos[i].sleep_time = ph->sleep_time;
		ph->philos[i].lock = &ph->lock;
	}
	i = -1;
	while (++i < ph->num_of_philos)
		pthread_mutex_init(&ph->philos[i].fork, NULL);
	pthread_mutex_init(&ph->lock, NULL);
	i = -1;
	while (++i < ph->num_of_philos)
		pthread_create(&ph->philos[i].philo_thread, NULL, philosopher, &ph->philos[i]);
	pthread_create(&ph->the_watcher, NULL, watcher, ph);
}

int	get_args(t_arg *ph, char ** argv)
{
	ph->philo_died = 0;
	ph->num_of_philos = atoi(argv[1]);
	ph->death_time = atoi(argv[2]);
	ph->eat_time = atoi(argv[3]);
	ph->sleep_time = atoi(argv[4]);
	if (ph->num_of_philos < 1)
		return (1);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

int	check_args(char **argv)
{
	int	i;
	int	j;

	j = 1;
	while(argv[j])
	{
		i = 0;
		while(argv[j][i])
		{
			if (!ft_isdigit(argv[j][i]))
				return (1);
			i++;
		}
		j++;	
	}
}

int main(int argc, char **argv)
{
	t_arg ph;

	if (argc < 5 || check_args(argv) == 1)
		return (1);
	get_args(&ph, argv);
	ph.philos = malloc(sizeof(t_philo_attr) * ph.num_of_philos);
	if (ph.philos == NULL)
		return (1);
	init(&ph);
	terminate(&ph);
}
