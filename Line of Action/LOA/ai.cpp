#include "ai.h"
Move AI::currentBestMove(0,0,0,0);
Move AI::bestMove(0,0,0,0);
int AI::count = 0;
int AI::prunedCount = 0;
AI::AI()
{


}

pair<float,float> AI::centerOfMass(vector<vector<int> > &boardMatrix, int playerNumber)
{
    int len = boardMatrix.size();
    float xDistance = 0;
    float yDistance = 0;
    int count = 0;
    for(int i=0;i<len;i++)
    {
        for(int j=0;j<len;j++)
        {
            if(boardMatrix[i][j]==playerNumber)
            {
                count++;
                xDistance += i;
                yDistance += j;
            }
        }
    }
    xDistance /= count;
    yDistance /= count;

    return make_pair(xDistance,yDistance);

}

float AI::density(vector<vector<int> > &boardMatrix, pair<float, float> &center, int playerNumber)
{
    float distance = 0;
    int len = boardMatrix.size();
    int count = 0;
    for(int i=0;i<len;i++)
    {
        for(int j=0;j<len;j++)
        {
            if(boardMatrix[i][j]==playerNumber)
            {
                count++;
                distance += (abs(center.first-i) + abs(center.second-j));
            }
        }
    }
    distance /= count;
    return (1/distance); // opposite of distance
}

float AI::connectedness(vector<vector<int> > &boardMatrix, int playerNumber)
{
    int count = 0;
    int len = boardMatrix.size();
    vector<vector<bool>> visited;
    for(int i=0;i<len;i++)
    {
        vector<bool> t;
        for(int j=0;j<len;j++)
        {
            t.push_back(false);
            if(boardMatrix[i][j]==playerNumber)
            {
                count++;
            }
        }
        visited.push_back(t);
    }
    int maxClusterSize = 0;

    for(int i=0;i<len;i++)
    {
        for(int j=0;j<len;j++)
        {
            if(boardMatrix[i][j]==playerNumber && visited[i][j]==false)
            {
                int k = dfs(i,j,playerNumber,visited,boardMatrix);
                maxClusterSize = max(k,maxClusterSize);
            }
        }
    }
    float maxClusterLength = maxClusterSize; //float version
    return ((maxClusterLength/count)*100);


}

Move AI::alphaBetaSearch(vector<vector<int> > &boardMatrix,int d,int turn)
{
    float alpha = -500;
    float beta = 500;
    maxValue(boardMatrix,alpha,beta,0,d,turn,turn);
    return currentBestMove;
}

float AI::maxValue(vector<vector<int> > &boardMatrix, float a, float b, int cd, int md,int turn,int firstCaller)
{
    if(cd==md)
    {
        return heuristicValue(boardMatrix,firstCaller);
    }
    float v = -500;
    vector<Move> possibleMoves;
    createPossibleMoves(possibleMoves,boardMatrix,turn);
    int l = possibleMoves.size();
    int nextTurn;
    if(turn==1)
    {
        nextTurn = 2;
    }
    else
    {
        nextTurn = 1;
    }
    for(int i=0;i<l;i++)
    {
        possibleMoves[i].giveMoveInMatrix(boardMatrix);
        float t = minValue(boardMatrix,a,b,cd+1,md,nextTurn,firstCaller);
        if(v<=t)
        {
            v = t;
            currentBestMove = possibleMoves[i];
        }
        possibleMoves[i].undoMoveInMatrix(boardMatrix);
        if(v>=b)
        {
            prunedCount += pow(l-i,md-cd);
            return v;
        }
        a = max(a,v);
    }
    return v;

}

float AI::minValue(vector<vector<int> > &boardMatrix, float a, float b, int cd, int md,int turn, int firstCaller)
{
    if(cd==md)
    {
        return heuristicValue(boardMatrix,firstCaller);
    }
    float v = 500;
    vector<Move> possibleMoves;
    createPossibleMoves(possibleMoves,boardMatrix,turn);
    int l = possibleMoves.size();
    int nextTurn;
    if(turn==1)
    {
        nextTurn = 2;
    }
    else
    {
        nextTurn = 1;
    }
    for(int i=0;i<l;i++)
    {
        possibleMoves[i].giveMoveInMatrix(boardMatrix);
        v = min(v,maxValue(boardMatrix,a,b,cd+1,md,nextTurn,firstCaller));
        possibleMoves[i].undoMoveInMatrix(boardMatrix);
        if(v<=a)
        {
            prunedCount += pow(40,md-cd);
            return v;
        }
        b = min(b,v);
    }
    return v;
}

Move AI::findMoveFromValue(float value)
{

}

float AI::heuristicValue(vector<vector<int> > &boardMatrix, int turn)
{
    count++;
    //qDebug()<<count;
    int myTurn = turn;
    int opponentTurn = 0;
    if(myTurn == 1 )
    {
        opponentTurn = 2;
    }
    else
    {
        opponentTurn = 1;
    }
    pair<float,float> p = centerOfMass(boardMatrix,myTurn);
    float den = density(boardMatrix,p,myTurn);
    float scale = 500;
    den = min(den*100,scale); // scalling in 500 range
    float con = connectedness(boardMatrix,myTurn);
    con = con*5; // con's maximum value 100 so getting in 500 range
    return (.9*den+.1*con); // taking equal share on range of 500
}

