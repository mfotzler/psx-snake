#include <stdbool.h>
#include <psxgpu.h>
#include <psxpad.h>
#include "types.h"

bool isColliding(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

bool isPlayerCollidingWithWall(int x, int y) {
    return isColliding(x, y, 10, 10, 0, 0, 320, 20) ||
           isColliding(x, y, 10, 10, 0, 220, 320, 20) ||
           isColliding(x, y, 10, 10, 0, 0, 20, 240) ||
           isColliding(x, y, 10, 10, 300, 0, 20, 240);
}

bool isController1Connected(GameState *gameState) {
    return gameState->pad->stat == 0;
}

bool isButtonPressed(PADTYPE *pad, PadButton button) {
    return !(pad->btn & button);
}

void updatePositionFromPad(GameState *gameState) {
    if (isController1Connected(gameState)) {
        if (isButtonPressed(gameState->pad, PAD_RIGHT))
            gameState->x += 4;
        if (isButtonPressed(gameState->pad, PAD_LEFT))
            gameState->x -= 4;
        if (isButtonPressed(gameState->pad, PAD_DOWN))
            gameState->y += 4;
        if (isButtonPressed(gameState->pad, PAD_UP))
            gameState->y -= 4;
    }
}

void processGameLogic(GameState *gameState) {
    if (!gameState->isGameOver) {
        updatePositionFromPad(gameState);
    }

    if (isPlayerCollidingWithWall(gameState->x, gameState->y)) {
        gameState->isGameOver = true;
    }

    if (gameState->isGameOver) {
        FntPrint(-1, "GAME OVER");
        FntFlush(-1);
    }
}