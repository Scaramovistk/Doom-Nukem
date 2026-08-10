/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm_internal_6.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NORM_INTERNAL_6_H
# define NORM_INTERNAL_6_H

t_decoration	*sprite_decoration(t_sprite_draw *s, t_game *g);
t_world_object	*sprite_object(t_sprite_draw *s, t_game *g);
void			set_sprite_bounds(t_sprite_draw *s, t_game *g);
bool			is_sprite_transparent(int transparent_color, int color);
t_texture		*get_sprite_texture(t_sprite_draw *s, t_game *g);
int				sprite_texture_size(t_texture *texture, t_game *g);
int				get_glass_pixel(t_transparent_hit *hit, int y, t_ray *ray,
					t_game *g);
int				blend_sprite_glass(int color, int y, t_sprite_glass *glass);
void			draw_sprite_stripe(t_sprite_draw *s, int stripe,
					t_sprite_stripe *ctx);
void			draw_one_sprite(t_sprite_draw *s, t_game *g, double *z_buffer,
					t_ray *rays);
void			draw_projectile_dot(t_game *g, t_sprite_draw *s,
					t_projectile *p, t_ray *rays);
void			init_projectile_draw(t_sprite_draw *s, t_projectile *p,
					t_game *g);
t_texture		*entity_sprite_texture(t_sprite_draw *s, t_game *g);
void			draw_sprite_pixel(t_sprite_draw *s, t_coord pixel,
					t_sprite_stripe *ctx);
void			adjust_laptop_sprite(t_sprite_draw *s, t_game *g,
					t_texture *texture);
bool			sprite_assets_available(t_game *g);
int				collect_visible_sprites(t_game *g, t_sprite_draw *sprites);
bool			projectile_pixel_visible(t_coord pixel, t_projectile_draw *ctx);
int				block_from_name(const char *name);
bool			parse_sector_action(t_authored_action *a, char *kind,
					char *args);
bool			parse_object_action(t_authored_action *a, char *kind,
					char *args);
bool			parse_wall_action(t_authored_action *a, char *kind, char *args);
bool			parse_authored_action(const char *line, t_authored_action *a);
bool			trigger_exists(t_game *g, t_coord trigger);
bool			texture_available(t_game *g, int texture);
bool			action_target_valid(t_game *g, t_authored_action *a);
void			set_grid_block(t_game *g, t_authored_action *a);
void			set_sector_property(t_game *g, t_authored_action *a);
void			set_object_property(t_game *g, t_authored_action *a);
void			set_wall_property(t_game *g, t_authored_action *a);

#endif
