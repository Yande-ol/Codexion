/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-23 22:54:16 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-09-23 22:54:16 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_status(t_coder *coder, const char *status)
{
	long long	timestamp;
	t_data		*data;

	data = coder->data;
	pthread_mutex_lock(&data->log_mutex);
	pthread_mutex_lock(&data->stop_mutex);
	if (!data->sim_stopped)
	{
		timestamp = get_time_in_ms() - data->sim_start_time;
		printf("%lld %d %s\n", timestamp, coder->id, status);
	}
	pthread_mutex_unlock(&data->stop_mutex);
	pthread_mutex_unlock(&data->log_mutex);
}
