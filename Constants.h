//
// Created by sebastian on 29.03.18.
//

#ifndef PONGV2_CONSTANTS_H
#define PONGV2_CONSTANTS_H
#include <mutex>

const int MAX_HEIGHT = 40;
const int PADDLE_WIDTH = 5;
const int MAX_WIDTH = 70;

enum Direction {
    UP = 0,
    UP_RIGHT = 1,
    RIGHT = 2,
    DOWN_RIGHT = 3,
    DOWN = 4,
    DOWN_LEFT = 5,
    LEFT = 6,
    UP_LEFT = 7
};
#endif //PONGV2_CONSTANTS_H
