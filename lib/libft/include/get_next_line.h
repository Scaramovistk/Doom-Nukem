/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 18:54:25 by gscarama          #+#    #+#             */
/*   Updated: 2024/06/27 18:23:45 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 512
# endif
# if BUFFER_SIZE > 125000 || BUFFER_SIZE < 1
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 512
# endif

# ifdef __linux__
#  define FDS_TO_OPEN 1024
# elif defined(__APPLE__)
#  define FDS_TO_OPEN 256
# else
#  error "Unsupported operating system"
# endif

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdbool.h>

char	*get_next_line(int fd, bool cleaner);
char	*extract_line(char *line, char *stash, int *eol_loc, int fd);
char	*init_line(char *stash, int *eol_loc);
size_t	locate_eol(char *line);

size_t	ft_strlen_gnl(const char *s);
void	ft_bezero_gnl(void *s, size_t n);
void	*ft_memcpy_gnl(void *dst, const void *src, size_t n);
char	*ft_strjoin_gnl(char *s1, char *s2, int *eol_loc);
void	ft_strlcpy_gnl(char *dst, const char *src, size_t dstsize);

#endif