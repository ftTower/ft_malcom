
#include "../headers/ft_malcom.h"


bool	waiting_arp_request(t_malcolm *malcolm)
{
    int	counter = 0;
    
    while (true) {
        display_waiting_request_arp(&counter);
        counter++;
        
        fcntl(malcolm->socket_fd, F_SETFL, O_NONBLOCK);
        ssize_t len = recvfrom(malcolm->socket_fd, malcolm->buffer, sizeof(malcolm->buffer), 0, NULL, NULL);
        if (len < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // Pas de données disponibles, attendre avant de réessayer
                usleep(200000);
                continue;
            } else {
                LOG_ERROR("Failed to read on buffer");
                continue;
            }
        }		
        
        struct ethhdr *eth = (struct ethhdr *) malcolm->buffer;
        
        if (ntohs(eth->h_proto) == ETH_P_ARP) {
			printf("\033[1A\033[2K\r");
			LOG_INFO("Found a ARP frame ");
            
            struct ether_arp *arp = (struct ether_arp *)(malcolm->buffer + sizeof(struct ethhdr));
        
            if (ntohs(arp->ea_hdr.ar_op) == ARPOP_REQUEST) {
                LOG_INFO("Found a ARP request\n");

                struct in_addr src_ip, dst_ip;
                memcpy(&dst_ip, arp->arp_spa, 4);
                memcpy(&src_ip, arp->arp_tpa, 4);
            
                printf("De : %s\n", inet_ntoa(src_ip));
                printf("Pour : %s\n\n\n", inet_ntoa(dst_ip));
            }
        }
        
        sleep(10);
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