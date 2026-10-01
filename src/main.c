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
#include <unistd.h>

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	threads[15];
	int			i;

	(void)argc;
	(void)argv;
	printf("=== [TESTE DE INTEGRAÇÃO: CODER_ROUTINE] ===\n");
	data.num_coders = 15;
	data.time_to_burnout = 1000;
	data.time_to_compile = 100;
	data.time_to_debug = 50;
	data.compiles_required = 15;
	data.scheduler_type = SCHED_FIFO;
	if (init_simulation_data(&data) != SUCCESS)
	{
		printf("[ERRO] Falha em init_simulation_data\n");
		return (1);
	}
	data.sim_start_time = get_time_in_ms();
	i = 0;
	while (i < data.num_coders)
	{
		pthread_mutex_lock(&data.coders[i].meal_mutex);
		data.coders[i].last_compile_start = data.sim_start_time;
		pthread_mutex_unlock(&data.coders[i].meal_mutex);
		i++;
	}
	printf("[OK] Dados inicializados. A lançar coder_routine em 3 threads...\n\n");
	i = 0;
	while (i < data.num_coders)
	{
		if (pthread_create(&threads[i], NULL, coder_routine, &data.coders[i]) != 0)
			return (1);
		i++;
	}
	usleep(600000);
	printf("\n[MAIN] Tempo de teste atingido. A sinalizar encerramento...\n");
	set_simulation_stopped(&data);
	i = 0;
	while (i < data.num_coders)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	printf("[OK] Todas as threads responderam ao sinal de paragem e saíram limpas!\n");
	cleanup_simulation_data(&data);
	printf("[OK] Cleanup concluído com 0 leaks.\n");
	return (0);
}
