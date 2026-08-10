/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm_internal_4.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NORM_INTERNAL_4_H
# define NORM_INTERNAL_4_H

bool	scan_map(t_editor_doc *doc);
void	make_sector_path(t_editor_doc *doc);
bool	append_line(char **lines, int *count, const char *text);
void	scan_sector_grid(t_editor_doc *doc);
bool	default_sectors(t_editor_doc *doc);
bool	load_document(t_editor_doc *doc, char *src, char *output);
bool	write_lines(const char *path, char **lines, int count);
bool	save_document(t_editor_doc *doc);
bool	valid_map_token(char token);
void	set_map_cell(t_editor_doc *doc, char *command);
bool	line_has_key(const char *line, const char *key);
bool	insert_line(char **lines, int *count, int index, const char *text);
void	set_texture(t_editor_doc *doc, char *command);
void	set_sector(t_editor_doc *doc, char *command);
void	edit_sector_cell(t_editor_doc *doc, char *command);
void	add_wall(t_editor_doc *doc, char *command);
void	clear_walls(t_editor_doc *doc);
void	add_action(t_editor_doc *doc, char *command);
void	clear_actions(t_editor_doc *doc);
void	show_map(t_editor_doc *doc);
void	show_sectors(t_editor_doc *doc);
void	print_help(void);
void	validate_document(t_editor_doc *doc);
void	pack_document(t_editor_doc *doc, char *command);
void	hud_pixel(t_game *g, int x, int y, int color);

#endif
