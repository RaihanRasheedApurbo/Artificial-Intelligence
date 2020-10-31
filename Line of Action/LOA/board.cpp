#include "board.h"
#include "ui_board.h"
#include <QPushButton>
#include <QVector>
#include <QLabel>
#include <string>
Board::Board(int len, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Board)
{
    ui->setupUi(this);
    //qDebug()<<len;

    bool white;// color for button
    int buttonDensity = 75; // used to offset the ui
    for(int i=0;i<len;i++)
    {
        QVector<CustomButton *> t;
        int currentRowY = i*buttonDensity;

        if(i%2==0)
        {
            white = true;
        }
        else
        {
            white = false;
        }
        for(int j=0;j<len;j++)
        {
            CustomButton *buttonA = new CustomButton(i,j,this);
            buttonA->setText("");
            buttonA->move(j*buttonDensity, currentRowY);
            buttonA->resize(buttonDensity,buttonDensity);
            QFont font = buttonA->font();
            font.setPointSize(50);
            buttonA->setFont(font);
            t.append(buttonA);

            QPalette pal = buttonA->palette();
            if(white)
            {
                pal.setColor(QPalette::Button, QColor(Qt::white));
                buttonA->blockColor = "white";
                white = false;
            }
            else
            {
                pal.setColor(QPalette::Button, QColor(Qt::yellow));
                buttonA->blockColor = "yellow";
                white = true;
            }


            buttonA->setAutoFillBackground(true);
            buttonA->setPalette(pal);
            buttonA->update();
            connect(buttonA,SIGNAL(clicked()),this,SLOT(handleClick()));


        }
        buttons.append(t);

    }


    for(int i=1;i<len-1;i++)
    {
        buttons[0][i]->setText("O");
        buttons[len-1][i]->setText("O");
        buttons[i][0]->setText("X");
        buttons[i][len-1]->setText("X");


    }

    label = new QLabel(this);
    //label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    //label->setText("f");
    //label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    label->move(625,550);




    //qDebug()<<buttons.size();
    //state assignment
    turn = 1;
    selected = 0;
    selectedX = -5;
    selectedY = -5;
    std::string s = "turn: "+std::to_string(turn) + " selected: " +std::to_string(selected);
    s += "\nselectedX: " + std::to_string(selectedX) + " selectedY: " + std::to_string(selectedY);
    label->setText(QString::fromStdString(s));

}

Board::~Board()
{
    delete ui;
}

void Board::handleClick()
{
    //QObject *t = sender();
    CustomButton *t1 = (CustomButton *) sender();

//    qDebug()<<"inside handle click";
//    qDebug()<<t1->text();
//    qDebug()<<t1->rowNumber<<t1->colNumber;
//    qDebug()<<t1->colNumber;
//    std::string s = std::to_string(5);
//    qDebug()<<QString::fromStdString(s);
//    std::string s;
//    s = std::to_string(turn);
//    qDebug()<<turn;
//    s = std::to_string(selected);
//    qDebug()<<selected;

    if(turn == 1 && selected == 0)
    {
        if(t1->text()=="O")
        {
            selected = 1;
            selectedX = t1->rowNumber;
            selectedY = t1->colNumber;
            QPalette pal = t1->palette();
            pal.setColor(QPalette::Button, QColor(Qt::red));
            t1->setAutoFillBackground(true);
            t1->setPalette(pal);
            t1->update();

        }
    }
    else if(turn == 1 && selected == 1)
    {
        if(selectedX == t1->rowNumber && selectedY == t1->colNumber) // selected the same block again so discard selection
        {
            selected = 0;
            selectedX = -5;
            selectedY = -5;
            QPalette pal = t1->palette();
            if(t1->blockColor == "white")
            {
                pal.setColor(QPalette::Button, QColor(Qt::white));
            }
            else
            {
                pal.setColor(QPalette::Button, QColor(Qt::yellow));
            }

            t1->setAutoFillBackground(true);
            t1->setPalette(pal);
            t1->update();
        }
        else if(t1->text() == "O") // can't go here because same team's piece exist in this block
        {
            // doing nothing
        }
        else if(t1->text() == "X")
        {
            // currently doing nothing but in future i will
        }
        else if(t1->text()== "") // empty place
        {


            t1->setText("O");

            CustomButton* t2 = buttons[selectedX][selectedY];
            t2->setText("");
            QPalette pal = t2->palette();
            if(t2->blockColor == "white")
            {
                pal.setColor(QPalette::Button, QColor(Qt::white));
            }
            else
            {
                pal.setColor(QPalette::Button, QColor(Qt::yellow));
            }

            t2->setAutoFillBackground(true);
            t2->setPalette(pal);
            t2->update();



            turn = 2;
            selected = 0;
            selectedX = -5;
            selectedY = -5;

        }
    }






    else if(turn == 2 && selected == 0)
    {
        if(t1->text()=="X")
        {
            selected = 1;
            selectedX = t1->rowNumber;
            selectedY = t1->colNumber;
            QPalette pal = t1->palette();
            pal.setColor(QPalette::Button, QColor(Qt::red));
            t1->setAutoFillBackground(true);
            t1->setPalette(pal);
            t1->update();

        }
    }
    else if(turn == 2 && selected == 1)
    {
        if(selectedX == t1->rowNumber && selectedY == t1->colNumber) // selected the same block again so discard selection
        {
            selected = 0;
            selectedX = -5;
            selectedY = -5;
            QPalette pal = t1->palette();
            if(t1->blockColor == "white")
            {
                pal.setColor(QPalette::Button, QColor(Qt::white));
            }
            else
            {
                pal.setColor(QPalette::Button, QColor(Qt::yellow));
            }

            t1->setAutoFillBackground(true);
            t1->setPalette(pal);
            t1->update();
        }
        else if(t1->text() == "X") // can't go here because same team's piece exist in this block
        {
            // doing nothing
        }
        else if(t1->text() == "O")
        {
            // currently doing nothing but in future i will
        }
        else if(t1->text()== "") // empty place
        {


            t1->setText("X");

            CustomButton* t2 = buttons[selectedX][selectedY];
            t2->setText("");
            QPalette pal = t2->palette();
            if(t2->blockColor == "white")
            {
                pal.setColor(QPalette::Button, QColor(Qt::white));
            }
            else
            {
                pal.setColor(QPalette::Button, QColor(Qt::yellow));
            }

            t2->setAutoFillBackground(true);
            t2->setPalette(pal);
            t2->update();



            turn = 1;
            selected = 0;
            selectedX = -5;
            selectedY = -5;

        }
    }

    std::string s = "turn: "+std::to_string(turn) + " selected: " +std::to_string(selected);
    s += "\nselectedX: " + std::to_string(selectedX) + " selectedY: " + std::to_string(selectedY);
    label->setText(QString::fromStdString(s));


}
