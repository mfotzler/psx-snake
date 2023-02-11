#ifndef PSX_SNAKE_TYPES_H
#define PSX_SNAKE_TYPES_H

#define PLAYER_SIZE 10

typedef struct {
    TILE *tile;
    int x;
    int y;
    int tailLength;
    short pixelsMovedSinceLastTailUpdate;
    short tail[100][2];
    short velocityX;
    short velocityY;
    int foodX;
    int foodY;
    bool isFoodEaten;
    bool isGameOver;
    PADTYPE *pad;
} GameState;

#endif //PSX_SNAKE_TYPES_H
