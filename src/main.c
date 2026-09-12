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

int	main(int argc, char **argv)
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
}
