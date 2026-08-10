/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_launcher.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/22 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	usage(void)
{
	printf("Doom-Nukem level editor/exporter\n");
	printf("  ./doom-nukem --edit <source.cub> [output.dnk]\n");
	printf("  ./doom-nukem --pack <source.cub> <output.dnk>\n");
	printf("  ./doom-nukem --edit <output.dnk>\n");
}

static void	default_dnk_path(char *dst, char *src)
{
	size_t	len;

	ft_strlcpy(dst, src, LINE_SIZE);
	len = ft_strlen(dst);
	if (len > 4 && ft_strcmp(dst + len - 4, ".cub") == 0)
		ft_strlcpy(dst + len - 4, ".dnk", LINE_SIZE - len + 4);
	else
		ft_strlcat(dst, ".dnk", LINE_SIZE);
}

static int	create_starter_level(char *dst)
{
	const char	*starter;

	starter = "tests/maps_src/door_map.cub";
	if (pack_level_file((char *)starter, dst))
	{
		printf("Created starter packed level: %s\n", dst);
		printf("Run it with:\n");
		printf("  ./doom-nukem %s\n", dst);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

static int	export_level(char *src, char *dst)
{
	if (!ft_cub_extension(src))
	{
		printf("Editor export expects a .cub source level.\n");
		return (EXIT_FAILURE);
	}
	if (!ft_dnk_extension(dst))
	{
		printf("Editor export expects a .dnk output file.\n");
		return (EXIT_FAILURE);
	}
	if (!pack_level_file(src, dst))
		return (EXIT_FAILURE);
	printf("Exported packed self-contained level: %s\n", dst);
	return (EXIT_SUCCESS);
}

int	run_editor(int argc, char *argv[])
{
	char	dst[LINE_SIZE];

	if (!ft_strcmp(argv[1], "--pack") && argc == 4)
		return (export_level(argv[2], argv[3]));
	if (!ft_strcmp(argv[1], "--pack") && argc == 3 && ft_cub_extension(argv[2]))
	{
		default_dnk_path(dst, argv[2]);
		return (export_level(argv[2], dst));
	}
	if (!ft_strcmp(argv[1], "--edit") && argc == 4 && ft_cub_extension(argv[2])
		&& ft_dnk_extension(argv[3]))
		return (interactive_level_editor(argv[2], argv[3]));
	if (!ft_strcmp(argv[1], "--edit") && argc == 3 && ft_cub_extension(argv[2]))
	{
		default_dnk_path(dst, argv[2]);
		return (interactive_level_editor(argv[2], dst));
	}
	if (argc == 3 && ft_dnk_extension(argv[2]))
		return (create_starter_level(argv[2]));
	usage();
	return (EXIT_FAILURE);
}
