# Doom-Nukem v3.1 compliance verdict

Audit date: 2026-08-04

## Overall verdict

**Not ready for hand-in as a complete mandatory submission.**

The repository has substantial Doom-Nukem functionality and demonstrates most
of the visible rendering/gameplay ideas. It builds cleanly and every packed
level passes the headless parser check. However, the subject says the mandatory
part must be integral and work without malfunctioning. There are still several
objective mandatory failures or serious defense blockers, so the honest answer
to “is everything there?” remains **no**. The original findings 1 and 2 have
now been fixed and are recorded below as resolved.

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
   creates a valid `.dnk`. The broader limitation on arbitrary runtime action
   scripting remains separately recorded in finding 9.

3. **FAIL — keys have no usable gameplay path.** Key pickups still occupy
   `ITEM_KEY`, but there is no key-consumption or locked-door implementation.
   Secret doors intentionally no longer consume a key. The subject explicitly
   asks for “keys, and ways to use them.”

4. **FAIL — keyboard arrows do not provide forward/back movement.** During play,
   Left/Right rotate, but Up/Down are only handled by the menu. Forward/backward
   movement uses W/S. The wording explicitly requests real-time movement via the
   keyboard arrows.

5. **FAIL — proximity hazard damage is effectively zero at normal frame rates.**
   `src/events/triggers.c` subtracts `(int)(10.0 * delta_time)`. At 60 FPS this
   casts approximately `0.166` to zero every frame, so walking on an `H` tile
   generally does not damage the player.

6. **FAIL — not every shipped/menu-selectable level has an end.** `blue_map.dnk`,
   `item_map.dnk`, `simple_map.dnk`, and `subject_map.dnk` contain no `X` exit.
   `update_level_flow()` has no completion path when a map has no exit, while the
   subject requires a beginning and an end for each level.

7. **FAIL / HIGH RISK — packed-level self-sufficiency is inconsistent.** Several
   `.dnk` files rely on repository fallback paths. `architecture_map.dnk` lacks
   the packed HUD and core sound assets. Seven packed maps lack `D5`, while
   `ft_int_assets()` assigns the elevator-button texture to the external path
   `tests/textures_doom/uac_panel_tech.xpm` and `load_all_textures()` loads it
   unconditionally. Therefore the binary plus any one of those level files is
   not self-sufficient outside the repository. `flight_ops.dnk` itself does pack
   `D5`, HUD assets, music, and sound effects correctly.

8. **FAIL / PARTIAL — blocking objects are not authorable in practice.** The
   `blocks_passage` field and collision check exist, but
   `item_default_blocks()` always returns false and the level format/editor has
   no way to configure proportional blocking for objects. Enemies and generic
   sprites are also not part of player collision.

9. **PARTIAL — actions cannot alter the full set of properties described by the
   subject.** Timed events can operate doors, score, messages, player damage, and
   elevator floor height. There is no level-authored action system for changing
   wall/room shape, wall or object texture, ceiling height, object placement, or
   other arbitrary properties at runtime.

10. **UNVERIFIED — memory leaks and crash-free graphical execution.** Cleanup
    code exists, and malformed-file smoke tests returned controlled errors.
    Valgrind could not run because the installed snap refuses to start without
    its AppArmor service. No X server was available for a complete game-session
    leak test.

## General instructions

| Requirement | Status | Evidence / finding |
|---|---|---|
| Executable named `doom-nukem` | PASS | Makefile target is `doom-nukem`. |
| Usual Makefile rules | PASS | `all`, `clean`, `fclean`, and `re` exist. A second `make` reports nothing to do. |
| Compile/relink only when necessary | PASS | Incremental no-op verified after build. |
| libft at repository root and built by Makefile | PASS | `lib/libft` has its own Makefile and is a dependency. |
| No global variables | **PASS** | Runtime timing is owned by `t_game`; a post-build `nm` scan finds no writable static-storage symbols in game objects. |
| Careful errors/no unexpected termination | PARTIAL | Missing file, directory, invalid extension, and invalid packed level produce controlled errors. GUI/runtime paths were not fuzzed. |
| No memory leaks | UNVERIFIED | Cleanup is extensive, but no usable Valgrind/X session was available. |
| Library restrictions | PASS / RISK | MLX is used for window/pixels/events; SDL2/ALSA for sound; pthread rendering is a listed bonus. Evaluators should confirm their interpretation of bundled MLX XPM loading. |
| Recode required image reader | PARTIAL / RISK | Game code calls `mlx_xpm_file_to_image`; the reader is bundled in MLX rather than clearly implemented as project code. |

## Core rendering and Wolf3D features

