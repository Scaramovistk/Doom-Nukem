/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

#ifdef __linux__
# include <X11/Xlib.h>

static void	send_fullscreen_event(Display *display, Window window)
{
	XEvent	event;
	Atom	state;
	Atom	fullscreen;

	state = XInternAtom(display, "_NET_WM_STATE", False);
	fullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
	ft_bzero(&event, sizeof(event));
	event.type = ClientMessage;
	event.xclient.window = window;
	event.xclient.message_type = state;
	event.xclient.format = 32;
	event.xclient.data.l[0] = 2;
	event.xclient.data.l[1] = fullscreen;
	XSendEvent(display, DefaultRootWindow(display), False,
		SubstructureRedirectMask | SubstructureNotifyMask, &event);
	XFlush(display);
}

void	toggle_fullscreen(void *mlx, void *win, t_game *g)
{
	Display	*display;
	Window	window;
	int		width;
	int		height;

	display = *(Display **)mlx;
	window = *(Window *)win;
	send_fullscreen_event(display, window);
	if (!g)
		return ;
	g->fullscreen = !g->fullscreen;
	width = WIN_WIDTH;
	height = WIN_HEIGHT;
	if (g->fullscreen)
		mlx_get_screen_size(mlx, &width, &height);
	g->window_width = width;
	g->window_height = height;
	g->render_x = (width - WIN_WIDTH) / 2;
	g->render_y = (height - WIN_HEIGHT) / 2;
}

#elif defined(__APPLE__)

void	toggle_fullscreen(void *mlx, void *win, t_game *g)
{
	(void)mlx;
	(void)win;
	(void)g;
}

#endif
