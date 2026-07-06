/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:37:56 by gscarama          #+#    #+#             */
/*   Updated: 2024/07/17 11:38:00 by gscarama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include "ft_structures.h"

// ----- CUB3D ----- //

// main.c
int			main(int argc, char *argv[]);

// ----- EVENTS ----- //

// hooks.c
void		setup_hooks(t_game *g);
int			pressed(int keycode, t_game *g);
int			released(int keycode, t_game *g);
int			mouse_move(int x, int y, void *param);

// mlx_mouse_pos.c
int			get_mouse_pos(void *mlx, void *win, int *x, int *y);
int			move_mouse(void *mlx, void *win);
int			hide_mouse(void *mlx, void *win);

// game_loop.c
void		start_game(t_game *g);
int			game_loop(t_game *g);

// update_player_pos.c
void		update_player_pos(t_player *p, t_game *g);
void		jump_player(t_player *p);
void		toggle_fly_mode(t_player *p);
bool		is_position_legal(t_position pos, t_game *g);
t_coord		get_adjacent_cell(t_position pos, const t_position check_pos);
bool		is_door_open(t_coord pos, t_door **doors);

// door.c
void		init_door_grid(t_game *g);
void		update_doors(bool *door_updated, t_game *g);
bool		update_one_door(t_door *door, double delta_time);
void		toggle_adjacent_door(t_game *g);

// hooks_door.c
void		toggle_door(t_game *g);
bool		is_in_bounds(t_coord pos, t_game *g);
bool		is_door(t_coord pos, t_game *g);
bool		is_on_player(t_coord pos, t_game *g);
void		activate_door(t_coord pos, t_game *g);

// stop_game.c
int			stop_game(void *param);

// pickup.c
void		update_item_pickups(t_game *g);
bool		is_item_blocking(t_coord cell, t_game *g);

// ----- GRAPHICS ----- //

// load_game.c
void		load_game(t_game *g);
void		load_all_textures(t_game *g);
void		load_texture(t_texture *t, int *size, t_game *g);
void		load_floor_ceiling_textures(t_game *g);
void		ft_adjust_player_view(t_game *g);
void		open_window(t_game *g);

// render.c
int			render(t_game *g);
void		ft_regenerate_image(t_game *g);
void		init_display_img(t_img *img, t_game *g);

// hud.c
void		draw_hud(t_game *g);

// ray_casting.c
void		cast_all_rays(t_ray *rays, t_game *g);
void		cast_one_ray(t_ray *ray, double angle, t_game *g);

// dda.c
void		perform_dda(t_dda *dda, t_ray *ray, t_game *g);
void		progress_dda(t_dda *dda);
bool		wall_collision(t_dda *dda, t_ray *ray, t_game *g);
bool		door_collision(t_dda *dda, t_ray *ray, t_game *g);
void		transparent_collision(t_dda *dda, t_ray *ray, t_game *g);
void		update_door_ray_info(t_dda *dda, t_ray *ray, t_game *g);

// dda_utils.c
void		init_dda(t_dda *dda, t_ray *ray, t_game *g);
void		calculate_step_and_side_dist(t_dda *dda, t_ray *ray, t_game *g);
void		apply_cross_distance(t_dda *dda, double *dist, int side, t_game *g);

// draw_scene.c
void		draw_scene(t_game *g);
void		draw_floor_ceiling(t_game *g);
void		draw_all_rays(t_ray *rays, t_game *g, double *z_buffer);
void		draw_one_ray(t_ray *ray, t_game *g);
void		draw_vertical_line(int x, t_dimensions dims, int color, t_game *g);

// draw_sprites.c
void		draw_sprites(t_game *g, double *z_buffer, t_ray *rays);

// draw_wall.c
void		draw_wall_slice(t_dimensions wall, t_ray *ray, t_game *g);
void		draw_transparent_walls(t_ray *ray, t_game *g);
void		draw_wall_decal(t_dimensions wall, t_ray *ray, t_game *g);
t_texture	*get_wall_texture(t_ray *ray, t_game *g);
void		get_wall_top_bottom(t_dimensions *wall, t_ray *ray, t_game *g);

// draw_door.c
void		draw_door_slice(t_ray *ray, t_game *g);
void		get_door_top_bottom(int *door_top, int *door_bottom,
				t_ray *ray, t_game *g);

// draw_utils.c
double		get_texture_x(t_ray *ray, double distance, int side, t_game *g);
void		draw_texture_slice(t_texture_slice *slice, t_game *g);
void		draw_texture_slice_alpha(t_texture_slice *slice, t_game *g);
int			get_pixel(t_img *img, int x, int y);
int			blend_color(int dst, int src, double alpha);

// mlx_utils.c
void		put_pixel(t_img *img, int x, int y, int color);

// time.c
void		update_time(t_game *g);
double		get_time(void);
double		calculate_delta_time(double *last_time);

// graphics_utils.c
double		deg_to_rad(double angle);
double		normalize_angle(double angle);
double		angle_diff(double angle1, double angle2);

// ----- PARSER ----- //
// bonus.c
int			ft_header_error(int *vals);
int			ft_check_amount(void);

// generate_map.c
void		ft_populate_map(char **map, int *vals, t_game *g);
void		ft_populate_info(t_header *h, t_game *g);

// get_header.c
void		ft_setup_header(t_header *header);
char		**ft_values_setup(int *readval);
int			ft_header_extractor(char *line, int *vals, t_header *p);

// get_map.c
void		ft_extract_line(char *src, char *dest, int chars);
int			ft_get_map(char *mapfile, int *vals, int *succes, t_game *g);

// parser.c
int			ft_is_file(char *path);
int			ft_parse_file(int argc, char *argv[], t_game *g);
int			ft_amount_of_commas(char **rgbs, int *values, char *line);

// valid_header.c
int			ft_get_header(char *map, int *ok, t_header *p);

// ft_valid_map.c
int			ft_extract_map(char **map, char *mapfile, int *dims);
char		**ft_map_dup(char **map, int lines, int width);
int			ft_valid_map(char **map, int lines, t_game *g);

// ----- PARSER UTILS ----- //
// parsing_error.c
int			ft_parsing_error(char *descrition, int ret);
int			ft_map_error(char **map, char *description, int lines, int ret);
int			ft_file_error(int file, char *to_free, char *description, int ret);
int			ft_end(int fd, char *to_free, char *to_free2, int ret);
void		ft_free_map(char **map, int lines);

// header.c
char		*ft_strcpy(char *dest, char *src);
int			ft_isspace(char c);
int			ft_trimspaces(char *str);
int			ft_skip_blanks(char *map, int skip);

// map.c
int			ft_map_line(const char *str);
int			ft_invalid_line(const char *str);
int			ft_find_access_map(char **map, int vert, int hor, int *vals);
int			ft_closed_map(char **map, int *vals);

// security.c
int			ft_hallway(char **map, int *vals);

// ----- UTILS ----- //

// init_game.c
void		init_game_struct(t_game *g);

// allocation.c
void		*s_alloc(void *pointer, t_game *g);
void		*calloc_s(size_t count, size_t size, t_game *g);
void		track_alloc(void *pointer, t_game *g);

// error.c
void		error(char *description, t_game *g);

// free.c
void		free_all(t_game *g);

#endif
