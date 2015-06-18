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

private:

    using Crd = QPair<qint8, qint8>;

    bool inField(int x, int y) const;
    void die();
    void resetTail();
    static const int cellCount = 34;
    static const int fieldWidht = 510;
    static const int fieldHeight = 510;
    static const int cellSize = 15;
    CellState cells[cellCount][cellCount];
    Crd head, tail;
    MoveDir nextMoveDir;
    QTimer timer;


signals:
    void diedNotify();
public slots:
    void makeMove();
};

#endif // FIELD_H
