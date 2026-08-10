/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound_backend.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/07 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

#ifdef AUDIO_SDL2

bool	audio_backend_init(void)
{
	return (SDL_Init(SDL_INIT_AUDIO) >= 0);
}

void	audio_backend_stop(void)
{
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

#else

bool	audio_backend_init(void)
{
	return (true);
}

void	audio_backend_stop(void)
{
}

#endif
