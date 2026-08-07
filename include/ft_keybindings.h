/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_keybindings.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:59:57 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 22:00:01 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_KEYBINDINGS_H
# define FT_KEYBINDINGS_H

// Same as X11 defines
# define EVENT_CLOSE_BTN 17

# define CLIENT_MESSAGE 17
# define STRUCTURE_NOTIFY_MASK 10001

# define KEY_PRESS 2
# define KEY_PRESS_MASK 1

# define KEY_RELEASE 3
# define KEY_RELEASE_MASK 10

# define MOUSE_MOVE 6
# define MOUSE_MOVE_MASK 64
# define MOUSE_PRESS 4
# define MOUSE_PRESS_MASK 4
# define MOUSE_LEFT 1

# ifdef __linux__
#  define KEY_ESC 65307
#  define KEY_W 119
#  define KEY_A 97
#  define KEY_S 115
#  define KEY_D 100
#  define KEY_UP 65362
#  define KEY_DOWN 65364
#  define KEY_LEFT 65361
#  define KEY_RIGHT 65363
#  define KEY_PAGE_UP 65365
#  define KEY_PAGE_DOWN 65366
#  define KEY_SPACE 32
#  define KEY_ENTER 65293
#  define KEY_E 101
#  define KEY_F 102
#  define KEY_Q 113
#  define KEY_R 114
#  define KEY_1 49
#  define KEY_2 50
#  define KEY_3 51
#  define KEY_4 52
#  define KEY_0 48
#  define KEY_5 53
#  define KEY_6 54
#  define KEY_7 55
#  define KEY_C 99
#  define KEY_K 107
#  define KEY_N 110
#  define KEY_P 112
#  define KEY_V 118
#  define KEY_X 120
#  define KEY_Z 122
#  define KEY_G 103
#  define KEY_H 104
#  define KEY_L 108
#  define KEY_T 116
#  define KEY_LEFT_BRACKET 91
#  define KEY_RIGHT_BRACKET 93
#  define KEY_F11 65480
#  define KEY_SHIFT 65505
#  define KEY_CTRL 65507
# elif defined(__APPLE__)
#  define KEY_ESC 53
#  define KEY_W 13
#  define KEY_A 0
#  define KEY_S 1
#  define KEY_D 2
#  define KEY_UP 126
#  define KEY_DOWN 125
#  define KEY_LEFT 123
#  define KEY_RIGHT 124
#  define KEY_PAGE_UP 116
#  define KEY_PAGE_DOWN 121
#  define KEY_SPACE 49
#  define KEY_ENTER 36
#  define KEY_E 14
#  define KEY_F 3
#  define KEY_Q 12
#  define KEY_R 15
#  define KEY_1 18
#  define KEY_2 19
#  define KEY_3 20
#  define KEY_4 21
#  define KEY_0 29
#  define KEY_5 23
#  define KEY_6 22
#  define KEY_7 26
#  define KEY_C 8
#  define KEY_K 40
#  define KEY_N 45
#  define KEY_P 35
#  define KEY_V 9
#  define KEY_X 7
#  define KEY_Z 6
#  define KEY_G 5
#  define KEY_H 4
#  define KEY_L 37
#  define KEY_T 17
#  define KEY_LEFT_BRACKET 33
#  define KEY_RIGHT_BRACKET 30
#  define KEY_F11 103
#  define KEY_SHIFT 257
#  define KEY_CTRL 256
# else
#  error "Unsupported operating system"
# endif
#endif
