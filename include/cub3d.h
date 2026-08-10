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

// app/main.c and editor/editor_launcher.c
int				main(int argc, char *argv[]);
int				run_editor(int argc, char *argv[]);
int				check_level_file(char *path);
int				interactive_level_editor(char *src, char *default_output);
bool			editor_open(t_editor_doc *doc, char *src, char *output);
void			editor_close(t_editor_doc *doc);
bool			editor_command(t_editor_doc *doc, char *command);
bool			editor_border_valid(t_editor_doc *doc);
int				terminal_level_editor(char *src, char *default_output);
int				editor_render(t_editor *editor);
void			editor_put_labels(t_editor *editor);
void			editor_put_tool_labels(t_editor *editor);
int				editor_key(int key, t_editor *editor);
int				editor_mouse(int button, int x, int y, t_editor *editor);
void			editor_run_command(t_editor *e, char *command, char *status);
void			editor_action_at_selection(t_editor *e);
void			editor_adjust_sector(t_editor *e, int property);
void			editor_texture_preset(t_editor *e);
void			editor_project_key(int key, t_editor *e);
bool			editor_select_tool(int key, t_editor *e);
char			editor_tool_from_key(int key);
int				editor_cell_color(char token);
void			editor_apply_selected_tool(t_editor *e);
bool			editor_panel_click(t_editor *e, int x, int y);
int				editor_exit(t_editor *editor);
void			editor_setup(t_editor *editor);
bool			parse_sector_command(char *command, t_sector *sector, int *id);
bool			replace_sector_line(t_editor_doc *doc, char *line, int id);
bool			editor_dispatch_edit(t_editor_doc *doc, char *command);
bool			editor_dispatch_project(t_editor_doc *doc, char *command);
bool			editor_terminal_loop(t_editor_doc *doc, char *command);

// ----- APP AND GAMEPLAY ----- //

// app/input_hooks.c
void			setup_hooks(t_game *g);
int				pressed(int keycode, t_game *g);
void			handle_action_key(int key, t_game *g);
int				released(int keycode, t_game *g);
int				mouse_move(int x, int y, void *param);
int				mouse_press(int button, int x, int y, void *param);
void			toggle_fullscreen(void *mlx, void *win, t_game *g);
void			end_display_loop(void *mlx);
void			destroy_display(void *mlx);

// app/mouse_linux.c and app/mouse_mac.c
int				get_mouse_pos(void *mlx, void *win, int *x, int *y);
int				move_mouse(void *mlx, void *win);
int				move_mouse_at(void *mlx, void *win, int x, int y);
int				hide_mouse(void *mlx, void *win);

// app/game_loop.c
void			start_game(t_game *g);
int				game_loop(t_game *g);

// gameplay/player_movement.c and world/sector files
void			update_player_pos(t_player *p, t_game *g);
bool			player_has_free_movement(t_player *p);
void			clamp_player_z(t_player *p, t_game *g);
void			update_vertical_physics(t_player *p, t_game *g);
void			jump_player(t_player *p);
void			toggle_fly_mode(t_player *p, t_game *g);
bool			is_position_legal(t_position pos, t_game *g);
t_coord			get_adjacent_cell(t_position pos, const t_position check_pos);
bool			is_door_open(t_coord pos, t_door **doors);
double			get_floor_z_at(t_game *g, t_position pos);
double			get_ceiling_z_at(t_game *g, t_position pos);
double			get_floor_z_at_cell(t_game *g, t_coord cell);
int				get_light_at(t_game *g, t_position pos);
int				get_light_at_cell(t_game *g, t_coord cell);
int				apply_light(int color, int light, double distance);
void			init_default_sectors(t_game *g);
bool			set_sector_cell(t_game *g, int x, int y, int sector);
bool			set_sector_info(t_game *g, int id, t_sector sector);
t_sector		*get_sector_at_cell(t_game *g, t_coord cell);
void			compute_sector_origins(t_game *g);
void			init_bsp_visibility(t_game *g);
void			update_bsp_visibility(t_game *g);
bool			bsp_tile_visible(t_game *g, t_coord cell);
bool			bsp_position_visible(t_game *g, t_position pos);
bool			add_wall_segment(t_game *g, t_wall_segment segment);
bool			segment_blocks_position(t_game *g, t_position pos);

