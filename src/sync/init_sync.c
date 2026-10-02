/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sync.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-25 23:24:23 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-09-25 23:24:23 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	destroy_dongles(t_data *data, int count)
{
	while (--count >= 0)
	{
		heap_destroy(&data->dongles[count].queue);
		pthread_cond_destroy(&data->dongles[count].cond);
		pthread_mutex_destroy(&data->dongles[count].mutex);
	}
	free(data->dongles);
	data->dongles = NULL;
}

static int	init_one_dongle(t_dongle *dongle, int id, t_data *data)
{
	dongle->id = id;
	dongle->is_in_use = 0;
	dongle->last_released_time = 0;
	if (pthread_mutex_init(&dongle->mutex, NULL) != 0)
		return (ERROR);
	if (pthread_cond_init(&dongle->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->mutex);
		return (ERROR);
	}
	if (heap_init(&dongle->queue, data->num_coders,
			data->scheduler_type) != SUCCESS)
	{
		pthread_cond_destroy(&dongle->cond);
		pthread_mutex_destroy(&dongle->mutex);
		return (ERROR);
	}
	return (SUCCESS);
}

static int	init_dongles(t_data *data)
{
	int	i;

	data->dongles = malloc(sizeof(t_dongle) * data->num_coders);
	if (!data->dongles)
		return (ERROR);
	i = 0;
	while (i < data->num_coders)
	{
		if (init_one_dongle(&data->dongles[i], i, data) != SUCCESS)
			break ;
		i++;
	}
	if (i == data->num_coders)
		return (SUCCESS);
	destroy_dongles(data, i);
	return (ERROR);
}

static int	init_coders(t_data *data)
{
	int	i;

	data->coders = malloc(sizeof(t_coder) * data->num_coders);
	if (!data->coders)
		return (ERROR);
	i = 0;
	while (i < data->num_coders)
	{
		data->coders[i] = (t_coder){.id = i + 1, .data = data,
			.left_dongle = &data->dongles[i],
			.right_dongle = &data->dongles[(i + 1) % data->num_coders]};
		if (pthread_mutex_init(&data->coders[i].meal_mutex, NULL) != 0)
			break ;
		i++;
	}
	if (i == data->num_coders)
		return (SUCCESS);
	while (--i >= 0)
		pthread_mutex_destroy(&data->coders[i].meal_mutex);
	free(data->coders);
	data->coders = NULL;
	return (ERROR);
}

int	init_simulation_data(t_data *data)
{
	data->sim_stopped = 0;
	data->sim_start_time = 0;
	data->dongles = NULL;
	data->coders = NULL;
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
		return (ERROR);
	if (pthread_mutex_init(&data->log_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&data->stop_mutex), ERROR);
	if (init_dongles(data) != SUCCESS || init_coders(data) != SUCCESS)
		return (cleanup_simulation_data(data), ERROR);
	return (SUCCESS);
}
