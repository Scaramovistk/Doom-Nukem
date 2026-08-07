/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_enumerations.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:38:05 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:38:08 by gscarama         ###   ########.fr       */
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
	DOOR_T,
	SPRITE_T,
	TRANSPARENT_T,
	DECAL_T
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
	DOOR,
	SPRITE,
	TRANSPARENT_WALL,
	DECAL_WALL
}		t_block;

typedef enum e_event_action
{
	EVENT_NONE,
	EVENT_TOGGLE_DOORS,
	EVENT_OPEN_DOORS,
	EVENT_CLOSE_DOORS,
	EVENT_ADD_SCORE,
	EVENT_DAMAGE_PLAYER,
	EVENT_SHOW_MESSAGE,
	EVENT_SECTOR_ANIMATE,
	EVENT_OPEN_ONE_DOOR,
	EVENT_AUTHORED_ACTION
}		t_event_action;

typedef enum e_authored_action_type
{
	AUTH_ACTION_NONE,
	AUTH_ACTION_BLOCK,
	AUTH_ACTION_FLOOR,
	AUTH_ACTION_CEILING,
	AUTH_ACTION_LIGHT,
	AUTH_ACTION_TEXTURE_SWAP,
	AUTH_ACTION_OBJECT_MOVE,
	AUTH_ACTION_OBJECT_BLOCK,
	AUTH_ACTION_OBJECT_SCALE,
	AUTH_ACTION_OBJECT_TEXTURE,
	AUTH_ACTION_WALL_MOVE,
	AUTH_ACTION_WALL_TEXTURE
}		t_authored_action_type;

typedef enum e_game_state
{
	STATE_MENU,
	STATE_PLAYING
}		t_game_state;

#endif
