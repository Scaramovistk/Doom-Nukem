/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_session_6.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	clear_actions(t_editor_doc *doc)
{
	int	i;

	i = 0;
	while (i < doc->sector_count)
	{
		if (!ft_strncmp(doc->sectors[i], "ACTION ", 7))
		{
			free(doc->sectors[i]);
			doc->sector_count--;
			while (i < doc->sector_count)
			{
				doc->sectors[i] = doc->sectors[i + 1];
				i++;
			}
			doc->dirty = true;
			scan_sector_grid(doc);
			return (clear_actions(doc));
		}
		i++;
	}
}

void	show_map(t_editor_doc *doc)
{
	int	y;

	printf("Map %dx%d (x across, y down):\n", doc->map_width, doc->map_height);
	y = 0;
	while (y < doc->map_height)
	{
		printf("%3d %s\n", y, doc->cub[doc->map_start + y]);
		y++;
	}
}

void	show_sectors(t_editor_doc *doc)
{
	int	i;

	i = 0;
	while (i < doc->sector_count)
	{
		printf("%3d %s\n", i, doc->sectors[i]);
		i++;
	}
}

void	print_help(void)
{
	printf("Editor commands:\n");
	printf("  show map | show sectors\n");
	printf("  set X Y TOKEN              place geometry/device/entity\n");
	printf("  texture KEY PATH_OR_RGB    set any cub header texture/value\n");
	printf("  sector ID FLOOR CEIL SX SY LIGHT\n");
	printf("  cell X Y ID                assign sector grid cell\n");
	printf("  wall add X1 Y1 X2 Y2 TEXTURE SECTOR TRANSPARENT\n");
	printf("  wall clear\n");
	printf("  action add TX TY DELAY TYPE ARGUMENTS\n");
	printf("  action clear\n");
	printf("  save | validate | pack [output.dnk] | quit\n");
	printf("Map tokens: 0 floor, 1 wall, 2 door, B locked, P secret,\n");
	printf(" L elevator, T timed switch, H hazard, M message, X exit,\n");
	printf(" 6 health, 7 ammo, 8 key, 9 jetpack, 3/K/I/D/C enemies,\n");
	printf(" V solid object, v pass-through object, a-f decorations.\n");
	printf("Action types: BLOCK X Y EMPTY|WALL|DOOR|GLASS|DECAL;\n");
	printf(" FLOOR/CEILING SECTOR HEIGHT; LIGHT SECTOR VALUE;\n");
	printf(" TEXTURE_SWAP A B; OBJECT_MOVE I X Y; OBJECT_BLOCK I 0|1;\n");
	printf(" OBJECT_SCALE I SCALE; OBJECT_TEXTURE I TEXTURE;\n");
	printf(" WALL_MOVE I X1 Y1 X2 Y2; WALL_TEXTURE I TEXTURE.\n");
}

void	validate_document(t_editor_doc *doc)
{
	char	tmp[LINE_SIZE];

	if (!save_document(doc))
		return ;
	snprintf(tmp, sizeof(tmp), "/tmp/doom_nukem_editor_%d.dnk", getpid());
	if (!pack_level_file(doc->cub_path, tmp))
		return ((void) printf("Validation pack failed.\n"));
	check_level_file(tmp);
	unlink(tmp);
}
