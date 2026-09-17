/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-17 13:03:16 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-09-17 13:03:16 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *heap, int capacity, t_scheduler policy)
{
	if (!heap || capacity <= 0)
		return (ERROR);
	heap->requests = (t_request *)malloc(sizeof(t_request) * capacity);
	if (!heap->requests)
		return (ERROR);
	heap->size = 0;
	heap->capacity = capacity;
	heap->policy = policy;
	return (SUCCESS);
}

void	heap_destroy(t_heap *heap)
{
	if (!heap)
		return ;
	if (heap->requests)
	{
		free(heap->requests);
		heap->requests = NULL;
	}
	heap->size = 0;
	heap->capacity = 0;
}
