#ifndef MSOLVER_H
#define MSOLVER_H

#include <QObject>
#include <QHash>
#include <QStack>
#include <QQueue>
#include <QList>
#include <QTimer>
#include <QElapsedTimer>

#include "mazenode.h"

/**
 * @brief The MSolver class holds the algorithms that solve the maze
 */
class MSolver : public QObject
{
    Q_OBJECT
public:
    explicit MSolver(QHash<int,MazeNode*>*listOfIds, int rows,int columns, int tick,QObject *parent = 0);
    qint64 getTimeElapsed(); ///< Gets the time needed to solve the maze
    void setParameters(QHash<int,MazeNode*>*listOfIds, int rows,int columns,int tick); ///< Sets some values that are needed later for solving
    void startDFS();
    void startBFS();
    void triggerStopSearch(); ///< Stops the search

private:

    QHash<int,MazeNode*> *nodeHash;
    QStack<MazeNode*> *stack;
    QQueue<MazeNode*> *queue;
    QTimer *tickerDFS; ///<  A timer which triggers a step in the DFS algorithm
    QTimer *tickerBFS; ///< A timer which triggers a step in the BFS algorithm
    QElapsedTimer *stopwatch;


    bool interruptSearch; ///< A flag to interrupt the search
    int rows;
    int columns;
    int tickInterval; ///< How fast should the DFS or BFS ticker run
    qint64 timeElapsed;
    void stopSearch(MazeNode* stopSearch);
    QList<MazeNode*>* getAdjacentUnvisitedNodes(int currentID); ///< Gets the adjacent nodes which have not been visited
    void traceBack(MazeNode* lastNode); ///< traces a path back from the given node to the start (if path has been set)
private slots:
    void takeFromStack();
    void takeFromQueue();
signals:
    void displayExit(MazeNode *exitNode);

};

#endif // MSOLVER_H
