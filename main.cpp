#include <iostream>
#include <ncurses.h>
#include <thread>
#include <vector>
#include <atomic>
#include <random>
#include <condition_variable>
#include "Paddle.h"
#include "Ball.h"

int BALLS_NUM = 4;
std::atomic<int> ballsRemoved{0};
auto PrintMutex = std::make_shared<std::mutex>();

std::condition_variable processingCollision;
bool collisionProcessed = true;
std::mutex processingCollisionMutex;

std::atomic<int> BALLS_MOVED = {0};

void initColors()
{
    init_pair(0, COLOR_RED, COLOR_BLACK);
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
}

void MovePaddle(std::shared_ptr<Paddle> paddle)
{
    for(int i = 0; ;i++)
    {

        paddle->Move();

        if(ballsRemoved >= BALLS_NUM)
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

}

void MoveBall(std::shared_ptr<Ball> ball)
{
    do
    {
        std::unique_lock<std::mutex> lock(processingCollisionMutex);
        processingCollision.wait(lock, [](){return collisionProcessed;});
            ball->Move();
            collisionProcessed = false;
            BALLS_MOVED++;
            lock.unlock();
        processingCollision.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }while(ball->GetX() > 1 && ball->GetX() < (MAX_WIDTH - 1) );

    ball->Remove();
    ballsRemoved++;
    std::cout<<ballsRemoved;
}

void ControlPaddle(std::shared_ptr<Paddle> paddle)
{
    for(int i = 0; ;i++)
    {
        PrintMutex->lock();
        int dir = getch();
        PrintMutex->unlock();

        switch(dir)
        {
            case 's':
            {
                paddle->MoveDown();
                break;
            }
            case 'w':
            {
                paddle->MoveUp();
                break;
            }
            case 'q':
            {
                ballsRemoved = BALLS_NUM;
                break;
            }
            default:
            {
                break;
            }
        }
        if(ballsRemoved >= BALLS_NUM)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void printScreen(std::vector<std::shared_ptr<Ball>> balls, std::shared_ptr<Paddle> left, std::shared_ptr<Paddle> right)
{
    while(ballsRemoved < BALLS_NUM)
    {
        PrintMutex->lock();
        clear();
        for (auto & ball : balls)
        {
            if(!ball->IsRemoved())
            {
                attron(COLOR_PAIR(ball->GetColor()));
                mvprintw(ball->GetY(), ball->GetX(), "o");
                attroff(COLOR_PAIR(ball->GetColor()));
            }
        }

        for (int i = 1; i <= PADDLE_WIDTH; i++) {
            mvprintw(left->GetY() - i, left->GetX(), "|");
            mvprintw(left->GetY() + i, left->GetX(), "|");
        }
        mvprintw(left->GetY(), left->GetX(), "|");

        for (int i = 1; i <= PADDLE_WIDTH; i++) {
            mvprintw(right->GetY() - i, right->GetX(), "|");
            mvprintw(right->GetY() + i, right->GetX(), "|");
        }
        mvprintw(right->GetY(), right->GetX(), "|");

        refresh();
        PrintMutex->unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int BallsAtPosition(int x, int y, std::vector<std::shared_ptr<Ball>> balls)
{
    int result = 0;
    for(auto ball : balls)
    {
        if(ball->GetX() == x && ball->GetY() == y)
            result++;
    }
    return result;
}

void ControlBallCollision(std::vector<std::shared_ptr<Ball>> balls)
{
    while(ballsRemoved < BALLS_NUM)
    {
        std::unique_lock<std::mutex> lock(processingCollisionMutex);
        processingCollision.wait(lock, [](){return !collisionProcessed || ballsRemoved < BALLS_NUM;});
            for(auto & ball : balls)
            {
                if(BallsAtPosition(ball->GetX(), ball->GetY(), balls) > 1)
                    ball->ChangeDirectionBallCollision();
            }
            BALLS_MOVED = 0;
            collisionProcessed = true;
            lock.unlock();
        processingCollision.notify_all();
    }
}



int main()
{

    srand(time(0));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> xRandom(1, MAX_WIDTH);
    std::uniform_int_distribution<> yRandom(1, MAX_HEIGHT);

    auto PaddleLeft = std::make_shared<Paddle>(1,20);
    auto PaddleRight = std::make_shared<Paddle>(MAX_WIDTH,20);
    std::thread ballThreads[BALLS_NUM];
    std::vector<std::shared_ptr<Ball>> balls;
    for(int i = 0; i < 2; i++)
    {
       // balls.push_back(std::make_shared<Ball>(xRandom(gen),yRandom(gen),balls, PaddleLeft, PaddleRight,i));
    }

    balls.push_back(std::make_shared<Ball>(5,MAX_HEIGHT / 2,balls, PaddleLeft, PaddleRight,0, RIGHT));
    balls.push_back(std::make_shared<Ball>(MAX_WIDTH - 5, MAX_HEIGHT / 2,balls, PaddleLeft, PaddleRight,3, LEFT));
    balls.push_back(std::make_shared<Ball>(5,MAX_HEIGHT / 2 + 5,balls, PaddleLeft, PaddleRight,2, RIGHT));
    balls.push_back(std::make_shared<Ball>(MAX_WIDTH - 5, MAX_HEIGHT / 2 + 5,balls, PaddleLeft, PaddleRight,6, LEFT));


    initscr();
    noecho();
    nodelay(stdscr, TRUE);
    start_color();
    initColors();
    curs_set(FALSE);

    for(int i = 0; i < balls.size(); i++)
    {
        ballThreads[i] = std::thread(MoveBall, balls[i]);
    }

    std::thread paddleLeftThread(ControlPaddle, PaddleLeft);
    std::thread paddleRightThread(MovePaddle, PaddleRight);
    std::thread refreshThread(printScreen, balls, PaddleLeft, PaddleRight);
    std::thread controlBallsCollisionThread(ControlBallCollision, balls);

    for(int i = 0; i < balls.size(); i++)
    {
        ballThreads[i].join();
    }

    paddleLeftThread.join();
    refreshThread.join();
    paddleRightThread.join();
    controlBallsCollisionThread.join();

    erase();
    refresh();
    endwin();
    return 0;
}