| Requirement | Status | Evidence / finding |
|---|---|---|
| Subjective ray-cast 3D; no 3D library | PASS | DDA ray casting and segment intersection are implemented in `src/graphics`; MLX presents a software-generated framebuffer. |
| Forward/back, 360-degree rotation via arrows | **FAIL** | W/S move and Left/Right rotate; Up/Down do not move during gameplay. |
| Escape closes cleanly | PASS | `KEY_ESC` calls `stop_game()`, which calls cleanup. |
| Window red cross closes cleanly | PASS | Client-message hook calls `stop_game()`. |
| Textured walls | PASS | Directional XPM wall textures and textured slices are implemented. |

## New graphic features

| Requirement | Status | Evidence / finding |
|---|---|---|
| Look up/down | PASS | Mouse and Page Up/Down change pitch with clamping. |
| Arbitrary room shapes/wall directions | PASS | Free wall segments and segment ray intersection exist; `architecture_map.dnk` contains a closed five-wall angled room. |
| Adjustable floor/ceiling heights | PASS | Sector floor/ceiling values, height-step bands, and risers exist. |
| Inclined floor/ceiling planes | PARTIAL / RISK | Sector slopes affect sampled heights, collision, walls, and step detection. The main floor-casting distance still assumes the viewer's horizontal plane, so true perspective-correct rendering of inclined textured planes is not proven. |
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
| Blocking/non-blocking objects proportional to visuals | **FAIL / PARTIAL** | Data field exists, but no authored object is configured as blocking and generic sprite collision is absent. |
| Pickups and inventory | PASS | Health, reserve ammo, keys, and artifacts are collected; ammo reload and jetpack use inventory selections. |
| Proximity and voluntary interactions | PARTIAL | Automatic secret doors and E-button interactions work in code; hazard damage is broken by integer truncation. |
| Timed actions/action sequences | PASS | World-event queue supports delayed/repeating actions; switch and elevator events are timed. |
| Actions alter shapes/properties broadly | PARTIAL | Doors and floor height change; arbitrary geometry/texture/property changes are not authorable. |
| Animated doors, keys, elevators, secret passages | **PARTIAL / FAIL** | Doors, elevator, and automatic disguised passages exist. Keys have no use. |
| Characters/objects with reactions/interactions | PASS | Multiple enemy types chase, attack, shoot, take damage, die, and award score. Buttons/doors react to interaction. |
| Projectiles interact with world, objects, characters, player | PASS / RISK | Player/enemy projectile ownership, wall collision/decals, sprite collision, enemy damage, and player damage exist. Decorations are intentionally projectile-transparent and items stop shots without taking damage. |
| Story and mission goal | PASS | Five-level campaign briefings/debriefings plus item/exit objectives exist. |
| Beginning and end for each level | **FAIL** | Four menu levels have no exit/completion path. |
| Sound effects | PASS / LIVE VERIFY | Packed WAV effects and SDL2/ALSA playback exist; audible output not tested. |
| Music | PASS / LIVE VERIFY | Looping packed music channel exists; audible output not tested. |

## Level editor and packed levels

| Requirement | Status | Evidence / finding |
|---|---|---|
| Mandatory level editor | **PASS** | Interactive `--edit` supports editing, saving, validation, and packing; the starter workflow also succeeds. |
| Editor defines geometry, height, textures, actions/interactions | **PASS / RISK** | It authors all properties and device tokens supported by this engine: grid/free-segment geometry, texture headers, sector height/slope/light, entities, pickups, and interaction devices. General data-driven runtime action scripts are not supported; see finding 9. |
| One self-sufficient packed file per level | **FAIL / PARTIAL** | DNK embedding works and `flight_ops.dnk` is well packed, but several shipped files fall back to external repository assets. |

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
nm -A --defined-only build/*.o | writable-symbol filter
                                                    # no output (passed)
git diff --check                                    # passed
```

Valgrind was attempted, but the installed snap refused to run because its
AppArmor service is unavailable. A graphical smoke test was impossible because
there is no X server or Xvfb in this environment.

## Recommended order before hand-in

1. Add locked doors and an actual key-use flow.
2. Bind gameplay Up/Down to forward/back movement while preserving W/S.
3. Accumulate fractional hazard damage instead of truncating each frame.
4. Give every menu-selectable level a valid start, mission, and exit/end.
5. Repack every level with all fallback assets and remove unconditional external
   texture dependencies from packed-level loading.
6. Add authorable blocking object properties and collision sized to objects.
7. Expand level-authored actions to texture, ceiling, object, and geometry
   changes, or prepare a strong defense if the school interprets the sentence
   less literally.
8. Run full playthroughs and clean-exit leak checks under X with Valgrind or
   sanitizers, including repeated campaign transitions and audio playback.

Only after those blockers are addressed should bonus-polish work be prioritized.