// gameplay/door_runtime.c
void			init_door_grid(t_game *g);
void			update_doors(bool *door_updated, t_game *g);
bool			update_one_door(t_door *door, double delta_time);
void			toggle_adjacent_door(t_game *g);
void			open_one_door(t_game *g, t_coord target);

// gameplay/door_interaction.c
bool			is_in_bounds(t_coord pos, t_game *g);
bool			is_door(t_coord pos, t_game *g);
bool			is_on_player(t_coord pos, t_game *g);
void			activate_door(t_coord pos, t_game *g);

// app/game_shutdown.c
int				stop_game(void *param);

// app/menu_controller.c
void			init_menu(t_game *g);
void			render_menu(t_game *g);
int				menu_key(int key, t_game *g);
void			menu_load_levels(t_menu *menu);

// app/level_loader.c
bool			load_level_path(t_game *g, char *path);

// gameplay/item_pickup.c
void			update_item_pickups(t_game *g);
bool			is_object_blocking(t_position pos, t_game *g);
void			collect_item(t_item *item, t_game *g);
void			remove_item_sprite(t_item *item, t_game *g);

// item_effects.c
void			apply_health_pickup(t_game *g, int amount);
void			apply_ammo_pickup(t_game *g, int amount);
bool			try_use_vending_machine_at(t_coord pos, t_game *g);
bool			consume_key(t_game *g);
void			use_selected_item(t_game *g);
void			reload_weapon(t_game *g);

// gameplay/player_interaction.c
void			interact(t_game *g);

// gameplay/proximity_triggers.c
bool			update_proximity_triggers(t_game *g);
void			show_context_message(t_game *g);
bool			update_laptop_proximity(t_game *g);

// gameplay/event_processing.c and action files
bool			update_world_events(t_game *g);
void			queue_world_event(t_game *g, t_world_event event);
void			trigger_switch_sequence(t_game *g);
bool			authored_action_line_valid(const char *line);
bool			add_authored_action(t_game *g, const char *line);
bool			trigger_authored_actions(t_game *g, t_coord trigger);
bool			parse_block_action(t_authored_action *a, const char *args);
bool			parse_texture_action(t_authored_action *a, const char *args);
bool			block_target_valid(t_game *g, t_authored_action *a);
bool			object_target_valid(t_game *g, t_authored_action *a);
bool			wall_target_valid(t_game *g, t_authored_action *a);
bool			queue_authored_action(t_game *g, int index);
void			run_authored_action(t_game *g, int index);

// gameplay/sector_animation.c
void			make_event_sector(t_world_event *event, int target,
					double from_value, double to_value);
void			make_event_door(t_world_event *event, t_coord target,
					double delay);
bool			animate_sector_event(t_game *g, t_world_event *event);

// gameplay/switch_targets.c
void			trigger_elevator_switch(t_game *g, t_coord pos);
void			trigger_secret_switch(t_game *g, t_coord pos);
bool			is_secret_cell(t_game *g, t_coord pos);
bool			is_locked_cell(t_game *g, t_coord pos);

// gameplay/game_messages.c
void			show_message(t_game *g, const char *text, double duration);
bool			update_message(t_game *g);

// app/campaign_progress.c and app/campaign_outcome.c
void			start_level_flow(t_game *g);
bool			update_level_flow(t_game *g);
void			load_next_level(t_game *g);
void			complete_level(t_game *g);
void			fail_level(t_game *g);
bool			update_end_state(t_game *g);

// gameplay/enemy_behavior.c
bool			damage_enemy_at_sprite(t_game *g, int sprite_index, int damage);
void			remove_enemy_sprite(t_game *g, t_enemy *enemy);
bool			enemy_chase(t_enemy *enemy, t_game *g, double distance);
bool			enemy_attack(t_enemy *enemy, t_game *g, double distance);

// enemy_update.c
bool			update_enemies(t_game *g);

