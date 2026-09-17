/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-05 13:35:20 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-09-05 13:35:20 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (parse_arguments(argc, argv, &data) != SUCCESS)
		return (ERROR);
	printf("Configuracao carregada com sucesso!\n");
	printf("Coders: %d | Scheduler: %s\n",
		data.num_coders,
		data.scheduler_type == POLICY_FIFO ? "FIFO" : "EDF");
	return (SUCCESS);
}*/


/* Teste para validar a Min-Heap nos dois modos: FIFO e EDF, incluindo o critério de desempate por coder_id. */
#include "codexion.h"

static void	test_fifo(void)
{
	t_heap		heap;
	t_request	req;

	printf("--- TESTE FIFO (Menor request_time primeiro, desempate menor ID) ---\n");
	heap_init(&heap, 10, POLICY_FIFO);

	/* Inserindo fora de ordem: */
	/* Coder 3 chegou no tempo 500 */
	heap_push(&heap, (t_request){.coder_id = 3, .request_time = 500, .deadline = 1000});
	/* Coder 1 chegou no tempo 100 */
	heap_push(&heap, (t_request){.coder_id = 1, .request_time = 100, .deadline = 900});
	/* Coder 2 chegou no tempo 300 */
	heap_push(&heap, (t_request){.coder_id = 2, .request_time = 300, .deadline = 800});
	/* Coder 4 chegou empatado no tempo 100 com o Coder 1 (Coder 1 deve sair antes pelo ID) */
	heap_push(&heap, (t_request){.coder_id = 4, .request_time = 100, .deadline = 700});

	printf("Ordem esperada de saida: Coder 1 -> Coder 4 -> Coder 2 -> Coder 3\n");
	printf("Ordem obtida:           ");
	while (heap_pop(&heap, &req) == SUCCESS)
		printf("Coder %d (req:%lld) -> ", req.coder_id, req.request_time);
	printf("FIM\n\n");

	heap_destroy(&heap);
}

static void	test_edf(void)
{
	t_heap		heap;
	t_request	req;

	printf("--- TESTE EDF (Menor deadline primeiro, desempate menor ID) ---\n");
	heap_init(&heap, 10, POLICY_EDF);

	/* Inserindo fora de ordem: */
	/* Coder 1 tem deadline 900 */
	heap_push(&heap, (t_request){.coder_id = 1, .request_time = 100, .deadline = 900});
	/* Coder 2 tem deadline 200 (mais urgente!) */
	heap_push(&heap, (t_request){.coder_id = 2, .request_time = 400, .deadline = 200});
	/* Coder 3 tem deadline 600 */
	heap_push(&heap, (t_request){.coder_id = 3, .request_time = 200, .deadline = 600});
	/* Coder 4 empata na deadline 200 com Coder 2 (Coder 2 deve sair antes pelo ID) */
	heap_push(&heap, (t_request){.coder_id = 4, .request_time = 100, .deadline = 200});

	printf("Ordem esperada de saida: Coder 2 -> Coder 4 -> Coder 3 -> Coder 1\n");
	printf("Ordem obtida:           ");
	while (heap_pop(&heap, &req) == SUCCESS)
		printf("Coder %d (deadline:%lld) -> ", req.coder_id, req.deadline);
	printf("FIM\n");

	heap_destroy(&heap);
}

int	main(void)
{
	test_fifo();
	test_edf();
	return (0);
}
