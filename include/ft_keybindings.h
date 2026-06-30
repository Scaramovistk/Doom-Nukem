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
# else
#  error "Unsupported operating system"
# endif
#endif
