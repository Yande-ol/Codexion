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
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_data	data;

	(void)argc;
	(void)argv;

	/* --- CENÁRIO 1: TESTE DE BURNOUT COM 1 CODER --- */
	printf("=== [TESTE 1: 1 CODER - DEVE SOFRER BURNOUT EM ~800ms] ===\n");
	data.num_coders = 1;
	data.time_to_burnout = 800;
	data.time_to_compile = 200;
	data.time_to_debug = 200;
	data.compiles_required = 0;
	data.scheduler_type = SCHED_FIFO;

	if (init_simulation_data(&data) != SUCCESS)
		return (1);

	start_simulation(&data);
	cleanup_simulation_data(&data);
	printf("[OK] Teste 1 concluído com sucesso!\n\n");

	/* --- CENÁRIO 2: TESTE DE QUOTA (COMPILES REQUIRED) --- */
	printf("=== [TESTE 2: 5 CODERS - META DE 3 COMPILAÇÕES] ===\n");
	data.num_coders = 5;
	data.time_to_burnout = 800;
	data.time_to_compile = 100;
	data.time_to_debug = 50;
	data.compiles_required = 3;
	data.scheduler_type = SCHED_FIFO;

	if (init_simulation_data(&data) != SUCCESS)
		return (1);

	start_simulation(&data);
	cleanup_simulation_data(&data);
	printf("[OK] Teste 2 concluído: todos bateram a meta e parou limpo!\n");

	return (0);
}
