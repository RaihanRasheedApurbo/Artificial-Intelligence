#include<QPushButton>
#include<QString>
class CustomButton : public QPushButton{
public:
    int rowNumber;
    int colNumber;
    QString blockColor;
    CustomButton(int row, int col, QWidget *parent = 0):QPushButton(parent)
    {
        rowNumber = row;
        colNumber = col;
    }

};
