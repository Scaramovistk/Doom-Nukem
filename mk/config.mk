# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    config.mk                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#              #
#    Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr        #
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

