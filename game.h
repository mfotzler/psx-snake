#include "types.h"

#ifndef PSX_SNAKE_GAME_H
#define PSX_SNAKE_GAME_H

void processGameLogic(GameState *gameState);
bool isButtonPressed(PADTYPE *pad, PadButton button);

#endif //PSX_SNAKE_GAME_H
