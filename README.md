# The river2D map editor!

a preview (outdated for this release, this was 0.0.1):

![preview](assets/preview.gif)

## current state

since I am currently working on a fairly small game (16x16 tiles on a 360p base canvas), the abilities of the editor reflect that right now.
but, you can:

- open the editor and start placing tiles!
- to load tiles, you must place your tilesheet in `assets/tiles/tilesheet.qoi`.
- saving and loading projects is possible. `mapedit` will load whatever `.rte` file you have placed in the root folder (where `src` and `include` live). It will save by pressing `s` in the editor.
- `t` from the editor will toggle the tile selection menu.
- `ctrl+z` and `ctrl+y` are redo and undo, there's now a proper action history.
- `0` through `9` will switch the layer you can place onto
- `escape` will toggle the main menu
- `q` from the main menu will quit the editor

## known issues / future plans
- scaling is blurry on windows.
- the smallest tilesize is hardcoded to `8x8` (for now).
