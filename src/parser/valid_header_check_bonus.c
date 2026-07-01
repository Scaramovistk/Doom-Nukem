/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_header_check_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:52:50 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 22:06:37 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	ft_header_error(int *vals)
{
	if (!(vals[0] && vals[1] && vals[2] && vals[3] && vals[4] && vals[5]))
		return (1);
	return (0);
}

int	ft_check_amount(void)
{
	return (DOOR_T + 1);
}
