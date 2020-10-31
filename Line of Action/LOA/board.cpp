#include "board.h"
#include "ui_board.h"
#include <QPushButton>
#include <QVector>
#include <QLabel>
#include <string>
#include <bits/stdc++.h>
using namespace std;
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
        std::vector<int> t1;
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

            t1.push_back(0);


        }
        boardMatrix.push_back(t1);
        buttons.append(t);

    }


    for(int i=1;i<len-1;i++)
    {
        buttons[0][i]->setText("O");
        buttons[len-1][i]->setText("O");
        boardMatrix[0][i] = 1;
        boardMatrix[len-1][i] = 1;


        buttons[i][0]->setText("X");
        buttons[i][len-1]->setText("X");
        boardMatrix[i][0] = 2;
        boardMatrix[i][len-1] = 2;


    }

    statusLabel = new QLabel(this);
    //label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    //label->setText("f");
    //label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    statusLabel->move(625,550);

    boardLabel = new QLabel(this);
    boardLabel->move(625,35);




    //qDebug()<<buttons.size();
    //state assignment
    turn = 1;
    selected = 0;
    selectedX = -5;
    selectedY = -5;
    player1Pieces = 2*(len-2);
    player2Pieces = 2*(len-2);

    printLabels();

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
            findNextMove();
            //markNextMOve();

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
        else if(t1->text()== "" || t1->text() == "X") // empty place or opponent piece
        {

            if(t1->text()=="X")
            {
                player2Pieces--;
            }
            t1->setText("O");

            CustomButton* t2 = buttons[selectedX][selectedY];
            t2->setText("");

            boardMatrix[selectedX][selectedY] = 0;
            boardMatrix[t1->rowNumber][t1->colNumber] = 1;



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





            bool over = checkGameOver();

            if(over==false)
            {
                turn = 2;

            }
            else
            {
                turn = 0;
            }

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
            findNextMove();



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
        else if(t1->text()== "" || t1->text() == "O") // empty place or opponent piece
        {

            if(t1->text()=="O")
            {
                player1Pieces--;
            }
            t1->setText("X");

            CustomButton* t2 = buttons[selectedX][selectedY];
            if(t2->text()=="O")
            {
                player1Pieces--;
            }
            t2->setText("");
            boardMatrix[selectedX][selectedY] = 0;
            boardMatrix[t1->rowNumber][t1->colNumber] = 2;


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




            bool over = checkGameOver();

            if(over==false)
            {
                turn = 1;

            }
            else
            {
                turn = 0;
            }

            selected = 0;
            selectedX = -5;
            selectedY = -5;



        }
    }

    printLabels();


}

void Board::printLabels()
{
    //printing status
    std::string s = "turn: "+std::to_string(turn) + " selected: " +std::to_string(selected);
    s += "\nselectedX: " + std::to_string(selectedX) + " selectedY: " + std::to_string(selectedY);
    statusLabel->setText(QString::fromStdString(s));

    //printing board
    s = "Board Matrix:\n";
    for(int i=0,len = boardMatrix.size();i<len;i++)
    {

        for(int j=0;j<len;j++)
        {
            s += std::to_string(boardMatrix[i][j]) + " ";
        }
        s += "\n";
    }
    s += "\n";
    s += "Player 1 has: " + std::to_string(player1Pieces) + "\n";
    s += "Player 2 has: " + std::to_string(player2Pieces) + "\n";
    boardLabel->setText(QString::fromStdString(s));

}

