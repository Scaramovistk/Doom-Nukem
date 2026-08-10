/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threaded_render.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	render_thread_count(void)
{
	long	cores;

	cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (cores < 1)
		cores = 1;
	if (cores > RENDER_THREADS_MAX)
		cores = RENDER_THREADS_MAX;
	if (cores > WIN_WIDTH)
		cores = WIN_WIDTH;
	return ((int)cores);
}

static void	*render_band(void *param)
{
	t_render_band	*band;
	t_game			*g;
	double			start_angle;
	double			angle_step;
	int				x;

	band = (t_render_band *)param;
	g = (t_game *)band->g;
	start_angle = g->player.orientation - deg_to_rad(FOV / 2);
	angle_step = deg_to_rad(FOV) / WIN_WIDTH;
	x = band->x_start;
	while (x < band->x_end)
	{
		cast_one_ray(&band->rays[x], start_angle + x * angle_step, g);
		band->rays[x].x = x;
		draw_one_ray(&band->rays[x], g);
		band->z_buffer[x] = band->rays[x].distance;
		x++;
	}
	return (NULL);
}

static void	launch_band(pthread_t *thread, t_render_band *band,
		int index, int count)
{
	band->x_start = index * WIN_WIDTH / count;
	band->x_end = (index + 1) * WIN_WIDTH / count;
	if (pthread_create(thread, NULL, render_band, band) != 0)
	{
		render_band(band);
		*thread = 0;
	}
}

static void	join_render_threads(pthread_t *threads, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (threads[i])
			pthread_join(threads[i], NULL);
		i++;
	}
}

void	draw_all_rays_threaded(t_ray *rays, t_game *g, double *z_buffer)
{
	pthread_t		threads[RENDER_THREADS_MAX];
	t_render_band	bands[RENDER_THREADS_MAX];
	int				count;
	int				i;

	count = render_thread_count();
	i = 0;
	while (i < count)
	{
		bands[i].g = g;
		bands[i].rays = rays;
		bands[i].z_buffer = z_buffer;
		launch_band(&threads[i], &bands[i], i, count);
		i++;
	}
	join_render_threads(threads, count);
}
