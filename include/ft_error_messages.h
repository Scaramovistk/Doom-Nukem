/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_messages.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 22:01:47 by rperez-t          #+#    #+#             */
/*   Updated: 2024/07/16 22:01:50 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ERROR_MESSAGES_H
# define FT_ERROR_MESSAGES_H

# define OPENF "Unable to open file."
# define NOMAP "Map is missing from file."
# define CORRUPTMAP "Map corruption."
# define PROBLEMW "Width can not be extracted from file."
# define PROBLEME "Map can not be extracted from file."
# define VALIDMAP "Invalid map configuration."
# define MAPMALLOC "Unable to allocate memory for **map."
# define MAPLINEMALLOC "Unable to allocate memory for *map."
# define MAPCHAR "Invalid char present in map."
# define MAPPLAYER "Invalid number of players."
# define MAPCLOSED "Map is not closed."
# define HALWAY "A door has not been placed in a halway."
# define PATHS "A texture from NO || EA || SO || WE || F || C || D isn't set."
# define SXPM "Source for .xpm is to large"
# define XPMFILE "A texture from NO || EA || SO || WE || D is not a .xpm."
# define NOXPMS "A texture from NO || EA || SO || WE || D does not exist."
# define DIRXPMS "A texture from NO || EA || SO || WE || D is a directory."
# define CUBFILE "The given argv is not a .cub file."
# define DIRFILE "The given argv is a directory."
# define NOFILE "The given argv could not be opened."
# define ARGSP "Wrong amount of arguments."
# define HEADERP "Wrong header info."
# define MAPP "Wrong map format."
# define PARSINGP "Parsing unsuccessful."
# define RGBF "Not a valid RGB format. Needs to be R,G,B."
# define RGBS "A texture from F || C does not have rgb values in range [0,255]."
# define EXCESSW "The width given for the map is to big to be displayed."
# define EXCESSH "The height given for the map is to big to be displayed."

#endif