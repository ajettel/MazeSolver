#ifndef MAZENODE_H
#define MAZENODE_H

#include <QWidget>
#include <QRect>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <QDebug>

/**
 * @brief This class holds all data necessary for a mazenode, including position, the graphical item
 * and helper functionality
 */
class MazeNode
{

public:
    explicit MazeNode(int newId, int x, int y, int rectSize);
    ~MazeNode();
    QGraphicsRectItem *getRectangle();
    QPointF *getPosition(); ///< Returns the position of the rectangle of the node
    int getId(); ///< Returns the unique ID of the node
    MazeNode *getPreviousNode(); ///< Returns the previously visited node
    bool isWall();
    bool isEntrance();
    bool isExit();
    bool hasBeenVisited();
    void setActive(bool active); ///< Sets the node to currently active
    void setDescripionToId();
    void setEntrance(bool isEntrance);
    void setExit(bool isExit);
    void setVisited(bool value);
    void setWall();
    void setPreviousNode(MazeNode *node);
    void tracePath(); ///< Highlights the node
    void unsetWall();

private:
    int rectSize; // The size of the rectangle in the UI
    int id; // The unique ID of the rectangle
    QGraphicsRectItem *rectangle; // The rectangle item
    QPointF *position; // The position of the rectangle
    QGraphicsTextItem *description;
    MazeNode *previousNode; // A pointer for the search algorithms to find the previous node

    bool nodeIsWall;
    bool nodeIsEntrance;
    bool nodeIsExit;
    bool visited;
    void createRectangle(int x,int y,int rectSize); ///< Creates a rectangle item

    
};

#endif // MAZENODE_H
