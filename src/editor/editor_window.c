/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_window.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	editor_display_init(t_editor *e)
{
	e->mlx = mlx_init();
	if (!e->mlx)
		return (false);
	e->win = mlx_new_window(e->mlx, WIN_WIDTH, WIN_HEIGHT,
			"Doom-Nukem Level Editor");
	if (!e->win)
		return (false);
	e->img.ptr = mlx_new_image(e->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!e->img.ptr)
		return (false);
	e->img.addr = mlx_get_data_addr(e->img.ptr, &e->img.bits_per_pixel,
			&e->img.line_length, &e->img.endian);
	return (e->img.addr != NULL);
}

int	editor_exit(t_editor *e)
{
	if (e->doc.dirty && !e->confirm_close)
		ft_strlcpy(e->status, "Save or Pack before closing", HUD_MESSAGE_LEN);
	if (e->doc.dirty && !e->confirm_close)
	{
		e->confirm_close = true;
		e->redraw = true;
		return (0);
	}
	e->running = false;
	end_display_loop(e->mlx);
	return (0);
}

static void	editor_destroy(t_editor *e)
{
	if (e->img.ptr)
		mlx_destroy_image(e->mlx, e->img.ptr);
	if (e->win)
		mlx_destroy_window(e->mlx, e->win);
	if (e->mlx)
		destroy_display(e->mlx);
	free(e->mlx);
	editor_close(&e->doc);
}

int	interactive_level_editor(char *src, char *default_output)
{
	t_editor	e;

	ft_bzero(&e, sizeof(e));
	if (!editor_open(&e.doc, src, default_output))
		return (printf("Unable to load editable project.\n"), EXIT_FAILURE);
	if (!editor_display_init(&e))
		return (editor_close(&e.doc), EXIT_FAILURE);
	editor_setup(&e);
	mlx_hook(e.win, KEY_PRESS, KEY_PRESS_MASK, editor_key, &e);
	mlx_hook(e.win, MOUSE_PRESS, MOUSE_PRESS_MASK, editor_mouse, &e);
	mlx_hook(e.win, CLIENT_MESSAGE, STRUCTURE_NOTIFY_MASK, editor_exit, &e);
	mlx_loop_hook(e.mlx, editor_render, &e);
	mlx_loop(e.mlx);
	editor_destroy(&e);
	return (EXIT_SUCCESS);
}
