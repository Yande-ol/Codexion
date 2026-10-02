/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-01 15:06:00 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-10-01 15:06:00 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>

/*void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!is_simulation_stopped(data))
		precise_sleep(1, data);
	return (NULL);
}*/

static int	check_all_compiled(t_data *data)
{
	int	i;
	int	finished_coders;

	if (data->compiles_required <= 0)
		return (0);
	finished_coders = 0;
	i = 0;
	while (i < data->num_coders)
	{
		pthread_mutex_lock(&data->coders[i].meal_mutex);
		if (data->coders[i].compiles_count >= data->compiles_required)
			finished_coders++;
		pthread_mutex_unlock(&data->coders[i].meal_mutex);
		i++;
	}
	if (finished_coders == data->num_coders)
	{
		set_simulation_stopped(data);
		return (1);
	}
	return (0);
}

static int	check_coder_burnout(t_data *data, t_coder *coder)
{
	long long	now;
	long long	elapsed;

	pthread_mutex_lock(&coder->meal_mutex);
	now = get_time_in_ms();
	elapsed = now - coder->last_compile_start;
	if (elapsed >= data->time_to_burnout)
	{
		pthread_mutex_unlock(&coder->meal_mutex);
		pthread_mutex_lock(&data->log_mutex);
		set_simulation_stopped(data);
		printf("%lld %d has burned out\n",
			now - data->sim_start_time, coder->id);
		pthread_mutex_unlock(&data->log_mutex);
		return (1);
	}
	pthread_mutex_unlock(&coder->meal_mutex);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!is_simulation_stopped(data))
	{
		i = 0;
		while (i < data->num_coders)
		{
			if (check_coder_burnout(data, &data->coders[i]))
				return (NULL);
			i++;
		}
		if (check_all_compiled(data))
			return (NULL);
		precise_sleep(1, data);
	}
	return (NULL);
}
