/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	minimap_pixel(t_game *g, int x, int y, int color)
{
	if (x < 0 || y < 0 || x >= WIN_WIDTH || y >= WIN_HEIGHT)
		return ;
	put_pixel(&g->img, x, y, color);
}

static void	minimap_rect(t_game *g, t_coord pos, t_coord size, int color)
{
	int	x;
	int	y;

	y = pos.y;
	while (y < pos.y + size.y)
	{
		x = pos.x;
		while (x < pos.x + size.x)
			minimap_pixel(g, x++, y, color);
		y++;
	}
}

static int	block_color(t_block block)
{
	if (block == WALL)
		return (GREY);
	if (block == DOOR)
		return (ORANGE);
	if (block == TRANSPARENT_WALL)
		return (BLUE);
	if (block == DECAL_WALL)
		return (YELLOW);
	return (DARK_GREY);
}

static void	draw_map_cells(t_game *g, t_coord origin)
{
	t_coord	cell;
	t_coord	pos;

	cell.y = 0;
	while (cell.y < g->map.height)
	{
		cell.x = 0;
		while (cell.x < g->map.width)
		{
			pos.x = origin.x + cell.x * MAP_SCALE;
			pos.y = origin.y + cell.y * MAP_SCALE;
			minimap_rect(g, pos, (t_coord){MAP_SCALE - 1, MAP_SCALE - 1},
				block_color(g->map.grid[cell.y][cell.x]));
			cell.x++;
		}
		cell.y++;
	}
}

static void	draw_minimap_items(t_game *g, t_coord origin)
{
	int		i;
	t_coord	pos;

	i = 0;
	while (i < g->map.item_count)
	{
		if (g->map.items[i].active)
		{
			pos.x = origin.x + (int)(g->map.items[i].pos.x * MAP_SCALE) - 2;
			pos.y = origin.y + (int)(g->map.items[i].pos.y * MAP_SCALE) - 2;
			minimap_rect(g, pos, (t_coord){4, 4}, GREEN);
		}
		i++;
	}
}

static void	draw_minimap_exits(t_game *g, t_coord origin)
{
	int		i;
	t_coord	pos;

	i = 0;
	while (i < g->map.exit_count)
	{
		pos.x = origin.x + g->map.exit_zones[i].x * MAP_SCALE + 2;
		pos.y = origin.y + g->map.exit_zones[i].y * MAP_SCALE + 2;
		minimap_rect(g, pos, (t_coord){MAP_SCALE - 4, MAP_SCALE - 4}, GREEN);
		i++;
	}
}

static void	draw_minimap_sprites(t_game *g, t_coord origin)
{
	int		i;
	t_coord	pos;

	i = 0;
	while (i < g->map.sprite_count)
	{
		pos.x = origin.x + (int)(g->map.sprites[i].x * MAP_SCALE) - 2;
		pos.y = origin.y + (int)(g->map.sprites[i].y * MAP_SCALE) - 2;
		minimap_rect(g, pos, (t_coord){4, 4}, RED);
		i++;
	}
}

static void	draw_player_marker(t_game *g, t_coord origin)
{
	t_coord	pos;
	t_coord	dir;

	pos.x = origin.x + (int)(g->player.pos.x * MAP_SCALE);
	pos.y = origin.y + (int)(g->player.pos.y * MAP_SCALE);
	minimap_rect(g, (t_coord){pos.x - 3, pos.y - 3}, (t_coord){6, 6}, WHITE);
	dir.x = pos.x + (int)(cos(g->player.orientation) * 12);
	dir.y = pos.y + (int)(sin(g->player.orientation) * 12);
	minimap_rect(g, (t_coord){dir.x - 1, dir.y - 1}, (t_coord){3, 3}, WHITE);
}

void	draw_minimap(t_game *g)
{
	t_coord	origin;
	t_coord	size;

	origin = (t_coord){PADDING, PADDING};
	size.x = g->map.width * MAP_SCALE + 8;
	size.y = g->map.height * MAP_SCALE + 8;
	minimap_rect(g, origin, size, HUD_BG);
	draw_map_cells(g, (t_coord){origin.x + 4, origin.y + 4});
	draw_minimap_exits(g, (t_coord){origin.x + 4, origin.y + 4});
	draw_minimap_sprites(g, (t_coord){origin.x + 4, origin.y + 4});
	draw_minimap_items(g, (t_coord){origin.x + 4, origin.y + 4});
	draw_player_marker(g, (t_coord){origin.x + 4, origin.y + 4});
}