bool Board::searchMatrix(int searchNumber)
{
    vector<vector<bool>> visited;
    int len = boardMatrix.size();
    for(int i=0;i<len;i++)
    {
        vector<bool> t;
        for(int j=0;j<len;j++)
        {
            t.push_back(false);
        }
        visited.push_back(t);
    }

    int x = -1;
    int y = -1;
    bool found = false;
    for(int i=0;i<len;i++)
    {
        for(int j=0;j<len;j++)
        {
            if(boardMatrix[i][j]==searchNumber)
            {
                found = true;
                x = i;
                y = j;
                break;
            }

        }
        if(found == true)
        {
            break;
        }
    }

    if(found == false)
    {
        qDebug()<<QString::fromStdString("player "+to_string(searchNumber)+" wins");
        return true;
    }

    stack<pair<int,int>> s;
    s.push(make_pair(x,y));
    visited[x][y] = true;
    int count = 1;

    while(s.empty()==false)
    {
        pair<int,int> top = s.top();
        s.pop();
        int x = top.first;
        int y = top.second;

        if(x-1>=0 && y-1>=0)
        {
            if(boardMatrix[x-1][y-1] == searchNumber && visited[x-1][y-1] == false)
            {
                s.push(make_pair(x-1,y-1));
                visited[x-1][y-1] = true;
                count++;
            }
        }

        if(x-1>=0 && y>=0)
        {
            if(boardMatrix[x-1][y] == searchNumber && visited[x-1][y] == false)
            {
                s.push(make_pair(x-1,y));
                visited[x-1][y] = true;
                count++;
            }
        }

        if(x-1>=0 && y+1<len)
        {
            if(boardMatrix[x-1][y+1] == searchNumber && visited[x-1][y+1] == false)
            {
                s.push(make_pair(x-1,y+1));
                visited[x-1][y+1] = true;
                count++;
            }
        }

        if(x>=0 && y-1>=0)
        {
            if(boardMatrix[x][y-1] == searchNumber && visited[x][y-1] == false)
            {
                s.push(make_pair(x,y-1));
                visited[x][y-1] = true;
                count++;
            }
        }

        if(x>=0 && y+1<len)
        {
            if(boardMatrix[x][y+1] == searchNumber && visited[x][y+1] == false)
            {
                s.push(make_pair(x,y+1));
                visited[x][y+1] = true;
                count++;
            }
        }

        if(x+1<len && y-1>=0)
        {
            if(boardMatrix[x+1][y-1] == searchNumber && visited[x+1][y-1] == false)
            {
                s.push(make_pair(x+1,y-1));
                visited[x+1][y-1] = true;
                count++;
            }
        }

        if(x+1<len && y>=0)
        {
            if(boardMatrix[x+1][y] == searchNumber && visited[x+1][y] == false)
            {
                s.push(make_pair(x+1,y));
                visited[x+1][y] = true;
                count++;
            }
        }

        if(x+1<len && y+1<len)
        {
            if(boardMatrix[x+1][y+1] == searchNumber && visited[x+1][y+1] == false)
            {
                s.push(make_pair(x+1,y+1));
                visited[x+1][y+1] = true;
                count++;
            }
        }


    }


    if(searchNumber == 1 && count==player1Pieces)
    {
        qDebug()<<QString::fromStdString("player "+to_string(searchNumber)+" wins");
        return true;
    }
    else if(searchNumber == 2 && count==player2Pieces)
    {
        qDebug()<<QString::fromStdString("player "+to_string(searchNumber)+" wins");
        return true;
    }

    return false;
}

