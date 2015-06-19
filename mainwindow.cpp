#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    field = new Field(ui->centralWidget);
    connect(field, SIGNAL(updateScore(int)), this, SLOT(updateScore(int)));
    connect(field, SIGNAL(diedNotify()), this, SLOT(diedNotify()));
    field->setFocus();
}

void MainWindow::diedNotify()
{
    ui->infoLabel->setText("Died");
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

void MainWindow::on_playButton_clicked()
{
    ui->infoLabel->clear();
    field->reset();
    field->setFocus();
}