void AI::createPossibleMoves(vector<Move> &nextMoves, vector<vector<int> > &boardMatrix, int turn)
{
    nextMoves.clear();
    // same row
    int selectedX = -5;
    int selectedY = -5;

    int len = boardMatrix.size();
    for(int i1=0;i1<len;i1++)
    {
        for(int j1=0;j1<len;j1++)
        {
            if(boardMatrix[i1][j1]==turn)
            {
                selectedX = i1;
                selectedY = j1;
                int sameRowCount = 0;
                for(int i=0;i<len;i++)
                {
                    if(boardMatrix[selectedX][i] == 1 || boardMatrix[selectedX][i] == 2)
                    {
                        sameRowCount++;
                    }
                }
                //QString s = "sameRowCount: " + QString::number(sameRowCount) + " \n";
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
                    if(boardMatrix[selectedX][selectedY+sameRowCount]!=turn)
                    {
                        nextMoves.push_back(Move(selectedX,selectedY,selectedX,selectedY+sameRowCount));
                    }

                    //s += "right move: " + QString::number(selectedX) + "," + QString::number(selectedY+sameRowCount) + " \n";
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
//                    nextMoves.insert(make_pair(selectedX,selectedY-sameRowCount));
//                    s += "left move: " + QString::number(selectedX) + "," + QString::number(selectedY-sameRowCount) + " \n";
                    if(boardMatrix[selectedX][selectedY-sameRowCount]!=turn)
                    {
                        nextMoves.push_back(Move(selectedX,selectedY,selectedX,selectedY-sameRowCount));
                    }
                }

                int sameColumnCount = 0;
                for(int i=0;i<len;i++)
                {
                    if(boardMatrix[i][selectedY] == 1 || boardMatrix[i][selectedY] == 2)
                    {
                        sameColumnCount++;
                    }
                }
                //s += "sameColumnCount: " + QString::number(sameColumnCount) + "\n";

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
//                    nextMoves.insert(make_pair(selectedX+sameColumnCount,selectedY));
//                    s += "down move: " + QString::number(selectedX+sameColumnCount) + "," + QString::number(selectedY) + " \n";
                    if(boardMatrix[selectedX+sameColumnCount][selectedY]!=turn)
                    {
                        nextMoves.push_back(Move(selectedX,selectedY,selectedX+sameColumnCount,selectedY));
                    }
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
//                    nextMoves.insert(make_pair(selectedX-sameColumnCount,selectedY));
//                    s += "up move: " + QString::number(selectedX-sameColumnCount) + "," + QString::number(selectedY) + " \n";
                    if(boardMatrix[selectedX-sameColumnCount][selectedY]!=turn)
                    {
                        nextMoves.push_back(Move(selectedX,selectedY,selectedX-sameColumnCount,selectedY));
                    }
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
                //s += "leftDiagonalCount: " + QString::number(leftDiagonalCount) + "\n";

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
//                    nextMoves.insert(make_pair(selectedX-leftDiagonalCount,selectedY-leftDiagonalCount));
//                    s += "topLeft move: " + QString::number(selectedX-leftDiagonalCount) + "," + QString::number(selectedY-leftDiagonalCount) + " \n";
                    if(boardMatrix[selectedX-leftDiagonalCount][selectedY-leftDiagonalCount]!=turn)
                    {
                        nextMoves.push_back(Move(selectedX,selectedY,selectedX-leftDiagonalCount,selectedY-leftDiagonalCount));
                    }
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
//                    nextMoves.insert(make_pair(selectedX+leftDiagonalCount,selectedY+leftDiagonalCount));
//                    s += "bottomRight move: " + QString::number(selectedX+leftDiagonalCount) + "," + QString::number(selectedY+leftDiagonalCount) + " \n";
                    if(boardMatrix[selectedX+leftDiagonalCount][selectedY+leftDiagonalCount]!=turn)
                    {
                        nextMoves.push_back(Move(selectedX,selectedY,selectedX+leftDiagonalCount,selectedY+leftDiagonalCount));
                    }
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
                //s += "rightDiagonalCount: " + QString::number(rightDiagonalCount) + "\n";

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
//                    nextMoves.insert(make_pair(selectedX-rightDiagonalCount,selectedY+rightDiagonalCount));
//                    s += "topRight move: " + QString::number(selectedX-rightDiagonalCount) + "," + QString::number(selectedY+rightDiagonalCount) + " \n";
                    if(boardMatrix[selectedX-rightDiagonalCount][selectedY+rightDiagonalCount]!=turn)
                    {
                        nextMoves.push_back(Move(selectedX,selectedY,selectedX-rightDiagonalCount,selectedY+rightDiagonalCount));
                    }
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
//                    nextMoves.insert(make_pair(selectedX+rightDiagonalCount,selectedY-rightDiagonalCount));
//                    s += "bottomLeft move: " + QString::number(selectedX+rightDiagonalCount) + "," + QString::number(selectedY-rightDiagonalCount) +" \n";
                    if(boardMatrix[selectedX+rightDiagonalCount][selectedY-rightDiagonalCount]!=turn)
                    {
                        nextMoves.push_back(Move(selectedX,selectedY,selectedX+rightDiagonalCount,selectedY-rightDiagonalCount));
                    }
                }

                //moveLabel->setText(s);
                //qDebug()<<s;

            }
        }
    }


}

int AI::dfs(int x1, int y1, int searchNumber, vector<vector<bool> > &visited,vector<vector<int> > &boardMatrix)
{
    int count = 0;
    int len = boardMatrix.size();
    stack<pair<int,int>> s;
    s.push(make_pair(x1,y1));
    visited[x1][y1] = true;
    count++;
    while(s.empty()==false)
    {

        int x = s.top().first;
        int y = s.top().second;
        s.pop();

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
    return count;
}


