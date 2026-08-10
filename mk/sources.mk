# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    sources.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#              #
#    Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Source and Object Files
SRC_APP		=	src/app/action_input.c \
				src/app/campaign_outcome.c \
				src/app/campaign_progress.c \
				src/app/display_lifecycle.c \
				src/app/game_loop.c \
				src/app/game_shutdown.c \
				src/app/input_hooks.c \
				src/app/key_press.c \
				src/app/level_loader.c \
				src/app/main.c \
				src/app/menu_controller.c \
				src/app/menu_level_scan.c \
				src/app/menu_render.c \
				src/app/mouse_linux.c \
				src/app/mouse_mac.c \
				src/app/window_mode.c

SRC_AUDIO		=	src/audio/alsa_channel.c \
				src/audio/alsa_playback.c \
				src/audio/audio_backend.c \
				src/audio/audio_controller.c \
				src/audio/audio_paths.c \
				src/audio/sdl_channel.c \
				src/audio/wav_parse.c \
				src/audio/wav_read.c

SRC_CORE		=	src/core/asset_initialization.c \
				src/core/fatal_error.c \
				src/core/game_cleanup.c \
				src/core/game_initialization.c \
				src/core/map_initialization.c \
				src/core/memory_tracking.c \
				src/core/player_initialization.c \
				src/core/runtime_initialization.c \
				src/core/temporary_level_cleanup.c \
				src/core/texture_cleanup.c

SRC_EDITOR		=	src/editor/document_io.c \
				src/editor/document_map_edit.c \
				src/editor/document_map_scan.c \
				src/editor/document_memory.c \
				src/editor/document_sector_edit.c \
				src/editor/editor_border.c \
				src/editor/editor_colors.c \
				src/editor/editor_command_dispatch.c \
				src/editor/editor_gui_commands.c \
				src/editor/editor_keyboard.c \
				src/editor/editor_launcher.c \
				src/editor/editor_map_input.c \
				src/editor/editor_map_render.c \
				src/editor/editor_panel_input.c \
				src/editor/editor_panel_render.c \
				src/editor/editor_state.c \
				src/editor/editor_tool_keys.c \
				src/editor/editor_tool_render.c \
				src/editor/editor_validation.c \
				src/editor/editor_window.c \
				src/editor/sector_command.c \
				src/editor/terminal_editor.c \
				src/editor/terminal_view.c \
				src/editor/texture_command.c \
				src/editor/texture_presets.c

SRC_GAMEPLAY		=	src/gameplay/action_execution.c \
				src/gameplay/action_parser.c \
				src/gameplay/action_registry.c \
				src/gameplay/action_targets.c \
				src/gameplay/action_triggers.c \
				src/gameplay/action_value_parser.c \
				src/gameplay/door_interaction.c \
				src/gameplay/door_runtime.c \
				src/gameplay/enemy_behavior.c \
				src/gameplay/enemy_ranged.c \
				src/gameplay/enemy_sprite_removal.c \
				src/gameplay/enemy_update.c \
				src/gameplay/event_processing.c \
				src/gameplay/event_queue.c \
				src/gameplay/game_messages.c \
				src/gameplay/item_effects.c \
				src/gameplay/item_pickup.c \
				src/gameplay/item_sprite_removal.c \
				src/gameplay/item_use.c \
				src/gameplay/laptop_interaction.c \
				src/gameplay/object_collision.c \
				src/gameplay/player_collision.c \
				src/gameplay/player_gravity.c \
				src/gameplay/player_interaction.c \
				src/gameplay/player_movement.c \
				src/gameplay/player_vertical.c \
				src/gameplay/projectile_collision.c \
				src/gameplay/projectile_fire.c \
				src/gameplay/projectile_hit.c \
				src/gameplay/projectile_sprite_removal.c \
				src/gameplay/projectile_targets.c \
				src/gameplay/projectile_update.c \
				src/gameplay/proximity_triggers.c \
				src/gameplay/sector_animation.c \
				src/gameplay/switch_sequence.c \
				src/gameplay/switch_targets.c

