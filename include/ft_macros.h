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
# define HEIGHT_STEP_MAX 8
# define MOVEMENT_SPEED 0.08
# define RUN_SPEED_MULTIPLIER 1.75
# define ROTATION_SPEED 0.03
# define PITCH_SPEED 6.0
# define MOUSE_SENSITIVITY 0.15
# define PITCH_LIMIT (WIN_HEIGHT / 3)
# define PLAYER_FLOOR_Z 0.0
# define PLAYER_STAND_HEIGHT 0.50
# define PLAYER_CROUCH_HEIGHT 0.25
# define PLAYER_JUMP_SPEED 0.10
# define PLAYER_GRAVITY 0.012
# define PLAYER_MIN_Z 0.0
# define PLAYER_MAX_Z 2.5
# define PLAYER_STEP_HEIGHT 0.35
# define FLY_VERTICAL_SPEED 0.08
# define FLY_PITCH_CLIMB_RATIO 0.85
# define COLLISION_SAFETY 0.1
# define DDA_CORNER_EPSILON 1e-9
# define RANGE_TOGGLE_DOOR 0.5
# define DOOR_ANIMATION_SPEED 0.5
# define MAP_SCALE 8
# define MAX_MAP_WIDTH 158
# define MAX_MAP_HEIGHT 99
# define LATERAL_MOVE_RATIO 0.5
# define ITEM_TYPES_NB 4
# define ENEMY_TYPES_NB 5
# define DECORATION_TYPES_NB 6
# define ELEVATOR_BUTTON_DECORATION 4
# define ELEVATOR_BUTTON_SCALE 0.38
# define ELEVATOR_BUTTON_Z 0.42
# define ELEVATOR_BUTTON_OFFSET 0.58
# define ELEVATOR_BUTTON_ICON "assets/images/textures/doom/uac_panel_tech.xpm"
# define WORLD_OBJECT_SCALE 1.0
# define WORLD_OBJECT_COLLISION_RATIO 0.35
# define ENEMY_COLLISION_RADIUS 0.30
# define ITEM_PICKUP_RADIUS 0.4
# define ITEM_HEALTH 0
# define ITEM_AMMO 1
# define ITEM_KEY 2
# define ITEM_ARTIFACT 3
# define PISTOL_MAGAZINE_SIZE 12
# define BLASTER_MAGAZINE_SIZE 18
# define STARTING_RESERVE_AMMO 24
# define SECRET_OPEN_RADIUS 1.15
# define INTERACT_RANGE 2.5
# define HAZARD_DAMAGE_PER_SEC 10.0
# define HAZARD_DAMAGE_EPSILON 0.000000001
# define HUD_MESSAGE_LEN 64
# define MESSAGE_DISPLAY_TIME 3.0
# define WORLD_EVENT_MAX 32
# define AUTHORED_ACTION_MAX 32
# define SWITCH_EVENT_DELAY 0.25
# define SWITCH_DOOR_CLOSE_DELAY 5.0
# define LEVEL_END_DELAY 3.0
# define PROJECTILE_MAX 32
# define PROJECTILE_SPEED 8.0
# define PROJECTILE_TTL 1.25
# define PROJECTILE_HIT_RADIUS 0.28
# define PROJECTILE_SIZE 6
# define WEAPON_NB 2
# define WEAPON_STATE_NB 2
# define WEAPON_FLASH_TIME 0.16
# define HUD_WEAPON_SCALE 5
# define HUD_PISTOL_IDLE "assets/images/hud/pistol_idle.xpm"
# define HUD_PISTOL_FIRE "assets/images/hud/pistol_fire.xpm"
# define HUD_BLASTER_IDLE "assets/images/hud/blaster_idle.xpm"
# define HUD_BLASTER_FIRE "assets/images/hud/blaster_fire.xpm"
# define HUD_AMMO_ICON "assets/images/hud/ammo.xpm"
# define HUD_ITEM0_ICON "assets/images/hud/item_health.xpm"
# define HUD_ITEM1_ICON "assets/images/hud/item_ammo.xpm"
# define HUD_ITEM2_ICON "assets/images/hud/item_key.xpm"
# define HUD_ITEM3_ICON "assets/images/hud/item_artifact.xpm"
# define HUD_ASSET_NB 9
# define MENU_LEVEL_DIR "tests/maps"
# define MENU_MAX_LEVELS 16
# define MENU_DIFFICULTIES 3
# define RENDER_THREADS_MAX 8
# define ENEMY_HEALTH 30
# define ENEMY_ALERT_RANGE 6.0
# define ENEMY_ATTACK_RANGE 0.75
# define ENEMY_ATTACK_DAMAGE 8
# define ENEMY_ATTACK_DELAY 1.0
# define ENEMY_MOVE_SPEED 1.2
# define ENEMY_MIN_DISTANCE 0.75
# define ENEMY_RANGED_RANGE 5.0
# define ENEMY_FIRE_DELAY 1.6
# define ENEMY_PROJECTILE_DAMAGE 6
# define ELEVATOR_RISE 1.0
# define ELEVATOR_DURATION 2.0
# define SOUND_DIR "assets/sounds/"
# define MUSIC_BASENAME "music"
# define SFX_CHANNELS_NB 4
# define SECTOR_MAX 36
# define SEGMENT_WALL_MAX 128
# define SEGMENT_EPSILON 0.05
# define DNK_MAGIC "DNK1"
# define DNK_MAX_LINES 2048
# define DNK_MAX_ASSETS 64
# define DNK_HEX_LINE 96
# define DNK_DEFAULT_LIGHT 255
# define DNK_UNPACK_ROOT "build/unpacked"

#endif
