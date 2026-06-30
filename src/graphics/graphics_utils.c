/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:12:55 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:12:57 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

double	deg_to_rad(double angle)
{
	return (angle * (M_PI / 180.0));
}

double	normalize_angle(double angle)
{
	double	new_angle;

	new_angle = remainder(angle, 2 * M_PI);
	if (new_angle < 0)
		new_angle += 2 * M_PI;
	return (new_angle);
}

double	angle_diff(double angle1, double angle2)
{
	double	diff;

	diff = fabs(angle1 - angle2);
	if (diff > M_PI)
		return ((2 * M_PI) - diff);
	else
		return (diff);
}

void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}
