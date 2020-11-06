#ifndef MOVE_H
#define MOVE_H

#include<bits/stdc++.h>
using namespace std;
class Move
{
public:
    int fromX;
    int fromY;
    int toX;
    int toY;
    int capture;
    Move(int x1,int y1,int x2,int y2, int c=-5);
    void giveMoveInMatrix(vector<vector<int>> &boardMatrix);
    void undoMoveInMatrix(vector<vector<int>> &boardMatrix);

};

#endif // MOVE_H
