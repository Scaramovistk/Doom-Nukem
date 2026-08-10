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
# include "norm_internal.h"

// ----- CUB3D ----- //

// main.c
int			main(int argc, char *argv[]);
int			run_editor(int argc, char *argv[]);
int			check_level_file(char *path);
int			interactive_level_editor(char *src, char *default_output);
bool		editor_open(t_editor_doc *doc, char *src, char *output);
void		editor_close(t_editor_doc *doc);
bool		editor_command(t_editor_doc *doc, char *command);
bool		editor_border_valid(t_editor_doc *doc);
int			terminal_level_editor(char *src, char *default_output);
int			editor_render(t_editor *editor);
void		editor_put_labels(t_editor *editor);
void		editor_put_tool_labels(t_editor *editor);
int			editor_key(int key, t_editor *editor);
int			editor_mouse(int button, int x, int y, t_editor *editor);
void		editor_run_command(t_editor *e, char *command, char *status);
void		editor_action_at_selection(t_editor *e);
void		editor_adjust_sector(t_editor *e, int property);
void		editor_texture_preset(t_editor *e);
void		editor_project_key(int key, t_editor *e);
bool		editor_select_tool(int key, t_editor *e);
bool		editor_panel_click(t_editor *e, int x, int y);
int			editor_exit(t_editor *editor);
void		editor_setup(t_editor *editor);
bool		parse_sector_command(char *command, t_sector *sector, int *id);
bool		replace_sector_line(t_editor_doc *doc, char *line, int id);
bool		editor_dispatch_edit(t_editor_doc *doc, char *command);
bool		editor_dispatch_project(t_editor_doc *doc, char *command);
bool		editor_terminal_loop(t_editor_doc *doc, char *command);

// ----- EVENTS ----- //

// hooks.c
void		setup_hooks(t_game *g);
int			pressed(int keycode, t_game *g);
void		handle_action_key(int key, t_game *g);
int			released(int keycode, t_game *g);
int			mouse_move(int x, int y, void *param);
int			mouse_press(int button, int x, int y, void *param);
void		toggle_fullscreen(void *mlx, void *win, t_game *g);
void		end_display_loop(void *mlx);
void		destroy_display(void *mlx);

// mlx_mouse_pos.c
int			get_mouse_pos(void *mlx, void *win, int *x, int *y);
int			move_mouse(void *mlx, void *win);
int			move_mouse_at(void *mlx, void *win, int x, int y);
int			hide_mouse(void *mlx, void *win);

// game_loop.c
void		start_game(t_game *g);
int			game_loop(t_game *g);

// update_player_pos.c
void		update_player_pos(t_player *p, t_game *g);
bool		player_has_free_movement(t_player *p);
void		clamp_player_z(t_player *p, t_game *g);
void		update_vertical_physics(t_player *p, t_game *g);
void		jump_player(t_player *p);
void		toggle_fly_mode(t_player *p, t_game *g);
bool		is_position_legal(t_position pos, t_game *g);
t_coord		get_adjacent_cell(t_position pos, const t_position check_pos);
bool		is_door_open(t_coord pos, t_door **doors);
double		get_floor_z_at(t_game *g, t_position pos);
double		get_ceiling_z_at(t_game *g, t_position pos);
double		get_floor_z_at_cell(t_game *g, t_coord cell);
int			get_light_at(t_game *g, t_position pos);
int			get_light_at_cell(t_game *g, t_coord cell);
int			apply_light(int color, int light, double distance);
void		init_default_sectors(t_game *g);
bool		set_sector_cell(t_game *g, int x, int y, int sector);
bool		set_sector_info(t_game *g, int id, t_sector sector);
t_sector	*get_sector_at_cell(t_game *g, t_coord cell);
void		compute_sector_origins(t_game *g);
void		init_bsp_visibility(t_game *g);
void		update_bsp_visibility(t_game *g);
bool		bsp_tile_visible(t_game *g, t_coord cell);
bool		bsp_position_visible(t_game *g, t_position pos);
bool		add_wall_segment(t_game *g, t_wall_segment segment);
bool		segment_blocks_position(t_game *g, t_position pos);

