/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/07/08 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static const char	*audio_player(void)
{
#ifdef __APPLE__
	return ("afplay");
#else
	return ("ffplay");
#endif
}

static bool	file_exists(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	close(fd);
	return (true);
}

static bool	sound_path(char *dst, size_t size, const char *dir,
	const char *name)
{
	static const char	*exts[3] = {".wav", ".ogg", ".mp3"};
	int					i;

	i = 0;
	while (i < 3)
	{
		ft_strlcpy(dst, dir, size);
		ft_strlcat(dst, name, size);
		ft_strlcat(dst, exts[i], size);
		if (file_exists(dst))
			return (true);
		i++;
	}
	dst[0] = '\0';
	return (false);
}

static void	exec_player(const char *path)
{
#ifdef __APPLE__
	execlp(audio_player(), audio_player(), path, (char *)NULL);
#else
	execlp(audio_player(), audio_player(), "-nodisp", "-autoexit", "-loglevel",
		"quiet", path, (char *)NULL);
#endif
	exit(EXIT_FAILURE);
}

void	update_audio(void)
{
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
}

void	play_sound_effect(t_game *g, const char *name)
{
	char	path[LINE_SIZE];
	pid_t	pid;

	if (!sound_path(path, sizeof(path), g->audio.sound_dir, name))
		return ;
	pid = fork();
	if (pid == 0)
		exec_player(path);
}

void	init_audio(t_game *g)
{
	g->audio.enabled = true;
	g->audio.music_pid = -1;
	if (!g->audio.sound_dir[0])
		ft_strlcpy(g->audio.sound_dir, SOUND_DIR, LINE_SIZE);
	sound_path(g->audio.music_path, sizeof(g->audio.music_path),
		g->audio.sound_dir, MUSIC_BASENAME);
}

static void	music_loop(const char *path)
{
	pid_t	player;

	setpgid(0, 0);
	while (true)
	{
		player = fork();
		if (player == 0)
			exec_player(path);
		if (player < 0)
			exit(EXIT_FAILURE);
		waitpid(player, NULL, 0);
	}
}

void	start_background_music(t_game *g)
{
	if (!g->audio.enabled || !g->audio.music_path[0] || g->audio.music_pid > 0)
		return ;
	g->audio.music_pid = fork();
	if (g->audio.music_pid == 0)
		music_loop(g->audio.music_path);
}

void	stop_audio(t_game *g)
{
	if (!g || g->audio.music_pid <= 0)
		return ;
	kill(-g->audio.music_pid, SIGTERM);
	waitpid(g->audio.music_pid, NULL, 0);
	g->audio.music_pid = -1;
	update_audio();
}
