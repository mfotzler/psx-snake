#include <stdbool.h>
#include <psxgpu.h>
#include <psxpad.h>
#include <stdlib.h>
#include "types.h"

#define SNAKE_SPEED 2

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
        if (isButtonPressed(gameState->pad, PAD_RIGHT)) {
            gameState->velocityX = SNAKE_SPEED;
            gameState->velocityY = 0;
        }
        if (isButtonPressed(gameState->pad, PAD_LEFT)) {
            gameState->velocityX = -SNAKE_SPEED;
            gameState->velocityY = 0;
        }
        if (isButtonPressed(gameState->pad, PAD_DOWN)) {
            gameState->velocityX = 0;
            gameState->velocityY = SNAKE_SPEED;
        }
        if (isButtonPressed(gameState->pad, PAD_UP)) {
            gameState->velocityX = 0;
            gameState->velocityY = -SNAKE_SPEED;
        }

        gameState->x += gameState->velocityX;
        gameState->y += gameState->velocityY;
        gameState->pixelsMovedSinceLastTailUpdate += abs(gameState->velocityX) + abs(gameState->velocityY);
    }
}

void updateIsFoodEaten(GameState *gameState) {
    if (isColliding(gameState->x, gameState->y, 10, 10, gameState->foodX, gameState->foodY, 10, 10)) {
        gameState->isFoodEaten = true;
    }
}

void updateTail(GameState *gameState) {
    for (int i = gameState->tailLength; i > 0; i--) {
        gameState->tail[i][0] = gameState->tail[i - 1][0];
        gameState->tail[i][1] = gameState->tail[i - 1][1];
    }
    gameState->tail[0][0] = gameState->x;
    gameState->tail[0][1] = gameState->y;
    gameState->pixelsMovedSinceLastTailUpdate = 0;
}

bool shouldUpdateTail(GameState *gameState) {
    return gameState->pixelsMovedSinceLastTailUpdate > PLAYER_SIZE;
}

void processGameLogic(GameState *gameState) {
    updateIsFoodEaten(gameState);
    if(gameState->isFoodEaten) {
        gameState->foodX = rand() % 280 + 20;
        gameState->foodY = rand() % 200 + 20;
        gameState->tailLength++;
        gameState->isFoodEaten = false;
    }

    if (!gameState->isGameOver) {
        updatePositionFromPad(gameState);
        if (shouldUpdateTail(gameState))
            updateTail(gameState);
    }

    if (isPlayerCollidingWithWall(gameState->x, gameState->y)) {
        gameState->isGameOver = true;
    }

    if (gameState->isGameOver) {
        FntPrint(-1, "GAME OVER. PRESS START TO RESTART");
        FntFlush(-1);
    }
}