// enemy_ranged.c
bool			enemy_ranged_attack(t_enemy *enemy, t_game *g, double distance);
void			fire_enemy_projectile(t_game *g, t_enemy *enemy);

// gameplay/projectile_collision.c
void			fire_projectile(t_game *g);
bool			update_projectiles(t_game *g);
bool			hit_sprite(t_game *g, t_projectile *p, t_position pos);
bool			projectile_is_decoration(t_game *g, int index);
bool			projectile_target_protected(t_game *g, int index);
void			remove_projectile_target(t_game *g, int index, int damage);
bool			hit_wall(t_game *g, t_projectile *p, t_position pos);
bool			projectile_path_is_blocked(t_game *g, t_position start,
					t_position end, double z);

// projectile_fire.c
int				fire_projectile_from(t_game *g, t_shot_spec spec);

// ----- RENDERING ----- //

// rendering/display_setup.c and texture load files
void			load_game(t_game *g);
void			load_all_textures(t_game *g);
void			load_texture(t_texture *t, int *size, t_game *g);
void			load_floor_ceiling_textures(t_game *g);
void			ft_adjust_player_view(t_game *g);
void			open_window(t_game *g);

// rendering/frame_render.c
int				render(t_game *g);
void			ft_regenerate_image(t_game *g);
void			init_display_img(t_img *img, t_game *g);

// rendering/hud files
void			draw_hud(t_game *g);

// rendering/minimap_render.c
void			draw_minimap(t_game *g);
void			minimap_pixel(t_game *g, int x, int y, int color);
void			minimap_rect(t_game *g, t_coord pos, t_coord size, int color);
void			draw_map_cells(t_game *g, t_coord origin);
void			draw_minimap_sprites(t_game *g, t_coord origin);
void			draw_minimap_markers(t_game *g, t_coord origin);

// rendering/minimap_segments.c
void			draw_map_segments(t_game *g, t_coord origin);

// rendering/ray_cast.c
void			cast_all_rays(t_ray *rays, t_game *g);
void			cast_one_ray(t_ray *ray, double angle, t_game *g);
void			cast_segment_walls(t_ray *ray, t_game *g);

// rendering/grid_traversal.c
void			perform_dda(t_dda *dda, t_ray *ray, t_game *g);
void			progress_dda(t_dda *dda);
bool			wall_collision(t_dda *dda, t_ray *ray, t_game *g);
bool			door_collision(t_dda *dda, t_ray *ray, t_game *g);
void			transparent_collision(t_dda *dda, t_ray *ray, t_game *g);
void			update_door_ray_info(t_dda *dda, t_ray *ray, t_game *g);

// rendering/grid_traversal_setup.c
void			init_dda(t_dda *dda, t_ray *ray, t_game *g);
void			calculate_step_and_side_dist(t_dda *dda, t_ray *ray, t_game *g);
void			apply_cross_distance(t_dda *dda, double *dist, int side,
					t_game *g);

// rendering/height_transitions.c
int				get_sector_id_at_cell(t_game *g, t_coord cell);
void			record_height_step(t_dda *dda, t_ray *ray, t_game *g,
					int *prev_sector);
int				band_sector(t_ray *ray, int i);
double			step_far_d(t_ray *ray, int i);
bool			height_step_occludes_pixel(t_ray *ray, double depth, int y,
					t_game *g);

// rendering/step_band_render.c
void			draw_step_band(t_step_ctx ctx, t_game *g);

// rendering/riser_render.c
void			draw_height_steps(t_ray *ray, t_game *g);

// rendering/scene_render.c
void			draw_scene(t_game *g);
void			draw_floor_ceiling(t_game *g);
void			init_floor_cast(t_floor_cast *cast, t_game *g);
void			set_surface_row(int y, int horizon, t_floor_cast *cast,
					t_game *g);
void			draw_all_rays(t_ray *rays, t_game *g, double *z_buffer);
void			draw_all_rays_threaded(t_ray *rays, t_game *g,
					double *z_buffer);
void			draw_one_ray(t_ray *ray, t_game *g);
void			draw_vertical_line(int x, t_dimensions dims, int color,
					t_game *g);

