/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Yande-ol <Yande-ol@student.42porto.com>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-12 12:54:40 by Yande-ol          #+#    #+#             */
/*   Updated: 2026-09-12 12:54:40 by Yande-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
** Verifica se a string contém apenas dígitos (0-9)
** e se não é uma string vazia.
*/
static int	is_numeric(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/*
** Converte string para long long positivo, tratando overflow básico.
*/
static long long	ft_atoll_pos(const char *str)
{
	long long	res;
	int			i;

	res = 0;
	i = 0;
	while (str[i])
	{
		res = (res * 10) + (str[i] - '0');
		if (res < 0)
			return (-1);
		i++;
	}
	return (res);
}

/*
** Valida o oitavo argumento e define o enum correspondente.
*/
static int	parse_scheduler(const char *str, t_scheduler *policy)
{
	if (strcmp(str, "fifo") == 0)
	{
		*policy = POLICY_FIFO;
		return (SUCCESS);
	}
	else if (strcmp(str, "edf") == 0)
	{
		*policy = POLICY_EDF;
		return (SUCCESS);
	}
	return (ERROR);
}

/*
** Orquestra a validação e preenche a estrutura t_data.
*/
int	parse_arguments(int argc, char **argv, t_data *data)
{
	int	i;

	if (argc != 9)
	{
		fprintf(stderr, "Error: Invalid number of arguments.\n");
		return (ERROR);
	}
	i = 1;
	while (i <= 7)
	{
		if (!is_numeric(argv[i]))
		{
			fprintf(stderr, "Error: Argument %d must be a positive integer.\n", i);
			return (ERROR);
		}
		i++;
	}
	data->num_coders = (int)ft_atoll_pos(argv[1]);
	data->time_to_burnout = ft_atoll_pos(argv[2]);
	data->time_to_compile = ft_atoll_pos(argv[3]);
	data->time_to_debug = ft_atoll_pos(argv[4]);
	data->time_to_refactor = ft_atoll_pos(argv[5]);
	data->compiles_required = (int)ft_atoll_pos(argv[6]);
	data->dongle_cooldown = ft_atoll_pos(argv[7]);
	if (data->num_coders <= 0 || data->time_to_burnout <= 0
		|| data->time_to_compile <= 0 || data->time_to_debug <= 0
		|| data->time_to_refactor <= 0 || data->compiles_required < 0
		|| data->dongle_cooldown < 0)
	{
		fprintf(stderr, "Error: Arguments contain values out of valid range.\n");
		return (ERROR);
	}
	if (parse_scheduler(argv[8], &data->scheduler_type) != SUCCESS)
	{
		fprintf(stderr, "Error: Scheduler must be either 'fifo' or 'edf'.\n");
		return (ERROR);
	}
	return (SUCCESS);
}