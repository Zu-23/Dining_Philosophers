/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhaddoum <zhaddoum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 21:05:04 by zhaddoum          #+#    #+#             */
/*   Updated: 2022/12/07 01:53:01 by zhaddoum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>

typedef struct philosophers
{
	long			death_time;
	long			eat_time;
	long			sleep_time;
	int				ph_id;
	int				ph_total;
	int				*philo_died;
	time_t			time_of_start;
	time_t			last_time_eaten;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*lock;
}t_philo;

typedef struct variables
{
	int				death_time;
	int				eat_time;
	int				sleep_time;
	int				ph_total;
	int				philo_died;
	pthread_t		*threads;
	pthread_mutex_t	lock;
	pthread_mutex_t	*forks;
	t_philo			*philo;
}t_arg;

void		threads_init(t_arg *ph);
void		mutex_and_philos(t_arg *ph);
int			get_args(int argc, char **argv, t_arg *ph);
void		*watcher(void *param);
void		*philosopher(void *param);
void		eat_sleep_think(t_philo *philo);
void		pick_up_forks(t_philo *philo);
void		pick_forks_two(t_philo *philo);
void		ft_usleep(long int time_in_ms);
long int	get_time(void);
void		print_status(char *str, t_philo *philo);
int			ft_atoi(const char *str);

#endif