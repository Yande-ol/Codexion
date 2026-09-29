/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_status.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-29 19:12:51 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-09-29 19:12:51 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_simulation_stopped(t_data *data)
{
	int	stopped;

	pthread_mutex_lock(&data->stop_mutex);
	stopped = data->sim_stopped;
	pthread_mutex_unlock(&data->stop_mutex);
	return (stopped);
}

void	set_simulation_stopped(t_data *data)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->sim_stopped = 1;
	pthread_mutex_unlock(&data->stop_mutex);
}
