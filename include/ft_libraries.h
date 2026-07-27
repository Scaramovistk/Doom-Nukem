/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_libraries.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:58:14 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/24 10:44:02 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LIBRARIES_H
# define FT_LIBRARIES_H

// Standard libraries
# include <fcntl.h>
# include <dirent.h>
# include <limits.h>
# include <math.h>
# include <pthread.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# if defined(__has_include)
#  if __has_include(<SDL2/SDL.h>)
#   include <SDL2/SDL.h>
#  elif __has_include(<SDL.h>)
#   include <SDL.h>
#  else
#   error "SDL2 header not found"
#  endif
# else
#  include <SDL2/SDL.h>
# endif

// Mlx
# ifdef __linux__
#  include "../lib/Minilbx_linux/mlx.h"
# elif defined(__APPLE__)
#  include "../lib/Minilbx_mac/mlx.h"
# else
#  error "Unsupported operating system"
# endif

// Personal libraries
# include "../lib/libft/include/libft.h"

#endif
