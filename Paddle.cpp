//
// Created by sebastian on 29.03.18.
//

#include <ncurses.h>
#include "Paddle.h"

int Paddle::WIDTH = PADDLE_WIDTH;

void Paddle::Move()
{
    if( (_y - WIDTH) == 0)
        _currentDir = DOWN;
    else if( (_y + WIDTH) >= MAX_HEIGHT)
        _currentDir = UP;

    if(_currentDir == DOWN)
        MoveDown();
    else
        MoveUp();
}

int Paddle::GetX()
{
    std::lock_guard<std::mutex> lock(paddleMutex);
    return _x;
}

int Paddle::GetY()
{
    std::lock_guard<std::mutex> lock(paddleMutex);
    return _y;
}

Paddle::Paddle() {

}

void Paddle::MoveDown()
{
    std::lock_guard<std::mutex> lock(paddleMutex);
    if(_y + PADDLE_WIDTH < MAX_WIDTH)
        _y++;
}

void Paddle::MoveUp()
{
    std::lock_guard<std::mutex> lock(paddleMutex);
    if (_y - PADDLE_WIDTH > 0)
        _y--;
}
