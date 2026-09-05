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