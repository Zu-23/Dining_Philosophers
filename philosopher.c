#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>


typedef enum stats
{
	THINKING,
	SLEEPING,
	EATING
}stat;

typedef struct variabls
{
	stat        		status[PHILOS];
	pthread_t   		philosophers[PHILOS];
	pthread_t   		forks[PHILOS];
	pthread_mutex_t		lock;
	int					sem[PHILOS];
	int					i;
}variable;

void    *philo(void *param)
{
	variable *var = (variable *)param;

	var->i = 0;
	while (1)
	{
		think();
		take_forks();
		eat();
		put_forks();
		sleep();
	}
}

void    take_forks(variable *var)
{
	pthread_mutex_lock(&var->lock);
	var->status[var->i] = THINKING;
	test(var);
	pthread_mutex_unlock(&var->lock);
	var->sem[var->i]--;//sem decr
}

void    put_forks(variable *var)
{
	pthread_mutex_lock(&var->lock);
	var->status[var->i] = THINKING;//prob sleeping
	test(var->i - 1);
	test(var->i + 1);
	pthread_mutex_unlock(&var->lock);
}

void    test(variable *var)
{
	if (var->status[var->i] == THINKING && var->status[var->i - 1] != EATING && var->status[var->i + 1] != EATING)
	{
		var->status[var->i] = EATING;
		var->sem[var->i] += 1;//sem incr
	}
}

int main()
{
	variable var;
	int     i;

	i = 0;
	pthread_mutex_init(&var.lock, NULL);
	while (i < 5)
	{
		pthread_create(var.philosophers + i, NULL, &philo, &var);
		var.status[i] = THINKING;
		var.sem[i] = 0;
		i++;
	}
	i = 0;
	while (i < 5)
	{
		pthread_join(var.philosophers[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&var.lock);
}