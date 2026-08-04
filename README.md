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

Open an editable `.cub` project in the interactive terminal editor:

```sh
./doom-nukem --edit tests/maps_src/door_map.cub tests/maps/door_map.dnk
```

The editor can place map devices/entities, change texture headers, define floor
and ceiling heights, slopes and lighting, assign sector cells, add arbitrary
wall segments, save the `.cub`/`.sectors` sources, validate them, and pack the
result. Type `help` in the editor for the complete command list.

For a non-interactive rebuild, use:

```sh
./doom-nukem --pack tests/maps_src/door_map.cub tests/maps/door_map.dnk
```

To create a starter packed level, use `./doom-nukem --edit output.dnk`.

Validate a level without opening a window:

```sh
./doom-nukem --check tests/maps/door_map.dnk
```

The packed format embeds XPM textures and sound assets, carries sector height,
slope, and lighting data, and can define angled wall segments. See `FORMAT.md`.

## Controls

- `W` / `S` or Arrow Up / Down: move forward and backward
- `A` / `D`: strafe
- Arrow left/right: rotate
- Mouse: look around
- Page Up / Page Down: keyboard pitch fallback
- Shift: run
- Ctrl: crouch, or descend while flying/swimming
- Space: jump, or ascend while flying/swimming
- `F`: engage/disengage the jetpack after collecting the artifact pickup
- `E`: interact with doors, switches, elevator panels, and keyed doors
- Left mouse: fire projectile
- `R`: reload the selected weapon from reserve ammo
- `Q`: switch weapon
- `1` / `2` / `3` / `4`: select carried artifact slot
- Enter: use the selected inventory slot (`2` reloads, `4` toggles jetpack)
- Esc or window close button: quit cleanly

## Menu

Launching without a map opens the level select menu. Up/Down chooses a `.cub`
or `.dnk` level from `tests/maps`, Left/Right changes difficulty, and Enter
starts the selected level.

## HUD

The frame buffer HUD renders health, the selected weapon's magazine, inventory
slots, and score/currency
as separate overlay elements. The minimap is shown in the top-left, score is
centered at the top, FPS is shown in the top-right, and the view includes a
crosshair plus a bottom-center XPM weapon sprite. Ammo and inventory slots use
the same HUD icon asset set as pickup items.

## Scripted Events

Switch interactions enqueue timed world events. Map `T` is the default switch:
it shows a message, adds score, toggles doors after a short delay, then closes
doors again after a timer. Two additional switch types target a single
specific device instead of every door on the map:

- `L` — solid wall device with a dedicated, hand-height button sprite. The
  sector assigned to its map cell is the lift target. Press `E` at the button
  to raise/lower that sector over `ELEVATOR_DURATION`; a grounded player on the
  platform rides it smoothly. The button sprite is separate from damage decals.
- `P` — disguised secret door. It uses the surrounding wall texture, is drawn
  as an ordinary wall on the minimap, and opens automatically when approached.
  It does not require a key or the interact key.
- `B` — locked door. Collect an `8` key pickup and press `E` at the door to
  consume one key, permanently unlock it, and begin the normal door animation.
  `door_map.dnk` demonstrates the complete pickup-to-unlock path.

## Reloading and Flight

Ammo pickups go into inventory slot 2 as reserve ammunition rather than
directly filling the gun. Each weapon has its own magazine. Select slot 2 and
press Enter to reload, or use `R` as the shortcut. Empty-magazine feedback
points the player back to the ammo inventory.

The slot-4 artifact is a jetpack. Once collected, select it and press Enter (or
press `F`) to engage flight. Space/Ctrl ascend and descend, and looking up or
down while moving adds smooth pitch-directed climb. Flight respects floors,
ceilings, walls, and raised ledges.

`tests/maps/flight_ops.dnk` is the hand-in showcase mission for all four
features: hidden passage, inventory reload, wall-panel elevator, and a required
jetpack crossing over a deep shaft.

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

## Audio

Sound effects and looping music are loaded from the active level's unpacked
sound directory for `.dnk` files, or from `assets/sounds/` for classic `.cub`
files. Missing files are skipped without interrupting gameplay. Playback uses
SDL2's audio queueing API: a dedicated device loops the music buffer, and a
small fixed pool of channels lets sound effects overlap.
