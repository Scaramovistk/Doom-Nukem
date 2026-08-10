/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_gui_draw.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include "../../include/editor.h"

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
			pixel = (int *)(e->img.addr + y * e->img.line_length
					+ x * (e->img.bits_per_pixel / 8));
			*pixel = color;
			x++;
		}
		y++;
	}
}

static int	cell_color(char token)
{
	if (token == '1' || token == 'L')
		return (0x38404D);
	if (token == '2' || token == 'B' || token == 'P')
		return (0xB87333);
	if (token == '4')
		return (0x408DA8);
	if (token == '5' || (token >= 'g' && token <= 'l'))
		return (0x7C4DFF);
	if (token == '0' || ft_strchr("NWSE", token))
		return (0x202830);
	if (ft_strchr("3KIDCVv", token))
		return (0x9B2C2C);
	if (ft_strchr("6789", token))
		return (0x2E8B57);
	if (ft_strchr("THMX", token))
		return (0xC18B22);
	return (0x5C426E);
}

static char	map_token_at(char *row, int x)
{
	if (x >= (int)ft_strlen(row))
		return (' ');
	return (row[x]);
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
			p = (t_coord){e->map_x + x * e->cell_size,
				e->map_y + y * e->cell_size};
			fill_rect(e, p, s, cell_color(map_token_at(row, x)));
			if (x == e->selected_x && y == e->selected_y)
				fill_rect(e, p, (t_coord){e->cell_size, 2}, YELLOW);
			x++;
		}
		y++;
	}
}

static void	draw_panels(t_editor *e)
{
	fill_rect(e, (t_coord){0, 0}, (t_coord){WIN_WIDTH, WIN_HEIGHT}, 0x0D1117);
	fill_rect(e, (t_coord){18, 64}, (t_coord){880, 690}, 0x161B22);
	fill_rect(e, (t_coord){918, 64}, (t_coord){344, 690}, 0x161B22);
	draw_map(e);
}

static void	put_labels(t_editor *e)
{
	void	*m;
	void	*w;
	char	line[LINE_SIZE];

	m = e->mlx;
	w = e->win;
	mlx_string_put(m, w, 24, 34, 0xFF8A35, "DOOM-NUKEM LEVEL EDITOR");
	mlx_string_put(m, w, 932, 94, WHITE, "GEOMETRY / OBJECTS");
	mlx_string_put(m, w, 932, 126, WHITE, "[1] Wall   [Z] Floor");
	mlx_string_put(m, w, 932, 150, WHITE, "[2] Door   [4] Glass");
	mlx_string_put(m, w, 932, 174, WHITE, "[5] Decal  [3] Enemy");
	mlx_string_put(m, w, 932, 198, WHITE, "[6] Health [7] Ammo");
	mlx_string_put(m, w, 932, 222, WHITE, "[V] Solid object");
	mlx_string_put(m, w, 932, 246, WHITE, "[N] Player [X] Exit");
	mlx_string_put(m, w, 932, 286, GREEN, "SECTORS / ACTIONS");
	mlx_string_put(m, w, 932, 318, WHITE, "[ / ] Sector id");
	mlx_string_put(m, w, 932, 342, WHITE, "C assign sector to cell");
	mlx_string_put(m, w, 932, 366, WHITE, "F floor  R ceiling");
	mlx_string_put(m, w, 932, 390, WHITE, "G/H slopes  L light");
	mlx_string_put(m, w, 932, 414, WHITE, "A add door action");
	mlx_string_put(m, w, 932, 454, GREEN, "TEXTURES / PROJECT");
	mlx_string_put(m, w, 932, 486, WHITE, "T Texture preset");
	mlx_string_put(m, w, 932, 510, WHITE, "S Save   P Pack");
	mlx_string_put(m, w, 932, 534, WHITE, "K Validate  ESC Close");
	mlx_string_put(m, w, 932, 558, WHITE, "0 / F11 Fullscreen/window");
	snprintf(line, sizeof(line), "Tool: %c  Sector: %d  Texture: %d",
		e->tool, e->sector, e->page + 1);
	mlx_string_put(m, w, 932, 608, YELLOW, line);
	mlx_string_put(m, w, 24, 782, 0xAAB2BF, e->status);
}

int	editor_render(t_editor *e)
{
	if (!e->redraw)
		return (0);
	draw_panels(e);
	mlx_put_image_to_window(e->mlx, e->win, e->img.ptr, 0, 0);
	put_labels(e);
	e->redraw = false;
	return (0);
}
