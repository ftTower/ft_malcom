#include "../headers/ft_malcom.h"

bool is_hex_pair(const char *s) {
    return isxdigit(s[0]) && isxdigit(s[1]);
}

bool	is_space(char c)
{
	return (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t'
		|| c == '\v');
}

bool is_valid_mac(const char *mac) {
    if (!mac) return false;

    for (int i = 0; i < 17; i++) {
        if ((i % 3) == 2) {
            if (mac[i] != ':') return false;
        } else {
            if (!isxdigit(mac[i])) return false;
        }
    }
    return mac[17] == '\0';
}

bool	is_valid_ipv4(char *ip_str) {
	struct sockaddr_in sa;
	return (inet_pton(AF_INET, ip_str, &(sa.sin_addr)) == 1);
}