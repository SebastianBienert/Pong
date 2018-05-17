//
// Created by sebastian on 29.03.18.
//

#ifndef PONGV2_BALL_H
#define PONGV2_BALL_H

#include <vector>
#include "Paddle.h"

class Ball
{
private:
    int _x;
    int _y;
    int _color;
    int _removed = false;
    bool notified = false;

    std::mutex _xMutex;
    std::mutex _yMutex;
    std::shared_ptr<Paddle> _left;
    std::shared_ptr<Paddle> _right;
    std::vector<std::shared_ptr<Ball>> _balls;
    Direction _curDirection;

    bool paddleCollision();
    void changeDirection();
    void changeDirectionScreenBorders();
public:
    Ball(int initX, int initY, std::vector<std::shared_ptr<Ball>> balls,
         std::shared_ptr<Paddle> left, std::shared_ptr<Paddle> right, int color,
        Direction dir = UP_LEFT)
    {
        _x = initX;
        _y = initY;
        _color = color;
        _left = left;
        _right = right;
        _balls = balls;
        _curDirection = dir;
//        switch(rand () % 4)
//        {
//            case 0:
//            {
//                _curDirection = UP_RIGHT;
//                break;
//            }
//            case 1:
//            {
//                _curDirection = UP_LEFT;
//                break;
//            }
//            case 2:
//            {
//                _curDirection = DOWN_RIGHT;
//                break;
//            }
//            case 3:
//            {
//                _curDirection = DOWN_LEFT;
//                break;
//            }
//        }
        if(_curDirection == UP || _curDirection == DOWN)
            _curDirection = UP_LEFT;
    }
    int GetX();
    int GetY();
    void SetX(int x);
    void SetY(int y);
    void ChangeDirectionBallCollision();
    bool IsRemoved();
    void Remove();
    int GetColor();
    void Move();

};


#endif //PONGV2_BALL_H
