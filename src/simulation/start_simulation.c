/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-01 15:02:49 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-10-01 15:02:49 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

static int	create_coder_threads(t_data *data, pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < data->num_coders)
	{
		pthread_mutex_lock(&data->coders[i].meal_mutex);
		data->coders[i].last_compile_start = data->sim_start_time;
		pthread_mutex_unlock(&data->coders[i].meal_mutex);
		if (pthread_create(&threads[i], NULL, coder_routine,
				&data->coders[i]) != 0)
		{
			set_simulation_stopped(data);
			while (--i >= 0)
				pthread_join(threads[i], NULL);
			return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

static void	join_all_threads(t_data *data, pthread_t *threads,
		pthread_t monitor_thread)
{
	int	i;

	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < data->num_coders)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

int	start_simulation(t_data *data)
{
	pthread_t	*threads;
	pthread_t	monitor_thread;

	threads = malloc(sizeof(pthread_t) * data->num_coders);
	if (!threads)
		return (ERROR);
	data->sim_start_time = get_time_in_ms();
	if (create_coder_threads(data, threads) != SUCCESS)
	{
		free(threads);
		return (ERROR);
	}
	if (pthread_create(&monitor_thread, NULL, monitor_routine, data) != 0)
	{
		set_simulation_stopped(data);
		join_all_threads(data, threads, monitor_thread);
		free(threads);
		return (ERROR);
	}
	join_all_threads(data, threads, monitor_thread);
	free(threads);
	return (SUCCESS);
}
