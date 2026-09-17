/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-17 13:15:24 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-09-17 13:15:24 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_pop(t_heap *heap, t_request *out_req)
{
	if (!heap || heap->size == 0)
		return (ERROR);
	if (out_req)
		*out_req = heap->requests[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->requests[0] = heap->requests[heap->size];
		sift_down(heap, 0);
	}
	return (SUCCESS);
}

int	heap_peek(t_heap *heap, t_request *out_req)
{
	if (!heap || heap->size == 0 || !out_req)
		return (ERROR);
	*out_req = heap->requests[0];
	return (SUCCESS);
}
