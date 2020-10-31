#ifndef GAME_H
#define GAME_H
#include <QGraphicsView>
#include <QGraphicsScene>

class Game: public QGraphicsView{
public:
    // constructors
    Game(QWidget* parrent=NULL);

    // public methods
    void start();

    //public atrtributes
    QGraphicsScene* scene;
    // HexBoard* hexBoard; TODO
    QString whosTurn;


};


#endif // GAME_H
