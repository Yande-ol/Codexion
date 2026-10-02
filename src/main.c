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
#include <unistd.h>

static void	print_usage(void)
{
	write(2, "Usage: ./codexion <num_coders> <burnout> <compile> <debug> ", 59);
	write(2, "<refactor> <compiles_req> <cooldown> <fifo|edf>\n", 48);
}

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (parse_arguments(argc, argv, &data) != SUCCESS)
	{
		print_usage();
		return (1);
	}
	if (init_simulation_data(&data) != SUCCESS)
		return (1);
	if (start_simulation(&data) != SUCCESS)
	{
		cleanup_simulation_data(&data);
		return (1);
	}
	cleanup_simulation_data(&data);
	return (0);
}
