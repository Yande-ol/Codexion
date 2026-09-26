/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-25 23:52:34 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-09-25 23:52:34 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	cleanup_dongles(t_data *data)
{
	int	i;

	if (!data->dongles)
		return ;
	i = 0;
	while (i < data->num_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		pthread_cond_destroy(&data->dongles[i].cond);
		heap_destroy(&data->dongles[i].queue);
		i++;
	}
	free(data->dongles);
	data->dongles = NULL;
}

static void	cleanup_coders(t_data *data)
{
	int	i;

	if (!data->coders)
		return ;
	i = 0;
	while (i < data->num_coders)
	{
		pthread_mutex_destroy(&data->coders[i].meal_mutex);
		i++;
	}
	free(data->coders);
	data->coders = NULL;
}

void	cleanup_simulation_data(t_data *data)
{
	if (!data)
		return ;
	cleanup_coders(data);
	cleanup_dongles(data);
	pthread_mutex_destroy(&data->stop_mutex);
	pthread_mutex_destroy(&data->log_mutex);
}
