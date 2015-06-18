#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    field = new Field( ui->centralWidget );
}

MainWindow::~MainWindow()
{
    delete field;
    delete ui;
}
