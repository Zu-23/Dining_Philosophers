/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_and_watcher.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhaddoum <zhaddoum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:15:05 by zhaddoum          #+#    #+#             */
/*   Updated: 2022/11/04 22:55:39 by zhaddoum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*philosopher(void *param)
{
	t_arg	*ph;

	ph = (t_arg *)param;
	ph->time_of_start = get_time();
	while (1)
	{
		pick_up_forks(ph);
		eat_sleep_think(ph);
	}
	return (NULL);
}

void	*watcher(void *param)
{
	t_arg	*ph;
	time_t	now;

	ph = (t_arg *)param;
	while (1)
	{
		now = get_time();
		if (now - ph->philo.last_time_eaten > ph->death_time)
		{
			sem_wait(ph->print_lock);
			printf("%ld %d has died\n", get_time() - ph->time_of_start,
				ph->philo.ph_id + 1);
			sem_post(ph->philo_died);
			exit(0);
		}
	}
	return (NULL);
}