// rendering/sprite files
void			draw_sprites(t_game *g, double *z_buffer, t_ray *rays);
void			draw_projectiles(t_game *g, double *z_buffer, t_ray *rays);

// rendering/wall_geometry.c and rendering/wall_render.c
void			draw_wall_slice(t_dimensions wall, t_ray *ray, t_game *g);
void			draw_transparent_walls(t_ray *ray, t_game *g);
void			draw_wall_decal(t_dimensions wall, t_ray *ray, t_game *g);
t_texture		*get_wall_texture(t_ray *ray, t_game *g);
void			get_wall_top_bottom(t_dimensions *wall, t_ray *ray, t_game *g);
t_position		ray_world_pos(t_ray *ray, double distance, t_game *g);
int				project_world_z(double world_z, double distance, t_game *g);

// rendering/door_render.c
void			draw_door_slice(t_ray *ray, t_game *g);
void			get_door_top_bottom(t_dimensions *door, t_ray *ray, t_game *g);
bool			door_occludes_pixel(t_ray *ray, double depth, int y, t_game *g);

// rendering/texture_render.c
double			get_texture_x(t_ray *ray, double distance, int side, t_game *g);
void			draw_texture_slice(t_texture_slice *slice, t_game *g);
void			draw_texture_slice_alpha(t_texture_slice *slice, t_game *g);
int				get_pixel(t_img *img, int x, int y);
int				blend_color(int dst, int src, double alpha);

// rendering/frame_render.c
void			put_pixel(t_img *img, int x, int y, int color);

// rendering/frame_time.c
void			update_time(t_game *g);
double			get_time(void);
double			calculate_delta_time(double *last_time);

// rendering/render_math.c
double			deg_to_rad(double angle);
double			normalize_angle(double angle);
double			angle_diff(double angle1, double angle2);
int				wall_light(int side, int light, bool hit_segment);

// ----- LEVEL AND PARSING ----- //
// bonus.c
int				ft_header_error(int *vals);
int				ft_check_amount(void);

// level/map population files
void			ft_populate_map(char **map, int *vals, t_game *g);
void			ft_populate_info(t_header *h, t_game *g);
t_block			ft_convert_tblock(char c);

// level/special_cell_population.c
void			add_elevators(char **map, int lines, int width, t_game *g);
void			add_secrets(char **map, int lines, int width, t_game *g);
void			add_locked_doors(char **map, int lines, int width, t_game *g);

// parsing/header_parse.c
void			ft_setup_header(t_header *header);
char			**ft_values_setup(int *readval);
int				ft_header_extractor(char *line, int *vals, t_header *p);
void			ft_get_xpm(char *dest, char *src, int *found, int *vals);
void			ft_get_rgb(int *rgb, int *found, char *line, int *values);
void			ft_get_surface(char *dest, int *rgb, int *found, char *line);
int				ft_get_asset_texture(char *text, t_header *header, int *vals);

// parsing/map_read.c
void			ft_extract_line(char *src, char *dest, int chars);
int				ft_get_map(char *mapfile, int *vals, int *succes, t_game *g);

// parsing/argument_parse.c and package files
int				ft_is_file(char *path);
int				ft_parse_file(int argc, char *argv[], t_game *g);
int				ft_amount_of_commas(char **rgbs, int *values, char *line);
int				ft_parse_packed_file(int argc, char *argv[], t_game *g);
int				pack_level_file(char *src, char *dst);
int				ft_cub_extension(char *path);
int				ft_dnk_extension(char *path);
int				ft_xpm_extension(const char *path);

// parsing/header validation files
int				ft_get_header(char *map, int *ok, t_header *p);
int				ft_xpm_extension(const char *path);
int				ft_good_xpms(t_header *h, int *ok, int *vals, int amount);
int				ft_good_rgb(t_header *header, int *ok);
int				ft_good_optional_assets(t_header *header, int *ok);
void			ft_ok(int *vals, int *ok, t_header *header);

