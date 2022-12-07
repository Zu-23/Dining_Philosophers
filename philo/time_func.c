/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhaddoum <zhaddoum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 19:52:56 by zhaddoum          #+#    #+#             */
/*   Updated: 2022/12/07 01:50:50 by zhaddoum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	get_time(void)
{
	struct timeval	tv;
	long			res;

	gettimeofday(&tv, NULL);
	res = 1000 * (size_t)tv.tv_sec + (size_t)tv.tv_usec / 1000;
	return (res);
}

void	ft_usleep(long int time_in_ms)
{
	long int	start_time;

	start_time = 0;
	start_time = get_time();
	while ((get_time() - start_time) < time_in_ms)
	{
		usleep(time_in_ms / 10);
	}
}

int	ft_atoi(const char *str)
{
	long		i;
	long		sign;
	long		ret;
	char		*ptr;

	i = 0;
	sign = 1;
	ret = 0;
	ptr = (char *) str;
	while ((ptr[i] == '\t') || (ptr[i] == '\n') || (ptr[i] == '\v')
		|| (ptr[i] == '\f') || (ptr[i] == ' ') || (ptr[i] == '\r'))
		i++;
	if (ptr[i] == '-' || ptr[i] == '+')
	{
		if (ptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (ptr[i] >= '0' && ptr[i] <= '9' && ptr[i])
	{
		ret = ret * 10 + (ptr[i] - 48);
		i++;
	}
	return (ret * sign);
}