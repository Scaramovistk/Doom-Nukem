# Doom-Nukem

`Doom-Nukem` is a software-rendered first-person game written in C. It starts
with the ray-casting ideas used by Wolfenstein 3D and cub3D, then adds the
rooms, heights, objects, enemies, interactions, projectiles, story, sound, and
level editor expected from a small Doom or Duke Nukem-style engine.

The executable does more than run the game. It can also edit source levels,
pack them with all their assets, and validate the resulting self-contained
level files.

## What kind of game is this?

The player explores a world from a first-person point of view. Walls are not
drawn with a 3D graphics library. Instead, the engine sends rays into a 2D map
and calculates which surfaces each ray reaches.

Think of each vertical screen column as one distance measurement:

```text
player -> ray -> nearest wall, door, glass, or angled segment
                  |
                  +----> distance
                  +----> texture position
                  +----> floor and ceiling height
                  +----> sector light
                              |
                              v
                       one screen column
```

Repeating this across the window creates the 3D view. Floors, ceilings,
sprites, transparent surfaces, projectiles, and the HUD are drawn around that
wall pass.

## A few basic words

- **Ray casting:** tracing rays through a 2D world to calculate a first-person
  image.
- **Sector:** an area with its own floor height, ceiling height, slopes, and
  light level.
- **Sprite:** a 2D image placed in the world and projected toward the camera.
- **Billboard:** a sprite that always faces the player.
- **HUD:** the health, ammunition, inventory, score, weapon, minimap, and other
  information drawn over the world.
- **Source level:** an editable `.cub` map and its optional `.sectors` file.
- **Packed level:** a self-contained `.dnk` file containing the map, sectors,
  textures, HUD images, sound effects, and music.

## How does the renderer work?

For every frame, the engine first calculates the player's direction and view
plane. A ray is then sent through every horizontal position in the window.

The grid DDA pass finds walls and devices such as doors, glass, secret
passages, and elevator panels. A second intersection pass handles arbitrary
angled wall segments. The renderer keeps the closest visible result while
also recording transparent surfaces and height changes that may affect later
drawing.

The frame is assembled in this order:

```text
floor, ceiling, and sky
          |
          v
walls, doors, glass, decals, and height transitions
          |
          v
objects, decorations, enemies, items, and projectiles
          |
          v
messages, minimap, crosshair, weapon, and HUD
```

Wall columns are divided into bands and rendered with POSIX threads. The
threads are joined before sprites and overlays are drawn, so depth information
is complete before the final passes begin.

## World and gameplay features

The engine supports:

- textured walls, floors, ceilings, and skies;
- rooms with different floor and ceiling heights;
- continuous sloped floors and ceilings;
- arbitrary angled wall segments and non-rectangular rooms;
- transparent walls, wall decorations, and projectile decals;
- billboard decorations and direction-aware object sprites;
- sector lighting applied to walls, objects, and characters;
- walking, strafing, running, jumping, crouching, falling, and flight;
- doors, locked doors, switches, elevators, hazards, and secret passages;
- pickups, inventory slots, magazines, reserve ammunition, and a jetpack;
- melee and ranged enemies with movement, attacks, health, and score rewards;
- player and enemy projectiles with world and entity collision;
- timed authored actions that can change geometry and world properties;
- campaign missions, exits, failure states, and capture-the-flag levels;
- overlapping sound effects, looping music, text messages, and a multi-part
  HUD.

## Project requirements

- The executable is named `doom-nukem`.
- The project is written in C and follows the 42 Norm.
- Rendering is performed in software without a hardware-accelerated 3D API.
- MinilibX is used for window, image, pixel, and input management.
- No mutable global variables are used.
- Errors must not cause crashes, double frees, or unexpected exits.
- A level editor and a self-contained packed level format are included.

## Dependencies

MinilibX and libft are included under `lib/`.

On Linux, MinilibX uses X11. SDL2 is preferred for audio when it is installed;
otherwise the Makefile uses ALSA. The usual development dependencies are:

```text
compiler, make, pkg-config, X11/Xext, and either SDL2 or ALSA
```

On macOS, MinilibX uses AppKit and OpenGL for window management, while SDL2 is
required for audio. OpenGL is not used as the game's 3D renderer.

## Building the program

Build the executable:

```sh
make
```

Build every editable map into `tests/maps/`:

```sh
make maps
```

Remove object files:

```sh
make clean
```

Remove the executable, objects, and generated packed maps:

```sh
make fclean
```

Rebuild everything:

```sh
make re
```

The Makefile lists every source explicitly and does not relink the executable
when its inputs have not changed.

## Running the game

Open the level-selection menu:

```sh
./doom-nukem
```

The menu scans `tests/maps/`. Run `make maps` first when that directory does
not contain packed levels.

Run an editable source map directly:

```sh
./doom-nukem tests/maps_src/e1m1.cub
```

Run a packed level:

```sh
./doom-nukem tests/maps/e1m1.dnk
```

In the menu, Up and Down select a level, Left and Right select Easy, Normal,
or Hard difficulty, and Enter starts the game. Choosing `e1m1.dnk` starts the
five-level campaign. Choosing `ctf_1.dnk` starts the three-map capture-the-flag
sequence.

## Game controls

- `W` / `S` or Up / Down: move forward and backward.
- `A` / `D`: strafe left and right.
- Left / Right: rotate without the mouse.
- Mouse: rotate and look up or down.
- Page Up / Page Down: keyboard look fallback.
- Shift: run.
- Space: jump, or ascend while flying.
- Ctrl: crouch, or descend while flying.
- `E`: interact with doors, switches, panels, vending machines, and locks.
- Left mouse button: fire the selected weapon.
- `Q`: switch weapon.
- `R`: reload from reserve ammunition.
- `1` / `2` / `3` / `4`: select an inventory slot.
- Enter: use the selected inventory item.
- `F`: toggle the jetpack after collecting it.
- `0` or F11: toggle fullscreen and windowed mode.
- Esc or the window close button: exit cleanly.

## The level editor

Open an existing `.cub` source project:

```sh
./doom-nukem --edit tests/maps_src/door_map.cub
```

Choose the packed output path explicitly:

```sh
./doom-nukem --edit tests/maps_src/door_map.cub tests/maps/door_map.dnk
```

The option is `--edit` with two hyphens. A single-hyphen `-edit` is not a
recognized editor option.

The editor opens the `.cub` file and the same-basename `.sectors` file when it
exists. The map is displayed as a clickable grid. Select a brush in the right
panel, then click a map cell to place it.

The `< TOOL PAGE >` heading changes between four brush pages. Together they
expose all 43 map-cell values accepted by the parser:

- walls, floor, doors, glass, and decal walls;
- every player direction and enemy type;
- health, ammunition, key, and jetpack pickups;
- switches, hazards, messages, exits, flags, locks, secrets, and elevators;
- solid and pass-through objects, vending machines, and laptop tables;
- six billboard decorations and six wall decorations;
- `_`, which writes an outside/void space for irregular map shapes.

The editor side panel and keyboard also provide:

- `[` / `]`: select a sector id;
- `C`: assign the selected sector to the selected map cell;
- `F` / `R`: adjust floor or ceiling height;
- `G` / `H`: adjust floor slopes;
- `L`: adjust sector light;
- `A`: add a door action at the selected cell;
- `T`: change the texture preset;
- `S`: save the `.cub` and `.sectors` sources;
- `K`: validate the project;
- `P`: pack the output `.dnk` file;
- Esc: close the editor.

Border cells are kept as solid walls. This prevents a ray from leaving the
map when a level is incomplete.

## Source and packed levels

An editable project has two layers:

```text
level.cub       map cells, player start, texture paths, and optional assets
level.sectors   heights, slopes, lights, angled walls, and timed actions
       |
       +---- doom-nukem --pack ----> level.dnk
                                      |
                                      +-- map and sector data
                                      +-- textures and HUD images
                                      +-- sounds and music
```

The `.sectors` file is optional. When it is missing, the packer creates a
default two-sector layout.

A sector definition looks like this:

```text
SECTOR 0 0.00 2.00 0.00 0.00 255
```

The values are the sector id, floor height, ceiling height, shared X/Y slope,
and light level. A `GRID` below the definitions assigns one sector id to every
map cell. `WALL` records add arbitrary angled segments, and `ACTION` records
describe delayed world changes triggered by a switch.

See [FORMAT.md](FORMAT.md) for the complete header, sector, wall, action, map
token, and packed-file reference.

## Packing and validating levels

Pack one source project without opening the editor:

