/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm_internal_1.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NORM_INTERNAL_1_H
# define NORM_INTERNAL_1_H

void		trim_eol(char *line);
bool		starts_with(const char *text, const char *prefix);
void		append_path(char *dst, const char *part);
void		make_unpack_dir(t_dnk *dnk, t_game *g);
int			hex_value(char c);
bool		write_hex_line(int fd, const char *line);
bool		valid_asset_part(const char *text);
bool		is_sound_asset(const char *key);
bool		is_hud_asset(const char *key);
void		asset_path(t_dnk *dnk, t_dnk_asset *asset, t_game *g);
bool		extract_asset(int fd, char *header, t_dnk *dnk, t_game *g);
char		*dup_dnk_line(char *line, t_game *g);
bool		store_section_line(t_dnk *dnk, char *line, int state, t_game *g);
bool		read_dnk(char *path, t_dnk *dnk, t_game *g);
bool		packed_cub_is_self_contained(t_dnk *dnk);
bool		extract_asset_lines(int fd, int out);
bool		read_dnk_line(int fd, char *line, t_dnk_reader *reader);
void		append_asset_reference(char *dst, const char **src, t_dnk *dnk);
bool		parse_packed_sector(t_game *g, char *line);
bool		parse_packed_wall(t_game *g, char *line);
bool		write_sound_entry(FILE *out, char *filename);
int			count_map_lines(char **lines, int count);
void		write_sector_rows(FILE *out, char **lines, int count, int width);
void		free_cub_lines(char **lines, int count);
void		write_cub_section(FILE *out, char **lines, int count);
void		clear_packed_fallbacks(t_game *g);
const char	*find_asset_path(t_dnk *dnk, const char *key);
void		append_replaced(char *dst, const char *src, t_dnk *dnk);
bool		write_temp_cub(t_dnk *dnk);
int			sector_id_from_char(char c);
void		apply_sector_grid_line(t_game *g, char *line, int y);
bool		parse_sector_line(t_game *g, char *line, int *grid_y);
bool		apply_packed_sectors(t_dnk *dnk, t_game *g);
void		set_hud_source(char **field, const char *key, t_dnk *dnk,
				t_game *g);
void		apply_packed_hud(t_dnk *dnk, t_game *g);

#endif
