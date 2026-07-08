# DooM-Nukem 3D — Project Tracker

> Team: **Rodolfo**, **Gabriel**, **Murillo**
> Base: cub3D raycaster (DDA, wall textures, WASD, collision, doors, minimap, mouse rotation)
> **Deadline: July 31, 2026** (~4.5 weeks from June 29)

---

## What We Already Have (from cub3D)

| Feature | Status |
|---|---|
| DDA raycasting engine | ✅ Done |
| Texture-mapped walls (N/S/E/W + door) | ✅ Done |
| WASD + arrow key movement | ✅ Done |
| Wall collision detection | ✅ Done |
| Map parsing (.cub format) | ✅ Done |
| Mouse rotation (horizontal) | ✅ Done |
| Doors (open/close animation) | ✅ Done |
| Minimap | ✅ Done |
| Floor & ceiling solid colors | ✅ Done |
| Delta time / frame-rate independent movement | ✅ Done |
| Window management (ESC, red X) | ✅ Done |
| Strafing (A/D) | ✅ Done |

---

## Timeline (4.5 weeks to July 31)

```
Jun 29 – Jul 6   WEEK 1  Engine + world foundations in parallel
Jul 7  – Jul 13  WEEK 2  Sprites, heights, gameplay systems
Jul 14 – Jul 20  WEEK 3  Full integration: enemies, shooting, events
Jul 21 – Jul 27  WEEK 4  Level editor, sound, polish, bonuses
Jul 28 – Jul 31  BUFFER  Bug fixes, evaluation prep, README
```

---

## Mandatory — Split by Owner

### 🔵 Gabriel (start now — all renderer work)

#### G0 · Fix mouse look → true FPS feel `[✅]` ⚡ first thing
Current problems in `src/events/hooks.c` + `mlx_mouse_pos_bonus.c`:
- Cursor is never hidden — visible and can escape the window
- `rotation_move` is only ever -2, 0, or +2 (binary, not proportional to speed)
- `move_mouse()` is only called once at startup, not each frame — mouse hits edge and rotation stops

Fix:
1. Hide cursor on game start: `mlx_mouse_hide(g->mlx, g->mlx_win)` (or platform equivalent)
2. In `mouse_move()`: compute `delta_x = x - WIN_WIDTH / 2` instead of comparing to stored position; set `rotation_move = delta_x` (proportional float, not int ±2)
3. At end of every `mouse_move()` call (or end of `game_loop()`): call `move_mouse()` to recenter to (WIN_WIDTH/2, WIN_HEIGHT/2)
4. Store `delta_y` the same way for pitch (G1 can consume it)
5. Change `rotation_move` in `t_player` from `int` to `double`

#### G1 · Vertical look (pitch / horizon shift) `[✅]`
- Offset the horizon line up/down based on a pitch value
- Mouse Y axis controls it; keyboard fallback (PgUp/PgDn)
- **Week 1**

#### G2 · Textured floors & ceilings `[✅]`
- Standard floor-casting raycasting algorithm, sample texture per pixel
- Replaces the solid-color floor/ceiling pass in `draw_scene.c`
- **Week 1**

#### G3 · Sky texture `[✅]`
- Replace ceiling with a scrolling sky image when sky flag is set in level
- Scrolls horizontally with player angle, shifts with pitch
- **Week 1–2**

#### G4 · Billboard sprites (always face player) `[✅]`
- Sort sprites by distance, project onto screen, respect Z-buffer
- **Week 2**

#### G5 · Multi-angle sprites (directional characters) `[✅]`
- Pick one of 8 rotation frames based on angle between player and object
- Needed by enemies (S5)
- **Week 2**

#### S1 · Transparent / semi-transparent walls `[✅]`
- Multi-pass raycasting: continue ray after transparent surface, blend pixels
- **Week 3**

#### S2 · Wall decorations / decals `[✅]`
- Sprites anchored flat to a wall face (paintings, posters)
- **Week 3**

---

### 🟠 Murillo (world system + editor)

#### M1 · New level file format (packed self-sufficient file) `[ ]`
- One file bundles geometry, embedded textures, entity placements, sounds
- Write spec in `FORMAT.md` first, then the loader
- Replaces .cub parser
- **Week 1**

#### M2 · Sector-based map system (non-rectangular rooms) `[ ]`
- Rooms as convex polygons; walls at any angle
- Each sector: floor_z, ceil_z, floor tex, ceil tex, light level
- Core architectural change — everything else builds on this
- **Week 1–2**