void Board::findNextMove()
{
    nextMoves.clear();
    // same row
    int sameRowCount = 0;
    int len = boardMatrix.size();
    for(int i=0;i<len;i++)
    {
        if(boardMatrix[selectedX][i] == 1 || boardMatrix[selectedX][i] == 2)
        {
            sameRowCount++;
        }
    }
    QString s = "sameRowCount: " + QString::number(sameRowCount) + " \n";

    //checking any opposite piece is in between or not
    bool right = true;
    for(int i=1;i<sameRowCount;i++)
    {
        if(selectedY+i>=len)
        {
            right = false;
            break;
        }
        if(selected==1)
        {
            if(boardMatrix[selectedX][selectedY+i] == 2)
            {
                right = false;
                break;
            }
        }
        else
        {
            if(boardMatrix[selectedX][selectedY+i] == 1)
            {
                right = false;
                break;
            }
        }
    }
    if(right == true && (selectedY+sameRowCount)<len)
    {
        nextMoves.insert(make_pair(selectedX,selectedY+sameRowCount));
        s += "right move: " + QString::number(selectedX) + "," + QString::number(selectedY+sameRowCount) + " \n";
    }

    bool left = true;
    for(int i=1;i<sameRowCount;i++)
    {
        if(selectedY-i<0)
        {
            left = false;
            break;
        }
        if(selected==1)
        {
            if(boardMatrix[selectedX][selectedY-i] == 2)
            {
                left = false;
                break;
            }
        }
        else
        {
            if(boardMatrix[selectedX][selectedY-i] == 1)
            {
                left = false;
                break;
            }
        }
    }
    if(left == true && (selectedY-sameRowCount)>=0)
    {
        nextMoves.insert(make_pair(selectedX,selectedY-sameRowCount));
        s += "left move: " + QString::number(selectedX) + "," + QString::number(selectedY-sameRowCount) + " \n";
    }

    int sameColumnCount = 0;
    for(int i=0;i<len;i++)
    {
        if(boardMatrix[i][selectedY] == 1 || boardMatrix[i][selectedY] == 2)
        {
            sameColumnCount++;
        }
    }
    s += "sameColumnCount: " + QString::number(sameColumnCount) + "\n";

    bool down = true;
    for(int i=1;i<sameRowCount;i++)
    {
        if(selectedX+i>=len)
        {
            down = false;
            break;
        }
        if(selected==1)
        {
            if(boardMatrix[selectedX+i][selectedY] == 2)
            {
                down = false;
                break;
            }
        }
        else
        {
            if(boardMatrix[selectedX+i][selectedY] == 1)
            {
                down = false;
                break;
            }
        }
    }
    if(down == true && (selectedX+sameColumnCount)<len)
    {
        nextMoves.insert(make_pair(selectedX+sameColumnCount,selectedY));
        s += "down move: " + QString::number(selectedX+sameColumnCount) + "," + QString::number(selectedY) + " \n";
    }

    bool up = true;
    for(int i=1;i<sameRowCount;i++)
    {
        if(selectedX-i<0)
        {
            up = false;
            break;
        }
        if(selected==1)
        {
            if(boardMatrix[selectedX-i][selectedY] == 2)
            {
                up = false;
                break;
            }
        }
        else
        {
            if(boardMatrix[selectedX-i][selectedY] == 1)
            {
                up = false;
                break;
            }
        }
    }
    if(up == true && (selectedX-sameColumnCount)>=0)
    {
        nextMoves.insert(make_pair(selectedX-sameColumnCount,selectedY));
        s += "up move: " + QString::number(selectedX-sameColumnCount) + "," + QString::number(selectedY) + " \n";
    }

    qDebug().noquote()<<s;

//    QLabel *testLabel = new QLabel(this);
//    //label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
//    //label->setText("f");
//    //label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
//    testLabel->move(625,450);
//    testLabel->setText(QString::fromStdString(s));

    // finding one side of diagonal
    /*int d = min(selectedX,selectedY);
    int startX = selectedX-d;
    int startY = selectedY-d;
    int leftDiagonalCount = 0;
    while(startX<len && startY<len)
    {
        if(boardMatrix[startX][startY] == 1 || boardMatrix[startX][startY] == 2)
        {
            leftDiagonalCount++;
        }

        startX++;
        startY++;
    }*/




}

bool Board::checkGameOver()
{
    bool ans = false;
    if(turn==1)
    {
        ans = searchMatrix(1);
        if(ans == false)
        {
            ans = searchMatrix(2);
        }

    }
    else
    {
        ans = searchMatrix(2);
        if(ans == false)
        {
            ans = searchMatrix(1);
        }
    }
    return ans;

}




