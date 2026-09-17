/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-05 13:37:52 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-09-05 13:37:52 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>

# include "constants.h"
# include "structures.h"

/* Parsing */
int		parse_arguments(int argc, char **argv, t_data *data);

/* Priority Queue (Min-Heap) */
int		heap_init(t_heap *heap, int capacity, t_scheduler policy);
void	heap_destroy(t_heap *heap);
void	sift_down(t_heap *heap, int index);
int		heap_push(t_heap *heap, t_request req);
int		heap_pop(t_heap *heap, t_request *out_req);
int		heap_peek(t_heap *heap, t_request *out_req);

#endif