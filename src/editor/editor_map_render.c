/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_map_render.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	fill_rect(t_editor *e, t_coord p, t_coord size, int color)
{
	int	x;
	int	y;
	int	*pixel;

	y = p.y;
	while (y < p.y + size.y && y < WIN_HEIGHT)
	{
		x = p.x;
		while (x < p.x + size.x && x < WIN_WIDTH)
		{
			pixel = (int *)(e->img.addr + y * e->img.line_length + x
					* (e->img.bits_per_pixel / 8));
			*pixel = color;
			x++;
		}
		y++;
	}
}

static char	map_token_at(char *row, int x)
{
	if (x >= (int)ft_strlen(row))
		return (' ');
	return (row[x]);
}

static void	draw_selection(t_editor *e, t_coord position)
{
	t_coord	size;

	if (!e->blink_visible)
		return ;
	size = (t_coord){e->cell_size, 2};
	fill_rect(e, position, size, WHITE);
	position.y += e->cell_size - 2;
	fill_rect(e, position, size, WHITE);
	position.y -= e->cell_size - 2;
	size = (t_coord){2, e->cell_size};
	fill_rect(e, position, size, WHITE);
	position.x += e->cell_size - 2;
	fill_rect(e, position, size, WHITE);
}

static void	draw_map(t_editor *e)
{
	t_coord	p;
	t_coord	s;
	char	*row;
	int		x;
	int		y;

	s = (t_coord){e->cell_size - 1, e->cell_size - 1};
	y = 0;
	while (y < e->doc.map_height)
	{
		row = e->doc.cub[e->doc.map_start + y];
		x = 0;
		while (x < e->doc.map_width)
		{
			p = (t_coord){e->map_x + x * e->cell_size, e->map_y + y
				* e->cell_size};
			fill_rect(e, p, s, editor_cell_color(map_token_at(row, x)));
			if (x == e->selected_x && y == e->selected_y)
				draw_selection(e, p);
			x++;
		}
		y++;
	}
}

int	editor_render(t_editor *e)
{
	double	now;

	now = get_time();
	if (e->selected_x >= 0 && now - e->blink_time >= 0.35)
	{
		e->blink_visible = !e->blink_visible;
		e->blink_time = now;
		e->redraw = true;
	}
	if (!e->redraw)
		return (0);
	fill_rect(e, (t_coord){0, 0}, (t_coord){WIN_WIDTH, WIN_HEIGHT}, 0x0D1117);
	fill_rect(e, (t_coord){18, 64}, (t_coord){880, 690}, 0x161B22);
	fill_rect(e, (t_coord){918, 64}, (t_coord){344, 690}, 0x161B22);
	draw_map(e);
	mlx_put_image_to_window(e->mlx, e->win, e->img.ptr, 0, 0);
	editor_put_labels(e);
	e->redraw = false;
	return (0);
}
