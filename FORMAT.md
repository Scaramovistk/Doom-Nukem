# Doom-Nukem Packed Level Format

`.dnk` levels are text files with embedded assets. The game unpacks them into
`build/unpacked_<pid>/`, generates a temporary `.cub`, then applies Doom-Nukem
sector metadata.

## Commands

```sh
./doom-nukem --edit tests/maps/door_map.cub tests/maps/door_map.dnk
./doom-nukem --pack tests/maps/door_map.cub tests/maps/door_map.dnk
./doom-nukem --check tests/maps/door_map.dnk
./doom-nukem tests/maps/door_map.dnk
```

`--edit` and `--pack` export a self-contained `.dnk` from a `.cub`. `--check`
parses a level without opening a window and prints map, sector, angled-wall,
sprite, and item counts.

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
SECTOR 1 0.35 1.35 0.03 0.00 190
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

`BEGIN_CUB` contains normal cub3D-compatible header and map data, except texture
paths can be asset references.

`SECTOR id floor_z ceil_z slope_x slope_y light` defines a room/area. Light is
0-255. Slopes are applied locally inside cells assigned to that sector.

`GRID` assigns one sector id per map cell. Digits are ids 0-9 and letters are
ids 10-35.

`WALL x1 y1 x2 y2 texture sector transparent` adds an arbitrary angled wall
segment. `texture` uses the texture enum order from `ft_enumerations.h`:
0 north, 1 south, 2 east, 3 west, 4 door, 5 sprite, 6 transparent, 7 decal.
