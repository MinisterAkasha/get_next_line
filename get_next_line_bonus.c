/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akasha <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 19:30:41 by akasha            #+#    #+#             */
/*   Updated: 2020/11/13 19:30:44 by akasha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static int		ft_check_stat(char **stat, char ***line)
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

static int		read_bytes(char **stat, int fd)
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

static int		ft_return_res(int bytes, char ***line, char **stat)
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

static t_list	*ft_get_correct_fd(int fd, t_list **stat_list)
{
	t_list	*elem;
	t_list	*tmp_list;

	tmp_list = *stat_list;
	while (tmp_list)
	{
		if (tmp_list->fd == fd)
		{
			return (tmp_list);
		}
		tmp_list = tmp_list->next;
	}
	if (!(elem = (t_list*)malloc(sizeof(t_list))))
		return (NULL);
	elem->fd = fd;
	elem->stat = ft_strdup("");
	elem->next = NULL;
	if (!*stat_list)
	{
		*stat_list = elem;
		return (*stat_list);
	}
	elem->next = *stat_list;
	*stat_list = elem;
	return (*stat_list);
}

int				get_next_line(int fd, char **line)
{
	t_list static	*stat_list;
	t_list			*tmp_list;
	int				bytes;

	if (fd < 0 || BUFFER_SIZE <= 0 || !line)
		return (-1);
	tmp_list = ft_get_correct_fd(fd, &stat_list);
	if (ft_check_stat(&tmp_list->stat, &line))
		return (1);
	bytes = read_bytes(&tmp_list->stat, fd);
	if (bytes == -1)
		return (-1);
	return (ft_return_res(bytes, &line, &tmp_list->stat));
}
