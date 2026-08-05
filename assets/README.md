# Assets

Project-owned runtime assets live under this directory:

- `images/hud/` — weapon, ammunition, and inventory HUD sprites
- `images/textures/blue/` — blue test-level surfaces, sky, decals, and sprites
- `images/textures/classic/` — classic cub3D/Wolfenstein-style textures
- `images/textures/doom/` — Doom-themed surfaces, decorations, and enemies
- `sounds/` — music and sound effects

Editable level sources in `tests/maps_src/` reference image files here. Packing
a level embeds every referenced image and sound in its self-contained `.dnk`
output.
