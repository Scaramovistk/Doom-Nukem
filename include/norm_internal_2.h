/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm_internal_2.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NORM_INTERNAL_2_H
# define NORM_INTERNAL_2_H

void		apply_packed_defaults(t_dnk *dnk, t_game *g);
bool		read_file_hex(FILE *out, const char *path);
bool		write_asset_block(FILE *out, const char *key, const char *path);
bool		header_asset_key(char *line, char *key, char *path);
void		write_cub_line(FILE *out, char *line);
bool		write_sound_assets(FILE *out);
bool		write_hud_assets(FILE *out);
bool		write_cub_assets(FILE *out, char *src);
bool		is_map_source_line(char *line);
int			max_map_width(char **lines, int count);
void		write_default_sector_grid(FILE *out, char **lines, int count);
bool		write_sector_sidecar(FILE *out, char *src);
bool		load_cub_text(char *src, char **lines, int *count);
bool		write_all_assets(FILE *out, char *src);
bool		pack_source_valid(char *src);
bool		ft_is_enemy_char(char c);
int			count_map_char(char **map, int lines, int width, char target);
int			count_decorations(char **map, int lines, int width);
int			count_billboard_decorations(char **map, int lines, int width);
bool		button_neighbor(char c);
t_position	button_position(t_map_scan *scan, int x, int y);
void		add_decorations(char **map, int lines, int width, t_game *g);
int			count_world_objects(char **map, int lines, int width);
void		add_world_objects(t_map_scan *scan);
void		add_decoration_cell(t_map_scan *scan, int x, int y);
void		add_world_object_cell(t_map_scan *scan, int x, int y);
void		add_laptop_cell(t_map_scan *scan, int x, int y);
void		add_item_cell(t_map_scan *scan, int x, int y);
void		count_sprite_entities(t_map_scan *scan, t_sprite_counts *counts);
void		prepare_sprite_storage(t_map_scan *scan, t_sprite_counts *counts);
void		add_enemy_sprite_cells(t_map_scan *scan, int *types);
void		add_remaining_sprites(t_map_scan *scan, t_sprite_counts *counts);
void		init_enemy(t_enemy *enemy, t_enemy_stats stats, int type,
				t_position position);
void		add_flag_sprite(t_map_scan *scan);
void		populate_map_grid(char **map, int lines, int width, t_game *g);
void		populate_wall_assets(t_header *h, t_game *g);
void		populate_plane_assets(t_header *h, t_game *g);
void		populate_special_assets(t_header *h, t_game *g);
void		populate_sprite_frames(t_header *h, t_game *g);
void		populate_entity_assets(t_header *h, t_game *g);
int			count_vending_machines(char **map, int lines, int width);

#endif
