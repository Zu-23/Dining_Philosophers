/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhaddoum <zhaddoum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 21:53:12 by zhaddoum          #+#    #+#             */
/*   Updated: 2022/12/01 18:19:03 by zhaddoum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	pick_up_forks(t_arg *ph)
{
	sem_wait(ph->fork);
	sem_wait(ph->fork);
	sem_wait(ph->print_lock);
	printf("%ld %d has taken a fork\n",
		get_time() - ph->time_of_start, ph->philo.ph_id + 1);
	printf("%ld %d has taken a fork\n",
		get_time() - ph->time_of_start, ph->philo.ph_id + 1);
	sem_post(ph->print_lock);
}

void	eat_sleep_think(t_arg *ph)
{
	sem_wait(ph->print_lock);
	printf("%ld %d is eating\n", get_time() - ph->time_of_start,
		ph->philo.ph_id + 1);
	sem_post(ph->print_lock);
	ph->philo.last_time_eaten = get_time();
	ft_usleep(ph->eat_time);
	sem_post(ph->fork);
	sem_post(ph->fork);
	sem_wait(ph->print_lock);
	printf("%ld %d is sleeping\n",
		get_time() - ph->time_of_start, ph->philo.ph_id + 1);
	sem_post(ph->print_lock);
	ft_usleep(ph->sleep_time);
	sem_wait(ph->print_lock);
	printf("%ld %d is thinking\n",
		get_time() - ph->time_of_start, ph->philo.ph_id + 1);
	sem_post(ph->print_lock);
}