```sh
./doom-nukem --pack tests/maps_src/door_map.cub tests/maps/door_map.dnk
```

When no destination is supplied, the output uses the source basename with a
`.dnk` extension:

```sh
./doom-nukem --pack tests/maps_src/door_map.cub
```

Validate a source or packed level without opening a game window:

```sh
./doom-nukem --check tests/maps/door_map.dnk
```

Packing stops if the source level or any required asset is invalid. An
incomplete packed output is removed instead of being left behind.

A packed file begins with `DNK1`. Its embedded files are hexadecimal text, so
the map needs only the `doom-nukem` executable and its `.dnk` file at runtime.
When loaded, the assets are unpacked into a temporary directory, referenced by
a generated `.cub`, and cleaned up when the game exits.

To generate a starter packed level from the bundled editor template:

```sh
./doom-nukem --edit new_level.dnk
```

Source editing should normally use `.cub` plus `.sectors`; `.dnk` is the final
self-contained deliverable.

## Important map cells

- `0` is empty floor and `1` is a wall.
- `2` is a door, `4` is glass, and `5` is a decal wall.
- `N`, `S`, `E`, or `W` is the single player start and facing direction.
- `3`, `K`, `I`, `D`, and `C` are the five enemy types.
- `6`, `7`, `8`, and `9` are health, ammo, key, and jetpack pickups.
- `T` is a switch, `H` a hazard, `M` a message, and `X` an exit.
- `L` is an elevator panel, `P` a secret door, and `B` a locked door.
- `G` is the capture-the-flag objective.
- `V` is a blocking object and `v` is its pass-through form.
- `Q` is a vending machine and `J` is a laptop table.
- `a` through `f` are billboard decorations.
- `g` through `l` are wall decorations.

## Missions, inventory, and interactions

Normal missions begin with an objective, require the active pickups to be
collected, and finish on an `X` exit. Reaching zero health fails the mission.
Campaign levels show briefing and debriefing text and can chain to the next
packed level.

Capture-the-flag levels use one `G`. The player takes the flag and returns to
the original start tile, which acts as the base. These missions do not require
normal pickups or an exit.

Ammo pickups enter inventory slot 2 as reserve ammunition. Reloading transfers
that reserve into the current weapon's magazine. The slot-4 artifact is a
jetpack; after it is collected, `F` or Enter while slot 4 is selected toggles
flight.

`E` opens doors, activates switches and elevators, unlocks `B` doors when a
key is available, and buys ammunition from a `Q` vending machine. A vending
purchase costs 5 score and supplies 10 ammunition.

## Audio and platform strategy

The game chooses its audio backend at build time:

```text
SDL2 available       -> SDL2 audio backend
SDL2 unavailable
and Linux + ALSA     -> native ALSA backend
neither available    -> build error
```

Music uses a looping channel. Sound effects use a small channel pool so shots,
doors, pickups, enemies, and environmental sounds can overlap. Missing optional
sound files are skipped without stopping the game.

Linux and macOS mouse/window differences are isolated in separate source
files. The gameplay, parser, editor, renderer, and packed format remain shared.

## Project layout

```text
assets/             textures, HUD images, sound effects, and music
include/            shared structures, constants, and prototypes
lib/libft/          bundled utility library
lib/Minilbx_*/      bundled window and image library
src/events/         input, gameplay, enemies, projectiles, menu, and editor
src/graphics/       ray casting, surfaces, sprites, HUD, and minimap
src/parser/         .cub/.sectors parsing and .dnk packing
src/utils/          initialization, cleanup, sectors, BSP, and audio
tests/maps_src/     editable source projects
tests/maps/         generated self-contained levels
```

## Validation and testing

Check the 42 Norm across the project-owned C and header files:

```sh
norminette src include lib/libft/src lib/libft/include
```

Build and validate every supplied level:

```sh
make
make maps
for level in tests/maps/*.dnk; do
    ./doom-nukem --check "$level"
done
```

The repository currently includes campaign, capture-the-flag, architecture,
door, item, gameplay, and flight-focused maps. `flight_ops.cub` is a compact
authoring example for a secret passage, inventory reload, elevator panel, and
jetpack crossing.

The full packed-level specification and advanced authoring examples are in
[FORMAT.md](FORMAT.md). Asset conventions are documented in
[assets/README.md](assets/README.md).
