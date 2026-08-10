/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_header_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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
	if (!ft_good_optional_assets(header, ok))
		return ;
	ft_good_rgb(header, ok);
}
