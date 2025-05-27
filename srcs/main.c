
#include "../headers/ft_malcom.h"
#include <unistd.h> // For usleep
#include <stdio.h>  // For printf, snprintf, fflush


bool	waiting_arp_request(t_malcolm *malcolm)
{
	char buffer[64];
	int	counter;
	
	counter = 0;
	
	while(true) {
		
		if (counter++ > 3)
			counter = 0;
		
		static const char loading[] = {'.', '.', '.', '.', '\0'};
		printf("\033[1A\033[2K\r");
		
		snprintf(buffer, sizeof(buffer), "Waiting for arp request %.*s", (counter + 1), loading);
		LOG_INFO(buffer);
		
		fflush(stdout);
		usleep(500000);
	}
	
	(void)malcolm;
	return (false);
}

int		main(int argc, char **argv) {
	t_malcolm *malcolm;
	
	if (argc != 5) {
		LOG_ERROR("./ft_malcom <source ip> <source mac adress> <target ip> <target mac address>");
		return (1);
	}
	
	malcolm = malcolm_constructor(argv + 1);
	if (!malcolm) {
		LOG_ERROR("Failed to build malcolm");
		return (1);
	}
	
	if (waiting_arp_request(malcolm)) {
		LOG_ERROR("Error occured when waiting for arp request");
		return (2);
	}
	
	
	return (safe_exit(malcolm), 0);
}