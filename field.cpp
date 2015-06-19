#include "field.h"
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QTime>

Field::Field(QWidget *parent) : QWidget(parent), score(0)
{
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
    setGeometry(0, 0, fieldWidht + 1, fieldHeight + 1);
    timer.connect(&timer, SIGNAL(timeout()), this, SLOT(makeMove()));
    timer.setInterval(100);
    reset();
}

void Field::reset()
{
    timer.stop();
    setFocusPolicy(Qt::ClickFocus);

    for(int i = 0; i < cellCount; i++)
        for(int j = 0; j < cellCount; j++)
            cells[i][j] = CellState::free;

    cells[rand() % cellCount][rand() % cellCount] = CellState::food;
    int sx = rand() % cellCount;
    int sy = rand() % cellCount;
    cells[sx][sy] = CellState::snake;
    tail = head = Crd(sx, sy);
    nextHeadMoveDir = nextTailMoveDir = MoveDir::STOP;
    timer.start();
    repaint();
}

Field::Crd Field::MoveDir2Crd(MoveDir dir)
{
    quint8 dx = 0, dy = 0;
    switch (dir) {
    case MoveDir::UP:
        dy = -1;
        break;
    case MoveDir::DOWN:
        dy = 1;
        break;
    case MoveDir::LEFT:
        dx = -1;
        break;
    case MoveDir::RIGHT:
        dx = 1;
        break;
    }
    return Crd(dx, dy);
}

void Field::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        if (nextHeadMoveDir != MoveDir::DOWN)
        nextHeadMoveDir = MoveDir::UP;
        break;
    case Qt::Key_Down:
        if (nextHeadMoveDir != MoveDir::UP)
        nextHeadMoveDir = MoveDir::DOWN;
        break;
    case Qt::Key_Left:
        if (nextHeadMoveDir != MoveDir::RIGHT)
        nextHeadMoveDir = MoveDir::LEFT;
        break;
    case Qt::Key_Right:
        if (nextHeadMoveDir != MoveDir::LEFT)
        nextHeadMoveDir = MoveDir::RIGHT;
        break;
    }
}

void Field::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QBrush redBrush(Qt::red, Qt::SolidPattern);
    QBrush whiteBrush(Qt::white, Qt::SolidPattern);
    QBrush grinBrush(Qt::green, Qt::SolidPattern);
    p.setBrush(whiteBrush);

    int cellsX = 0, cellsY = 0;
    for (int x = 0; x < fieldWidht; x += cellSize)
    {
        cellsY = 0;
        for (int y = 0; y < fieldHeight; y += cellSize)
        {
            switch (cells[cellsX][cellsY]) {
            case CellState::snake:
                p.setBrush(redBrush);
                p.drawRect(QRect(x, y, cellSize, cellSize));
                p.setBrush(whiteBrush);
                break;
            case CellState::food:
                p.setBrush(grinBrush);
                p.drawRect(QRect(x, y, cellSize, cellSize));
                p.setBrush(whiteBrush);
                break;
            default:
                p.drawRect(QRect(x, y, cellSize, cellSize));
                break;
            }
           cellsY++;
        }
        cellsX++;
    }
}

bool Field::inField(int x, int y) const
{
    if ((x < 0 || x > fieldWidht ) ||
        (y < 0 || y > fieldHeight))
        return false;
    return true;
}

void Field::die()
{
    setFocusPolicy(Qt::NoFocus);
    timer.stop();
    emit diedNotify();
}

void Field::resetTail()
{
    int x = tail.first, y = tail.second;
    cells[x][y] = CellState::free;

    Crd prob = MoveDir2Crd(nextTailMoveDir);
    if ((cells[x + prob.first][y + prob.second] == CellState::snake) &&
        inField(x + prob.first, y + prob.second))
    {
        tail = Crd(x + prob.first, y + prob.second);
        return;
    }

    if ((cells[x + 1][y] == CellState::snake) &&
            inField(x + 1, y))
    {
        tail = Crd(x + 1, y);
        nextTailMoveDir = MoveDir::RIGHT;
        return;
    }

    if ((cells[x - 1][y] == CellState::snake) &&
            inField(x - 1, y))
    {
        tail = Crd(x - 1, y);
        nextTailMoveDir = MoveDir::LEFT;
        return;
    }

    if ((cells[x][y - 1] == CellState::snake) &&
            inField(x, y - 1))
    {
        tail = Crd(x, y - 1);
        nextTailMoveDir = MoveDir::UP;
        return;
    }

    if ((cells[x][y + 1] == CellState::snake) &&
            inField(x, y + 1))
    {
        tail = Crd(x, y + 1);
        nextTailMoveDir = MoveDir::DOWN;
        return;
    }
}

void Field::makeMove()
{
    Crd move = MoveDir2Crd(nextHeadMoveDir);
    int dx = move.first;
    int dy = move.second;

    if (dx == dy) // dx == dy == 0 (Stoped)
        return;

    head.first += dx; head.second += dy;

    if (!inField(head.first, head.second) ||
         cells[head.first][head.second] == CellState::snake )
    {
        die();
        return;
    }

    if (cells[head.first][head.second] == CellState::food)
    {
       cells[head.first][head.second] = CellState::snake;
       placeFood(true);
       score += 10;
       emit updateScore(score);
    } else
    {
       cells[head.first][head.second] = CellState::snake; //needs to be before resetTail() for correct resetting
       resetTail();
    }

    placeFood();
    repaint();
}

void Field::placeFood(bool immediatly)
{
    int x, y;
    if ((qrand() % 50) == 1 || immediatly)
    {
        x = rand() % cellCount;
        y = rand() % cellCount;
        if (cells[x][y] == CellState::free)
        {
            cells[x][y] = CellState::food;
        }
    }
}
