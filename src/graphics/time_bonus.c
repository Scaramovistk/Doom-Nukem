/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:18:56 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:18:58 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	update_time(t_game *g)
{
	static double	last_time = 0;

	if (last_time == 0)
		last_time = get_time();
	g->delta_time = calculate_delta_time(&last_time);
}

double	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((double)tv.tv_sec + (double)tv.tv_usec / 1000000.0);
}

double	calculate_delta_time(double *last_time)
{
	double	current_time;
	double	delta_time;

	current_time = get_time();
	delta_time = current_time - *last_time;
	*last_time = current_time;
	return (delta_time);
}
