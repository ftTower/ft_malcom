#include "../headers/ft_malcom.h"

t_machine *machine_constructor(char *ip_str, char *mac_adress, bool is_target) {
	t_machine *machine;
	
	machine = malloc(sizeof(machine));
	if (!machine) {
		LOG_ERROR("Failed to malloc a machine");
		return (NULL);
	}
	
	machine->ip = ip_str;
	machine->mac = mac_adress;
	machine->is_target = is_target;
	return (machine);
}

t_malcom *malcolm_constructor(char **argv) {
	
	t_malcom *malcolm;
	
	malcolm = malloc(sizeof(malcolm));
	if (!malcolm) {
		LOG_ERROR("Failed to malloc malcolm");
		return (NULL);
	}
	
	if (!is_valid_ipv4(argv[0]) || !is_valid_ipv4(argv[2])) {
		LOG_ERROR("Invalid ipv4 detected ! must be 4 numbers <0.0.0.0>");
		return (free(malcolm), NULL);
	}
	else if (!is_valid_mac(argv[1]) || !is_valid_mac(argv[3])) {
		LOG_ERROR("Invalid mac adress detected ! must be 6 pairs <XX:XX:XX:XX:XX:XX>");
		return (free(malcolm), NULL);
	}
	
	malcolm->source = machine_constructor(argv[0], argv[1], false);
	malcolm->target = machine_constructor(argv[2], argv[3], true);
	
	return (LOG_INFO("Malcolm is ready !"), display_malcom(malcolm), malcolm);
}