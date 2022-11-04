/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhaddoum <zhaddoum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 21:38:55 by zhaddoum          #+#    #+#             */
/*   Updated: 2022/11/04 22:57:32 by zhaddoum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <fcntl.h>
# include <semaphore.h>
# include <signal.h>

typedef struct philosophers
{
	sem_t		*actions;
	int			ph_id;
	time_t		last_time_eaten;
	int			nbr_of_meals;
}t_philo;

typedef struct variables
{
	int				death_time;
	int				eat_time;
	int				sleep_time;
	int				ph_total;
	pid_t			*pids;
	sem_t			*print_lock;
	sem_t			*fork;
	sem_t			*philo_died;
	t_philo			philo;
	pthread_t		the_watcher;
	time_t			time_of_start;
}t_arg;

long int	get_time(void);
void		ft_usleep(long int time_in_ms);
int			get_args(int argc, char **argv, t_arg *ph);
int			semaphores(t_arg *ph);
void		*watcher(void *param);
void		pick_up_forks(t_arg *ph);
void		eat_sleep_think(t_arg *ph);
void		*philosopher(void *param);
void		process_create(t_arg *ph);
#endif