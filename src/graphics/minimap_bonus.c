/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:55:51 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 22:02:52 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	draw_square_on_minimap(t_img *img, int *map, int size)
{
	int	dy;
	int	dx;

	dy = -1;
	while (++dy < size)
	{
		dx = -1;
		while (++dx < size)
			put_pixel(img, map[3] + dx, map[4] + dy, map[2]);
	}
}

void	draw_player_on_minimap(t_img *img, int *map, int radius)
{
	int	x;
	int	y;

	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			if (x * x + y * y <= radius * radius)
				put_pixel(img, map[5] + x, map[6] + y, map[2]);
			x++;
		}
		y++;
	}
}

int	*ft_values_calculation(int *map, int *calc)
{
	ft_values_setup(calc);
	calc[0] = abs(map[7] - map[5]);
	calc[1] = -abs(map[8] - map[6]);
	calc[2] = calc[0] + calc[1];
	calc[3] = WIN_WIDTH;
	calc[4] = WIN_HEIGHT;
	if (map[5] < map[7])
		calc[6] = 1;
	else
		calc[6] = -1;
	if (map[6] < map[8])
		calc[7] = 1;
	else
		calc[7] = -1;
	return (calc);
}

void	draw_player_orientation(t_img *img, int *map, int color)
{
	int	calc[9];

	ft_values_calculation(map, calc);
	while (1)
	{
		if (map[5] < 0 || map[5] >= calc[3] || map[6] < 0 || map[6] >= calc[4])
			break ;
		put_pixel(img, map[5], map[6], color);
		if (map[5] == map[7] && map[6] == map[8])
			break ;
		calc[5] = 2 * calc[2];
		if (calc[5] >= calc[1])
		{
			calc[2] += calc[1];
			map[5] += calc[6];
		}
		if (calc[5] <= calc[0])
		{
			calc[2] += calc[0];
			map[6] += calc[7];
		}
	}
}

static void	draw_sprites_on_minimap(t_game *g, int *map)
{
	int	i;

	map[2] = RED;
	i = 0;
	while (i < g->map.sprite_count)
	{
		map[5] = (int)(g->map.sprites[i].x * MAP_SCALE) + PADDING;
		map[6] = (int)(g->map.sprites[i].y * MAP_SCALE) + PADDING;
		draw_player_on_minimap(&(g->img), map, MAP_SCALE / 3);
		i++;
	}
}

void	draw_minimap(t_game *g, int *map)
{
	map[0] = -1;
	while (++map[0] < g->map.height)
	{
		map[1] = -1;
		while (++map[1] < g->map.width)
		{
			map[2] = WHITE;
			if (g->map.grid[map[0]][map[1]] == WALL
				|| g->map.grid[map[0]][map[1]] == TRANSPARENT_WALL
				|| g->map.grid[map[0]][map[1]] == DECAL_WALL)
				map[2] = GREY;
			else if (g->map.grid[map[0]][map[1]] == DOOR)
				map[2] = GREEN;
			if (g->map.grid[map[0]][map[1]] == NULL_BLOCK)
				continue ;
			map[3] = PADDING + map[1] * MAP_SCALE;
			map[4] = PADDING + map[0] * MAP_SCALE;
			draw_square_on_minimap(&(g->img), map, MAP_SCALE);
		}
	}
	draw_sprites_on_minimap(g, map);
	map[5] = (int)(g->player.pos.x * MAP_SCALE) + PADDING;
	map[6] = (int)(g->player.pos.y * MAP_SCALE) + PADDING;
	map[2] = BLUE;
	draw_player_on_minimap(&(g->img), map, MAP_SCALE / 2);
	map[7] = map[5] + MAP_SCALE * cos(g->player.orientation);
	map[8] = map[6] + MAP_SCALE * sin(g->player.orientation);
	draw_player_orientation(&(g->img), map, BLUE);
}
