/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-23 22:45:36 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-09-23 22:45:36 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time_in_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (-1);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL));
}

void	precise_sleep(long long duration_ms, t_data *data)
{
	long long	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < duration_ms)
	{
		pthread_mutex_lock(&data->stop_mutex);
		if (data->sim_stopped)
		{
			pthread_mutex_unlock(&data->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->stop_mutex);
		usleep(500);
	}
}
