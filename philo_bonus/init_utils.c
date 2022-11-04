/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhaddoum <zhaddoum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 21:43:08 by zhaddoum          #+#    #+#             */
/*   Updated: 2022/11/04 22:50:46 by zhaddoum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

void	summon_watcher_and_philo(t_arg *ph)
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
			summon_watcher_and_philo(ph);
		i++;
	}
	sem_wait(ph->philo_died);
}
