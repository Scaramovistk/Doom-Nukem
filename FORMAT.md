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

`--edit` opens a graphical editor for the `.cub` and its optional same-basename
`.sectors` file. The map canvas and side panel edit map tokens, texture sets,
sector heights/slopes/light, sector-grid assignments, objects, start/end cells,
and delayed runtime actions. Save, Validate, and Pack finish the workflow.
`--pack` is the non-interactive
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

## `BEGIN_CUB` header reference

The seven base directives are required. Each texture directive takes an `.xpm`
path (or an `@asset_key` in a packed level); `F` and `C` additionally accept an
`R,G,B` colour in place of a texture path.

```text
NO path/to/north-wall.xpm       # required: north-facing walls
SO path/to/south-wall.xpm       # required: south-facing walls
EA path/to/east-wall.xpm        # required: east-facing walls
WE path/to/west-wall.xpm        # required: west-facing walls
DO path/to/door.xpm             # required: normal and locked doors
F path/to/floor.xpm | R,G,B     # required: floor surface
C path/to/ceiling.xpm | R,G,B   # required: ceiling surface
```

The following directives are optional. They are particularly useful for a
mission such as `flight_ops`:

```text
SK path/to/sky.xpm              # sky, used instead of a textured/colour ceiling
DC path/to/damage-decal.xpm     # overlay left on a `5` decal wall when shot
TR path/to/glass.xpm            # transparent-wall texture for `4` tiles
SP path/to/object.xpm           # default visual for `V` and `v` objects
SP0 ... SP7 path/to/frame.xpm   # optional eight directional object frames; all eight are required
E0 ... E4 path/to/enemy.xpm     # visuals for enemy tiles 3, K, I, D, and C
D1 ... D6 path/to/decal.xpm     # decoration visuals for a-f and g-l; D5 is also the lift-button icon
VM path/to/vending_machine.xpm  # visual for `Q`
LT path/to/laptop_table.xpm     # visual for `J`
NEXT path/to/next-level.dnk     # optional level to load after a completed mission
```

`flight_ops.cub` is a minimal example of the required base set plus `SK`,
`DC`, `D5`, `SP`, and the `E0`-`E4` enemy set. Its `F` directive uses a floor
texture and its `C` directive uses the colour `18, 12, 10`.

`SECTOR id floor_z ceil_z slope_x slope_y light` defines a room/area. Light is
0-255. Slopes are applied continuously across every cell assigned to that
sector, anchored to the sector's bounding-box origin, so a multi-cell sector
forms a single unbroken ramp instead of resetting at each cell boundary.

`GRID` assigns one sector id per map cell. Digits are ids 0-9 and letters are
ids 10-35.

`WALL x1 y1 x2 y2 texture sector transparent` adds an arbitrary angled wall
segment. `texture` uses the texture enum order from `ft_enumerations.h`:
0 north, 1 south, 2 east, 3 west, 4 door, 5 sprite, 6 transparent, 7 decal.

## Authored runtime actions

Place `ACTION` records after `SECTOR`/`WALL` declarations and before `GRID` in
the `.sectors` sidecar. `trigger_x trigger_y` must identify a `T` switch in the
map. Each matching action is queued independently, so increasing delays define
a sequence:

```text
ACTION trigger_x trigger_y delay BLOCK x y EMPTY|WALL|DOOR|GLASS|DECAL
ACTION trigger_x trigger_y delay FLOOR sector height
ACTION trigger_x trigger_y delay CEILING sector height
ACTION trigger_x trigger_y delay LIGHT sector value
ACTION trigger_x trigger_y delay TEXTURE_SWAP texture_a texture_b
ACTION trigger_x trigger_y delay OBJECT_MOVE object_index x y
ACTION trigger_x trigger_y delay OBJECT_BLOCK object_index 0|1
ACTION trigger_x trigger_y delay OBJECT_SCALE object_index scale
ACTION trigger_x trigger_y delay OBJECT_TEXTURE object_index texture
ACTION trigger_x trigger_y delay WALL_MOVE wall_index x1 y1 x2 y2
ACTION trigger_x trigger_y delay WALL_TEXTURE wall_index texture
```

Texture numbers use the enum order listed above. Object indices follow `V`/`v`
map scan order, top-to-bottom then left-to-right; wall indices follow `WALL`
declaration order. A level supports up to 32 authored actions. Invalid syntax,
missing switches, and out-of-range sector/object/wall references make packed
validation fail. In the editor, use `action add TX TY DELAY TYPE ARGUMENTS` or
`action clear`.

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

`VM path/to/vending_machine.xpm` supplies the sprite used for a `Q` vending
machine map tile. A map may contain at most one `Q`; interact with it to buy
10 ammo for 5 score points. `LT path/to/laptop_table.xpm` supplies the sprite
used for a `J` laptop-table tile. Entering its proximity range plays a short
sound once; leaving and returning plays it again.

## Map tile reference

The map symbols are documented below.

`0` is empty floor, `1` is a solid wall, and `2` is a door.

`3` is the default melee enemy using `E0`. `K`, `I`, `D`, and `C` are enemy
variants using `E1`, `E2`, `E3`, and `E4`, respectively.

`4` is a transparent wall / glass. `5` is a decal wall, which can display the
`DC` decal after a projectile hit.

`6` is a health pickup (full the health), `7` is an ammo pickup (+10 ammo), `8` is a
key pickup (+1 key), and `9` is an artifact pickup (+5 artifacts).

`N`, `S`, `E`, and `W` set the player start and initial facing direction;
exactly one start tile is required.

`T` is the general switch: it adds score, toggles doors after a short delay,
then closes them. It uses the decal-wall rendering.

`H` is a hazard zone, `M` is a message zone, and `X` is an exit zone.

`G` is a capture-the-flag objective. Take the flag and return to the player
start tile (your base) to complete the map; regular pickups and exits are not
required for this mode. The player start is marked blue and the flag yellow on
the minimap. Chain CTF maps with the normal `NEXT path/to/next-level.dnk`
header directive.

`a` through `f` are pass-through billboard decorations using header textures
`D1` through `D6`. `g` through `l` are decorative wall decals using those same
six textures; like `DC`, they overlay their wall surface. `Q` is the vending
machine and `J` is the laptop table, using `VM` and `LT` respectively.

## `flight_ops` authoring example

`tests/maps_src/flight_ops.cub` and its `.sectors` sidecar are a compact
reference mission for the four feature requirements. Start at `N`, collect the
`7` ammo pickup and reload it with `R` (or inventory slot 2 + Enter), then use
the `P` cell as the automatically opening hidden passage. The `L` panel is at
map coordinate `(13, 6)` (zero-based `x, y`); it controls sector `2`, because
that is the sector assigned to the panel cell. Give the map a `D5` header to
supply its button icon.

The lower area demonstrates a flight-gated crossing. Sector `3` has a raised
floor (`1.00`) while the shaft, sector `4`, has floor `-1.00` and lower light
(`105`); both share a ceiling of `2.40`. The `9` artifact lies beyond the lift
and is required to fly across the deep shaft to the `X` exit. The relevant
sidecar data is:

```text
SECTOR 2 0.00 2.25 0.00 0.00 255
SECTOR 3 1.00 2.40 0.00 0.00 205
SECTOR 4 -1.00 2.40 0.00 0.00 105
```

`GRID` rows use the same zero-based map coordinates as the `.cub` map. In the
example, the lift platform is sector `2`; the rows below it assign sector `3`
to the raised approach and sector `4` to the shaft. The outer `0` assignments
retain the base sector 0 values.

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
