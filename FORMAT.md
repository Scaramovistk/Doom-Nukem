# Doom-Nukem Packed Level Format

`.dnk` levels are text files with embedded assets. The game unpacks them into
`build/unpacked_<pid>/`, generates a temporary `.cub`, then applies Doom-Nukem
sector metadata.

## Commands

```sh
./doom-nukem --edit tests/maps_src/door_map.cub tests/maps/door_map.dnk
./doom-nukem --pack tests/maps_src/door_map.cub tests/maps/door_map.dnk
./doom-nukem --check tests/maps/door_map.dnk
./doom-nukem tests/maps/door_map.dnk
```

`--edit` opens an interactive terminal editor for the `.cub` and its optional
same-basename `.sectors` file. Its commands edit map tokens, texture/header
values, sector heights/slopes/light, sector-grid assignments, and arbitrary
wall segments; `save`, `validate`, and `pack [output.dnk]` finish the workflow.
Type `help` in the editor for exact syntax. `--pack` is the non-interactive
export command. `--check` parses a level without opening a window and prints
map, sector, angled-wall, sprite, and item counts.

`./doom-nukem --edit output.dnk` creates a starter packed level. Source editing
is done with `--edit source.cub [output.dnk]` so that the `.cub` and `.sectors`
files remain the editable project and the `.dnk` remains the self-contained
deliverable.

## Directory convention

`tests/maps_src/` holds editable `.cub` sources. `tests/maps/` holds only the
packed `.dnk` builds — this is what the in-game menu scans (`MENU_LEVEL_DIR`),
so keeping `.cub` files out of it avoids duplicate menu entries for the same
level. After editing a `.cub` source, re-run `--pack` to refresh its `.dnk` in
`tests/maps/`.

For authored heights and devices, place a same-basename `.sectors` file beside
the source (for example `flight_ops.cub` + `flight_ops.sectors`). It contains
the `SECTOR`, optional `WALL`, and `GRID` lines without section wrappers. The
packer embeds it as `BEGIN_SECTORS`; if absent, it generates the legacy default
two-sector grid.

## Layout

```text
DNK1
ASSET NO xpm
<hex encoded file bytes>
END_ASSET

BEGIN_CUB
NO @NO
SO @SO
WE @WE
EA @EA
F @F
C @C
111
1N1
111
END_CUB

BEGIN_SECTORS
SECTOR 0 0.00 1.00 0.00 0.00 255
SECTOR 1 0.20 1.20 0.00 0.00 190
WALL 2.5 2.5 5.5 4.5 0 1 0
GRID
000
010
000
END_SECTORS
```

## Sections

`ASSET <key> <ext>` embeds one file as hexadecimal bytes. Any `@key` reference
inside `BEGIN_CUB` is replaced with the unpacked path. Sound assets use keys
like `sound_shoot`, `sound_door`, and `sound_music`.

HUD assets are embedded automatically by `--pack`/`--edit` under fixed keys:
`hud_pistol_idle`, `hud_pistol_fire`, `hud_blaster_idle`, `hud_blaster_fire`,
`hud_ammo`, and `hud_item0` through `hud_item3` (health/ammo/key/artifact
pickup icons). They unpack to `hud/` inside the level's temp directory and
override the game's built-in HUD sprites, making a packed `.dnk` fully
self-sufficient. Export also embeds sound/music and the default elevator-button
sprite. Missing source assets make packing fail and remove the incomplete
output. Packed levels never fall back to repository texture, HUD, or audio
paths; legacy packs containing direct `.xpm` paths are rejected.

`BEGIN_CUB` contains normal cub3D-compatible header and map data, except texture
paths can be asset references.

`SECTOR id floor_z ceil_z slope_x slope_y light` defines a room/area. Light is
0-255. Slopes are applied continuously across every cell assigned to that
sector, anchored to the sector's bounding-box origin, so a multi-cell sector
forms a single unbroken ramp instead of resetting at each cell boundary.

`GRID` assigns one sector id per map cell. Digits are ids 0-9 and letters are
ids 10-35.

`WALL x1 y1 x2 y2 texture sector transparent` adds an arbitrary angled wall
segment. `texture` uses the texture enum order from `ft_enumerations.h`:
0 north, 1 south, 2 east, 3 west, 4 door, 5 sprite, 6 transparent, 7 decal.

`DC` supplies the transparent decal drawn over a wall after it is hit by a
projectile.

Map device tokens are preserved inside the packed `BEGIN_CUB` data:

- `L` is a solid wall device with a small billboard control sprite placed just
  in front of its accessible face. Interacting with it animates every cell
  assigned to the panel's sector. Its sprite uses decoration texture `D5`,
  independently from the projectile-damage decal supplied by `DC`.
- `P` is the actual secret-door cell. It renders with the directional wall
  texture and is deliberately indistinguishable from a wall on the minimap;
  proximity opens it automatically.
- `B` is a locked door. Without a key it remains closed and displays feedback;
  pressing `E` with an `8` key pickup in inventory consumes one key, unlocks
  the door permanently, and opens it. Timed/global door events respect locks.
- `9` is the jetpack/artifact pickup used to authorize flight.
- `8` is a key pickup used by `B` doors.
- `7` is reserve ammunition; selecting inventory slot 2 and pressing Enter
  transfers it into the active weapon's magazine.
- `V` places a solid generic world object using the `SP`/`SP0`-`SP7` visual.
  `v` places the same object as pass-through. Collision is circular and derived
  from the rendered object's scale rather than occupying its entire map cell.
  Active enemies use the same radius-based player collision; pickups and
  ordinary `a`-`f` decorations remain pass-through.

## Non-grid rooms

A room with any number of walls in any direction is authored by leaving the
grid cells empty (`0`) and drawing the actual room boundary as a closed loop
of `WALL` segments over that empty space. The grid keeps its normal outer
ring of `1`s as a backstop — rays that miss every segment still terminate at
the grid wall, so a broken loop cannot hang the raycaster. Example: a
pentagon room inside an otherwise empty 13x8 grid area, using vertices
`(5,2) (8,2) (9,4) (6.5,6) (4,4)`:

```text
WALL 5.0 2.0 8.0 2.0 0 1 0
WALL 8.0 2.0 9.0 4.0 0 1 0
WALL 9.0 4.0 6.5 6.0 0 1 0
WALL 6.5 6.0 4.0 4.0 0 1 0
WALL 4.0 4.0 5.0 2.0 0 1 0
```

Each segment's endpoint must land within `SEGMENT_EPSILON` (0.05 map units)
of another segment's endpoint for the loop to close. `--check` runs
`check_segment_loops()` after parsing and prints a warning (not an error) to
stderr if any segment endpoint is unmatched — an open loop is a visual
light-leak (the ray falls through to the distant grid wall instead of
hitting a wall face), not a crash. The minimap draws every segment as a line
over the grid so open loops are also visible there.
