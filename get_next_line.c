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

#include "get_next_line_bonus.h"
#include <stdio.h> //!DELETE
#include <unistd.h> //!DELETE
#include <stdio.h> //!DELETE
#include <fcntl.h> //!DELETE


//!
char	*ft_strchr(const char *s, int c)
{
	char ch;

	if (!s)
		return (0);
	ch = (char)c;
	while (*s && *s != ch)
		s++;
	return (*s == ch ? (char *)s : NULL);
}

char	*ft_strdup(const char *s1)
{
	char	*dup;
	size_t	i;
	size_t	len;

	len = ft_strlen(s1);
	dup = (char *)malloc((len * sizeof(char)) + 1);
	i = 0;
	if (!dup)
		return (0);
	while (i < len)
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = 0;
	return (dup);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;
	char	*tmp;
	size_t	lens1;
	size_t	lens2;

	if (!s1 || !s2)
		return (NULL);
	lens1 = ft_strlen(s1);
	lens2 = ft_strlen(s2);
	join = (char *)malloc(lens1 + lens2 + 1);
	if (!join)
		return (NULL);
	tmp = join;
	while (*s1)
		*tmp++ = *s1++;
	while (*s2)
		*tmp++ = *s2++;
	*tmp = '\0';
	return (join);
}

size_t	ft_strlen(const char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;

	if (!s && !start)
		return (0);
	if (start > ft_strlen(s))
	{
		str = ft_strdup("");
		return (str);
	}
	i = 0;
	while (s[i] && i < len)
		i++;
	if (!(str = (char *)malloc(sizeof(char) * i + 1)))
		return (0);
	i = 0;
	while (i < len && s[start])
		str[i++] = s[start++];
	str[i] = '\0';
	return (str);
}
//!


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
		if (!(**line = ft_substr(*stat, 0, s - *stat)))
			return (-1);
		copy = *stat;
		*stat = ft_strdup(++s);
		free(copy);
		return (1);
	}
	else if (!bytes && **stat)
	{
		if (!(**line = ft_strdup(*stat)))
			return (-1);
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

int ft_exit_gnl(int exit_code, t_list **tmp_list, t_list **stat_list)
{
	t_list *tmp_copy;
	t_list *stat_copy;

	tmp_copy = (*tmp_list)->next;
	stat_copy = *stat_list;
	while (stat_copy->next != (*tmp_list) && !((*tmp_list)->next))
		stat_copy = stat_copy->next;
	stat_copy->next = tmp_copy;
	free((*tmp_list)->stat);
	free(*tmp_list);
	return (exit_code);
}

int				get_next_line(int fd, char **line)
{
	t_list static	*stat_list;
	t_list			*tmp_list;
	int				bytes;
	int				exit_code; //!            REFACT

	if (fd < 0 || BUFFER_SIZE <= 0 || !line)
		return (-1);
	tmp_list = ft_get_correct_fd(fd, &stat_list);
	if (ft_check_stat(&tmp_list->stat, &line))
		return (1);
	bytes = read_bytes(&tmp_list->stat, fd);
	exit_code = ft_return_res(bytes, &line, &tmp_list->stat);
	if (bytes == -1)
		exit_code = -1;
	// ft_exit_gnl(exit_code, &tmp_list, &stat_list);
	return (exit_code == 1 ? 1 : ft_exit_gnl(exit_code, &tmp_list, &stat_list));
}


int	main() //! DELETE MAIN
{
	int		fd1;
	int		fd2;
	int		fd3;
	char	*line;
	int		ret;

	ret = 1;
	fd1 = open("TEST.TXT", O_RDONLY);
	fd2 = open("3TEST.TXT", O_RDONLY);
	fd3 = open("2TEST.TXT", O_RDONLY);
	while (ret > 0)
	{
		ret = get_next_line(fd1, &line);
		printf("%s\n", line);
		free(line);
	}
	ret = 1;
	while (ret > 0)
	{
		ret = get_next_line(fd2, &line);
		printf("%s\n", line);
		free(line);
	}
	ret = 1;
	while (ret > 0)
	{
		ret = get_next_line(fd3, &line);
		printf("%s\n", line);
		free(line);
	}
	get_next_line(fd1, &line);
	printf("%s\n", line);
	free(line);
	get_next_line(fd2, &line);
	printf("%s\n", line);
	free(line);
	get_next_line(fd3, &line);
	printf("%s\n", line);
	free(line);
	close(fd1);
	close(fd2);
	close(fd3);
	// while (1)
	// {
		
	// }
	return (0);
}