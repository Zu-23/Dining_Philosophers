#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
typedef enum status
{
	THINKING,
	EATING,
	SLEEPING
}status;

typedef struct philo_attr
{
	int				ph_id;
	int				time_last_meal;
	int				sleep_time;
	int				eat_time;
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
	pthread_mutex_t lock;
	t_philo_attr	*philos;

}t_var;

void	pick_up_forks(t_philo_attr *);
void    *philosopher(void *param)
{
	t_philo_attr *philo = (t_philo_attr *)param;
	int i = 0;
	while (i < 8)
	{
		i++;
		// think();
		// lock(mutex);
		pick_up_forks(philo);
		// eat();
		// put_fork(l);
		// put_fork(r);
		// unlock(mutex);
	}
	return (NULL);
}

void	terminate(t_var *ph)
{
	int i;

	i = -1;
	while (++i < ph->num_of_philos)
		pthread_mutex_destroy(&ph->philos[i].fork);
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
		id = 0;
	else
		id = 1;
	pthread_mutex_lock(&philo->fork);
	pthread_mutex_lock(&(philo + id + 1)->fork);
	if (*philo->philo_died != 1)
	{
		// printf("%d has taken a fork\n", philo->ph_id);
		// printf("%d has taken %d philo fork\n",philo->ph_id, philo[id % ph_num  + 1].ph_id);
		printf("philo fork[%d] %p next philo[%d] fork %p\n",id, &philo->fork,id % ph_num + 1, &(philo + id)->fork);

	}
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&(philo + id + 1)->fork);
}

void	pick_up_forks(t_philo_attr *philo)
{
	int i;
	int id;
	int	ph_num;

	id = philo->ph_id;
	ph_num = philo->num_of_philos;
	i = 0;
	if (philo->ph_id % 2 == 0 && philo->ph_id + 1 != philo->num_of_philos)
	{
		pthread_mutex_lock(&philo->fork);
		pthread_mutex_lock(&(philo + id + 1)->fork);
		if (*philo->philo_died != 1)
		{
			// printf("%d has taken his fork\n", philo->ph_id);
			// printf("%d has taken %d philo fork\n",philo->ph_id, philo[id % ph_num  + 1].ph_id);
			printf("philo[%d] fork %p next philo[%d] fork %p\n",id, &philo->fork,id % ph_num + 1, &(philo + 1)->fork);
		}
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&(philo + id + 1)->fork);

	}
	else
		pick_up_forks_two(philo);
}

void	init(t_var *ph)
{
	int i;

	i = 0;
	while (i < ph->num_of_philos)
	{
		ph->philos[i].ph_id = i + 1;
		ph->philos[i].num_of_philos = ph->num_of_philos;
		ph->philos[i].philo_died = &ph->philo_died;
		i++;
	}
	i = -1;
	while (++i < ph->num_of_philos)
	{
		pthread_mutex_init(&ph->philos[i].fork, NULL);
		printf("philo[%d] fork add %p\n",ph->philos[i].ph_id,&((ph->philos) + i)->fork);
	}
	i = -1;
	while (++i < ph->num_of_philos)
		pthread_create(&ph->philos[i].philo_thread, NULL, philosopher, &ph->philos[i]);

}

int main()
{
	t_var ph;


	ph.num_of_philos = 5;
	ph.philos = malloc(sizeof(t_philo_attr) * ph.num_of_philos);
	if (ph.philos == NULL)
		return (1);
	ph.philo_died = 0;
	init(&ph);
	terminate(&ph);
}
