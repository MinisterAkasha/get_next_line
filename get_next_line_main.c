/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get-next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akasha <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 19:42:20 by akasha            #+#    #+#             */
/*   Updated: 2020/11/11 19:42:22 by akasha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	ft_check_stat(char **stat, char ***line)
{
	char	*copy;
	char	*s;

	if (*stat)
	{
		if ((s = ft_strchr(*stat, '\n')))
		{
			**line = ft_substr(*stat, 0, s - *stat);
			copy = *stat;
			*stat = ft_strdup(++s);
			free(copy);
			return (1);
		}
	}
	return (0);
}

static int	read_bytes(char **stat, int fd)
{
	char	*copy;
	char	*buffer;
	int		bytes;

	buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	while ((bytes = read(fd, buffer, BUFFER_SIZE)))
	{
		if (bytes < 0)
			return (-1);
		buffer[bytes] = 0;
		if (!*stat)
			*stat = ft_strdup(buffer);
		else
		{
			copy = *stat;
			*stat = ft_strjoin(copy, buffer);
			free(copy);
		}
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	free(buffer);
	return (bytes);
}

static int	ft_return_res(int bytes, char ***line, char **stat)
{
	char			*s;
	char			*copy;

	if (!bytes && !*stat)
		**line = ft_strdup("");
	else if ((s = ft_strchr(*stat, '\n')))
	{
		**line = ft_substr(*stat, 0, s - *stat);
		copy = *stat;
		*stat = ft_strdup(++s);
		free(copy);
		return (1);
	}
	else if (!bytes && **stat)
	{
		**line = ft_strdup(*stat);
		copy = *stat;
		*stat = ft_strdup("");
		free(copy);
		return (0);
	}
	else if (!bytes)
		**line = ft_strdup("");
	return (0);
}

int			get_next_line(int fd, char **line)
{
	static char		*stat;
	int				bytes;
	int				ret;

	if (fd < 0 || BUFFER_SIZE <= 0 || !line)
		return (-1);
	if (ft_check_stat(&stat, &line))
		return (1);
	bytes = read_bytes(&stat, fd);
	if (bytes == -1)
		return (-1);
	ret = ft_return_res(bytes, &line, &stat);
	if (ret == 0 || ret == -1)
		free(stat);
	return (ret);
}
