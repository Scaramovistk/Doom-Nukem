# Doom-Nukem

Single-executable Doom-Nukem raycaster built from the cub3D base.

## Dependencies

- MinilibX (bundled under `lib/`)
- SDL2 or ALSA (sound and music playback). SDL2 is preferred when available;
  on Linux the Makefile automatically falls back to native ALSA
  (`libasound2-dev`) when SDL2 is not installed.

MiniLibX uses AppKit/OpenGL on macOS and X11 on Linux. The game itself always
renders ray-cast pixels in software; it does not use OpenGL as a 3D renderer,
which keeps it within the subject's no-hardware-acceleration requirement.

## Build

```sh
make
```

This produces:

```text
./doom-nukem
```

## Run

```sh
./doom-nukem tests/maps/simple_map.cub
```

Packed, self-contained `.dnk` levels are also supported:

```sh
./doom-nukem tests/maps/door_map.dnk
```

Or open the level select menu:

```sh
./doom-nukem
```

## Editor / Packing

Create or refresh a self-contained packed level:

```sh
./doom-nukem --edit tests/maps/door_map.cub tests/maps/door_map.dnk
```

Validate a level without opening a window:

```sh
./doom-nukem --check tests/maps/door_map.dnk
```

The packed format embeds XPM textures and sound assets, carries sector height,
slope, and lighting data, and can define angled wall segments. See `FORMAT.md`.

## Controls

- `W` / `S`: move forward and backward
- `A` / `D`: strafe
- Arrow left/right: rotate
- Mouse: look around
- Page Up / Page Down: keyboard pitch fallback
- Shift: run
- Ctrl: crouch, or descend while flying/swimming
- Space: jump, or ascend while flying/swimming
- `F`: toggle fly mode
- `E`: interact with doors
- Left mouse or `R`: fire projectile
- `Q`: switch weapon
- `1` / `2` / `3` / `4`: select carried artifact slot
- Esc or window close button: quit cleanly

Campaign maps use one `V` vending-machine tile. Interact with it to spend
5 score points on 10 ammo. Its texture is configured with the map's `VM`
header and its replaceable sound is `assets/sounds/vending.wav`.

## Menu

Launching without a map opens the level select menu. Up/Down chooses a `.cub`
or `.dnk` level from `tests/maps`, Left/Right changes difficulty, and Enter
starts the selected level.

## HUD

The frame buffer HUD renders health, ammo, inventory slots, and score/currency
as separate overlay elements. The minimap is shown in the top-left, score is
centered at the top, FPS is shown in the top-right, and the view includes a
crosshair plus a bottom-center XPM weapon sprite. Ammo and inventory slots use
the same HUD icon asset set as pickup items.

## Scripted Events

Switch interactions enqueue timed world events. Map `T` is the default switch:
it shows a message, adds score, toggles doors after a short delay, then closes
doors again after a timer. Two additional switch types target a single
specific device instead of every door on the map:

- `L` — elevator switch. Animates the floor of the sector beneath it between
  its resting height and a raised height over `ELEVATOR_DURATION` seconds,
  clamped so it can never rise into the ceiling. Triggering it again lowers it
  back down.
- `P` — secret-passage switch. Opens the single nearest `2` door tile after a
  short delay, leaving every other door on the map untouched.

## Text Overlay

Timed overlay messages are drawn over the frame for pickups, story/message
zones, and scripted events.

## Projectiles

The player can fire physical projectiles that consume HUD ammo, travel forward,
stop on solid walls/closed doors, leave wall decals, and hit non-item sprite
targets. `Q` switches between pistol and blaster behavior. Enemy-fired
projectiles use the same travel/collision code but can only damage the player,
never other sprites, and player-fired projectiles can never hit the player.

## Enemies

Map `3` sprites are melee enemies: they alert when the player is nearby, chase
through legal floor cells, deal timed contact damage, take projectile damage,
and award score when defeated. Map `K` sprites are ranged enemies: instead of
contact damage they fire a projectile at the player on a cooldown once in
range, and fall back to melee-style chasing outside that range.

## Rendering

The wall/ray column pass is split into pthread bands based on available CPU
cores, then joined before sprites, projectiles, HUD, and the final blit.
Packed levels can add sector floor and ceiling heights, slopes, room lighting,
and arbitrary angled wall segments on top of the grid fallback.

## Level Flow

Levels can use `X` in the map as an exit tile. The current mission flow starts
with an objective message, requires active pickup items to be collected, then
completes when the player reaches an exit. Dropping health to zero fails the
mission after a short message.

Capture-the-flag maps use `G` for the flag. Their only objective is to take it
and return to the player start tile (the base); no enemy kills, pickups, or
exit tile are required. The menu's **Capture the Flag (3 maps)** mode starts
`ctf_1.dnk` and chains through `ctf_2.dnk` and `ctf_3.dnk`. On its minimap,
the base is blue and the flag is yellow.

To author another CTF map, use exactly one player start (`N`, `S`, `E`, or
`W`), place one `G`, and optionally add `NEXT path/to/next-level.dnk` to
chain it to another map. The player start is automatically the return base.

## Laptop Tables

Laptop tables are decorative, non-interactive `B` map tiles. Add this header
line to use the supplied texture:

```text
LT tests/textures_doom/laptop_table.xpm
```

When the player enters a laptop's proximity radius, it plays a one-shot sound.
It does not repeat while the player remains nearby; leaving the radius and
returning triggers it again. Laptop tables sit on the ground and are hidden
from the minimap.

## Audio

Sound effects and looping music are loaded from the active level's unpacked
sound directory for `.dnk` files, or from `assets/sounds/` for classic `.cub`
files. Missing files are skipped without interrupting gameplay. Playback uses
SDL2's audio queueing API: a dedicated device loops the music buffer, and a
small fixed pool of channels lets sound effects overlap.
