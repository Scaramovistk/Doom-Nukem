/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_enumerations.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:38:05 by ugerkens          #+#    #+#             */
/*   Updated: 2024/07/17 11:38:08 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ENUMERATIONS_H
# define FT_ENUMERATIONS_H

enum	e_texture_type
{
	NORTH,
	SOUTH,
	EAST,
	WEST,
	DOOR_T
};

enum	e_texture_length
{
	color = 1,
	wall
};

typedef enum t_block
{
	NULL_BLOCK,
	EMPTY,
	WALL,
	PLAYER,
	DOOR
}		t_block;

#endif