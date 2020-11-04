#ifndef BOARD_H
#define BOARD_H

#include <QDialog>
#include <QDebug>
#include "CustomButton.h"
#include <QLabel>
#include <vector>
#include <bits/stdc++.h>

#include <QPushButton>
#include <QVector>
#include <QLabel>
#include <string>
#include <bits/stdc++.h>
#include <CustomButton.h>
using namespace std;

using namespace std;

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

namespace Ui {
class Board;
}

class Board : public QDialog
{
    Q_OBJECT

public:
    explicit Board(int len,bool vsCom, QWidget *parent = nullptr );

    ~Board();
private slots:
    void handleClick();
    void handleClickVsCom();
private:
    Ui::Board *ui;
    QVector<QVector<CustomButton *>> buttons;
    QLabel *statusLabel;
    QLabel *boardLabel;
    QLabel *moveLabel;
    unordered_set<pair<int,int>, hash_pair> nextMoves;
    int turn;
    int selected;
    int selectedX;
    int selectedY;
    int player1Pieces;
    int player2Pieces;
    std::vector<std::vector<int>> boardMatrix;
    void printLabels();
    bool checkGameOver();
    bool searchMatrix(int searchNumber);
    void findNextMove();
    void markNextMove();
    void unMarkNextMove();
    void colorNextMovePlaces();
    void undoColorNextMovePlaces();
    void spinAI();
    bool nextMove(CustomButton *selectedMove);
    bool vsCom;
};

#endif // BOARD_H
