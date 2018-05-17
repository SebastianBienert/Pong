//
// Created by sebastian on 29.03.18.
//

#include "Ball.h"
#include "Ball.h"
#include <ncurses.h>
#include <iostream>
#include <sstream>

int Ball::GetX()
{
    std::lock_guard<std::mutex> lock(_xMutex);
    return _x;
}

int Ball::GetY()
{
    std::lock_guard<std::mutex> lock(_yMutex);
    return _y;
}
void Ball::SetY(int y)
{
    std::lock_guard<std::mutex> lock(_yMutex);
    _y = y;
}
void Ball::SetX(int x)
{
    std::lock_guard<std::mutex> lock(_xMutex);
    _x = x;
}

void Ball::Move()
{
    if(paddleCollision())
        changeDirection();
    else if( GetY() < 2 || GetY() > MAX_HEIGHT)
        changeDirectionScreenBorders();


    int x = GetX();
    int y = GetY();

    if(_curDirection == UP)
        SetY(--y);
    else if(_curDirection == DOWN)
        SetY(++y);
    else if(_curDirection == LEFT)
        SetX(--x);
    else if(_curDirection == RIGHT)
        SetX(++x);
    else if(_curDirection == UP_RIGHT)
    {
        SetY(--y);
        SetX(++x);
    }
    else if(_curDirection == DOWN_RIGHT)
    {
        SetY(++y);
        SetX(++x);
    }
    else if(_curDirection == DOWN_LEFT)
    {
        SetY(++y);
        SetX(--y);
    }
    else if(_curDirection == UP_LEFT)
    {
        SetY(--y);
        SetY(--x);
    }
}

void Ball::changeDirection()
{
    switch(_curDirection)
    {
        case UP:
        {
            _curDirection = DOWN;
            break;
        }
        case UP_RIGHT:
        {
            _curDirection = UP_LEFT;
            break;
        }
        case RIGHT:
        {
            _curDirection = LEFT;
            break;
        }
        case DOWN_RIGHT:
        {
            _curDirection = DOWN_LEFT;
            break;
        }
        case DOWN:
        {
            _curDirection = UP;
            break;
        }
        case DOWN_LEFT:
        {
            _curDirection = DOWN_RIGHT;
            break;
        }
        case LEFT:
        {
            _curDirection = RIGHT;
            break;
        }
        case UP_LEFT:
        {
            _curDirection = UP_RIGHT;
            break;
        }
    }
}

bool Ball::paddleCollision()
{
    int leftPaddleX = _left->GetX();
    int rightPaddleX = _right->GetX();

    if( GetX() > (leftPaddleX + 2) && GetX() < (rightPaddleX - 2) )
        return false;

    //LEFT PADDLE
    int _leftPaddleY = _left->GetY();
    if(GetY() <= (_leftPaddleY + Paddle::WIDTH) && GetY() >= (_leftPaddleY- Paddle::WIDTH) && GetX() < MAX_WIDTH / 2)
        return true;

    //RIGHT PADDLE
    int _rightPaddleY = _right->GetY();
    if(GetY() <= (_rightPaddleY + Paddle::WIDTH) && GetY() >= (_rightPaddleY - Paddle::WIDTH) && GetX() > MAX_WIDTH / 2 )
        return true;

    return false;
}

void Ball::ChangeDirectionBallCollision()
{
    switch(_curDirection)
    {
        case UP:
        {
            _curDirection = DOWN;
            break;
        }
        case UP_RIGHT:
        {
            _curDirection = DOWN_LEFT;
            break;
        }
        case RIGHT:
        {
            _curDirection = LEFT;
            break;
        }
        case DOWN_RIGHT:
        {
            _curDirection = UP_LEFT;
            break;
        }
        case DOWN:
        {
            _curDirection = UP;
            break;
        }
        case DOWN_LEFT:
        {
            _curDirection = UP_RIGHT;
            break;
        }
        case LEFT:
        {
            _curDirection = RIGHT;
            break;
        }
        case UP_LEFT:
        {
            _curDirection = DOWN_RIGHT;
            break;
        }
    }
}

void Ball::changeDirectionScreenBorders()
{
    switch(_curDirection)
    {
        case UP:
        {
            _curDirection = DOWN;
            break;
        }
        case UP_RIGHT:
        {
            _curDirection = DOWN_RIGHT;
            break;
        }
        case RIGHT:
        {
            _curDirection = LEFT;
            break;
        }
        case DOWN_RIGHT:
        {
            _curDirection = UP_RIGHT;
            break;
        }
        case DOWN:
        {
            _curDirection = UP;
            break;
        }
        case DOWN_LEFT:
        {
            _curDirection = UP_LEFT;
            break;
        }
        case LEFT:
        {
            _curDirection = RIGHT;
            break;
        }
        case UP_LEFT:
        {
            _curDirection = DOWN_LEFT;
            break;
        }
    }
}

int Ball::GetColor()
{
    return _color;
}

bool Ball::IsRemoved()
{
    return _removed;
}
void Ball::Remove()
{
    _removed = true;
}

