/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_story_text.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/08/10 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

const char	*briefing_text(int level, int line)
{
	const char *const	text[5][4] = {
	{"UAC PHOBOS BASE HAS GONE SILENT.",
		"DEMONIC SIGNALS FLOOD THE MOON.",
		"FIND THE EXIT AND REACH THE HANGAR.", ""},
	{"THE HANGAR LEADS TO THE DEIMOS GATE.",
		"THE INVASION IS SPREADING.",
		"PUSH THROUGH THE INFESTED LABS.", ""},
	{"THE GATE HAS PULLED YOU INTO HELL.",
		"DEIMOS HANGS ABOVE A RIVER OF FIRE.",
		"FIND A WAY BACK TO THE SURFACE.", ""},
	{"YOU HAVE REACHED THE HEART OF DEIMOS.",
		"THE DEMONS GUARD A STOLEN UAC TELEPORTER.",
		"SEIZE IT BEFORE THE BREACH WIDENS.", ""},
	{"THE FINAL CHAMBER SHAKES WITH RAGE.",
		"ONE LAST ASSAULT STANDS BETWEEN YOU AND HOME.",
		"END THE INVASION.", ""}};

	return (text[level][line]);
}

const char	*debriefing_text(int level, int line)
{
	const char *const	text[5][4] = {
	{"PHOBOS IS LOST.", "A STRONGER SIGNAL COMES FROM DEIMOS.",
		"THE HANGAR GATE IS YOUR ONLY WAY FORWARD.", ""},
	{"THE DEIMOS GATE IS OPEN.",
		"THE DEMONS HAVE DRAGGED THE MOON INTO HELL.",
		"FOLLOW THEM AND SHUT THE BREACH.", ""},
	{"YOU ESCAPED THE FIRST HELLISH SECTOR.",
		"THE DEMONIC FORTRESS STILL RISES AHEAD.", "KEEP MOVING.", ""},
	{"THE TELEPORTER IS IN YOUR HANDS.",
		"ONE FINAL FORTRESS BLOCKS THE WAY HOME.",
		"MAKE THE LAST PUSH.", ""},
	{"THE INVASION IS BROKEN.",
		"EARTH WILL LIVE TO FIGHT ANOTHER DAY.",
		"YOU ARE THE LAST MARINE STANDING.", ""}};

	return (text[level][line]);
}
