/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:09:15 by dkhalfao          #+#    #+#             */
/*   Updated: 2024/11/20 19:12:11 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		i++;
	}
	return (i);
}

char	*ft_strchr(const char *str, int c)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == (unsigned char)c)
			return ((char *)&str[i]);
		i++;
	}
	return (NULL);
}

char	*fill_stock(char *stock, char *buffer, int fd)
{
	char	*tmp;
	ssize_t	size;

	while (!ft_strchr(stock, '\n'))
	{
		size = read(fd, buffer, BUFFER_SIZE);
		if (size < 0)
		{
			free(stock);
			stock = NULL;
			return (NULL);
		}
		if (size == 0)
			break ;
		buffer[size] = '\0';
		tmp = stock;
		if (!stock)
			stock = ft_strdup(buffer);
		else
			stock = ft_strjoin(tmp, buffer);
		free(tmp);
	}
	return (stock);
}

char	*split_stock(char **stock)
{
	char	*tmp;
	char	*line;

	if (ft_strchr(*stock, '\n'))
	{
		line = ft_substr(*stock, 0, ft_strchr(*stock, '\n') - *stock + 1);
		tmp = ft_strdup(ft_strchr(*stock, '\n') + 1);
		free(*stock);
		*stock = tmp;
		if (**stock == '\0')
		{
			free(*stock);
			*stock = NULL;
		}
	}
	else if (*stock != NULL && **stock != '\0')
	{
		line = ft_strdup(*stock);
		free(*stock);
		*stock = NULL;
	}
	else
		line = NULL;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stock;
	char		*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	stock = fill_stock(stock, buffer, fd);
	if (!stock)
	{
		free(buffer);
		return (NULL);
	}
	line = split_stock(&stock);
	free(buffer);
	return (line);
}
// #include <fcntl.h>
// #include <stdio.h>

// int	main(void)
// {
// 	int fd;
// 	char *line;

// 	fd = open("text.txt", O_RDONLY);
// 	line = get_next_line(fd);
// 	while (line)
// 	{
// 		printf("%s", line);
// 		free(line);
// 		line = get_next_line(fd);
// 	}
// 	// free(line);
// 	close(fd);
// }