// door.c
void		init_door_grid(t_game *g);
void		update_doors(bool *door_updated, t_game *g);
bool		update_one_door(t_door *door, double delta_time);
void		toggle_adjacent_door(t_game *g);
void		open_one_door(t_game *g, t_coord target);

// hooks_door_bonus.c
bool		is_in_bounds(t_coord pos, t_game *g);
bool		is_door(t_coord pos, t_game *g);
bool		is_on_player(t_coord pos, t_game *g);
void		activate_door(t_coord pos, t_game *g);

// stop_game.c
int			stop_game(void *param);

// menu.c
void		init_menu(t_game *g);
void		render_menu(t_game *g);
int			menu_key(int key, t_game *g);
void		menu_load_levels(t_menu *menu);

// level_loader.c
bool		load_level_path(t_game *g, char *path);

// pickup.c
void		update_item_pickups(t_game *g);
bool		is_object_blocking(t_position pos, t_game *g);
void		collect_item(t_item *item, t_game *g);
void		remove_item_sprite(t_item *item, t_game *g);

// item_effects.c
void		apply_health_pickup(t_game *g, int amount);
void		apply_ammo_pickup(t_game *g, int amount);
bool		try_use_vending_machine_at(t_coord pos, t_game *g);
bool		consume_key(t_game *g);
void		use_selected_item(t_game *g);
void		reload_weapon(t_game *g);

// interact.c
void		interact(t_game *g);

// triggers.c
bool		update_proximity_triggers(t_game *g);
void		show_context_message(t_game *g);
bool		update_laptop_proximity(t_game *g);

// events.c
bool		update_world_events(t_game *g);
void		queue_world_event(t_game *g, t_world_event event);
void		trigger_switch_sequence(t_game *g);
bool		authored_action_line_valid(const char *line);
bool		add_authored_action(t_game *g, const char *line);
bool		trigger_authored_actions(t_game *g, t_coord trigger);
bool		parse_block_action(t_authored_action *a, const char *args);
bool		parse_texture_action(t_authored_action *a, const char *args);
bool		block_target_valid(t_game *g, t_authored_action *a);
bool		object_target_valid(t_game *g, t_authored_action *a);
bool		wall_target_valid(t_game *g, t_authored_action *a);
bool		queue_authored_action(t_game *g, int index);
void		run_authored_action(t_game *g, int index);

// sector_events.c
void		make_event_sector(t_world_event *event, int target,
				double from_value, double to_value);
void		make_event_door(t_world_event *event, t_coord target, double delay);
bool		animate_sector_event(t_game *g, t_world_event *event);

// switch_targets.c
void		trigger_elevator_switch(t_game *g, t_coord pos);
void		trigger_secret_switch(t_game *g, t_coord pos);
bool		is_secret_cell(t_game *g, t_coord pos);
bool		is_locked_cell(t_game *g, t_coord pos);

// message.c
void		show_message(t_game *g, const char *text, double duration);
bool		update_message(t_game *g);

// level_flow.c
void		start_level_flow(t_game *g);
bool		update_level_flow(t_game *g);
void		load_next_level(t_game *g);
void		complete_level(t_game *g);
void		fail_level(t_game *g);
bool		update_end_state(t_game *g);

// enemies.c
bool		damage_enemy_at_sprite(t_game *g, int sprite_index, int damage);
void		remove_enemy_sprite(t_game *g, t_enemy *enemy);
bool		enemy_chase(t_enemy *enemy, t_game *g, double distance);
bool		enemy_attack(t_enemy *enemy, t_game *g, double distance);

// enemy_update.c
bool		update_enemies(t_game *g);

// enemy_ranged.c
bool		enemy_ranged_attack(t_enemy *enemy, t_game *g, double distance);
void		fire_enemy_projectile(t_game *g, t_enemy *enemy);

