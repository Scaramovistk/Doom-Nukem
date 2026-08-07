/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:51:24 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:51:27 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d.h"

int	ft_parsing_error(char *descrition, int ret)
{
	if (descrition)
		printf(TRED "%s\n" TRESET, descrition);
	return (ret);
}

int	ft_map_error(char **map, char *description, int lines, int ret)
{
	if (map)
		ft_free_map(map, lines);
	return (ft_parsing_error(description, ret));
}

int	ft_end(int file, char *to_free, char *to_free2, int ret)
{
	if (file)
		close(file);
	if (to_free)
		free(to_free);
	if (to_free2)
		free(to_free2);
	get_next_line(file, true);
	return (ft_parsing_error(NULL, ret));
}

int	ft_file_error(int fd, char *to_free, char *description, int ret)
{
	if (fd)
		close(fd);
	if (to_free)
		free(to_free);
	get_next_line(fd, true);
	return (ft_parsing_error(description, ret));
}

void	ft_free_map(char **map, int lines)
{
	int	i;

	i = -1;
	while (++i, i < lines)
		free(map[i]);
	free(map);
}
