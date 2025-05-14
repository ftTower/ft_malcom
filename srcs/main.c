
#include "../headers/ft_malcom.h"

int		main(int argc, char **argv) {
	t_malcolm *malcolm;
	
	if (argc != 5)
		LOG_ERROR("./ft_malcom <source ip> <source mac adress> <target ip> <target mac address>");
	
	malcolm = malcolm_constructor(argv + 1);
	if (!malcolm) {
		LOG_ERROR("Failed to build malcolm");
		return (1);
	}
	
	return (safe_exit(malcolm), 0);
}