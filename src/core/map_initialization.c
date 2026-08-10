/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_initialization.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	init_sector_data(t_map *map)
{
	int	i;

	map->sector_grid = NULL;
	map->sector_count = 0;
	map->segment_count = 0;
	i = 0;
	while (i < SECTOR_MAX)
	{
		map->sectors[i].floor_z = PLAYER_FLOOR_Z;
		map->sectors[i].ceil_z = 1.0;
		map->sectors[i].slope_x = 0.0;
		map->sectors[i].slope_y = 0.0;
		map->sectors[i].light = DNK_DEFAULT_LIGHT;
		map->sectors[i].active = false;
		map->sectors[i].elevator_raised = false;
		i++;
	}
}

static void	init_entity_data(t_map *map)
{
	map->sprites = NULL;
	map->sprite_count = 0;
	map->enemies = NULL;
	map->enemy_count = 0;
	map->items = NULL;
	map->item_count = 0;
	map->decorations = NULL;
	map->decoration_count = 0;
	map->objects = NULL;
	map->object_count = 0;
	map->action_count = 0;
	map->vending_machine.pos = (t_position){0.0, 0.0};
	map->vending_machine.sprite_index = -1;
	map->vending_machine.active = false;
	map->laptops = NULL;
	map->laptop_count = 0;
	map->switches = NULL;
	map->switch_count = 0;
}

static void	init_trigger_data(t_map *map)
{
	map->elevators = NULL;
	map->elevator_count = 0;
	map->secrets = NULL;
	map->secret_count = 0;
	map->locked_doors = NULL;
	map->locked_door_count = 0;
	map->hazard_zones = NULL;
	map->hazard_count = 0;
	map->message_zones = NULL;
	map->message_count = 0;
	map->exit_zones = NULL;
	map->exit_count = 0;
	map->has_flag = false;
	map->flag_carried = false;
	map->flag_sprite_index = -1;
	map->flag_pos = (t_position){0.0, 0.0};
	map->flag_base = (t_position){0.0, 0.0};
}

static void	init_visibility_data(t_map *map)
{
	map->width = 0;
	map->height = 0;
	map->bsp_nodes = NULL;
	map->bsp_node_count = 0;
	map->visible_tiles = NULL;
	map->visibility_origin = (t_position){0.0, 0.0};
	map->visibility_angle = 0.0;
	map->visibility_valid = false;
}

void	ft_init_map(t_map *map)
{
	map->grid = NULL;
	map->doors = NULL;
	init_sector_data(map);
	init_entity_data(map);
	init_trigger_data(map);
	init_visibility_data(map);
}
