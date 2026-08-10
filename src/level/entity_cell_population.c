/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_cell_population.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	add_decoration_cell(t_map_scan *scan, int x, int y)
{
	t_decoration	*decoration;
	char			cell;

	cell = scan->map[y][x];
	if (!((cell >= 'a' && cell <= 'l') || cell == 'L'))
		return ;
	decoration = &scan->g->map.decorations[scan->index++];
	decoration->pos = (t_position){x + 0.5, y + 0.5};
	decoration->type = cell - 'a';
	decoration->sprite_index = -2;
	decoration->z_offset = 0.0;
	decoration->scale = 1.0;
	if (cell >= 'g' && cell <= 'l')
	{
		decoration->type = cell - 'g';
		decoration->sprite_index = -1;
	}
	if (cell == 'L')
	{
		decoration->pos = button_position(scan, x, y);
		decoration->type = ELEVATOR_BUTTON_DECORATION;
		decoration->z_offset = ELEVATOR_BUTTON_Z;
		decoration->scale = ELEVATOR_BUTTON_SCALE;
	}
}

void	add_world_object_cell(t_map_scan *scan, int x, int y)
{
	t_world_object	*object;

	object = &scan->g->map.objects[scan->index - scan->start];
	object->pos = (t_position){x + 0.5, y + 0.5};
	object->sprite_index = scan->index;
	object->texture = SPRITE_T;
	object->scale = WORLD_OBJECT_SCALE;
	object->collision_radius = WORLD_OBJECT_SCALE
		* WORLD_OBJECT_COLLISION_RATIO;
	object->blocks_passage = (scan->map[y][x] == 'V');
	scan->g->map.sprites[scan->index] = object->pos;
	scan->index++;
}

void	add_laptop_cell(t_map_scan *scan, int x, int y)
{
	t_laptop	*laptop;

	laptop = &scan->g->map.laptops[scan->index - scan->start];
	laptop->pos = (t_position){x + 0.5, y + 0.5};
	laptop->sprite_index = scan->index;
	scan->g->map.sprites[scan->index] = laptop->pos;
	scan->index++;
}

void	add_item_cell(t_map_scan *scan, int x, int y)
{
	t_item	*item;
	int		type;

	type = scan->map[y][x] - '6';
	item = &scan->g->map.items[scan->index - scan->start];
	item->pos = (t_position){x + 0.5, y + 0.5};
	item->type = type;
	item->quantity = item_default_quantity(type);
	item->blocks_passage = item_default_blocks(type);
	item->active = true;
	item->sprite_index = scan->index;
	scan->g->map.sprites[scan->index] = item->pos;
	scan->index++;
}
