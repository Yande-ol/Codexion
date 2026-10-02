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

	(void)dongle;
	if (coder->data->scheduler_type == POLICY_FIFO)
		priority = get_time_in_ms();
	else
	{
		pthread_mutex_lock(&coder->meal_mutex);
		priority = coder->last_compile_start + coder->data->time_to_burnout;
		pthread_mutex_unlock(&coder->meal_mutex);
	}
	return (priority);
}

static int	wait_for_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	top;
	long long	elapsed;

	while (!is_simulation_stopped(coder->data))
	{
		if (!dongle->is_in_use && heap_peek(&dongle->queue, &top) == SUCCESS
			&& top.coder_id == coder->id)
		{
			elapsed = get_time_in_ms() - dongle->last_released_time;
			if (dongle->last_released_time > 0
				&& elapsed < coder->data->dongle_cooldown)
				precise_sleep(coder->data->dongle_cooldown - elapsed,
					coder->data);
			if (is_simulation_stopped(coder->data))
				break ;
			return (SUCCESS);
		}
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	pthread_mutex_unlock(&dongle->mutex);
	return (ERROR);
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
	if (wait_for_dongle(coder, dongle) != SUCCESS)
		return (ERROR);
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

	first = coder->left_dongle;
	second = coder->right_dongle;
	if (first->id > second->id)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (take_single_dongle(coder, first) != SUCCESS)
		return (ERROR);
	if (take_single_dongle(coder, second) != SUCCESS)
	{
		pthread_mutex_lock(&first->mutex);
		first->is_in_use = 0;
		first->last_released_time = get_time_in_ms();
		pthread_cond_broadcast(&first->cond);
		pthread_mutex_unlock(&first->mutex);
		return (ERROR);
	}
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
