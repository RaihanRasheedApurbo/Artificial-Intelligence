#include "CustomButton.h"

CustomButton::CustomButton(int row, int col, QWidget *parent):QPushButton(parent)
{
    rowNumber = row;
    colNumber = col;
}

void CustomButton::setRed()
{

    this->setColor(Qt::red);

}

void CustomButton::setColor(Qt::GlobalColor color)
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Button, QColor(color));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->update();
}
