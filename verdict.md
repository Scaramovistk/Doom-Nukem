# Doom-Nukem v3.1 compliance verdict

Audit date: 2026-08-05

## Overall verdict

**Not yet ready to claim a perfect mandatory submission.**

The repository has substantial Doom-Nukem functionality and demonstrates most
of the visible rendering/gameplay ideas. It builds cleanly and every packed
level passes the headless parser check. The original findings 1 through 9 have
been fixed. The remaining concrete concern is inclined-plane rendering: slope
data affects world height and collision, but the textured plane caster still
projects rows as horizontal planes. There are also live graphical/audio/leak
checks and an image-reader interpretation that cannot be closed headlessly.
Because the subject requires the mandatory part to be perfect, the honest answer
to “is everything proven ready?” remains **no** until those items are addressed.

This is a static/headless audit. The environment has no X server, so rendering,
input feel, audio output, and full mission playthroughs were not visually tested.

Status meanings:

- **PASS**: direct code and/or level evidence exists.
- **PARTIAL / RISK**: some implementation exists, but it is incomplete or needs
  a live defense test.
- **FAIL**: a concrete contradiction with the subject was found.
- **UNVERIFIED**: the available environment cannot prove the requirement.

## Audit findings

1. **RESOLVED — no-global-variables rule.** Frame timing now lives in `t_game`
   and the former static pointer tables are automatic read-only lookup arrays.
   After rebuilding, `nm` reports no defined writable `b/B/d/D/g/G/s/S`
   symbols in `build/*.o`.

2. **RESOLVED — mandatory level editor.** `--edit source.cub [output.dnk]`
   launches an interactive terminal editor. It can place all supported map
   devices/entities, edit texture/header values, define sector floor/ceiling
   height, slope and light, assign the sector grid, add/clear arbitrary wall
   segments, save, validate, and pack. `--pack` remains available for scripted
   export. The starter command now uses the correct source path and successfully
   creates a valid `.dnk`.

3. **RESOLVED — keys now have a usable gameplay path.** The authored `B` token
   creates a locked door. Pressing `E` without a key gives clear feedback;
   collecting an `8` pickup and interacting consumes exactly one key, unlocks
   that door permanently, and starts its normal animation. Timed/global events
   cannot bypass locks. `door_map.dnk` demonstrates this path.

4. **RESOLVED — keyboard arrows provide the required movement.** During play,
   Up/Down now drive the same forward/back movement state as W/S, while
   Left/Right retain 360-degree rotation. In menu state, Up/Down continue to
   select levels.

5. **RESOLVED — proximity hazards now apply frame-rate-independent damage.**
   `src/events/triggers.c` accumulates fractional damage between frames before
   subtracting whole health points. At 60 FPS, sustained contact with an `H`
   tile therefore applies the configured 10 damage per second. The remainder
   resets after leaving the hazard and when loading another level.

6. **RESOLVED — every shipped/menu-selectable level has an end.** Reachable `X`
   exits were authored in `blue_map.cub`, `item_map.cub`, `simple_map.cub`, and
   `subject_map.cub`, then their packed `.dnk` levels were regenerated. Every
   shipped packed level now contains an exit and passes the headless parser
   check.

7. **RESOLVED — every shipped packed level is asset-self-sufficient.** All 13
   `.dnk` files now embed their referenced textures, nine HUD assets, seven
   sound/music assets, and the elevator-button control sprite. Packed parsing
   clears repository HUD/audio fallbacks and rejects legacy direct `.xpm`
   paths. Export now fails and removes partial output if any asset cannot be
   read. The editable source and sector sidecar for `architecture_map.dnk` were
   restored so it can be regenerated through the same workflow.

8. **RESOLVED — blocking and pass-through objects are authorable.** `V` authors
   a solid generic sprite and `v` its pass-through counterpart through both the
   map format and editor. World-object metadata stores visual scale, blocking
   state, and a proportional circular collision radius instead of blocking a
   whole tile. Active enemies also participate in radius-based player collision;
   pickups and ordinary decorations deliberately remain pass-through.

