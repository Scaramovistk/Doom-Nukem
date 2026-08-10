# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/17 11:38:26 by gscarama          #+#    #+#              #
#    Updated: 2024/07/17 12:12:40 by rperez-t         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror -O3 -MMD -MP
RM				=	rm -rf
NAME			=	doom-nukem

# Libraries

# libft
LIBFT_DIR		=	lib/libft
LIBFT			=	$(LIBFT_DIR)/libft.a

# minilibx
OS				:=	$(shell uname)
ifeq ($(OS),Darwin)
    MLX_DIR = lib/Minilbx_mac/
    MLX_LNK = -framework OpenGL -framework AppKit
else ifeq ($(OS),Linux)
    MLX_DIR = lib/Minilbx_linux/
    MLX_LNK = -lXext -lX11 -lm
else
    $(error Unsupported OS: $(OS))
endif
MLX_LIB			=	$(addprefix $(MLX_DIR),libmlx.a)
MLX				=	libmlx.a
MLX_FILE		=	$(MLX_DIR)$(MLX)

# Include directories
INC_DIRS		=	include src $(LIBFT_DIR)/include $(MLX_DIR)
CFLAGS			+=	$(addprefix -I, $(INC_DIRS))
LDFLAGS			=	-L$(LIBFT_DIR) -L$(MLX_DIR) -lft -lmlx $(MLX_LNK) -pthread

# Audio backend.  SDL2 is preferred when present; Linux can use its native
# ALSA API instead, so a missing optional SDL2 installation does not prevent
# the game from building or remove the mandatory sound support.
SDL_CONFIG		:=	$(shell command -v sdl2-config 2>/dev/null)
ifneq ($(SDL_CONFIG),)
    CFLAGS += $(shell $(SDL_CONFIG) --cflags) -DAUDIO_SDL2
    LDFLAGS += $(shell $(SDL_CONFIG) --libs)
else ifeq ($(shell pkg-config --exists sdl2 2>/dev/null && echo yes),yes)
    CFLAGS += $(shell pkg-config --cflags sdl2) -DAUDIO_SDL2
    LDFLAGS += $(shell pkg-config --libs sdl2)
else ifeq ($(OS),Linux)
    ifeq ($(shell pkg-config --exists alsa 2>/dev/null && echo yes),yes)
        CFLAGS += $(shell pkg-config --cflags alsa) -DAUDIO_ALSA
        LDFLAGS += $(shell pkg-config --libs alsa)
    else
        $(error No audio development library found (install SDL2 or ALSA))
    endif
else
    $(error SDL2 is required for audio on $(OS))
endif

# Source and Object Files
VPATH			=	src src/events src/graphics src/parser src/parser/utils src/utils

# Source Files
SRC_MAIN		=	main.c
SRC_EVENTS		=	authored_actions.c authored_actions_2.c authored_actions_3.c authored_actions_4.c authored_parse_helpers.c authored_target_helpers.c display_control.c door_bonus.c editor.c editor_border.c editor_check.c editor_click.c editor_commands.c editor_gui.c editor_gui_draw.c editor_gui_input.c editor_gui_labels.c editor_gui_setup.c editor_gui_tools.c editor_keys.c editor_session.c editor_session_2.c editor_session_3.c editor_session_4.c editor_session_5.c editor_session_6.c editor_session_7.c editor_sector_helpers.c editor_dispatch.c editor_texture.c editor_texture_command.c enemies.c enemy_ranged.c enemy_sprite.c enemy_update.c event_queue.c events.c game_loop.c hooks.c hooks_door_bonus.c hooks_items.c hooks_press.c interact.c item_effects.c item_use.c laptop.c level_end.c level_flow.c level_loader.c menu.c menu_render.c menu_scan.c message.c \
					mlx_mouse_linux.c mlx_mouse_mac.c object_collision.c pickup.c pickup_sprite.c player_collision.c player_gravity.c player_vertical.c projectile.c projectile_fire.c projectile_hit.c projectile_sprite.c projectile_targets.c projectile_update.c sector_events.c stop_game.c switch_sequence.c switch_targets.c triggers.c update_player_pos.c window_mode.c