// parsing/map_validation.c
int				ft_extract_map(char **map, char *mapfile, int *dims);
char			**ft_map_dup(char **map, int lines, int width);
int				ft_valid_map(char **map, int lines, t_game *g);
int				ft_solid_map_border(char **map, int lines, int width);
void			ft_set_player_from_map(t_game *g, char **map, int *player);

// ----- PARSING HELPERS ----- //
// parsing/parse_errors.c
int				ft_parsing_error(char *descrition, int ret);
int				ft_map_error(char **map, char *description, int lines, int ret);
int				ft_file_error(int file, char *to_free, char *description,
					int ret);
int				ft_end(int fd, char *to_free, char *to_free2, int ret);
void			ft_free_map(char **map, int lines);

// parsing/text_parse.c
char			*ft_strcpy(char *dest, char *src);
int				ft_isspace(char c);
int				ft_trimspaces(char *str);
int				ft_skip_blanks(char *map, int skip);

// parsing/map_line_validation.c
int				ft_map_line(const char *str);
int				ft_invalid_line(const char *str);
int				ft_find_access_map(char **map, int vert, int hor, int *vals);
int				ft_closed_map(char **map, int *vals);

// parsing/map_shape_validation.c
int				ft_hallway(char **map, int *vals);

// parsing/segment_validation.c
void			check_segment_loops(t_game *g);

// ----- CORE AND AUDIO ----- //

// core initialization files
void			init_game_struct(t_game *g);
void			ft_int_image(t_img *img);
void			ft_int_assets(t_assets *assets);
void			ft_int_player(t_player *p);
void			ft_int_hud(t_hud *hud);
void			ft_init_map(t_map *map);
void			ft_int_message(t_message *message);
void			ft_int_level_flow(t_level_flow *level);
void			ft_int_audio(t_audio *audio);
void			ft_int_projectiles(t_projectile *projectiles);
void			ft_int_events(t_world_event *events);

// core/memory_tracking.c
void			*s_alloc(void *pointer, t_game *g);
void			*calloc_s(size_t count, size_t size, t_game *g);
void			track_alloc(void *pointer, t_game *g);

// core/fatal_error.c
void			error(char *description, t_game *g);

// core cleanup files
void			free_all(t_game *g);
void			ft_destroy_textures(t_game *g);
void			cleanup_unpacked_level(t_game *g);

// audio/audio_controller.c
void			play_sound_effect(t_game *g, const char *name);
void			init_audio(t_game *g);
bool			audio_backend_init(void);
void			audio_backend_stop(void);
void			start_background_music(t_game *g);
void			stop_audio(t_game *g);
void			update_audio(void);

// audio/audio_paths.c
bool			sound_path(char *dst, size_t size, const char *dir,
					const char *name);
void			close_channel(t_channel *channel);
void			load_channel_wav(t_channel *channel, const char *path,
					bool loop);
t_channel		*pick_sfx_channel(t_game *g);
void			audio_close_channel(t_channel *channel);
bool			audio_channel_ready(t_channel *channel);

// ALSA helpers
t_wav_uint		wav_u16(const unsigned char *data);
t_wav_uint		wav_u32(const unsigned char *data);
bool			read_wav_file(const char *path, unsigned char **file,
					size_t *size);
void			parse_wav_chunk(t_channel *channel, unsigned char *chunk,
					unsigned char **data, unsigned int *data_len);
bool			parse_wav(t_channel *channel, unsigned char *file, size_t size);
void			*audio_channel_thread(void *data);

// ----- INTERNAL HELPERS ----- //

void			trim_eol(char *line);
bool			starts_with(const char *text, const char *prefix);
void			append_path(char *dst, const char *part);
void			make_unpack_dir(t_dnk *dnk, t_game *g);
int				hex_value(char c);
bool			write_hex_line(int fd, const char *line);
bool			valid_asset_part(const char *text);
bool			is_sound_asset(const char *key);
bool			is_hud_asset(const char *key);
void			asset_path(t_dnk *dnk, t_dnk_asset *asset, t_game *g);
bool			extract_asset(int fd, char *header, t_dnk *dnk, t_game *g);
char			*dup_dnk_line(char *line, t_game *g);
bool			store_section_line(t_dnk *dnk, char *line, int state,
					t_game *g);
