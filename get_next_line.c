/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezanotti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 08:07:23 by ezanotti          #+#    #+#             */
/*   Updated: 2022/11/24 10:10:05 by ezanotti         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_join_buffer(char *save, char *buffer)
{
	char	*new_save;

	new_save = ft_strjoin(save, buffer);
	if (!new_save)
		return (free(save), NULL);
	free(save);
	return (new_save);
}

char	*ft_finish_line(char *save)
{
	int		i;
	char	*line;

	i = 0;
	if (!save[i])
		return (NULL);
	while (save[i] && save[i] != '\n')
		i++;
	if (save[i] == '\n')
		line = ft_calloc(i + 2, sizeof(char));
	else
		line = ft_calloc(i + 1, sizeof(char));
	if (!line)
		return (free(save), NULL);
	i = 0;
	while (save[i] && save[i] != '\n')
	{
		line[i] = save[i];
		i++;
	}
	if (save[i] == '\n')
		line[i] = '\n';
	return (line);
}

static char	*ft_get_new_save(char *save)
{
	char	*new_save;
	int		i;
	int		j;

	i = 0;
	while (save[i] && save[i] != '\n')
		i++;
	if (!save[i])
		return (free(save), NULL);
	new_save = ft_calloc(ft_strlen(save) - i + 1, sizeof(char));
	if (!new_save)
		return (free(save), NULL);
	i++;
	j = 0;
	while (save[i])
		new_save[j++] = save[i++];
	free(save);
	return (new_save);
}

static char	*ft_read_buffer(char *save, int fd)
{
	char	*buffer;
	int		ret;

	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer)
		return (free(save), NULL);
	ret = 1;
	while (ret && !ft_isnl(save))
	{
		ret = read(fd, buffer, BUFFER_SIZE);
		if (ret < 0)
		{
			free(buffer);
			return (free(save), NULL);
		}
		buffer[ret] = '\0';
		save = ft_join_buffer(save, buffer);
		if (!save)
			return (free(buffer), NULL);
	}
	free(buffer);
	return (save);
}

char	*get_next_line(int fd)
{
	static char	*save;
	char		*line;

	if (fd < 0 || read(fd, 0, 0) < 0 || BUFFER_SIZE <= 0)
	{
		if (save)
			return (free(save), NULL);
		return (NULL);
	}
	if (!save)
	{
		save = ft_calloc(1, sizeof(char));
		if (!save)
			return (NULL);
	}
	save = ft_read_buffer(save, fd);
	if (!save)
		return (NULL);
	line = ft_finish_line(save);
	save = ft_get_new_save(save);
	return (line);
}
