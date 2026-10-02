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
#include <limits.h>

static long long	ft_atoll_pos(const char *str)
{
	long long	res;
	int			i;

	res = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		if (res > (LLONG_MAX - (str[i] - '0')) / 10)
			return (-1);
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res);
}

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

static int	validate_ranges(t_data *data)
{
	if (data->num_coders <= 0 || data->time_to_burnout <= 0
		|| data->time_to_compile <= 0 || data->time_to_debug <= 0
		|| data->time_to_refactor <= 0 || data->compiles_required <= 0
		|| data->dongle_cooldown < 0)
	{
		fprintf(stderr, "Error: Arguments out of valid range.\n");
		return (ERROR);
	}
	return (SUCCESS);
}

static int	parse_values(char **argv, long long *values)
{
	int	i;

	i = 0;
	while (i < 7)
	{
		if (!argv[i + 1][0])
			return (ERROR);
		values[i] = ft_atoll_pos(argv[i + 1]);
		if (values[i] < 0)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

int	parse_arguments(int argc, char **argv, t_data *data)
{
	long long	parsed_values[7];

	if (argc != 9)
		return (fprintf(stderr, "Error: Invalid number of args.\n"), ERROR);
	if (parse_values(argv, parsed_values) != SUCCESS)
		return (fprintf(stderr, "Error: Invalid numeric argument.\n"), ERROR);
	if (parsed_values[0] > INT_MAX || parsed_values[5] > INT_MAX)
		return (fprintf(stderr, "Error: Integer argument out of range.\n"),
			ERROR);
	data->num_coders = (int)parsed_values[0];
	data->time_to_burnout = parsed_values[1];
	data->time_to_compile = parsed_values[2];
	data->time_to_debug = parsed_values[3];
	data->time_to_refactor = parsed_values[4];
	data->compiles_required = (int)parsed_values[5];
	data->dongle_cooldown = parsed_values[6];
	if (validate_ranges(data) != SUCCESS)
		return (ERROR);
	if (parse_scheduler(argv[8], &data->scheduler_type) != SUCCESS)
		return (fprintf(stderr, "Error: Scheduler must be fifo/edf.\n"), ERROR);
	return (SUCCESS);
}
