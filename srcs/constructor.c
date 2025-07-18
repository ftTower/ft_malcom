#include "../headers/ft_malcom.h"
#include <linux/if_ether.h> // For ETH_P_ARP

bool mac_aton(const char *mac_str, unsigned char *mac_addr)
{
    if (sscanf(mac_str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
               &mac_addr[0], &mac_addr[1], &mac_addr[2],
               &mac_addr[3], &mac_addr[4], &mac_addr[5]) == 6) {
        return true;
    }
    return false;
}

t_machine *machine_constructor(char *ip_str, char *mac_address, bool is_target) {
	t_machine *machine;
	
	//!MALLOC MACHINE
	machine = malloc(sizeof(t_machine));
	if (!machine) {
		LOG_ERROR("Failed to malloc a machine");
		return (NULL);
	}
	
	//!SET MACHINE INFO
	machine->ip = ip_str;
	machine->mac = mac_address;
	machine->is_target = is_target;
	
	if (inet_pton(AF_INET, ip_str, &machine->ip_addr) <= 0){
		LOG_ERROR("Invalid IP address in machine_constructor");
        free(machine->ip); free(machine->mac); free(machine); return NULL;
	} 

	if (!mac_aton(machine->mac, machine->mac_addr)) {
		LOG_ERROR("Failed to aton a mac adress");
		return (free(machine), NULL);
	}

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
	
	malcolm->interface_name = NULL;
	if (!get_malcolm_interface(malcolm)) {
		LOG_ERROR("Failed to get malcolm network interface");
		return (free(malcolm), NULL);
	}


	//! SOCKET CREATION
	malcolm->socket_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (malcolm->socket_fd < 0) {
		LOG_ERROR("Failed to create socket");
		return (free(malcolm->interface_name),free(malcolm), NULL);
	}
	
	//! IPV4 & MAC VERIFICATION
	if (!is_valid_ipv4(argv[0]) || !is_valid_ipv4(argv[2])) {
		LOG_ERROR("Invalid ipv4 detected ! must be 4 numbers <0.0.0.0>");
		return (free(malcolm->interface_name),free(malcolm), NULL);
	}
	else if (!is_valid_mac(argv[1]) || !is_valid_mac(argv[3])) {
		LOG_ERROR("Invalid mac adress detected ! must be 6 pairs <XX:XX:XX:XX:XX:XX>");
		return (free(malcolm->interface_name),free(malcolm), NULL);
	}
	
	//!MACHINES
	malcolm->source = machine_constructor(argv[0], argv[1], false);
	if (!malcolm->source) {
		LOG_ERROR("Failed to build source machine");
		return (free(malcolm->interface_name),free(malcolm), NULL);
	}
	malcolm->target = machine_constructor(argv[2], argv[3], true);
	if (!malcolm->target) {
		LOG_ERROR("Failed to build machines");
		return (free(malcolm->source), free(malcolm->interface_name),free(malcolm), NULL);
	}
	
	
	ft_memset(&malcolm->device, 0, sizeof(malcolm->device));
	
	//! FINISHING
	if (debug)
		LOG_DEBUG("Malcolm is ready ");
	return (display_malcom(malcolm), malcolm);
}