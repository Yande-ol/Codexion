#ifndef STRUCTURES_H
# define STRUCTURES_H

# include <pthread.h>
# include "constants.h"

/* Forward declaration da estrutura principal */
typedef struct s_data	t_data;

/* 
 * t_request:
 * Representa um pedido de uso do dongle colocado na fila.
 * - coder_id: quem está pedindo.
 * - request_time: carimbo de data/hora (ms) de quando fez o pedido (para FIFO).
 * - deadline: prazo fatal (ms) de burnout desse coder (para EDF).
 */
typedef struct s_request
{
	int			coder_id;
	long long	request_time;
	long long	deadline;
}	t_request;


typedef struct s_heap
{
	t_request	*requests;
	int			size;
	int			capacity;
	t_scheduler	policy;
}	t_heap;


typedef struct s_dongle
{
	int				id;
	int				is_in_use;
	long long		last_released_time;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	t_heap			queue;
}	t_dongle;


typedef struct s_coder
{
	int				id;
	int				compiles_count;
	long long		last_compile_start;
	pthread_t		thread;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	pthread_mutex_t	meal_mutex;
	t_data			*data;
}	t_coder;


struct s_data
{
	int				num_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	int				compiles_required;
	long long		dongle_cooldown;
	t_scheduler		scheduler_type;
	long long		sim_start_time;
	int				sim_stopped;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	log_mutex;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_t		monitor_thread;
};

#endif