9. **RESOLVED — broad runtime mutations are level-authored.** `ACTION` records
   attach delayed sequences to specific `T` switches. Supported mutations alter
   grid geometry/block type, floor and ceiling height, sector light, loaded
   texture-slot assignments, object position/blocking/scale/texture, and
   arbitrary-wall endpoints/texture. The editor can add or clear these records,
   and packed validation rejects malformed actions or invalid switch, sector,
   object, and wall references. `gameplay_map.dnk` contains a nine-step sequence
   demonstrating geometry, height, light, texture, and object changes.

10. **PARTIAL / UNVERIFIED — memory leaks and crash-free graphical execution.**
    All packed levels plus invalid-path cases pass an isolated ASan/UBSan build,
    and malformed-file smoke tests return controlled errors. LeakSanitizer cannot
    run under this environment's ptrace wrapper, while the installed Valgrind
    snap refuses to start without its AppArmor service. No X server is available
    for a complete game-session leak or crash test. Clang's static analyzer also
    reports two possible safety paths, but source review shows both depend on
    infeasible assumptions: sprite draw records are initialized before sorting,
    and enemy storage is sized with the same predicate used to populate it. It
    also reports two harmless dead stores (`corridor`). These findings should be
    cleaned up or locally asserted if a warning-free analyzer report is desired.

11. **FAIL / PARTIAL — inclined textured planes are not perspective-correct.**
    Sector slopes feed `get_floor_z_at()`/`get_ceiling_z_at()` for collision,
    wall endpoints, sprites, and step detection. However,
    `src/graphics/draw_scene.c:set_surface_row()` derives one row distance from
    the height at the player's position, and `draw_step_bands.c` uses the
    sector's base height. Neither solves the ray/plane intersection using the
    sampled sector slope. A sloped sector therefore behaves as sloped geometry
    but its floor/ceiling texture is still projected as a horizontal plane.

12. **DEFENSE RISK — the required image-reader interpretation is unresolved.**
    Project code loads XPM files through `mlx_xpm_file_to_image()`. The reader is
    bundled source inside the submitted MLX, but it is not a reader authored in
    the game code. The subject allows all MLX functions while also saying the
    needed image reader must be recoded; confirm the evaluator's interpretation
    or provide a project-owned reader before claiming an unqualified pass.

## General instructions

| Requirement | Status | Evidence / finding |
|---|---|---|
| Executable named `doom-nukem` | PASS | Makefile target is `doom-nukem`. |
| Usual Makefile rules | PASS | `all`, `clean`, `fclean`, and `re` exist. A second `make` reports nothing to do. |
| Compile/relink only when necessary | PASS | Incremental no-op verified after build. |
| libft at repository root and built by Makefile | PASS | `lib/libft` has its own Makefile and is a dependency. |
| No global variables | **PASS** | Runtime timing is owned by `t_game`; a post-build `nm` scan finds no writable static-storage symbols in game objects. |
| Careful errors/no unexpected termination | PARTIAL | Missing file, directory, invalid extension, and invalid packed level produce controlled errors. GUI/runtime paths were not fuzzed. |
| No memory leaks | PARTIAL / UNVERIFIED | ASan/UBSan headless parsing/export passes; LSan, Valgrind, and a graphical-session leak test remain unavailable. |
| Library restrictions | PASS / RISK | MLX is used for window/pixels/events; SDL2/ALSA for sound; pthread rendering is a listed bonus. Evaluators should confirm their interpretation of bundled MLX XPM loading. |
| Recode required image reader | PARTIAL / RISK | Game code calls `mlx_xpm_file_to_image`; the reader is bundled in MLX rather than clearly implemented as project code. |

## Core rendering and Wolf3D features

| Requirement | Status | Evidence / finding |
|---|---|---|
| Subjective ray-cast 3D; no 3D library | PASS | DDA ray casting and segment intersection are implemented in `src/graphics`; MLX presents a software-generated framebuffer. |
| Forward/back, 360-degree rotation via arrows | **PASS** | Gameplay Up/Down move forward/back and Left/Right rotate; W/S remain available. |
| Escape closes cleanly | PASS | `KEY_ESC` calls `stop_game()`, which calls cleanup. |
| Window red cross closes cleanly | PASS | Client-message hook calls `stop_game()`. |
| Textured walls | PASS | Directional XPM wall textures and textured slices are implemented. |

