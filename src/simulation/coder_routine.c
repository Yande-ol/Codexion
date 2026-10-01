/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-30 20:35:43 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-09-30 20:35:43 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	handle_single_coder(t_coder *coder)
{
	print_status(coder, "is thinking");
	pthread_mutex_lock(&coder->left_dongle->mutex);
	print_status(coder, "has taken a dongle");
	pthread_mutex_unlock(&coder->left_dongle->mutex);
	while (!is_simulation_stopped(coder->data))
		precise_sleep(1, coder->data);
}

static void	coder_compile(t_coder *coder)
{
	print_status(coder, "is compiling");
	pthread_mutex_lock(&coder->meal_mutex);
	coder->last_compile_start = get_time_in_ms();
	coder->compiles_count++;
	pthread_mutex_unlock(&coder->meal_mutex);
	precise_sleep(coder->data->time_to_compile, coder->data);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->data->num_coders == 1)
	{
		handle_single_coder(coder);
		return (NULL);
	}
	if (coder->id % 2 == 0)
		precise_sleep(1, coder->data);
	while (!is_simulation_stopped(coder->data))
	{
		print_status(coder, "is thinking");
		if (take_dongles(coder) != SUCCESS)
			break ;
		coder_compile(coder);
		release_dongles(coder);
		if (is_simulation_stopped(coder->data))
			break ;
		print_status(coder, "is debugging");
		precise_sleep(coder->data->time_to_debug, coder->data);
	}
	return (NULL);
}
