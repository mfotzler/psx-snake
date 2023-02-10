//
// Created by mfotz on 2/10/2023.
//

#ifndef PSX_SNAKE_TYPES_H
#define PSX_SNAKE_TYPES_H

typedef struct {
    TILE *tile;
    int x;
    int y;
    bool isGameOver;
    PADTYPE *pad;
} GameState;

#endif //PSX_SNAKE_TYPES_H
