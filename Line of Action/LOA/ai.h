#ifndef AI_H
#define AI_H
#include<bits/stdc++.h>
#include "move.h"
#include <QString>
#include <QDebug>
using namespace std;

class AI
{
public:
    AI();
    static pair<float,float> centerOfMass(vector<vector<int>> &boardMatrix, int playerNumber);
    static float density(vector<vector<int>> &boardMatrix, pair<float,float> &center, int playerNumber);
    static float connectedness(vector<vector<int>> &boardMatrix, int playerNumber);
    static Move alphaBetaSearch(vector<vector<int>> &boardMatrix, int d, int turn);
    static float maxValue(vector<vector<int>> &boardMatrix, float a, float b, int cd, int md, int turn,int firstCaller);
    static float minValue(vector<vector<int>> &boardMatrix, float a, float b, int cd, int md, int turn,int firstCaller);
    static Move findMoveFromValue(float value);
    static float heuristicValue(vector<vector<int>> &boardMatrix, int turn);
    static void createPossibleMoves(vector<Move> &possibleMoves,vector<vector<int>> &boardMatrix, int turn);
    static Move currentBestMove;
    static Move bestMove;
    static int count;
    static int prunedCount;
private:
    static int dfs(int x1,int y1,int playerNumber,vector<vector<bool>> &visited,vector<vector<int> > &boardMatrix);
};

#endif // AI_H
