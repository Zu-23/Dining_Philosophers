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
	int			philo_id;
	pthread_t	philo_thread;
	int			time_last_meal;
	pthread_mutex_t	fork;
	
}philo_attr;

typedef struct variables
{
	int				num_of_philos;
	pthread_mutex_t lock;
	philo_attr		*philos;

}var;

void	pick_up_forks(var*);
void    *philosopher(void *param)
{
	var *ph = (var *)param;

	int i = 0;
	while (i++ < 10)
	{
		// think();
		// lock(mutex);
		pick_up_forks(ph);
		// eat();
		// put_fork(l);
		// put_fork(r);
		// unlock(mutex);
	}
}
void	terminate(var *ph)
{
	int i;

	i = 0;
	while (i < ph->num_of_philos)
	{
		pthread_mutex_destroy(&ph->philos[i].fork);
		pthread_join(ph->philos[i].philo_thread, NULL);
		i++;
	}
}
void	pick_up_forks(var *ph)
{
	int id;

	int i = 0;
	while (i < 3)
	{
		id = ph->philos[i].philo_id;
		printf("%d\n",id);
		i++;

	}
	if (ph->philos[id].philo_id % 2 == 0 && ph->philos[id + 1].philo_id != ph->num_of_philos)
	{
		pthread_mutex_lock(&ph->philos[id].fork);
		pthread_mutex_lock(&ph->philos[id + 1].fork);
		printf("\n",id);
		pthread_mutex_unlock(&ph->philos[id].fork);
		pthread_mutex_unlock(&ph->philos[id + 1].fork);

	}
	else
		printf("we shall see\n");
}
void	init(var *ph)
{
	int i;

	i = 0;
	while (i < ph->num_of_philos)
	{
		ph->philos[i].philo_id = i;
		pthread_mutex_init(&ph->philos[i].fork, NULL);
		pthread_create(&ph->philos[i].philo_thread, NULL, philosopher, ph);
		i++;
	}
}

int main()
{
	var ph;

	ph.num_of_philos = 5;
	ph.philos = malloc(sizeof(philo_attr) * ph.num_of_philos);
	if (ph.philos == NULL)
		return (1);
	init(&ph);
	terminate(&ph);
}