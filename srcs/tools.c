#include "../headers/ft_malcom.h"

void	err(char *err)
{
	if (err)
		write(2, err, strlen(err));
	else
		write(2, "Fatal error", 11);
	write(2, "\n", 1);
	exit(1);
}