SRC_GRAPHICS	=	dda.c dda_utils.c draw_alpha.c draw_door_bonus.c draw_rays.c draw_risers.c draw_scene.c draw_sprites.c draw_sprites_2.c draw_sprites_3.c draw_sprites_4.c draw_sprite_lookup.c draw_sprite_entities.c draw_sprite_helpers.c draw_step_bands.c draw_utils.c draw_wall.c draw_wall_slice.c floor_cast.c floor_rows.c graphics_utils.c \
					height_occlusion.c height_step.c hud.c hud_2.c hud_3.c hud_4.c hud_5.c hud_story_text.c load_game.c load_surface.c load_textures.c minimap.c minimap_base.c minimap_markers.c minimap_sprites.c minimap_utils.c ray_casting.c render.c render_threads.c time_bonus.c
SRC_PARSER		=	generate_map.c generate_map_2.c generate_map_3.c generate_map_4.c generate_map_5.c generate_map_6.c generate_map_7.c generate_map_utils.c generate_entity_cells.c generate_sprite_helpers.c generate_enemy_helpers.c generate_grid.c generate_assets.c get_header.c get_map.c header_assets.c header_setup.c header_values.c map_border.c packed_level.c packed_level_2.c packed_level_3.c packed_level_4.c packed_level_5.c packed_level_6.c packed_level_7.c packed_level_8.c packed_level_9.c packed_read_helpers.c packed_write_helpers.c packed_asset_helpers.c parser.c parser_rgb.c valid_header.c valid_map.c \
					parsing_error.c header.c map.c map_access.c security.c valid_header_check.c valid_header_check_bonus.c valid_header_optional.c segment_check.c
SRC_UTILS		=	allocation.c bsp_build.c bsp_visibility.c cleanup_level.c error.c free.c free_textures.c init_assets.c init_game.c init_map.c init_player.c init_runtime.c sectors.c sector_height.c sector_light.c sector_origin.c segment_collision.c sound.c sound_backend.c sound_utils.c sound_sdl.c sound_alsa.c sound_alsa_file.c sound_alsa_parse.c sound_alsa_thread.c

SRC				=	$(SRC_MAIN) $(SRC_EVENTS) $(SRC_GRAPHICS) $(SRC_PARSER) $(SRC_UTILS)

# Source and Object Files
OBJ				=	$(SRC:%.c=build/%.o)
DEP				=	$(OBJ:%.o=%.d)
OBJ_DIR_NAME	=	build

# Rules
all:			$(NAME)

$(NAME):		$(LIBFT) $(MLX_FILE) $(OBJ)
				$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

$(LIBFT):
				$(MAKE) --no-print-directory -C $(LIBFT_DIR)

$(MLX_FILE):
				@$(MAKE) --no-print-directory -sC $(MLX_DIR)

$(OBJ_DIR_NAME):
				@mkdir -p $(OBJ_DIR_NAME)

build/%.o:		%.c | $(OBJ_DIR_NAME)
				$(CC) $(CFLAGS) -c $< -o $@

clean:
				@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean
				@$(MAKE) --no-print-directory -sC $(MLX_DIR) clean
				$(RM) build

fclean:			clean
				$(RM) $(NAME)
				$(RM) ./tests/maps/
				@mkdir -p ./tests/maps/
				@touch ./tests/maps/hold.txt
				@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re:				fclean all

maps:			$(NAME)
				@set -e; for map in tests/maps_src/*.cub; do \
					output="tests/maps/$$(basename "$$map" .cub).dnk"; \
					./$(NAME) --pack "$$map" "$$output"; \
				done

-include $(DEP)

.PHONY:			all clean fclean re maps