## New graphic features

| Requirement | Status | Evidence / finding |
|---|---|---|
| Look up/down | PASS | Mouse and Page Up/Down change pitch with clamping. |
| Arbitrary room shapes/wall directions | PASS | Free wall segments and segment ray intersection exist; `architecture_map.dnk` contains a closed five-wall angled room. |
| Adjustable floor/ceiling heights | PASS | Sector floor/ceiling values, height-step bands, and risers exist. |
| Inclined floor/ceiling planes | **FAIL / PARTIAL** | Slopes affect geometry and collision, but floor/ceiling texture casting still uses horizontal-plane row distances rather than ray/sloped-plane intersection. |
| Textured floors and ceilings | PASS | Floor/ceiling casting samples textures. |
| Sky instead of ceiling | PASS | `SK` texture support and sky-row rendering exist; several levels use it. |
| Partially transparent walls | PASS | Transparent hits are retained and alpha blended. |
| Decorations over wall texture | PASS | `DECAL_WALL` renders a second alpha texture; projectiles can turn walls into damaged/decal walls. |
| Always-facing sprites | PASS | Billboard sprite projection exists for objects, items, decorations, and enemies. |
| Orientation-dependent multi-sprites | PASS | `SP0` through `SP7` are selected from player/object angle; demonstration maps contain all eight frames. |
| Per-room light control affects walls/objects | PASS | Sector light values feed wall, floor/ceiling, sprite, and projectile-adjacent rendering. |
| Text messages over graphics | PASS | Timed framebuffer text messages exist. |
| Multi-element HUD | PASS | Separate health, magazine, inventory, score, FPS, crosshair, and weapon-view elements exist. |

## Gameplay features

| Requirement | Status | Evidence / finding |
|---|---|---|
| Smooth mouse 360 view and pitch | PASS / LIVE VERIFY | Relative recentering, rotation, and pitch code exist; feel must be tested with X. |
| Strafing | PASS | A/D lateral movement exists. |
| Wall collision and reasonable steps | PASS / LIVE VERIFY | Four-corner collision, door checks, ceiling clearance, and step-height logic exist. |
| Run, jump, fall, crouch, stand | PASS / LIVE VERIFY | State/input and vertical physics exist. |
| Fly or swim | PASS | Jetpack-gated flight exists and `flight_ops.dnk` requires crossing a deep shaft. |
| Blocking/non-blocking objects proportional to visuals | **PASS** | `V`/`v` author solid/pass-through generic sprites; collision radius follows visual scale, and active enemies also block proportionally. |
| Pickups and inventory | PASS | Health, reserve ammo, keys, and artifacts are collected; keys unlock `B` doors, while ammo reload and jetpack use inventory selections. |
| Proximity and voluntary interactions | **PASS** | Automatic secret doors and E-button interactions work; hazard contact accumulates fractional per-frame damage and applies the configured rate. |
| Timed actions/action sequences | PASS | World-event queue supports delayed/repeating actions; switch and elevator events are timed. |
| Actions alter shapes/properties broadly | **PASS** | Delayed level-authored actions mutate grid/segment geometry, floor/ceiling/light, texture slots, and object position/blocking/scale/texture. |
| Animated doors, keys, elevators, secret passages | **PASS** | Ordinary and keyed animated doors, consumed inventory keys, elevators, and automatic disguised passages are implemented. |
| Characters/objects with reactions/interactions | PASS | Multiple enemy types chase, attack, shoot, take damage, die, and award score. Buttons/doors react to interaction. |
| Projectiles interact with world, objects, characters, player | PASS / RISK | Player/enemy projectile ownership, wall collision/decals, sprite collision, enemy damage, and player damage exist. Decorations are intentionally projectile-transparent and items stop shots without taking damage. |
| Story and mission goal | PASS | Five-level campaign briefings/debriefings plus item/exit objectives exist. |
| Beginning and end for each level | **PASS** | Every shipped/menu-selectable packed level has a reachable authored exit and a completion path. |
| Sound effects | PASS / LIVE VERIFY | Packed WAV effects and SDL2/ALSA playback exist; audible output not tested. |
| Music | PASS / LIVE VERIFY | Looping packed music channel exists; audible output not tested. |

