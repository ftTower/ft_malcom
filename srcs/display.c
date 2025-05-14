#include "../headers/ft_malcom.h"

void	display_machine(t_machine *machine) {
	if (!machine->is_target) 
		printf("\n\t%s SOURCE %s : \n", GREEN, RESET);
	else
		printf("\n\t%s TARGET %s : \n", RED, RESET);
	printf("\t\tip : %s %s %s %s\n",YELLOW ,BOLD, machine->ip ,RESET);
	printf("\t\tmac : %s %s %s %s\n",YELLOW ,BOLD, machine->mac ,RESET);
	
}

void	display_malcom(t_malcom *malcolm) {
	if (!malcolm) {
		LOG_ERROR("No malcolm to print, aborted for safety reason");
		return ;
	}
	
	display_machine(malcolm->source);
	display_machine(malcolm->target);
}