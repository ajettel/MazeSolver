#include "mazenode.h"
/**
 * @brief Creates a Mazenode.
 * @param newId the ID of the new node
 * @param x the x value of the new node
 * @param y the y value of the new node
 * @param rectSize the size of the rectangle
 */
MazeNode::MazeNode(int newId, int x, int y, int rectSize)
{
    // Create the rectangle item
    createRectangle(x,y,rectSize);

    // Set some default values
    nodeIsWall = false;
    visited = false;
    nodeIsEntrance= false;
    nodeIsExit = false;
    position = new QPointF(x,y);
    id = newId;
    previousNode = 0;

    // Sets description of the rectangle
    description = new QGraphicsTextItem("",rectangle);
    description->setPos(position->x(),position->y());

}
/**
 * @brief Destructor
 */
MazeNode::~MazeNode()
{
    delete position;
}
/**
 * @brief Returns the visible rectangle of the node
 * @return QGraphicsRectItem The rectangle of the node
 */
QGraphicsRectItem *MazeNode::getRectangle()
{
    return rectangle;
}
/**
 * @brief Gets the position of the drawn rectangle
 * @return QPointF The point (0,0) of the drawn rectangle on the scene
 */
QPointF *MazeNode::getPosition()
{
    return position;
}
/**
 * @brief Returns the ID of the node
 * @return int the ID of the node
 */
int MazeNode::getId(){
    return id;
}
/**
 * @brief Gets the node that was previoulsy visited
 * @return MazeNode The previous node that has been visited
 */
MazeNode *MazeNode::getPreviousNode()
{
    return previousNode;
}
/**
 * @brief Checks if the node is a wall
 * @return bool True/False
 */
bool MazeNode::isWall()
{
    return nodeIsWall;
}

/**
 * @brief Checks if the node is the entrance
 * @return bool True/False
 */
bool MazeNode::isEntrance()
{
    return nodeIsEntrance;
}
/**
 * @brief Checks if the node is the exit
 * @return bool True/False
 */
bool MazeNode::isExit()
{
    return nodeIsExit;
}
/**
 * @brief Checks if the node has been visited
 * @return bool True/False
 */
bool MazeNode::hasBeenVisited()
{
    return visited;
}
/**
 * @brief Sets the node to active, meaning it changes color to yellow
 * @param bool True/False
 */
void MazeNode::setActive(bool active)
{
    if(active){
        rectangle->setBrush(QBrush(Qt::yellow));
    }
    else{
        rectangle->setBrush(QBrush(Qt::white));
    }
}
/**
 * @brief Sets the descriptive text of the rectangle to the current node ID
 */
void MazeNode::setDescripionToId()
{
    description->setPlainText(QString::number(id));
}
/**
 * @brief Sets the current node as the entrance
 */
void MazeNode::setEntrance(bool isEntrance)
{
    nodeIsEntrance = isEntrance;
    if(isEntrance){
        rectangle->setBrush(QBrush(Qt::yellow));
    }
}

/**
 * @brief Sets the current node as the exit
 * @param bool True/False
 */

void MazeNode::setExit(bool isExit)
{
    nodeIsExit = isExit;

    if(isExit){
        rectangle->setBrush(QBrush(Qt::blue));
    }

}
/**
 * @brief Sets the current node as visited to keep track
 * @param bool True/False
 */

void MazeNode::setVisited(bool value)
{
    visited = value;

    if(value){
        rectangle->setBrush(QBrush(Qt::red));
    }
    else{
        rectangle->setBrush(QBrush(Qt::white));
    }
}
/**
 * @brief Sets the node to a wall node and paints it black
 */
void MazeNode::setWall()
{
    if(nodeIsEntrance || nodeIsExit){
        return;
    }

    nodeIsWall = true;
    rectangle->setBrush(QBrush(Qt::black));
}

/**
 * @brief Sets the previously visited node pointer
 * @param MazeNode The node which has been previously visited
 */
void MazeNode::setPreviousNode(MazeNode *node)
{
    previousNode = node;
}

/**
 * @brief Paints the node green
 */
void MazeNode::tracePath()
{
    rectangle->setBrush(QBrush(Qt::green));

}
/**
 * @brief Sets the node to be a walkable node and paints it white
 */
void MazeNode::unsetWall()
{
    nodeIsWall = false;
    rectangle->setBrush(QBrush(Qt::white));

}

/**
 * @brief Creates the rectangle that's getting drawn for the node
 * @param x the point on the x axis where it should be drawn
 * @param y the point on the y axis where it should be drawn
 * @param rectSize the size of the rectangle side
 */
void MazeNode::createRectangle(int x, int y, int rectSize)
{
    rectangle = new QGraphicsRectItem(x,y,rectSize,rectSize);
}

