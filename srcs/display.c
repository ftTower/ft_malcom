#include "../headers/ft_malcom.h"

void	display_waiting_request_arp(int *counter)
{
	char buffer[64];
	static const char loading[] = {'.', '.', '.', '.', '\0'};
	
	if (*counter > 3)
			*counter = 0;
	
	printf("\033[1A\033[2K\r");
	snprintf(buffer, sizeof(buffer), "Waiting for arp request %.*s", (*counter + 1), loading);
	LOG_WARN(buffer);
	fflush(stdout);
	
}

void	display_machine(t_machine *machine) {
	if (!machine) {
		LOG_ERROR("No Machine to print, aborted for safety reason");
		return ;
	}
	else if (!machine->is_target) 
		printf("\n\t%s SOURCE %s : \n", GREEN, RESET);
	else
		printf("\n\t%s TARGET %s : \n", RED, RESET);
	printf("\t\tip : %s %s %s %s\n",YELLOW ,BOLD, machine->ip ,RESET);
	printf("\t\tmac : %s %s %s %s\n\n\n\n",YELLOW ,BOLD, machine->mac ,RESET);
	
}

void	display_malcom(t_malcolm *malcolm) {
	if (!malcolm) {
		LOG_ERROR("No malcolm to print, aborted for safety reason");
		return ;
	}
	
	display_machine(malcolm->source);
	display_machine(malcolm->target);
}