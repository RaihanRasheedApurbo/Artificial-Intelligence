#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H




#include<QPushButton>
#include<QString>
class CustomButton : public QPushButton{
public:
    int rowNumber;
    int colNumber;
    QString blockColor;
    CustomButton(int row, int col, QWidget *parent = 0);
    void setRed();
    void setColor(Qt::GlobalColor color);


};

#endif // CUSTOMBUTTON_H
