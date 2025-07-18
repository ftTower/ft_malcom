#include "../headers/ft_malcom.h"

void	safe_exit(t_malcolm *malcolm) {
	if (malcolm) {
		if (malcolm->source)
			free(malcolm->source);
		if (malcolm->target)
			free(malcolm->target);
		free(malcolm);
		if (debug)
			LOG_DEBUG("Malcolm is freed ! bye bye..");
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