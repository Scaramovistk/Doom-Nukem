/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_dispatch.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	editor_dispatch_edit(t_editor_doc *doc, char *command)
{
	if (!ft_strcmp(command, "help"))
		print_help();
	else if (!ft_strcmp(command, "show map"))
		show_map(doc);
	else if (!ft_strcmp(command, "show sectors"))
		show_sectors(doc);
	else if (!ft_strncmp(command, "set ", 4))
		set_map_cell(doc, command);
	else if (!ft_strncmp(command, "texture ", 8))
		set_texture(doc, command);
	else if (!ft_strncmp(command, "sector ", 7))
		set_sector(doc, command);
	else if (!ft_strncmp(command, "cell ", 5))
		edit_sector_cell(doc, command);
	else
		return (false);
	return (true);
}

bool	editor_dispatch_project(t_editor_doc *doc, char *command)
{
	if (!ft_strncmp(command, "wall add ", 9))
		add_wall(doc, command);
	else if (!ft_strcmp(command, "wall clear"))
		clear_walls(doc);
	else if (!ft_strncmp(command, "action add ", 11))
		add_action(doc, command);
	else if (!ft_strcmp(command, "action clear"))
		clear_actions(doc);
	else if (!ft_strcmp(command, "save"))
		save_document(doc);
	else if (!ft_strcmp(command, "validate"))
		validate_document(doc);
	else if (!ft_strncmp(command, "pack", 4))
		pack_document(doc, command);
	else
		return (false);
	return (true);
}

bool	editor_terminal_loop(t_editor_doc *doc, char *command)
{
	bool	running;

	running = true;
	while (running)
	{
		if (isatty(STDIN_FILENO))
			printf("dnk-edit> ");
		if (!fgets(command, LINE_SIZE, stdin))
			break ;
		running = editor_command(doc, command);
	}
	return (running);
}
