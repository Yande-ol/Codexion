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
	(void)argv;
	if (argc != 9)
	{
		write(2, "Error: Invalid number of arguments\n", 35);
		return (ERROR);
	}
	write(1, "Codexion iniciado com sucesso!\n", 31);
	return (SUCCESS);
}
