#include "../headers/ft_malcom.h"
#include <linux/if_ether.h> // For ETH_P_ARP

t_machine *machine_constructor(char *ip_str, char *mac_adress, bool is_target) {
	t_machine *machine;
	
	//!MALLOC MACHINE
	machine = malloc(sizeof(t_machine));
	if (!machine) {
		LOG_ERROR("Failed to malloc a machine");
		return (NULL);
	}
	
	//!SET MACHINE INFO
	machine->ip = ip_str;
	machine->mac = mac_adress;
	machine->is_target = is_target;
	
	//!DEBUG
	if (is_target && debug)
		LOG_DEBUG("Target machine is ready ");
	else if (debug)
		LOG_DEBUG("Source machine is ready ");

	return (machine);
}

t_malcolm *malcolm_constructor(char **argv) {
	
	t_malcolm *malcolm;
	
	//! MALLOC MALCOLM
	malcolm = malloc(sizeof(t_malcolm));
	if (!malcolm) {
		LOG_ERROR("Failed to malloc malcolm");
		return (NULL);
	}
	
	//! SOCKET CREATION
	malcolm->socket_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (malcolm->socket_fd < 0) {
		LOG_ERROR("Failed to create socket");
		return (free(malcolm), NULL);
	}
	
	//! IPV4 & MAC VERIFICATION
	if (!is_valid_ipv4(argv[0]) || !is_valid_ipv4(argv[2])) {
		LOG_ERROR("Invalid ipv4 detected ! must be 4 numbers <0.0.0.0>");
		return (free(malcolm), NULL);
	}
	else if (!is_valid_mac(argv[1]) || !is_valid_mac(argv[3])) {
		LOG_ERROR("Invalid mac adress detected ! must be 6 pairs <XX:XX:XX:XX:XX:XX>");
		return (free(malcolm), NULL);
	}
	
	//!MACHINES
	malcolm->source = machine_constructor(argv[0], argv[1], false);
	if (!malcolm->source) {
		LOG_ERROR("Failed to build source machine");
		return (free(malcolm), NULL);
	}
	malcolm->target = machine_constructor(argv[2], argv[3], true);
	if (!malcolm->target) {
		LOG_ERROR("Failed to build machines");
		return (free(malcolm->source), free(malcolm), NULL);
	}
	
	
	memset(&malcolm->device, 0, sizeof(malcolm->device));
	
	//! FINISHING
	if (debug)
		LOG_DEBUG("Malcolm is ready ");
	return (display_malcom(malcolm), malcolm);
}