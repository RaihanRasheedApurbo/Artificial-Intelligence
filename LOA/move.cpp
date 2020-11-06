#include "move.h"


Move::Move(int x1, int y1, int x2, int y2, int c)
{
    fromX = x1;
    fromY = y1;
    toX = x2;
    toY = y2;
    capture = c;
}

void Move::giveMoveInMatrix(vector<vector<int> > &boardMatrix)
{
    capture = boardMatrix[toX][toY];
    boardMatrix[toX][toY] = boardMatrix[fromX][fromY];
    boardMatrix[fromX][fromY] = 0;


}

void Move::undoMoveInMatrix(vector<vector<int> > &boardMatrix)
{
    boardMatrix[fromX][fromY] = boardMatrix[toX][toY];
    boardMatrix[toX][toY] = capture;
    capture = -5;

}
