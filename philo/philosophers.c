/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhaddoum <zhaddoum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:46:33 by zhaddoum          #+#    #+#             */
/*   Updated: 2022/11/04 21:04:46 by zhaddoum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	terminate(t_arg *ph)
{
	int	i;

	i = 0;
	while (i < ph->ph_total)
	{
		pthread_join(ph->threads[i], NULL);
		i++;
	}
	i = 0;
	while (i < ph->ph_total)
	{
		pthread_mutex_unlock(&ph->forks[i]);
		pthread_mutex_destroy(&ph->forks[i]);
		i++;
	}
	pthread_mutex_unlock(&ph->lock);
	pthread_mutex_destroy(&ph->lock);
	free(ph->philo);
	free(ph->forks);
	free(ph->threads);
}

int	main(int argc, char **argv)
{
	t_arg	ph;

	if (get_args(argc, argv, &ph))
	{
		printf("incorrect arguments\n");
		return (1);
	}
	mutex_and_philos(&ph);
	threads_init(&ph);
	terminate(&ph);
}
