#ifndef BOARD_H
#define BOARD_H

#include <QDialog>
#include <QDebug>
#include "CustomButton.cpp"
#include <QLabel>
namespace Ui {
class Board;
}

class Board : public QDialog
{
    Q_OBJECT

public:
    explicit Board(int len,QWidget *parent = nullptr );

    ~Board();
private slots:
    void handleClick();
private:
    Ui::Board *ui;
    QVector<QVector<CustomButton *>> buttons;
    QLabel *label;
    int turn;
    int selected;
    int selectedX;
    int selectedY;
};

#endif // BOARD_H
