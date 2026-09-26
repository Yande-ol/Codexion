#include "codexion.h"

static int	init_dongles(t_data *data)
{
	int	i;

	data->dongles = malloc(sizeof(t_dongle) * data->num_coders);
	if (!data->dongles)
		return (ERROR);
	i = 0;
	while (i < data->num_coders)
	{
		data->dongles[i].id = i;
		data->dongles[i].is_in_use = 0;
		data->dongles[i].last_released_time = 0;
		if (pthread_mutex_init(&data->dongles[i].mutex, NULL) != 0)
			return (ERROR);
		if (pthread_cond_init(&data->dongles[i].cond, NULL) != 0)
			return (ERROR);
		if (heap_init(&data->dongles[i].queue, data->num_coders,
				data->scheduler_type) != SUCCESS)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

static int	init_coders(t_data *data)
{
	int	i;

	data->coders = malloc(sizeof(t_coder) * data->num_coders);
	if (!data->coders)
		return (ERROR);
	i = 0;
	while (i < data->num_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].compiles_count = 0;
		data->coders[i].last_compile_start = 0;
		data->coders[i].data = data;
		data->coders[i].left_dongle = &data->dongles[i];
		data->coders[i].right_dongle = &data->dongles[(i + 1)
			% data->num_coders];
		if (pthread_mutex_init(&data->coders[i].meal_mutex, NULL) != 0)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

int	init_simulation_data(t_data *data)
{
	data->sim_stopped = 0;
	data->sim_start_time = 0;
	data->dongles = NULL;
	data->coders = NULL;
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
		return (ERROR);
	if (pthread_mutex_init(&data->log_mutex, NULL) != 0)
		return (ERROR);
	if (init_dongles(data) != SUCCESS)
		return (ERROR);
	if (init_coders(data) != SUCCESS)
		return (ERROR);
	return (SUCCESS);
}