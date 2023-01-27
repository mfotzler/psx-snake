# psx-snake
Snake game for playstation

## Building
Assuming you have the PSn00bSDK installed and working:

`cmake --preset default .` will setup the build directory
`cmake --build ./build` will build the game

Upon successful build, the disc image will be `./build/template.cue` and `./build/template.bin`.