#### M3 · Variable floor & ceiling heights `[ ]`
- Each sector has its own floor_z and ceil_z
- Player camera adjusts when walking between sectors
- **Week 2**

#### M4 · Inclined floors & ceilings `[ ]`
- Slope plane (ax + by + c = z) per sector floor/ceiling
- Camera bobs as player walks the slope
- **Week 2–3**

#### M5 · Per-room lighting `[ ]`
- Each sector has light level 0–255
- Darken walls/floor/ceiling by distance + sector light; sprites inherit it
- **Week 2**

#### S8 · Level editor (mandatory binary) `[ ]`
- `doom-nukem --edit <file>` or separate `doom-nukem-editor` binary
- Draw sectors, set heights, assign textures, place entities/triggers
- Export to packed level format (M1)
- **Week 3–4**

---

### 🟣 Rodolfo (gameplay systems)

#### R1 · Physics: jump, fall, crouch, run `[✅]`
- Jump + gravity; fall when stepping off ledge to lower sector
- Crouch shrinks eye height and collision box (Ctrl); run on Shift
- Current flat-grid implementation lands on the base floor; sector-height fall plugs
  into the same `PLAYER_FLOOR_Z` path when M3 exists.
- **Week 1**

#### R2 · Fly / swim mode `[✅]`
- Gravity disabled; vertical look moves player up/down
- Triggered by fly toggle (F) or entering a water sector
- Current implementation supports the fly toggle and the shared free-movement
  state; water sectors can enable the same state when sector metadata exists.
- **Week 2**

#### R3 · HUD system `[✅]`
- Distinct elements: health bar, ammo counter, inventory slots, currency/score
- Each element drawn individually on top of the frame buffer each frame
- **Week 1**

#### R4 · Inventory & item pickup `[✅]`
- Walk-over pickup; inventory struct with counts per type
- Items optionally block passage
- **Week 2**

#### R5 · Interaction system — proximity & action `[✅]`
- E key interact ray; proximity triggers (damage zones, text, scripts)
- Actions: open door, toggle switch, pick up item, play sound, display text
- Map chars: `T` switch (reuses `DECAL_WALL`), `H` hazard zone, `M` message zone.
  Switches enqueue the R6 timed sequence for doors/score/messages (no per-entity
  linking yet — needs M1/M2 level format). `play_sound_effect()` now uses the
  S7 file-backed audio backend and skips missing assets quietly.
- **Week 2**

#### R6 · Dynamic world events & scripted sequences `[✅]`
- Trigger → action map per level; timers, repeatable vs one-shot
- Supports elevators, texture swaps, height changes, secret passages
- Current grid implementation has a generic event queue and switch-driven timed
  sequences: message, score, delayed door toggle, and timed auto-close.
- **Week 3**

#### S3 · Text messages overlay `[✅]`
- Draw timed string on screen: story text, pickup messages
- Central `show_message()` / `update_message()` API is used by pickups,
  message zones, and scripted events.
- **Week 2**

#### S6 · Story, mission & level flow `[✅]`
- Objectives per level (reach exit, collect keys, kill boss)
- Level start/end condition, transition to next level or game over
- Current grid implementation adds `X` exit tiles, mission start messaging,
  collect-items-before-exit objective checks, mission complete/fail states, and
  delayed shutdown after the end message.
- **Week 3**

#### S7 · Sound effects & music `[✅]`
- WAV/OGG events (footstep, door, pickup, shoot, death) + looping music per level
- Use miniaudio (single-header, no extra deps) or SDL_mixer
- Current implementation plays event files from `assets/sounds/<event>.wav|ogg|mp3`
  asynchronously and loops `assets/sounds/music.wav|ogg|mp3` when present.
- **Week 4**

---

### 🤝 Shared — Week 3, whoever is unblocked first

#### S4 · Projectile system `[✅]`
- Shoot a ray or physical object (pos + velocity)
- Wall collision → decal; sprite collision → damage
- Current implementation fires fixed-pool physical projectiles with left mouse
  or `R`, consumes HUD ammo, marks wall hits as decal walls, and removes
  non-item sprite targets on hit.

#### S5 · Enemy AI & characters `[ ]`
- State machine: idle → patrol → alert → chase → attack
- Uses G5 multi-angle sprites; line-of-sight ray; takes damage, has health

---

## Bonuses — One Each, Low Effort

> Only evaluated if mandatory is perfect. Target: done by July 27 to leave buffer.

