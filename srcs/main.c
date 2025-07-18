
#include "../headers/ft_malcom.h"
#include <net/if_arp.h>

// struct ethhdr {
//     unsigned char   h_dest[ETH_ALEN];   /* Destination Host Address */
//     unsigned char   h_source[ETH_ALEN]; /* Source Host Address      */
//     unsigned short  h_proto;            /* Protocol type            */
// };

// struct arphdr {
//     unsigned short  ar_hrd;     /* Format of hardware address   */
//     unsigned short  ar_pro;     /* Format of protocol address   */
//     unsigned char   ar_hln;     /* Length of hardware address   */
//     unsigned char   ar_pln;     /* Length of protocol address   */
//     unsigned short  ar_op;      /* ARP opcode (command)         */
// };

// struct ether_arp {
//     struct arphdr   ea_hdr;     /* ARP hrd & proto et al */
//     unsigned char   arp_sha[6];  /* Sender hardware address */
//     unsigned char   arp_spa[4];  /* Sender protocol address */
//     unsigned char   arp_tha[6];  /* Target hardware address */
//     unsigned char   arp_tpa[4];  /* Target protocol address */
// };

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
			LOG_INFO("Found a ARP frame\n");
            printf("ethhdr->h_dest   : %02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
            printf("ethhdr->h_source : %02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
            printf("ethhdr->h_proto  : %d\n", ntohs(eth->h_proto));
            
            struct ether_arp *arp = (struct ether_arp *)(malcolm->buffer + sizeof(struct ethhdr));
        
            if (ntohs(arp->ea_hdr.ar_op) == ARPOP_REQUEST) {
                LOG_INFO("Found a ARP request\n");


                struct in_addr src_ip, dst_ip;
                unsigned char sha[6], tha[6];
                memcpy(tha, arp->arp_tha, 6);
                memcpy(&dst_ip, arp->arp_tpa, 4);
                memcpy(sha, arp->arp_sha, 6);
                memcpy(&src_ip, arp->arp_spa, 4);

                char *src_ip_str = inet_ntoa(src_ip);
                char *dst_ip_str = inet_ntoa(dst_ip);
                printf("in_addr->De   : %s\t%02x:%02x:%02x:%02x:%02x:%02x\n", src_ip_str,
                       sha[0], sha[1], sha[2], sha[3], sha[4], sha[5]);
                printf("in_addr->Pour : %s\t%02x:%02x:%02x:%02x:%02x:%02x\n\n\n", dst_ip_str,
                       tha[0], tha[1], tha[2], tha[3], tha[4], tha[5]);

                if (!ft_strncmp(src_ip_str, malcolm->source->ip, strlen(src_ip_str)))
                    printf("\ntarget spoted\n");
            }
            
        }
        
        usleep(100000);
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