/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-17 13:08:59 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-09-17 13:08:59 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	has_higher_priority(t_request a, t_request b, t_scheduler policy)
{
	if (policy == POLICY_FIFO)
	{
		if (a.request_time != b.request_time)
			return (a.request_time < b.request_time);
		return (a.coder_id < b.coder_id);
	}
	if (a.deadline != b.deadline)
		return (a.deadline < b.deadline);
	return (a.coder_id < b.coder_id);
}

static void	swap_requests(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sift_up(t_heap *heap, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (has_higher_priority(heap->requests[index],
				heap->requests[parent], heap->policy))
		{
			swap_requests(&heap->requests[index], &heap->requests[parent]);
			index = parent;
		}
		else
			break ;
	}
}

void	sift_down(t_heap *heap, int index)
{
	int	smallest;
	int	left;
	int	right;

	while (1)
	{
		smallest = index;
		left = (2 * index) + 1;
		right = (2 * index) + 2;
		if (left < heap->size && has_higher_priority(heap->requests[left],
				heap->requests[smallest], heap->policy))
			smallest = left;
		if (right < heap->size && has_higher_priority(heap->requests[right],
				heap->requests[smallest], heap->policy))
			smallest = right;
		if (smallest == index)
			break ;
		swap_requests(&heap->requests[index], &heap->requests[smallest]);
		index = smallest;
	}
}

int	heap_push(t_heap *heap, t_request req)
{
	if (!heap || heap->size >= heap->capacity)
		return (ERROR);
	heap->requests[heap->size] = req;
	sift_up(heap, heap->size);
	heap->size++;
	return (SUCCESS);
}
