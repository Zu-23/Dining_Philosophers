/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhaddoum <zhaddoum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 20:04:59 by zhaddoum          #+#    #+#             */
/*   Updated: 2022/11/03 20:05:49 by zhaddoum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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
