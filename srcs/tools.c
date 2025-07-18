#include "../headers/ft_malcom.h"
#include <net/if.h>

void	safe_exit(t_malcolm *malcolm) {
	if (malcolm) {
		if (malcolm->socket_fd >= 0) {
			close(malcolm->socket_fd);
			if (debug)
				LOG_EXIT("Socket closed");
		}
		if (malcolm->interface_name)
			free(malcolm->interface_name);
		if (malcolm->source)
			free(malcolm->source);
		if (malcolm->target)
			free(malcolm->target);
		free(malcolm);
		if (debug)
			LOG_EXIT("Malcolm is freed ! bye bye..");
	}
	exit(0);
}

void	err(char *err)
{
	if (err)
		write(2, err, strlen(err));
	else
		write(2, "Fatal error", 11);
	write(2, "\n", 1);
	exit(1);
}

size_t ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_atoi(const char *str)
{
	size_t		i;
	int			sign;
	long int	num;

	i = 0;
	sign = 1;
	num = 0;
	while (str[i] && is_space(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign *= -1;
	while (str[i] >= '0' && str[i] <= '9')
		num = (num * 10) + (str[i++] - '0');
	return (num * sign);
}

void handle_sigint(int sig) {
    (void)sig;
    keep_running = 0;
}

bool    signal_handler()
{
    signal(SIGINT, handle_sigint);
    return keep_running;
}

bool get_malcolm_interface(t_malcolm *malcolm)
{
	struct ifaddrs *ifaddr, *ifa;
	int index = 0;

	if (getifaddrs(&ifaddr) == -1) {
		LOG_ERROR("getifaddrs failed");
		return false;
	}

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next, index++)
	{
		if (ifa->ifa_addr == NULL)
			continue;

		if (strcmp(ifa->ifa_name, "lo") == 0)
			continue;

		if (malcolm->interface_name == NULL) {
			malcolm->interface_name = strdup(ifa->ifa_name); // Duplicate to store it
			if (malcolm->interface_name == NULL) {
				LOG_ERROR("Memory allocation failed for interface name.");
				freeifaddrs(ifaddr);
				return false;
			}
			malcolm->interface_index = if_nametoindex(ifa->ifa_name);
			printf("interface : %s (index: %u)\n", malcolm->interface_name, malcolm->interface_index);
			freeifaddrs(ifaddr);
			return true;
		}
	}
	LOG_ERROR("No suitable network interface found.");
	freeifaddrs(ifaddr);
	return false;
}