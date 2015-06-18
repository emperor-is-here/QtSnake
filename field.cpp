#include "field.h"
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QTime>

Field::Field(QWidget *parent) : QWidget(parent), score(0)
{
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
    setFocusPolicy(Qt::ClickFocus);
    setGeometry(0, 0, fieldWidht + 1, fieldHeight + 1);

    for(int i = 0; i < cellCount; i++)
        for(int j = 0; j < cellCount; j++)
            cells[i][j] = CellState::free;

    cells[10][5] = CellState::snake;
    tail = head = Crd(10, 5);
    nextMoveDir = MoveDir::STOP;

    timer.connect(&timer, SIGNAL(timeout()), this, SLOT(makeMove()));
    timer.setInterval(200);
    timer.start();
}

void Field::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        nextMoveDir = MoveDir::UP;
        break;
    case Qt::Key_Down:
        nextMoveDir = MoveDir::DOWN;
        break;
    case Qt::Key_Left:
        nextMoveDir = MoveDir::LEFT;
        break;
    case Qt::Key_Right:
        nextMoveDir = MoveDir::RIGHT;
        break;

    default:
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

   // qDebug() << x << " " << y << inField(x, y);

    if ((cells[x + 1][y] == CellState::snake) &&
            inField(x + 1, y))
    {
        tail = Crd(x + 1, y);
        return;
    }

    if ((cells[x - 1][y] == CellState::snake) &&
            inField(x - 1, y))
    {
        tail = Crd(x - 1, y);
        return;
    }

    if ((cells[x][y - 1] == CellState::snake) &&
            inField(x, y - 1))
    {
        tail = Crd(x, y - 1);
        return;
    }

    if ((cells[x][y + 1] == CellState::snake) &&
            inField(x, y + 1))
    {
        tail = Crd(x, y + 1);
        return;
    }
}

void Field::makeMove()
{
    int dx = 0, dy = 0;
    switch (nextMoveDir) {
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
    default:
        return;
        break;
    }

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
    if ((qrand() % 10) == 1 || immediatly)
    {
        x = rand() % cellCount;
        y = rand() % cellCount;
        if (cells[x][y] == CellState::free)
        {
            cells[x][y] = CellState::food;
        }
    }
}