SRC_LEVEL		=	src/level/asset_population.c \
				src/level/decoration_population.c \
				src/level/enemy_population.c \
				src/level/entity_cell_population.c \
				src/level/entity_population.c \
				src/level/grid_population.c \
				src/level/interaction_population.c \
				src/level/item_token_count.c \
				src/level/map_population.c \
				src/level/map_token_count.c \
				src/level/package_apply_helpers.c \
				src/level/package_asset_parse.c \
				src/level/package_asset_write.c \
				src/level/package_create.c \
				src/level/package_cub_assets.c \
				src/level/package_cub_section.c \
				src/level/package_cub_write.c \
				src/level/package_read.c \
				src/level/package_read_helpers.c \
				src/level/package_sector_apply.c \
				src/level/package_source_write.c \
				src/level/package_text.c \
				src/level/special_cell_population.c \
				src/level/special_object_population.c \
				src/level/sprite_population.c

SRC_PARSING		=	src/parsing/argument_parse.c \
				src/parsing/color_parse.c \
				src/parsing/header_asset_parse.c \
				src/parsing/header_defaults.c \
				src/parsing/header_parse.c \
				src/parsing/header_validation.c \
				src/parsing/header_validation_count.c \
				src/parsing/header_validation_state.c \
				src/parsing/header_value_parse.c \
				src/parsing/map_access_validation.c \
				src/parsing/map_border_validation.c \
				src/parsing/map_line_validation.c \
				src/parsing/map_read.c \
				src/parsing/map_shape_validation.c \
				src/parsing/map_validation.c \
				src/parsing/optional_asset_validation.c \
				src/parsing/parse_errors.c \
				src/parsing/segment_validation.c \
				src/parsing/text_parse.c

SRC_RENDERING		=	src/rendering/alpha_texture_render.c \
				src/rendering/display_setup.c \
				src/rendering/door_render.c \
				src/rendering/frame_render.c \
				src/rendering/frame_time.c \
				src/rendering/grid_traversal.c \
				src/rendering/grid_traversal_setup.c \
				src/rendering/height_occlusion.c \
				src/rendering/height_transitions.c \
				src/rendering/hud_overlays.c \
				src/rendering/hud_primitives.c \
				src/rendering/hud_render.c \
				src/rendering/hud_status.c \
				src/rendering/hud_text.c \
				src/rendering/minimap_cells.c \
				src/rendering/minimap_entities.c \
				src/rendering/minimap_markers.c \
				src/rendering/minimap_render.c \
				src/rendering/minimap_segments.c \
				src/rendering/projectile_render.c \
				src/rendering/ray_cast.c \
				src/rendering/ray_column_render.c \
				src/rendering/render_math.c \
				src/rendering/riser_render.c \
				src/rendering/scene_render.c \
				src/rendering/sprite_entity_lookup.c \
				src/rendering/sprite_pixels.c \
				src/rendering/sprite_projection.c \
				src/rendering/sprite_render.c \
				src/rendering/sprite_render_helpers.c \
				src/rendering/sprite_texture_lookup.c \
				src/rendering/step_band_render.c \
				src/rendering/story_text.c \
				src/rendering/surface_projection.c \
				src/rendering/surface_render.c \
				src/rendering/surface_texture_load.c \
				src/rendering/texture_load.c \
				src/rendering/texture_render.c \
				src/rendering/threaded_render.c \
				src/rendering/wall_geometry.c \
				src/rendering/wall_render.c

SRC_WORLD		=	src/world/sector_data.c \
				src/world/sector_height.c \
				src/world/sector_lighting.c \
				src/world/sector_origins.c \
				src/world/visibility_queries.c \
				src/world/visibility_tree_build.c \
				src/world/wall_segment_collision.c

SRC				=	$(SRC_APP) $(SRC_AUDIO) $(SRC_CORE) $(SRC_EDITOR) \
				$(SRC_GAMEPLAY) $(SRC_LEVEL) $(SRC_PARSING) \
				$(SRC_RENDERING) $(SRC_WORLD)

OBJ				=	$(SRC:src/%.c=build/%.o)
DEP				=	$(OBJ:%.o=%.d)

