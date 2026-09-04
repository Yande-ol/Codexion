#ifndef CONSTANTS_H
# define CONSTANTS_H

/* Códigos de retorno e erros */
# define SUCCESS 0
# define ERROR 1

/* Políticas do escalonador (scheduler) */
typedef enum e_scheduler
{
	SCHED_FIFO,
	SCHED_EDF
}	t_scheduler;

/* Mensagens de log */
# define MSG_DONGLE "has taken a dongle"
# define MSG_COMPILE "is compiling"
# define MSG_DEBUG "is debugging"
# define MSG_REFACTOR "is refactoring"
# define MSG_BURNOUT "burned out"

#endif

/* Este arquivo guarda apenas números mágicos, textos fixos e identificadores de estado. Ele não depende de nada externo */