### 🔵 Gabriel — B7 · Weapon view model sprite
- Draw a static or frame-animated sprite anchored to the bottom-center of the screen
- Bob slightly with movement; swap sprite on fire
- **Why easy**: reuses the sprite rendering pipeline from G4 — just drawn at a fixed screen position instead of a world position. No raycasting involved.

### 🟠 Murillo — B6 · Multithreaded rendering (pthread)
- Divide screen columns into N equal bands (one per core)
- Spawn N pthreads, each renders its band independently, join before blit
- **Why easy**: the renderer already writes to a pixel buffer — just partition the column loop. No shared state between bands except the read-only world data.

### 🟣 Rodolfo — B1 · Level select menu
- Add a `MENU` game state before the main loop
- Render a simple screen: game title, list of available level files, difficulty selector
- Arrow keys navigate, Enter loads the selected level
- **Why easy**: it's a state machine with a few lines of text drawn to the screen. No new engine work — just reads the level file list and passes the chosen path to the existing loader.

---

## Architecture Notes

```
cub3D base
│
├── ENGINE (Gabriel owns)
│   ├── Renderer (draw_scene, ray_casting, dda)
│   ├── Sprite system (new — G4/G5)
│   └── Sky / pitch system (new — G1/G3)
│
├── WORLD (Murillo owns)
│   ├── Level format (replaces .cub parser — M1)
│   ├── Sector map (replaces grid map — M2)
│   └── Lighting (M5)
│
└── GAMEPLAY (Rodolfo owns)
    ├── Physics (jump/fall/crouch — R1)
    ├── HUD (R3)
    ├── Inventory (R4)
    └── Interaction / event system (R5/R6)
```

**Critical integration points — sync before touching these:**
| File / struct | Who owns it | Who else touches it |
|---|---|---|
| `t_game` | shared | all three add fields — agree on naming first |
| `t_player` | Rodolfo (physics) | Gabriel adds `pitch` field |
| Level file format | Murillo defines | Gabriel + Rodolfo consume |
| `game_loop.c` | shared | clear update order: input → physics → events → render |

---

## Progress Legend

```
[ ]  Not started
[~]  In progress
[x]  Done / merged
```

---

## Full Task Summary

| ID | Owner | Task | Week | Status |
|---|---|---|---|---|
| G0 | Gabriel | Fix mouse look (cursor lock, proportional delta, recenter) | 1 | `[x]` |
| G1 | Gabriel | Vertical look / pitch | 1 | `[x]` |
| G2 | Gabriel | Textured floors & ceilings | 1 | `[x]` |
| G3 | Gabriel | Sky texture | 1–2 | `[x]` |
| G4 | Gabriel | Billboard sprites | 2 | `[x]` |
| G5 | Gabriel | Multi-angle sprites | 2 | `[x]` |
| S1 | Gabriel | Transparent walls | 3 | `[x]` |
| S2 | Gabriel | Wall decals | 3 | `[x]` |
| M1 | Murillo | Level file format | 1 | `[ ]` |
| M2 | Murillo | Sector-based map | 1–2 | `[ ]` |
| M3 | Murillo | Variable floor/ceil heights | 2 | `[ ]` |
| M4 | Murillo | Inclined planes | 2–3 | `[ ]` |
| M5 | Murillo | Per-room lighting | 2 | `[ ]` |
| S8 | Murillo | Level editor | 3–4 | `[ ]` |
| R1 | Rodolfo | Physics (jump/fall/crouch/run) | 1 | `[x]` |
| R2 | Rodolfo | Fly / swim mode | 2 | `[x]` |
| R3 | Rodolfo | HUD system | 1 | `[x]` |
| R4 | Rodolfo | Inventory & item pickup | 2 | `[x]` |
| R5 | Rodolfo | Interaction system | 2 | `[x]` |
| R6 | Rodolfo | World events & triggers | 3 | `[x]` |
| S3 | Rodolfo | Text overlay | 2 | `[x]` |
| S4 | Shared | Projectile system | 3 | `[x]` |
| S5 | Shared | Enemy AI | 3 | `[ ]` |
| S6 | Rodolfo | Story / level flow | 3 | `[x]` |
| S7 | Rodolfo | Sound & music | 4 | `[x]` |
| B7 | Gabriel | Weapon view model (bonus) | 4 | `[ ]` |
| B6 | Murillo | Multithreaded rendering (bonus) | 4 | `[ ]` |
| B1 | Rodolfo | Level select menu (bonus) | 4 | `[ ]` |
