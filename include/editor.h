/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_H
# define EDITOR_H

# include "ft_structures.h"

typedef struct s_editor_doc
{
	char	*cub[DNK_MAX_LINES];
	int		cub_count;
	int		map_start;
	int		map_height;
	int		map_width;
	char	*sectors[DNK_MAX_LINES];
	int		sector_count;
	int		grid_start;
	char	cub_path[LINE_SIZE];
	char	sector_path[LINE_SIZE];
	char	output_path[LINE_SIZE];
	bool	dirty;
}				t_editor_doc;

typedef struct s_editor
{
	t_editor_doc	doc;
	void			*mlx;
	void			*win;
	t_img			img;
	char			tool;
	int				sector;
	int				cell_size;
	int				map_x;
	int				map_y;
	int				selected_x;
	int				selected_y;
	int				page;
	bool			running;
	bool			redraw;
	bool			confirm_close;
	char			status[HUD_MESSAGE_LEN];
}				t_editor;

bool	editor_open(t_editor_doc *doc, char *src, char *output);
void	editor_close(t_editor_doc *doc);
bool	editor_command(t_editor_doc *doc, char *command);
bool	editor_border_valid(t_editor_doc *doc);
int		terminal_level_editor(char *src, char *default_output);
int		editor_render(t_editor *editor);
int		editor_key(int key, t_editor *editor);
int		editor_mouse(int button, int x, int y, t_editor *editor);
int		editor_exit(t_editor *editor);

#endif
