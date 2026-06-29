/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_libraries.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:58:14 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 21:58:16 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LIBRARIES_H
# define FT_LIBRARIES_H

// Standard libraries
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>

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