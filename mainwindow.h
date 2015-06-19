#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "field.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateScore(int);
    void diedNotify();

private slots:
    void on_playButton_clicked();

private:
    Ui::MainWindow *ui;
    Field* field;
};

#endif // MAINWINDOW_H