bool			read_dnk(char *path, t_dnk *dnk, t_game *g);
bool			packed_cub_is_self_contained(t_dnk *dnk);
bool			extract_asset_lines(int fd, int out);
bool			read_dnk_line(int fd, char *line, t_dnk_reader *reader);
void			append_asset_reference(char *dst, const char **src, t_dnk *dnk);
bool			parse_packed_sector(t_game *g, char *line);
bool			parse_packed_wall(t_game *g, char *line);
bool			write_sound_entry(FILE *out, char *filename);
int				count_map_lines(char **lines, int count);
void			write_sector_rows(FILE *out, char **lines, int count,
					int width);
void			free_cub_lines(char **lines, int count);
void			write_cub_section(FILE *out, char **lines, int count);
void			clear_packed_fallbacks(t_game *g);
const char		*find_asset_path(t_dnk *dnk, const char *key);
void			append_replaced(char *dst, const char *src, t_dnk *dnk);
bool			write_temp_cub(t_dnk *dnk);
int				sector_id_from_char(char c);
void			apply_sector_grid_line(t_game *g, char *line, int y);
bool			parse_sector_line(t_game *g, char *line, int *grid_y);
bool			apply_packed_sectors(t_dnk *dnk, t_game *g);
void			set_hud_source(char **field, const char *key, t_dnk *dnk,
					t_game *g);
void			apply_packed_hud(t_dnk *dnk, t_game *g);
void			apply_packed_defaults(t_dnk *dnk, t_game *g);
bool			read_file_hex(FILE *out, const char *path);
bool			write_asset_block(FILE *out, const char *key, const char *path);
bool			header_asset_key(char *line, char *key, char *path);
void			write_cub_line(FILE *out, char *line);
bool			write_sound_assets(FILE *out);
bool			write_hud_assets(FILE *out);
bool			write_cub_assets(FILE *out, char *src);
bool			is_map_source_line(char *line);
int				max_map_width(char **lines, int count);
void			write_default_sector_grid(FILE *out, char **lines, int count);
bool			write_sector_sidecar(FILE *out, char *src);
bool			load_cub_text(char *src, char **lines, int *count);
bool			write_all_assets(FILE *out, char *src);
bool			pack_source_valid(char *src);
bool			ft_is_enemy_char(char c);
int				count_map_char(char **map, int lines, int width, char target);
int				count_decorations(char **map, int lines, int width);
int				count_billboard_decorations(char **map, int lines, int width);
bool			button_neighbor(char c);
t_position		button_position(t_map_scan *scan, int x, int y);
void			add_decorations(char **map, int lines, int width, t_game *g);
int				count_world_objects(char **map, int lines, int width);
void			add_world_objects(t_map_scan *scan);
void			add_decoration_cell(t_map_scan *scan, int x, int y);
void			add_world_object_cell(t_map_scan *scan, int x, int y);
void			add_laptop_cell(t_map_scan *scan, int x, int y);
void			add_item_cell(t_map_scan *scan, int x, int y);
void			count_sprite_entities(t_map_scan *scan,
					t_sprite_counts *counts);
void			prepare_sprite_storage(t_map_scan *scan,
					t_sprite_counts *counts);
void			add_enemy_sprite_cells(t_map_scan *scan, int *types);
void			add_remaining_sprites(t_map_scan *scan,
					t_sprite_counts *counts);
void			init_enemy(t_enemy *enemy, t_enemy_stats stats, int type,
					t_position position);
void			add_flag_sprite(t_map_scan *scan);
void			populate_map_grid(char **map, int lines, int width, t_game *g);
void			populate_wall_assets(t_header *h, t_game *g);
void			populate_plane_assets(t_header *h, t_game *g);
void			populate_special_assets(t_header *h, t_game *g);
void			populate_sprite_frames(t_header *h, t_game *g);
void			populate_entity_assets(t_header *h, t_game *g);
int				count_vending_machines(char **map, int lines, int width);
bool			add_vending_machine(t_map_scan *scan);
int				count_laptops(char **map, int lines, int width);
void			add_laptops(t_map_scan *scan);
int				enemy_type_from_char(char c);
int				count_sprites(char **map, int lines, int width);
int				count_flags(char **map, int lines, int width);
bool			ft_is_item_char(char c);
int				count_items(char **map, int lines, int width);
int				item_default_quantity(int type);
bool			item_default_blocks(int type);
void			add_items(t_map_scan *scan);

