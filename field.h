#ifndef FIELD_H
#define FIELD_H

#include <QWidget>
#include <QPair>
#include <QTimer>

enum class CellState : quint8
{
    free,
    snake,
    food
};

enum class MoveDir : quint8
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STOP
};

class Field : public QWidget
{
    Q_OBJECT
public:
    explicit Field(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent * event);
    void paintEvent(QPaintEvent *);
    void reset();

private:
    static const int cellCount = 34;
    static const int fieldWidht = 510;
    static const int fieldHeight = 510;
    static const int cellSize = 15;
    static const int defStepDeley = 220;

    using Crd = QPair<qint8, qint8>;

    Crd MoveDir2Crd(MoveDir dir);
    bool inField(int x, int y) const;
    void die();
    void resetTail();
    void placeFood(bool immediatly = false);

    CellState cells[cellCount][cellCount];
    Crd head, tail;
    MoveDir nextHeadMoveDir;
    MoveDir nextTailMoveDir;
    QTimer timer;
    int score;
    int stepDeley;

signals:
    void diedNotify();
    void updateScore(int);
public slots:
    void makeMove();
};

#endif // FIELD_H
