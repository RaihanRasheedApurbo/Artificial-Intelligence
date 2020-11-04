#include "board.h"
#include "ui_board.h"


Board::Board(int len, bool vsCom, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Board)
{
    this->vsCom = vsCom;
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

            if(white)
            {
                buttonA->setColor(Qt::white);
                buttonA->blockColor = "white";
                white = false;
            }
            else
            {
                buttonA->setColor(Qt::yellow);
                buttonA->blockColor = "yellow";
                white = true;
            }


            if(vsCom == true)
            {
                connect(buttonA,SIGNAL(clicked()),this,SLOT(handleClickVsCom()));
            }
            else
            {
                connect(buttonA,SIGNAL(clicked()),this,SLOT(handleClick()));
            }

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
    statusLabel->move(625,550);

    boardLabel = new QLabel(this);
    boardLabel->move(625,35);

    moveLabel = new QLabel(this);
    moveLabel->move(625,300);
    moveLabel->setText("hi");




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

    CustomButton *t1 = (CustomButton *) sender();

    if(turn == 1 && selected == 0)
    {
        if(t1->text()=="O")
        {
            selected = 1;
            selectedX = t1->rowNumber;
            selectedY = t1->colNumber;
            t1->setRed();

            findNextMove();
            colorNextMovePlaces();

        }
    }
    else if(turn == 1 && selected == 1)
    {


        if(selectedX == t1->rowNumber && selectedY == t1->colNumber) // selected the same block again so discard selection
        {
            selected = 0;
            selectedX = -5;
            selectedY = -5;

            if(t1->blockColor == "white")
            {
                t1->setColor(Qt::white);
            }
            else
            {
                t1->setColor(Qt::yellow);
            }


            undoColorNextMovePlaces();
            moveLabel->setText("");

        }
        else if(nextMove(t1))
        {
            if(t1->text()=="O")
            {
                //doing nothing kept here for case analysis
            }
            else if(t1->text()== "" || t1->text() == "X") // empty place or opponent piece
            {
                moveLabel->setText("");
                undoColorNextMovePlaces();
                if(t1->text()=="X")
                {
                    player2Pieces--;
                }
                t1->setText("O");


                boardMatrix[selectedX][selectedY] = 0;
                boardMatrix[t1->rowNumber][t1->colNumber] = 1;

                CustomButton* t2 = buttons[selectedX][selectedY];
                t2->setText("");

                if(t2->blockColor == "white")
                {
                    t2->setColor(Qt::white);
                }
                else
                {
                    t2->setColor(Qt::yellow);
                }



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


    }
    else if(turn == 2 && selected == 0)
    {
        if(t1->text()=="X")
        {
            selected = 1;
            selectedX = t1->rowNumber;
            selectedY = t1->colNumber;
            t1->setRed();
            findNextMove();
            //qDebug()<<"hi at line 260 its okay";
            colorNextMovePlaces();
            //qDebug()<<"hi at line 262 its okay";




        }
    }
    else if(turn == 2 && selected == 1)
    {

        if(selectedX == t1->rowNumber && selectedY == t1->colNumber) // selected the same block again so discard selection
        {
            selected = 0;
            selectedX = -5;
            selectedY = -5;
            if(t1->blockColor == "white")
            {
                t1->setColor(Qt::white);
            }
            else
            {
                t1->setColor(Qt::yellow);
            }
            undoColorNextMovePlaces();
        }
        else if(nextMove(t1))
        {

            if(t1->text() == "X") // can't go here because same team's piece exist in this block
            {
                // doing nothing kept here for case analysis
            }
            else if(t1->text()== "" || t1->text() == "O") // empty place or opponent piece
            {
                undoColorNextMovePlaces(); // changing possible moves buttons to normal color
                if(t1->text()=="O")
                {
                    player1Pieces--;
                }
                t1->setText("X");

                CustomButton* t2 = buttons[selectedX][selectedY];
                t2->setText("");
                boardMatrix[selectedX][selectedY] = 0;
                boardMatrix[t1->rowNumber][t1->colNumber] = 2;


                if(t2->blockColor == "white")
                {
                    t2->setColor(Qt::white);
                }
                else
                {
                    t2->setColor(Qt::yellow);
                }




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

    }

    printLabels();


}

void Board::handleClickVsCom()
{
    //qDebug()<<"inside vsCom";

    CustomButton *t1 = (CustomButton *) sender();

    if(turn == 1 && selected == 0)
    {
        if(t1->text()=="O")
        {
            selected = 1;
            selectedX = t1->rowNumber;
            selectedY = t1->colNumber;
            t1->setRed();

            findNextMove();
            colorNextMovePlaces();

        }
    }
    else if(turn == 1 && selected == 1)
    {


        if(selectedX == t1->rowNumber && selectedY == t1->colNumber) // selected the same block again so discard selection
        {
            selected = 0;
            selectedX = -5;
            selectedY = -5;

            if(t1->blockColor == "white")
            {
                t1->setColor(Qt::white);
            }
            else
            {
                t1->setColor(Qt::yellow);
            }


            undoColorNextMovePlaces();
            moveLabel->setText("");

        }
        else if(nextMove(t1))
        {
            if(t1->text()=="O")
            {
                //doing nothing kept here for case analysis
            }
            else if(t1->text()== "" || t1->text() == "X") // empty place or opponent piece
            {
                moveLabel->setText("");
                undoColorNextMovePlaces();
                if(t1->text()=="X")
                {
                    player2Pieces--;
                }
                t1->setText("O");


                boardMatrix[selectedX][selectedY] = 0;
                boardMatrix[t1->rowNumber][t1->colNumber] = 1;

                CustomButton* t2 = buttons[selectedX][selectedY];
                t2->setText("");

                if(t2->blockColor == "white")
                {
                    t2->setColor(Qt::white);
                }
                else
                {
                    t2->setColor(Qt::yellow);
                }



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

                spinAI();





            }
        }


    }
    else if(turn == 2 && selected == 0)
    {
        qDebug()<<"clicked";
        if(t1->text()=="X")
        {
            selected = 1;
            selectedX = t1->rowNumber;
            selectedY = t1->colNumber;
            t1->setRed();
            findNextMove();
            //qDebug()<<"hi at line 260 its okay";
            colorNextMovePlaces();
            //qDebug()<<"hi at line 262 its okay";




        }
    }
    else if(turn == 2 && selected == 1)
    {

        if(selectedX == t1->rowNumber && selectedY == t1->colNumber) // selected the same block again so discard selection
        {
            selected = 0;
            selectedX = -5;
            selectedY = -5;
            if(t1->blockColor == "white")
            {
                t1->setColor(Qt::white);
            }
            else
            {
                t1->setColor(Qt::yellow);
            }
            undoColorNextMovePlaces();
        }
        else if(nextMove(t1))
        {

            if(t1->text() == "X") // can't go here because same team's piece exist in this block
            {
                // doing nothing kept here for case analysis
            }
            else if(t1->text()== "" || t1->text() == "O") // empty place or opponent piece
            {
                undoColorNextMovePlaces(); // changing possible moves buttons to normal color
                if(t1->text()=="O")
                {
                    player1Pieces--;
                }
                t1->setText("X");

                CustomButton* t2 = buttons[selectedX][selectedY];
                t2->setText("");
                boardMatrix[selectedX][selectedY] = 0;
                boardMatrix[t1->rowNumber][t1->colNumber] = 2;


                if(t2->blockColor == "white")
                {
                    t2->setColor(Qt::white);
                }
                else
                {
                    t2->setColor(Qt::yellow);
                }




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
        if(turn==1)
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
        if(turn==1)
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
    for(int i=1;i<sameColumnCount;i++)
    {
        if(selectedX+i>=len)
        {
            down = false;
            break;
        }
        if(turn==1)
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
    for(int i=1;i<sameColumnCount;i++)
    {
        if(selectedX-i<0)
        {
            up = false;
            break;
        }
        if(turn==1)
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






//    QLabel *testLabel = new QLabel(this);
//    //label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
//    //label->setText("f");
//    //label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
//    testLabel->move(625,450);
//    testLabel->setText(QString::fromStdString(s));

    // finding left diagonal
    int d = min(selectedX,selectedY);
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
    }
    s += "leftDiagonalCount: " + QString::number(leftDiagonalCount) + "\n";

    bool topLeft = true;
    for(int i=1;i<leftDiagonalCount;i++)
    {
        if(selectedX-i<0 || selectedY-i<0)
        {
            topLeft = false;
            break;
        }
        if(turn==1)
        {
            if(boardMatrix[selectedX-i][selectedY-i] == 2)
            {
                topLeft = false;
                break;
            }
        }
        else
        {
            if(boardMatrix[selectedX-i][selectedY-i] == 1)
            {
                topLeft = false;
                break;
            }
        }
    }
    if(topLeft == true && (selectedX-leftDiagonalCount)>=0 && (selectedY-leftDiagonalCount)>=0)
    {
        nextMoves.insert(make_pair(selectedX-leftDiagonalCount,selectedY-leftDiagonalCount));
        s += "topLeft move: " + QString::number(selectedX-leftDiagonalCount) + "," + QString::number(selectedY-leftDiagonalCount) + " \n";
    }


    bool bottomRight = true;
    for(int i=1;i<leftDiagonalCount;i++)
    {
        if(selectedX+i>=len || selectedY+i>=len)
        {
            bottomRight = false;
            break;
        }
        if(turn==1)
        {
            if(boardMatrix[selectedX+i][selectedY+i] == 2)
            {
                bottomRight = false;
                break;
            }
        }
        else
        {
            if(boardMatrix[selectedX+i][selectedY+i] == 1)
            {
                bottomRight = false;
                break;
            }
        }
    }
    if(bottomRight == true && (selectedX+leftDiagonalCount)<len && (selectedY+leftDiagonalCount)<len)
    {
        nextMoves.insert(make_pair(selectedX+leftDiagonalCount,selectedY+leftDiagonalCount));
        s += "bottomRight move: " + QString::number(selectedX+leftDiagonalCount) + "," + QString::number(selectedY+leftDiagonalCount) + " \n";
    }


    //finding right diagonal diiferent from left diagonal at first finding left down corner of the diagonal
    int currentCellX = selectedX;
    int currentCellY = selectedY;
    while(true)
    {
        if(currentCellX+1>=len || currentCellY-1<0)
        {
            break;
        }
        currentCellX++;
        currentCellY--;
    }

    startX = currentCellX;
    startY = currentCellY;
    int rightDiagonalCount = 0;
    while(startX>=0 && startY<len)
    {
        if(boardMatrix[startX][startY] == 1 || boardMatrix[startX][startY] == 2)
        {
            rightDiagonalCount++;
        }

        startX--;
        startY++;
    }
    s += "rightDiagonalCount: " + QString::number(rightDiagonalCount) + "\n";

    bool topRight = true;
    for(int i=1;i<rightDiagonalCount;i++)
    {
        if(selectedX-i<0 || selectedY+i>=len)
        {
            topRight = false;
            break;
        }
        if(turn==1)
        {
            if(boardMatrix[selectedX-i][selectedY+i] == 2)
            {
                topRight = false;
                break;
            }
        }
        else
        {
            if(boardMatrix[selectedX-i][selectedY+i] == 1)
            {
                topRight = false;
                break;
            }
        }
    }
    if(topRight == true && (selectedX-rightDiagonalCount)>=0 && (selectedY+rightDiagonalCount)<len)
    {
        nextMoves.insert(make_pair(selectedX-rightDiagonalCount,selectedY+rightDiagonalCount));
        s += "topRight move: " + QString::number(selectedX-rightDiagonalCount) + "," + QString::number(selectedY+rightDiagonalCount) + " \n";
    }

    bool bottomLeft = true;
    for(int i=1;i<rightDiagonalCount;i++)
    {
        if(selectedX+i>=len || selectedY-i<0)
        {
            bottomLeft = false;
            break;
        }
        if(turn==1)
        {
            if(boardMatrix[selectedX+i][selectedY-i] == 2)
            {
                bottomLeft = false;
                break;
            }
        }
        else
        {
            if(boardMatrix[selectedX+i][selectedY-i] == 1)
            {
                bottomLeft = false;
                break;
            }
        }
    }
    if(bottomLeft == true && (selectedX+rightDiagonalCount)<len && (selectedY-rightDiagonalCount)>=0)
    {
        nextMoves.insert(make_pair(selectedX+rightDiagonalCount,selectedY-rightDiagonalCount));
        s += "bottomLeft move: " + QString::number(selectedX+rightDiagonalCount) + "," + QString::number(selectedY-rightDiagonalCount) +" \n";
    }

    moveLabel->setText(s);
    qDebug()<<s;




}

void Board::colorNextMovePlaces()
{

    for(auto &t : nextMoves)
    {
        int x = t.first;
        int y = t.second;
        qDebug()<<x<<" "<<y;
        if(turn==1)
        {
            if(boardMatrix[x][y]==1)
            {
                // doing nothing... kept here for case analysis
                // not erasing because it creates problem in set iterator pointer
            }
            else
            {

                CustomButton *t1 = buttons[x][y];
                t1->setRed();
            }
        }
        else
        {
            if(boardMatrix[x][y]==2)
            {
                //doing nothing... kept here for case analysis
            }
            else
            {
                CustomButton *t1 = buttons[x][y];
                t1->setRed();
            }
        }
    }

}

void Board::undoColorNextMovePlaces()
{


    for(auto &t : nextMoves)
    {
        int x = t.first;
        int y = t.second;

        CustomButton *t1 = buttons[x][y];

        if(t1->blockColor == "white")
        {
            t1->setColor(Qt::white);
        }
        else
        {
            t1->setColor(Qt::yellow);
        }




    }
}

void Board::spinAI()
{
    int len = boardMatrix.size();
    bool done = false;
    for(int i=0;i<len;i++)
    {
        if(done == true)
        {
            break;
        }
        for(int j=0;j<len;j++)
        {
            if(boardMatrix[i][j]==2)
            {
                buttons[i][j]->clicked();

            }
        }
    }
}

bool Board::nextMove(CustomButton *selectedMove)
{
    pair<int,int> p = make_pair(selectedMove->rowNumber,selectedMove->colNumber);

    return (nextMoves.find(p) != nextMoves.end());
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




