# Doom-Nukem

Single-executable Doom-Nukem raycaster built from the cub3D base.

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

Switch interactions enqueue timed world events. The current sequence shows a
message, adds score, toggles doors after a short delay, then closes doors again
after a timer.

## Text Overlay

Timed overlay messages are drawn over the frame for pickups, story/message
zones, and scripted events.

## Projectiles

The player can fire physical projectiles that consume HUD ammo, travel forward,
stop on solid walls/closed doors, leave wall decals, and hit non-item sprite
targets. `Q` switches between pistol and blaster behavior.

## Enemies

Map `3` sprites act as simple enemies. They alert when the player is nearby,
chase through legal floor cells, deal timed contact damage, take projectile
damage, and award score when defeated.

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
files. Missing files are skipped without interrupting gameplay.
