//
// Created by sebastian on 29.03.18.
//

#ifndef PONGV2_PADDLE_H
#define PONGV2_PADDLE_H
#include "Constants.h"
#include <memory>
#include <atomic>
#include <mutex>
class Paddle
{
private:
    int _x;
    int _y;
    Direction _currentDir;
    std::mutex paddleMutex;
public:
    static int WIDTH;
    Paddle(int x, int y){
        _x = x;
        _y = y;
        _currentDir = DOWN;
    }

    Paddle();
    int GetY();
    int GetX();

    void MoveDown();
    void MoveUp();
    void Move();
};


#endif //PONGV2_PADDLE_H