## Level editor and packed levels

| Requirement | Status | Evidence / finding |
|---|---|---|
| Mandatory level editor | **PASS** | Interactive `--edit` supports editing, saving, validation, and packing; the starter workflow also succeeds. |
| Editor defines geometry, height, textures, actions/interactions | **PASS** | It authors grid/free-segment geometry, texture headers, sectors, entities/devices, and validated delayed runtime mutation sequences attached to switches. |
| One self-sufficient packed file per level | **PASS** | All 13 shipped DNKs embed level textures, HUD, audio, and the elevator control; packed loading rejects direct repository XPM paths. |

## Bonus status

Bonuses do not repair mandatory failures.

| Bonus | Status | Evidence / finding |
|---|---|---|
| Level/difficulty menu | PASS | No-argument menu scans levels and offers three difficulties. |
| Aesthetic research/atmosphere | SUBJECTIVE / PARTIAL | Doom/UAC textures, sky, lighting, decorations, music, HUD, and campaign maps exist; human judgment required. |
| Complex story/scenario | PASS / SUBJECTIVE | Five connected campaign briefings/debriefings exist. |
| Useless immersive interactions/animations | PARTIAL | Vending-machine sprites and projectile stains exist, but vending machines have no actual interaction. |
| Network multiplayer | NOT IMPLEMENTED | No networking/lobby/team modes found. |
| pthread multithreaded rendering | PASS | Ray columns are split across available CPU cores with pthread creation/join and serial fallback. |
| Other extras | PASS | Reloadable per-weapon magazines, jetpack, minimap, enemies, difficulty, and secret doors are present. |

## Commands and checks performed

```text
make -j4
make                                  # final no-op confirmed
./doom-nukem --check tests/maps/*.dnk # all 13 packed levels passed
./doom-nukem --check does-not-exist.dnk
./doom-nukem --check .
./doom-nukem --check README.md
./doom-nukem --edit /tmp/verdict-editor-starter.dnk # succeeded
./doom-nukem --check /tmp/verdict-editor-starter.dnk # passed
interactive edit/save/validate/pack smoke test       # passed
./doom-nukem --pack tests/maps_src/door_map.cub tests/maps/door_map.dnk
./doom-nukem --check tests/maps/door_map.dnk         # 1 key + 1 locked door
repack tests/maps_src/*.cub to tests/maps/*.dnk      # all 13 succeeded
packed asset-reference/HUD/audio/control audit       # all 13 passed
run all DNKs from an isolated working directory      # all 13 passed
missing source-asset export                           # rejected; no partial file
legacy DNK with direct repository XPM paths           # rejected
editor V/v authoring smoke test                       # 2 objects, 1 blocking
solid/pass-through/enemy collision harness            # passed
gameplay_map authored-action parse                     # 9 actions
editor action clear/add/pack/check smoke test          # 1 valid action
11-form authored mutation/queue harness                # passed
malformed authored action in packed level              # rejected
isolated ASan/UBSan build + all 13 packed checks       # passed
isolated ASan/UBSan repack/check of every source       # passed
ASan/UBSan missing-file check                           # controlled failure
clang --analyze over all source translation units       # 4 warnings reviewed
nm -A --defined-only build/*.o | writable-symbol filter
                                                    # no output (passed)
git diff --check                                    # passed
```

Valgrind was attempted, but the installed snap refused to run because its
AppArmor service is unavailable. LeakSanitizer also cannot operate under the
environment's ptrace wrapper. AddressSanitizer and UndefinedBehaviorSanitizer
did run successfully with leak detection disabled. A graphical smoke test was
impossible because there is no X server or Xvfb in this environment.

## Recommended order before hand-in

1. Replace horizontal row-distance floor casting in sloped sectors with proper
   ray/plane intersection for both floor and ceiling textures.
2. Confirm that the bundled MLX XPM reader satisfies the evaluator, or add a
   project-owned XPM/image reader and use it from the game.
3. Run full playthroughs and clean-exit leak checks under X with Valgrind or
   sanitizers, including repeated campaign transitions and audio playback.

Only after those blockers are addressed should bonus-polish work be prioritized.
