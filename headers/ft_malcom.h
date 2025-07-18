#pragma once


#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netpacket/packet.h>
#include <stdio.h>              // printf
#include <unistd.h>             // usleep
#include <string.h>             // memcpy, memset
#include <netinet/if_ether.h>   // struct ethhdr
#include <net/if_arp.h>         // struct ether_arp, ARPOP_REQUEST
#include <net/ethernet.h>       // ETH_P_ARP
#include <net/if_arp.h>         // ARPOP_REQUEST
#include <errno.h>              // errno
#include <arpa/inet.h>          // ntohs()
#include <sys/socket.h>         // recvfrom()
#include <fcntl.h>              // fcntl, F_SETFL
#include <ifaddrs.h>
#include <signal.h>

#include "./AINSI.h"

#define debug true
#define BUFFER_SIZE 665536

static volatile sig_atomic_t keep_running = 1;


typedef struct s_machine
{
	bool			is_target;
	char			*ip;
	char			*mac;
	unsigned char	mac_addr[6];
}	t_machine;

typedef struct s_malcolm
{
	t_machine		*source;
	t_machine 		*target;
	
	int				socket_fd;
	unsigned char 	buffer[BUFFER_SIZE];
	char			*interface_name;

	struct sockaddr_ll 	device;
	
}	t_malcolm;

void				err(char *err);
int					ft_atoi(const char *str);
void				safe_exit(t_malcolm *malcolm);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
size_t				ft_strlen(const char *s);	
bool    			get_malcolm_interface(t_malcolm *malcolm);
void 				handle_sigint(int sig);
bool    			signal_handler();

bool				is_space(char c);
bool 				is_valid_mac(const char *mac);
bool				is_valid_ipv4(char *ip_str);
bool 				is_hex_pair(const char *s);

t_malcolm 			*malcolm_constructor(char **argv);
t_machine 			*machine_constructor(char *ip_str, char *mac_adress, bool is_target);

void				display_malcom(t_malcolm *malcolm);
void				display_machine(t_machine *machine);
void				display_waiting_request_arp(int *counter);