/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_and_watcher.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhaddoum <zhaddoum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 20:02:35 by zhaddoum          #+#    #+#             */
/*   Updated: 2022/12/07 01:39:16 by zhaddoum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher(void *param)
{
	t_philo	*philo;

	philo = (t_philo *) param;
	philo->last_time_eaten = get_time();
	philo->time_of_start = get_time();
	while (*philo->philo_died == 0)
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
	while (*philo->philo_died == 0)
	{
		if (*philo->philo_died == 1)
		{
			pthread_mutex_lock(philo->lock);
			printf("%d\n",*philo->philo_died);
			pthread_mutex_unlock(philo->lock);
		}
		i = -1;
		while (++i < philo->ph_total)
		{
			now = get_time();
			if (now - philo[i].last_time_eaten > philo[i].death_time)
			{
				*philo->philo_died = 1;
				pthread_mutex_lock(philo->lock);
				printf("%ld %d died\n", get_time()
					- philo->last_time_eaten, philo[i].ph_id + 1);
				// pthread_mutex_unlock(philo->lock);
				return (NULL);
			}
		}
	}
	return (NULL);
}
