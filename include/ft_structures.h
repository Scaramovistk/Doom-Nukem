/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_structures.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:38:52 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:38:54 by gscarama         ###   ########.fr       */
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

typedef struct s_item
{
	t_position	pos;
	int			type;
	int			quantity;
	bool		blocks_passage;
	bool		active;
	int			sprite_index;
}				t_item;

typedef struct s_map
{
	t_block		**grid;
	t_door		**doors;
	t_position	*sprites;
	int			sprite_count;
	t_item		*items;
	int			item_count;
	t_coord		*switches;
	int			switch_count;
	t_coord		*hazard_zones;
	int			hazard_count;
	t_coord		*message_zones;
	int			message_count;
	int			width;
	int			height;
}				t_map;

typedef struct s_player
{
	t_position	pos;
	t_coord		mouse;
	char		look;
	double		orientation;
	double		rotation_move;
	double		key_rotation_move;
	double		pitch;
	double		pitch_move;
	double		key_pitch_move;
	double		z;
	double		z_velocity;
	double		eye_height;
	bool		mouse_move_pending;
	bool		is_running;
	bool		is_crouching;
	bool		is_flying;
	bool		is_swimming;
	bool		on_ground;
	int			fly_move;
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
	t_texture	floor_texture;
	t_texture	ceiling_texture;
	t_texture	sky_texture;
	t_texture	sprite_frames[SPRITE_FRAME_NB];
	bool		has_sky;
	bool		has_sprite_frames;
	int			floor_color;
	int			ceiling_color;
}				t_assets;

typedef struct s_floor_cast
{
	double	ray_dir_x0;
	double	ray_dir_y0;
	double	ray_dir_x1;
	double	ray_dir_y1;
	double	floor_x;
	double	floor_y;
	double	step_x;
	double	step_y;
	double	row_distance;
}			t_floor_cast;

typedef struct s_transparent_hit
{
	double	distance;
	int		side;
}			t_transparent_hit;

typedef struct s_ray
{
	int			x;
	double		angle;
	t_position	dir;

	double		distance;
	int			side;
	t_block		hit_block;

	t_door		*hit_door;
	double		door_distance;
	int			door_side;
	t_transparent_hit	transparent_hits[TRANSPARENT_HIT_MAX];
	int					transparent_count;
}				t_ray;

typedef struct s_sprite_draw
{
	t_position	pos;
	double		distance;
	double		transform_x;
	double		transform_y;
	int			screen_x;
	int			height;
	int			width;
	int			top;
	int			bottom;
	int			left;
	int			right;
}				t_sprite_draw;

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

// ----- GAMEPLAY ----- //

typedef struct s_hud
{
	int			health;
	int			max_health;
	int			ammo;
	int			score;
	int			fps;
	int			inventory[4];
}				t_hud;

typedef struct s_message
{
	char		text[HUD_MESSAGE_LEN];
	double		timer;
}				t_message;

typedef struct s_world_event
{
	t_event_action	action;
	double			timer;
	double			reload;
	int				value;
	bool			repeat;
	bool			active;
	char			message[HUD_MESSAGE_LEN];
}				t_world_event;

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
	t_hud		hud;
	t_message	message;
	t_world_event	events[WORLD_EVENT_MAX];

	double		delta_time;

}				t_game;

typedef struct s_header
{
	char		no[LINE_SIZE];
	char		so[LINE_SIZE];
	char		ea[LINE_SIZE];
	char		we[LINE_SIZE];
	char		door[LINE_SIZE];
	char		floor_texture[LINE_SIZE];
	char		ceiling_texture[LINE_SIZE];
	char		sky_texture[LINE_SIZE];
	char		sprite_texture[LINE_SIZE];
	char		transparent_texture[LINE_SIZE];
	char		decal_texture[LINE_SIZE];
	char		sprite_frame_textures[SPRITE_FRAME_NB][LINE_SIZE];
	int			floor[3];
	int			ceiling[3];
}				t_header;

#endif
