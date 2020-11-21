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
			if (!*stat)
				return (-1);
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
		if (!(**line = ft_substr(*stat, 0, s - *stat)))
			return (-1);
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

static int		ft_exit(int exit_code, t_list **stat_list, t_list **tmp_list)
{
	t_list	*stat_copy;

	stat_copy = *stat_list;
	if (stat_copy->next == NULL || stat_copy == *tmp_list)
	{
		(*stat_list) = (*stat_list)->next;
		free((*tmp_list)->stat);
		free(*tmp_list);
		return (exit_code);
	}
	while (stat_copy->next != (*tmp_list) && stat_copy != (*tmp_list))
		stat_copy = stat_copy->next;
	stat_copy->next = (*tmp_list)->next;
	free((*tmp_list)->stat);
	free(*tmp_list);
	return (exit_code);
}

int				get_next_line(int fd, char **line)
{
	static t_list	*stat_list;
	t_list			*tmp_list;
	int				bytes;
	int				ret;
	char			*copy;

	if (fd < 0 || BUFFER_SIZE <= 0 || !line)
		return (-1);
	tmp_list = ft_get_correct_fd(fd, &stat_list);
	if (tmp_list->stat)
	{
		if ((ft_strchr(tmp_list->stat, '\n')))
		{
			if (!(*line = ft_substr(tmp_list->stat, 0,
				ft_strchr(tmp_list->stat, '\n') - tmp_list->stat)))
				return (ft_exit(-1, &stat_list, &tmp_list));
			copy = tmp_list->stat;
			tmp_list->stat = ft_strdup(ft_strchr(tmp_list->stat, '\n') + 1);
			free(copy);
			return (1);
		}
	}
	bytes = read_bytes(&tmp_list->stat, fd);
	ret = bytes == -1 ? -1 : ft_return_res(bytes, &line, &tmp_list->stat);
	return (ret == 1 ? 1 : ft_exit(ret, &stat_list, &tmp_list));
}
