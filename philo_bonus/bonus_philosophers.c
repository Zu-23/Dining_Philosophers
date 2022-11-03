# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <fcntl.h>
# include <semaphore.h>
# include <signal.h>

typedef struct philosophers
{
	sem_t		*actions;
	int 		ph_id;
	time_t		last_time_eaten;
	int			nbr_of_meals;
}t_philo;

typedef struct variables
{
 int				death_time;
    int				eat_time;
	int				sleep_time;
	int				ph_total;
	pid_t			*pids;
	sem_t			*print_lock;
	sem_t			*fork;
	sem_t			*philo_died;
	t_philo			philo;
	pthread_t		the_watcher;
	time_t			time_of_start;
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







int	get_args(int argc, char **argv, t_arg *ph)
{
	int	i;

	i = 1;
	if (argc != 5)
		return (1);
	// while (argv[i])
	// {
	// 	if (digit_check(argv[i]))
	// 		return (1);
	// 	i++;
	// }
	ph->ph_total = atoi(argv[1]);
	ph->death_time = atoi(argv[2]);
	ph->eat_time = atoi(argv[3]);
	ph->sleep_time = atoi(argv[4]);
	ph->pids = malloc(sizeof(pid_t) * ph->ph_total);
	if (argc == 5 && ph->ph_total < 1)
		return (1);
	return (0);
}
int	semaphores(t_arg *ph)
{
	sem_unlink("fork");
	sem_unlink("print_lock");
	sem_unlink("philo_died");
	ph->fork = sem_open("fork", O_CREAT | O_EXCL, S_IRWXU, ph->ph_total);
	ph->print_lock = sem_open("print_lock", O_CREAT | O_EXCL, S_IRWXU, 1);
	ph->philo_died = sem_open("philo_died", O_CREAT | O_EXCL, S_IRWXU, 0);
	return (0);
}

void	*watcher(void *param)
{
	t_arg *ph;
	time_t	now;

	ph = (t_arg *)param;
	while(1)
	{
		now = get_time();
		if (now - ph->philo.last_time_eaten > ph->death_time)
		{
			sem_wait(ph->print_lock);
			printf("%ld %d has died\n", get_time() - ph->time_of_start, ph->philo.ph_id + 1);
			//sem_wait(ph->print_lock);
			sem_post(ph->philo_died);
			exit(0);
		}
	}
	return (NULL);
}

void	pick_up_forks(t_arg *ph)
{
	sem_wait(ph->fork);
	sem_wait(ph->fork);
	sem_wait(ph->print_lock);
	printf("%ld %lu has taken a fork\n",
		get_time() - ph->time_of_start, ph->philo.ph_id + 1);
	printf("%ld %lu has taken a fork\n",
		get_time() - ph->time_of_start, ph->philo.ph_id + 1);
	sem_post(ph->print_lock);
}

void	eat_sleep_think(t_arg *ph)
{
	sem_wait(ph->print_lock);
	printf("%ld %lu is eating\n", get_time() - ph->time_of_start,
		ph->philo.ph_id + 1);
	sem_post(ph->print_lock);
	ph->philo.last_time_eaten = get_time();
	ft_usleep(ph->eat_time);
	sem_post(ph->fork);
	sem_post(ph->fork);
	sem_wait(ph->print_lock);
	printf("%ld %lu is sleeping\n",
		get_time() - ph->time_of_start, ph->philo.ph_id + 1);
	sem_post(ph->print_lock);
	ft_usleep(ph->sleep_time);
	sem_wait(ph->print_lock);
	printf("%ld %lu is thinking\n",
		get_time() - ph->time_of_start, ph->philo.ph_id + 1);
	sem_post(ph->print_lock);
}

void	*philosopher(void *param)
{
	t_arg	*ph;

	ph = (t_arg *)param;
	ph->time_of_start= get_time();
	while (1)
	{
		pick_up_forks(ph);
		eat_sleep_think(ph);
	}
	return (NULL);
}

void	process_create(t_arg *ph)
{
	int	i;

	i = 0;
	ph->time_of_start = get_time();
	while (i < ph->ph_total)
	{
		ph->philo.ph_id = i;
		ph->pids[i] = fork();
		if (ph->pids[i] == -1)
		{
			while (--i >= 0)
				kill(ph->pids[i], SIGKILL);
			printf("error creating process!\n");
			exit(1);
		}
		else if (ph->pids[i] == 0)
		{
			ph->philo.last_time_eaten = get_time();
			if (pthread_create(&ph->the_watcher, NULL, &watcher, ph) == -1)
			{
				printf("error creating thread!\n");
				exit(1);
			}
			pthread_detach(ph->the_watcher);
			philosopher(ph);
		}
		i++;
	}
	sem_wait(ph->philo_died);
}
static void	kill_philosophers(t_arg *args)
{
	int	i;

	i = 0;
	while (i < args->ph_total)
	{
		kill(args->pids[i], SIGKILL);
		i++;
	}
}

int main(int argc, char **argv)
{
	t_arg	ph;


    get_args(argc, argv, &ph);
	semaphores(&ph);
	process_create(&ph);
	kill_philosophers(&ph);
}