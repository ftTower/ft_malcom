#pragma once


#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netpacket/packet.h>

#include "./AINSI.h"

#define debug true
#define BUFFER_SIZE 665536

typedef struct s_machine
{
	bool			is_target;
	char			*ip;
	char			*mac;
}	t_machine;

typedef struct s_malcolm
{
	t_machine		*source;
	t_machine 		*target;
	
	int				socket_fd;
	unsigned char 	buffer[BUFFER_SIZE];
	
	struct sockaddr_ll 	device;
	
}	t_malcolm;

void				err(char *err);
int					ft_atoi(const char *str);
void				safe_exit(t_malcolm *malcolm);

bool				is_space(char c);
bool 				is_valid_mac(const char *mac);
bool				is_valid_ipv4(char *ip_str);
bool 				is_hex_pair(const char *s);

t_malcolm 			*malcolm_constructor(char **argv);
t_machine 			*machine_constructor(char *ip_str, char *mac_adress, bool is_target);

void				display_malcom(t_malcolm *malcolm);
void				display_machine(t_machine *machine);