t_enemy_stats	enemy_stats_for_type(int type);

void			add_enemies(int count, int *types, t_game *g);
void			add_sprites(char **map, int lines, int width, t_game *g);
int				count_char(char **map, int lines, int width, char target);
void			add_switches(char **map, int lines, int width, t_game *g);
void			add_hazards(char **map, int lines, int width, t_game *g);
void			add_messages(char **map, int lines, int width, t_game *g);
void			add_exits(char **map, int lines, int width, t_game *g);
void			add_interactables(char **map, int lines, int width, t_game *g);
char			*editor_dup(const char *text);
void			free_lines(char **lines, int count);
void			free_document(t_editor_doc *doc);
bool			read_lines(const char *path, char **lines, int *count);
bool			is_map_line(const char *line);
bool			scan_map(t_editor_doc *doc);
void			make_sector_path(t_editor_doc *doc);
bool			append_line(char **lines, int *count, const char *text);
void			scan_sector_grid(t_editor_doc *doc);
bool			default_sectors(t_editor_doc *doc);
bool			load_document(t_editor_doc *doc, char *src, char *output);
bool			write_lines(const char *path, char **lines, int count);
bool			save_document(t_editor_doc *doc);
bool			valid_map_token(char token);
void			set_map_cell(t_editor_doc *doc, char *command);
bool			line_has_key(const char *line, const char *key);
bool			insert_line(char **lines, int *count, int index,
					const char *text);
void			set_texture(t_editor_doc *doc, char *command);
void			set_sector(t_editor_doc *doc, char *command);
void			edit_sector_cell(t_editor_doc *doc, char *command);
void			add_wall(t_editor_doc *doc, char *command);
void			clear_walls(t_editor_doc *doc);
void			add_action(t_editor_doc *doc, char *command);
void			clear_actions(t_editor_doc *doc);
void			show_map(t_editor_doc *doc);
void			show_sectors(t_editor_doc *doc);
void			print_help(void);
void			validate_document(t_editor_doc *doc);
void			pack_document(t_editor_doc *doc, char *command);
void			hud_pixel(t_game *g, int x, int y, int color);
void			hud_rect(t_game *g, t_coord pos, t_coord size, int color);
void			hud_frame(t_game *g, t_coord pos, t_coord size, int color);
const char		*digit_pattern(int digit);
void			draw_digit(t_game *g, int digit, t_coord pos, int scale);
void			draw_number(t_game *g, int value, t_coord pos, int scale);
void			draw_health(t_game *g);
void			draw_ammo(t_game *g);
void			draw_inventory(t_game *g);
void			draw_crosshair(t_game *g);
void			draw_score(t_game *g);
const char		*letter_pattern(char c);
void			draw_char(t_game *g, char c, t_coord pos, int scale);
void			draw_punctuation(t_game *g, char c, t_coord pos, int scale);
void			draw_text(t_game *g, const char *text, t_coord pos, int scale);
void			draw_message(t_game *g);
void			draw_fps(t_game *g);
void			draw_weapon_name(t_game *g, t_coord pos);
void			draw_hud_texture(t_game *g, t_texture *texture, t_coord pos,
					int scale);
t_texture		*weapon_texture(t_game *g);
void			draw_weapon_view(t_game *g);
const char		*story_line(int level, bool debrief, int line);
const char		*briefing_text(int level, int line);
const char		*debriefing_text(int level, int line);
void			draw_story_line(t_game *g, const char *line, int y);
void			draw_campaign_story(t_game *g);
void			sort_sprites(t_sprite_draw *sprites, int count);
void			init_sprite_draw(t_sprite_draw *s, t_position pos, int index,
					t_game *g);

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
