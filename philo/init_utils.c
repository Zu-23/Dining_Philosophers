/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhaddoum <zhaddoum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 19:56:06 by zhaddoum          #+#    #+#             */
/*   Updated: 2022/12/07 01:51:45 by zhaddoum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(char *str, t_philo *philo)
{
	pthread_mutex_lock(philo->lock);
	printf("%ld %d %s", get_time() - philo->time_of_start,
		philo->ph_id + 1, str);
	pthread_mutex_unlock(philo->lock);
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

void	threads_init(t_arg *ph)
{
	pthread_t	the_watcher;
	int			i;

	ph->threads = malloc(sizeof(pthread_t) * ph->ph_total);
	i = -1;
	while (++i < ph->ph_total)
		pthread_create(&ph->threads[i], NULL, philosopher, &ph->philo[i]);
	pthread_create(&the_watcher, NULL, watcher, ph->philo);
	pthread_join(the_watcher, NULL);
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
	ph->ph_total = ft_atoi(argv[1]);
	ph->death_time = ft_atoi(argv[2]);
	ph->eat_time = ft_atoi(argv[3]);
	ph->sleep_time = ft_atoi(argv[4]);
	ph->philo_died = 0;
	if (argc == 5 && ph->ph_total < 1)
		return (1);
	return (0);
}
