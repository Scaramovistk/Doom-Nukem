/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_structures.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:38:52 by ugerkens          #+#    #+#             */
/*   Updated: 2024/07/17 11:38:54 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRUCTURES_H
# define FT_STRUCTURES_H
# include "ft_enumerations.h"
# include "ft_libraries.h"
# include "ft_macros.h"

// ----- MAP ----- //

typedef struct s_position
{
	double		x;
	double		y;
}				t_position;

typedef struct s_coord
{
	int			x;
	int			y;
}				t_coord;

typedef struct s_dimensions
{
	int			top;
	int			bottom;
}				t_dimensions;

typedef struct s_door
{
	double		opening_state;
	bool		is_opening;
}				t_door;

typedef struct s_map
{
	t_block		**grid;
	t_door		**doors;
	int			width;
	int			height;
}				t_map;

typedef struct s_player
{
	t_position	pos;
	t_coord		mouse;
	char		look;
	double		orientation;
	int			rotation_move;
	int			vertical_move;
	int			lateral_move;
}				t_player;

// ----- GRAPHICS ----- //

typedef struct s_resolution
{
	int32_t		width;
	int32_t		height;
}				t_resolution;

typedef struct s_img
{
	void		*ptr;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}				t_img;

typedef struct s_texture
{
	char		*source;
	t_img		img;
}				t_texture;

typedef struct s_assets
{
	t_texture	textures[TEXTURES_NB];
	int			floor_color;
	int			ceiling_color;
}				t_assets;

typedef struct s_ray
{
	int			x;
	double		angle;
	t_position	dir;

	double		distance;
	int			side;

	t_door		*hit_door;
	double		door_distance;
	int			door_side;
}				t_ray;

typedef struct s_dda
{
	t_position	step;
	t_position	side_dist;
	t_position	delta_dist;
	t_coord		map;
	int			side;
	t_ray		*ray;
}				t_dda;

typedef struct s_texture_slice
{
	int			screen_x;
	int			y_start;
	int			y_end;
	t_texture	*texture;
	double		texture_x;
	double		viewer_distance;

	int			texture_x_size;
	double		height;
	double		texture_step;
}				t_texture_slice;

// ----- GENERAL ----- //

typedef struct s_game
{
	t_list		*allocated_pointers;
	int			exit_status;

	t_map		map;
	t_player	player;

	void		*mlx;
	void		*mlx_win;
	t_img		img;
	t_assets	assets;

	double		delta_time;

}				t_game;

typedef struct s_header
{
	char		no[LINE_SIZE];
	char		so[LINE_SIZE];
	char		ea[LINE_SIZE];
	char		we[LINE_SIZE];
	char		door[LINE_SIZE];
	int			floor[3];
	int			ceiling[3];
}				t_header;

#endif