// projectile.c
void		fire_projectile(t_game *g);
bool		update_projectiles(t_game *g);
bool		hit_sprite(t_game *g, t_projectile *p, t_position pos);
bool		projectile_is_decoration(t_game *g, int index);
bool		projectile_target_protected(t_game *g, int index);
void		remove_projectile_target(t_game *g, int index, int damage);
bool		hit_wall(t_game *g, t_projectile *p, t_position pos);
bool		projectile_path_is_blocked(t_game *g, t_position start,
				t_position end, double z);

// projectile_fire.c
int			fire_projectile_from(t_game *g, t_shot_spec spec);

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

// minimap.c
void		draw_minimap(t_game *g);
void		minimap_pixel(t_game *g, int x, int y, int color);
void		minimap_rect(t_game *g, t_coord pos, t_coord size, int color);
void		draw_map_cells(t_game *g, t_coord origin);
void		draw_minimap_sprites(t_game *g, t_coord origin);
void		draw_minimap_markers(t_game *g, t_coord origin);

// minimap_utils.c
void		draw_map_segments(t_game *g, t_coord origin);

// ray_casting.c
void		cast_all_rays(t_ray *rays, t_game *g);
void		cast_one_ray(t_ray *ray, double angle, t_game *g);
void		cast_segment_walls(t_ray *ray, t_game *g);

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

// height_step.c
int			get_sector_id_at_cell(t_game *g, t_coord cell);
void		record_height_step(t_dda *dda, t_ray *ray, t_game *g,
				int *prev_sector);
int			band_sector(t_ray *ray, int i);
double		step_far_d(t_ray *ray, int i);
bool		height_step_occludes_pixel(t_ray *ray, double depth, int y,
				t_game *g);

// draw_step_bands.c
void		draw_step_band(t_step_ctx ctx, t_game *g);

// draw_risers.c
void		draw_height_steps(t_ray *ray, t_game *g);

// draw_scene.c
void		draw_scene(t_game *g);
void		draw_floor_ceiling(t_game *g);
void		init_floor_cast(t_floor_cast *cast, t_game *g);
void		set_surface_row(int y, int horizon, t_floor_cast *cast, t_game *g);
void		draw_all_rays(t_ray *rays, t_game *g, double *z_buffer);
void		draw_all_rays_threaded(t_ray *rays, t_game *g, double *z_buffer);
void		draw_one_ray(t_ray *ray, t_game *g);
void		draw_vertical_line(int x, t_dimensions dims, int color, t_game *g);

// draw_sprites.c
void		draw_sprites(t_game *g, double *z_buffer, t_ray *rays);
void		draw_projectiles(t_game *g, double *z_buffer, t_ray *rays);

// draw_wall.c
void		draw_wall_slice(t_dimensions wall, t_ray *ray, t_game *g);
void		draw_transparent_walls(t_ray *ray, t_game *g);
void		draw_wall_decal(t_dimensions wall, t_ray *ray, t_game *g);
t_texture	*get_wall_texture(t_ray *ray, t_game *g);
void		get_wall_top_bottom(t_dimensions *wall, t_ray *ray, t_game *g);
t_position	ray_world_pos(t_ray *ray, double distance, t_game *g);
int			project_world_z(double world_z, double distance, t_game *g);

// draw_door.c
void		draw_door_slice(t_ray *ray, t_game *g);
void		get_door_top_bottom(t_dimensions *door, t_ray *ray, t_game *g);
bool		door_occludes_pixel(t_ray *ray, double depth, int y, t_game *g);

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
int			wall_light(int side, int light, bool hit_segment);

// ----- PARSER ----- //
// bonus.c
int			ft_header_error(int *vals);
int			ft_check_amount(void);

// generate_map.c
void		ft_populate_map(char **map, int *vals, t_game *g);
void		ft_populate_info(t_header *h, t_game *g);
t_block		ft_convert_tblock(char c);

// generate_map_utils.c
void		add_elevators(char **map, int lines, int width, t_game *g);
void		add_secrets(char **map, int lines, int width, t_game *g);
void		add_locked_doors(char **map, int lines, int width, t_game *g);

