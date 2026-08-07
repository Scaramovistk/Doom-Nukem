/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:50:54 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:50:57 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d.h"

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_isspace(char c)
{
	int	space;
	int	whitechars;

	space = (c == ' ');
	whitechars = ('\t' <= c && c <= '\r');
	return (space || whitechars);
}

int	ft_trimspaces(char *str)
{
	char	*end;
	int		amount;

	amount = 0;
	while (ft_isspace((unsigned char)*str))
	{
		str++;
		amount++;
	}
	end = str + ft_strlen(str) - 1;
	while (end > str && !ft_isprint((unsigned char)*end))
		end--;
	*(end + 1) = 0;
	return (amount);
}

int	ft_skip_blanks(char *map, int skip)
{
	int		fd;
	char	*line;
	char	*trim;

	fd = open(map, O_RDONLY);
	if (!fd)
		return (ft_parsing_error(OPENF, 0));
	while (--skip)
	{
		trim = get_next_line(fd, false);
		free(trim);
	}
	line = get_next_line(fd, false);
	while (line != NULL)
	{
		trim = ft_strtrim(line, " \t");
		if (ft_strcmp(trim, "\n") == 0)
			skip++;
		else if (ft_strcmp(trim, "\n") != 0)
			return (free(line), ft_file_error(fd, trim, NULL, skip));
		free(trim);
		free(line);
		line = get_next_line(fd, false);
	}
	return (ft_file_error(fd, line, NULL, skip));
}
