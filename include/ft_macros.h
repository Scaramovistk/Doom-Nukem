/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_macros.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:58:02 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:58:05 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MACROS_H
# define FT_MACROS_H

# include "ft_colors.h"
# include "ft_error_messages.h"
# include "ft_keybindings.h"
# ifdef __linux__
#  define DIRREAD __O_DIRECTORY
#  define M_PI 3.14159265358979323846
#  define M_PI_2 1.57079632679489661923
#  define M_PI_4 0.78539816339744830962
#  define M_1_PI 0.31830988618379067154
#  define M_2_PI 0.63661977236758134308
#  define M_2_SQRTPI 1.12837916709551257390
#  define M_SQRT2 1.41421356237309504880
#  define M_SQRT1_2 0.70710678118654752440
# elif defined(__APPLE__)
#  define DIRREAD O_DIRECTORY
# else
#  error "Unsupported operating system"
# endif

# define WIN_HEIGHT 800
# define WIN_WIDTH 1280
# define LINE_SIZE 300
# define TEXTURE_SIZE 64
# define PADDING 10
# define FOV 60
# define TEXTURES_NB 8
# define SPRITE_FRAME_NB 8
# define TRANSPARENT_HIT_MAX 8
# define MOVEMENT_SPEED 0.08
# define ROTATION_SPEED 0.03
# define PITCH_SPEED 6.0
# define PITCH_LIMIT (WIN_HEIGHT / 3)
# define COLLISION_SAFETY 0.1
# define RANGE_TOGGLE_DOOR 0.5
# define DOOR_ANIMATION_SPEED 0.5
# define MAP_SCALE 8
# define MAX_MAP_WIDTH 158
# define MAX_MAP_HEIGHT 99
# define LATERAL_MOVE_RATIO 0.5

# ifndef MODE_BONUS
#  define MODE_BONUS 0
# endif

#endif
