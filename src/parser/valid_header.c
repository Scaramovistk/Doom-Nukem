/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:52:58 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:53:02 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	ft_xpm_extension(const char *path)
{
	int				len;
	const char		*xpm = ".xpm";
	const size_t	size = ft_strlen(xpm);

	len = ft_strlen(path);
	if (size <= (size_t)len)
	{
		path += len - 4;
		if (ft_strcmp(path, xpm) == 0)
			return (1);
	}
	return (0);
}

int	ft_good_xpms(t_header *h, int *ok, int *vals, int amount)
{
	const char	*textures[TEXTURES_NB] = {h->no, h->so, h->ea, h->we, h->door};

	vals[0] = -1;
	while (++vals[0] < amount)
	{
		if (!ft_xpm_extension(textures[vals[0]]))
		{
			*ok = ft_parsing_error(XPMFILE, 0);
			return (0);
		}
		vals[1] = open(textures[vals[0]], O_RDONLY);
		if (vals[1] == -1)
		{
			*ok = ft_parsing_error(NOXPMS, 0);
			return (0);
		}
		close(vals[1]);
		if (!ft_is_file((char *)textures[vals[0]]))
		{
			*ok = ft_parsing_error(DIRXPMS, 0);
			return (0);
		}
	}
	return (1);
}

int	ft_good_rgb(t_header *header, int *ok)
{
	int			i;
	int			texture;
	int			invalid;
	const int	*rgb[2] = {header->floor, header->ceiling};
	const char	*xpm[2] = {header->floor_texture, header->ceiling_texture};

	texture = 0;
	while (texture < 2)
	{
		if (xpm[texture][0])
		{
			texture++;
			continue ;
		}
		i = 0;
		while (i < 3)
		{
			invalid = (0 <= rgb[texture][i] && rgb[texture][i] <= 255);
			if (!invalid)
				*ok = ft_parsing_error(RGBS, 0);
			if (!(*ok))
				return (0);
			i++;
		}
		texture++;
	}
	return (1);
}

static int	ft_good_optional_xpm(char *path, int *ok)
{
	int	fd;

	if (!path[0])
		return (1);
	if (!ft_xpm_extension(path))
	{
		*ok = ft_parsing_error(XPMFILE, 0);
		return (0);
	}
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		*ok = ft_parsing_error(NOXPMS, 0);
		return (0);
	}
	close(fd);
	if (!ft_is_file(path))
	{
		*ok = ft_parsing_error(DIRXPMS, 0);
		return (0);
	}
	return (1);
}

static int	ft_good_sprite_frames(t_header *header, int *ok)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < SPRITE_FRAME_NB)
	{
		if (header->sprite_frame_textures[i][0])
			count++;
		i++;
	}
	if (count != 0 && count != SPRITE_FRAME_NB)
	{
		*ok = ft_parsing_error(PATHS, 0);
		return (0);
	}
	i = 0;
	while (i < SPRITE_FRAME_NB)
	{
		if (!ft_good_optional_xpm(header->sprite_frame_textures[i], ok))
			return (0);
		i++;
	}
	return (1);
}

void	ft_ok(int *vals, int *ok, t_header *header)
{
	int	values[9];

	if (ft_header_error(vals))
	{
		*ok = ft_parsing_error(PATHS, 0);
		return ;
	}
	ft_values_setup(values);
	values[2] = ft_check_amount();
	if (!ft_good_xpms(header, ok, values, values[2]))
		return ;
	if (!ft_good_optional_xpm(header->floor_texture, ok))
		return ;
	if (!ft_good_optional_xpm(header->ceiling_texture, ok))
		return ;
	if (!ft_good_optional_xpm(header->sky_texture, ok))
		return ;
	if (!ft_good_optional_xpm(header->sprite_texture, ok))
		return ;
	if (!ft_good_sprite_frames(header, ok))
		return ;
	if (!ft_good_rgb(header, ok))
		return ;
}

int	ft_get_header(char *map, int *ok, t_header *h)
{
	char	*line;
	char	*trim;
	int		val[10];

	ft_values_setup(val);
	val[9] = open(map, O_RDONLY);
	if (!val[9])
		return (ft_parsing_error(OPENF, 0));
	line = get_next_line(val[9], false);
	while (++val[8], line != NULL)
	{
		val[6] = ft_trimspaces(line);
		if (!ft_header_extractor(line, val, h))
		{
			trim = ft_strtrim(line, " \t");
			if (ft_strcmp(trim, "\n") != 0)
				return (ft_ok(val, ok, h), ft_end(val[9], trim, line, val[8]));
			free(trim);
		}
		free(line);
		line = get_next_line(val[9], false);
	}
	return (ft_ok(val, ok, h), ft_file_error(val[9], line, 0, val[8]));
}