// get_header.c
void		ft_setup_header(t_header *header);
char		**ft_values_setup(int *readval);
int			ft_header_extractor(char *line, int *vals, t_header *p);
void		ft_get_xpm(char *dest, char *src, int *found, int *vals);
void		ft_get_rgb(int *rgb, int *found, char *line, int *values);
void		ft_get_surface(char *dest, int *rgb, int *found, char *line);
int			ft_get_asset_texture(char *text, t_header *header, int *vals);

// get_map.c
void		ft_extract_line(char *src, char *dest, int chars);
int			ft_get_map(char *mapfile, int *vals, int *succes, t_game *g);

// parser.c
int			ft_is_file(char *path);
int			ft_parse_file(int argc, char *argv[], t_game *g);
int			ft_amount_of_commas(char **rgbs, int *values, char *line);
int			ft_parse_packed_file(int argc, char *argv[], t_game *g);
int			pack_level_file(char *src, char *dst);
int			ft_cub_extension(char *path);
int			ft_dnk_extension(char *path);
int			ft_xpm_extension(const char *path);

// valid_header.c
int			ft_get_header(char *map, int *ok, t_header *p);
int			ft_xpm_extension(const char *path);
int			ft_good_xpms(t_header *h, int *ok, int *vals, int amount);
int			ft_good_rgb(t_header *header, int *ok);
int			ft_good_optional_assets(t_header *header, int *ok);
void		ft_ok(int *vals, int *ok, t_header *header);

// ft_valid_map.c
int			ft_extract_map(char **map, char *mapfile, int *dims);
char		**ft_map_dup(char **map, int lines, int width);
int			ft_valid_map(char **map, int lines, t_game *g);
int			ft_solid_map_border(char **map, int lines, int width);
void		ft_set_player_from_map(t_game *g, char **map, int *player);

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

// segment_check.c
void		check_segment_loops(t_game *g);

// ----- UTILS ----- //

// init_game.c
void		init_game_struct(t_game *g);
void		ft_int_image(t_img *img);
void		ft_int_assets(t_assets *assets);
void		ft_int_player(t_player *p);
void		ft_int_hud(t_hud *hud);
void		ft_init_map(t_map *map);
void		ft_int_message(t_message *message);
void		ft_int_level_flow(t_level_flow *level);
void		ft_int_audio(t_audio *audio);
void		ft_int_projectiles(t_projectile *projectiles);
void		ft_int_events(t_world_event *events);

// allocation.c
void		*s_alloc(void *pointer, t_game *g);
void		*calloc_s(size_t count, size_t size, t_game *g);
void		track_alloc(void *pointer, t_game *g);

// error.c
void		error(char *description, t_game *g);

// free.c
void		free_all(t_game *g);
void		ft_destroy_textures(t_game *g);
void		cleanup_unpacked_level(t_game *g);

// sound.c
void		play_sound_effect(t_game *g, const char *name);
void		init_audio(t_game *g);
bool		audio_backend_init(void);
void		audio_backend_stop(void);
void		start_background_music(t_game *g);
void		stop_audio(t_game *g);
void		update_audio(void);

// sound_utils.c
bool		sound_path(char *dst, size_t size, const char *dir,
				const char *name);
void		close_channel(t_channel *channel);
void		load_channel_wav(t_channel *channel, const char *path, bool loop);
t_channel	*pick_sfx_channel(t_game *g);
void		audio_close_channel(t_channel *channel);
bool		audio_channel_ready(t_channel *channel);

// ALSA helpers
t_wav_uint	wav_u16(const unsigned char *data);
t_wav_uint	wav_u32(const unsigned char *data);
bool		read_wav_file(const char *path, unsigned char **file, size_t *size);
void		parse_wav_chunk(t_channel *channel, unsigned char *chunk,
				unsigned char **data, unsigned int *data_len);
bool		parse_wav(t_channel *channel, unsigned char *file, size_t size);
void		*audio_channel_thread(void *data);

#endif
