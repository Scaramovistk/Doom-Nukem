/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_editor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	pack_document(t_editor_doc *doc, char *command)
{
	char	output[LINE_SIZE];

	if (sscanf(command, "pack %299s", output) != 1)
		ft_strlcpy(output, doc->output_path, LINE_SIZE);
	if (!ft_dnk_extension(output))
		return ((void) printf("Pack output must end in .dnk\n"));
	if (save_document(doc) && pack_level_file(doc->cub_path, output))
		printf("Packed %s\n", output);
}

bool	editor_command(t_editor_doc *doc, char *command)
{
	size_t	len;

	len = ft_strlen(command);
	while (len && (command[len - 1] == '\n' || command[len - 1] == '\r'))
		command[--len] = '\0';
	if (!ft_strcmp(command, "quit") || !ft_strcmp(command, "exit"))
		return (false);
	if (!editor_dispatch_edit(doc, command)
		&& !editor_dispatch_project(doc, command) && command[0])
		printf("Unknown command. Type help.\n");
	return (true);
}

int	terminal_level_editor(char *src, char *default_output)
{
	t_editor_doc	doc;
	char			command[LINE_SIZE];

	if (!load_document(&doc, src, default_output))
		return (printf("Unable to load editable .cub/.sectors project.\n"),
			EXIT_FAILURE);
	printf("Doom-Nukem interactive level editor: %s\n", src);
	print_help();
	editor_terminal_loop(&doc, command);
	if (doc.dirty)
		printf("Unsaved changes discarded (use 'save' or 'pack').\n");
	free_document(&doc);
	return (EXIT_SUCCESS);
}
