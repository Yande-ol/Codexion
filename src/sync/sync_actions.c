/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_actions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-29 18:52:37 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-09-29 18:52:37 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long long	calc_priority(t_coder *coder, t_dongle *dongle)
{
	long long	priority;

	if (coder->data->scheduler_type == SCHED_FIFO)
		priority = get_time_in_ms();
	else
	{
		pthread_mutex_lock(&coder->meal_mutex);
		priority = coder->last_compile_start + coder->data->time_to_burnout;
		pthread_mutex_unlock(&coder->meal_mutex);
	}
	(void)dongle;
	return (priority);
}

static int	take_single_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	req;
	t_request	top;

	req.coder_id = coder->id;
	req.deadline = calc_priority(coder, dongle);
	req.request_time = get_time_in_ms();
	pthread_mutex_lock(&dongle->mutex);
	if (heap_push(&dongle->queue, req) != SUCCESS)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (ERROR);
	}
	while (1)
	{
		if (is_simulation_stopped(coder->data))
		{
			pthread_mutex_unlock(&dongle->mutex);
			return (ERROR);
		}
		if (!dongle->is_in_use && heap_peek(&dongle->queue, &top) == SUCCESS
			&& top.coder_id == coder->id)
			break ;
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	heap_pop(&dongle->queue, &top);
	dongle->is_in_use = 1;
	pthread_mutex_unlock(&dongle->mutex);
	print_status(coder, "has taken a dongle");
	return (SUCCESS);
}

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left_dongle->id < coder->right_dongle->id)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (take_single_dongle(coder, first) != SUCCESS)
		return (ERROR);
	if (take_single_dongle(coder, second) != SUCCESS)
		return (ERROR);
	return (SUCCESS);
}

void	release_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->mutex);
	coder->left_dongle->is_in_use = 0;
	coder->left_dongle->last_released_time = get_time_in_ms();
	pthread_cond_broadcast(&coder->left_dongle->cond);
	pthread_mutex_unlock(&coder->left_dongle->mutex);

	pthread_mutex_lock(&coder->right_dongle->mutex);
	coder->right_dongle->is_in_use = 0;
	coder->right_dongle->last_released_time = get_time_in_ms();
	pthread_cond_broadcast(&coder->right_dongle->cond);
	pthread_mutex_unlock(&coder->right_dongle->mutex);
}
