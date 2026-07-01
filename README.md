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
- Esc or window close button: quit cleanly
