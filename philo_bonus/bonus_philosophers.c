/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_philosophers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhaddoum <zhaddoum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:28:23 by zhaddoum          #+#    #+#             */
/*   Updated: 2022/11/04 22:38:26 by zhaddoum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	kill_philosophers(t_arg *args)
{
	int	i;

	i = 0;
	while (i < args->ph_total)
	{
		kill(args->pids[i], SIGKILL);
		i++;
	}
	free(args->pids);
}

int	main(int argc, char **argv)
{
	t_arg	ph;

	if (get_args(argc, argv, &ph))
	{
		printf("Incorrect arguments\n");
		return (1);
	}
	semaphores(&ph);
	process_create(&ph);
	kill_philosophers(&ph);
}
