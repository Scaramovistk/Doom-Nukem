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
- Esc or window close button: quit cleanly

## HUD

The frame buffer HUD renders health, ammo, inventory slots, and score/currency
as separate overlay elements. The minimap is shown in the top-left, score is
centered at the top, and FPS is shown in the top-right.

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
targets.

## Level Flow

Levels can use `X` in the map as an exit tile. The current mission flow starts
with an objective message, requires active pickup items to be collected, then
completes when the player reaches an exit. Dropping health to zero fails the
mission after a short message.

## Audio

Sound effects and looping music are loaded from `assets/sounds/` when matching
`.wav`, `.ogg`, or `.mp3` files exist. Missing files are skipped without
interrupting gameplay.
