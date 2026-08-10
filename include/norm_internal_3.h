/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm_internal_3.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NORM_INTERNAL_3_H
# define NORM_INTERNAL_3_H

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

#endif
