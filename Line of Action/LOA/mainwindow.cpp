#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setFixedSize(800,800);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_button1vs1_clicked()
{

    hide();
    int boardLength = 8;
    board = new Board(boardLength,this);
    board->show();

}
