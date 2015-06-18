#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    field = new Field( ui->centralWidget );
    connect(field, SIGNAL(updateScore(int)), this, SLOT(updateScore(int)));
}

MainWindow::~MainWindow()
{
    delete field;
    delete ui;
}

void MainWindow::updateScore(int score)
{
    ui->scoreDisplay->display(score);
}
