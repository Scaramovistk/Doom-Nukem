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

# Source and Object Files
VPATH			=	src src/events src/graphics src/parser src/parser/utils src/utils

# Source Files
SRC_MAIN		=	main.c
SRC_EVENTS		=	door_bonus.c enemies.c events.c game_loop.c hooks.c hooks_door_bonus.c interact.c level_flow.c menu.c message.c \
					mlx_mouse_pos_bonus.c pickup.c projectile.c stop_game.c triggers.c update_player_pos.c
SRC_GRAPHICS	=	dda.c dda_utils.c draw_door_bonus.c draw_scene.c draw_sprites.c draw_utils.c draw_wall.c graphics_utils.c \
					hud.c load_game.c minimap.c ray_casting.c render.c time_bonus.c
SRC_PARSER		=	generate_map.c get_header.c get_map.c parser.c valid_header.c valid_map.c \
					parsing_error.c header.c map.c security.c valid_header_check_bonus.c
SRC_UTILS		=	allocation.c error.c free.c init_game.c sound.c

SRC				=	$(SRC_MAIN) $(SRC_EVENTS) $(SRC_GRAPHICS) $(SRC_PARSER) $(SRC_UTILS)

# Source and Object Files
OBJ				=	$(SRC:%.c=build/%.o)
DEP				=	$(OBJ:%.o=%.d)
OBJ_DIR_NAME	=	build

# Rules
all:			$(NAME)

$(NAME):		$(LIBFT) $(MLX_FILE) $(OBJ_DIR_NAME) $(OBJ)
				$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

$(LIBFT):
				$(MAKE) --no-print-directory -C $(LIBFT_DIR)

$(MLX_FILE):
				@$(MAKE) --no-print-directory -sC $(MLX_DIR)

$(OBJ_DIR_NAME):
				@mkdir -p $(OBJ_DIR_NAME)

build/%.o:		%.c
				$(CC) $(CFLAGS) -c $< -o $@

clean:
				@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean
				@$(MAKE) --no-print-directory -sC $(MLX_DIR) clean
				$(RM) build

fclean:			clean
				$(RM) $(NAME)
				@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re:				fclean all

-include $(DEP)

.PHONY:			all clean fclean re
