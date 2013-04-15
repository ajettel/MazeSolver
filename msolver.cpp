#include "msolver.h"

#include <QElapsedTimer>

/**
 * @brief Constructor for the maze solver
 * @param listOfIds A hashlist with all the nodes that we need to search through
 * @param nrows Number of rows in the maze
 * @param ncolumns Number of columns in the maze
 * @param tick The tick interval for each search step
 * @param parent The parent for reference purposes
 */
MSolver::MSolver(QHash<int, MazeNode *> *listOfIds, int nrows, int ncolumns,int tick, QObject *parent) :
    QObject(parent)
{
    // Keep track of the values passed n
    rows = nrows;
    columns = ncolumns;
    nodeHash = listOfIds;
    tickInterval = tick;
    interruptSearch = false;

    // Initialize timers
    tickerDFS = new QTimer(this);
    tickerBFS = new QTimer(this);
    stopwatch = new QElapsedTimer();
    timeElapsed = 0;

    connect(tickerDFS, SIGNAL(timeout()), this, SLOT(takeFromStack()));
    connect(tickerBFS, SIGNAL(timeout()), this, SLOT(takeFromQueue()));

}
/**
 * @brief Gets the time elapsed during search
 * @return The time elapsed
 */
qint64 MSolver::getTimeElapsed()
{
    return timeElapsed;
}

/**
 * @brief This function sets some rudimentary parameters. Can be set in the constructor as well
 * @param listOfIds A hashlist with all the nodes that we need to search through
 * @param nrows Number of rows in the maze
 * @param ncolumns Number of columns in the maze
 * @param tick The tick interval for each search step
 */
void MSolver::setParameters(QHash<int, MazeNode *> *listOfIds, int nrows, int ncolumns,int tick)
{
    rows = nrows;
    columns = ncolumns;
    nodeHash = listOfIds;
    tickInterval = tick;
}

/**
 * @brief Starts the DFS algorithm on the list of nodes.
 */
void MSolver::startDFS(){
    // Initialize the stack and start the timer
    stack = new QStack<MazeNode*>();
    stopwatch->restart();

    MazeNode *start = nodeHash->value(0);
    stack->push(start);

    tickerDFS->blockSignals(false);
    tickerDFS->start(tickInterval); // On each tick, take one node off the stack and search
}
/**
 * @brief Starts the BFS algorithm on the list of nodes.
 */
void MSolver::startBFS(){
    // Initialize the queue and start the timer
    queue = new QQueue<MazeNode*>();
    stopwatch->restart();

    MazeNode *start = nodeHash->value(0);
    queue->enqueue(start);

    tickerBFS->blockSignals(false);
    tickerBFS->start(tickInterval);  // On each tick, take one node off the queue and search
}
/**
 * @brief Stops the search, triggered from the UI
 */
void MSolver::triggerStopSearch(){
    timeElapsed = stopwatch->elapsed();
    stopwatch->invalidate();
    interruptSearch = true;
}

/**
 * @brief Stops the search, triggered from the algorithm
 * @param exitNode The exitnode
 */
void MSolver::stopSearch(MazeNode *exitNode)
{

    // Stop all the timers
    timeElapsed = stopwatch->elapsed();
    stopwatch->invalidate();

    tickerDFS->blockSignals(true);
    tickerDFS->stop();

    tickerBFS->blockSignals(true);
    tickerBFS->stop();

    // Clear stacks and queue
    if(stack != 0){
        stack->clear();
    }
    if(queue != 0){
        queue->clear();
    }

    // Display the path/exit
    emit displayExit(exitNode);
}
/**
 * @brief Gets the unvisited adjacent nodes of the passed in node
 * @param currentID The node ID we need the adjacent nodes for
 * @return The list of neighboring nodes that have been unvisited
 */
QList<MazeNode *> *MSolver::getAdjacentUnvisitedNodes(int currentID)
{
    QList<MazeNode*> *neighbours = new QList<MazeNode*>();
    int totalNumberOfNodes = rows * columns;

    MazeNode *adjacentNode;

    // Node to the south
    if((currentID) < (totalNumberOfNodes-columns)){
        adjacentNode = nodeHash->value(currentID+columns);
        if(!adjacentNode->isWall() && !adjacentNode->hasBeenVisited()){
            neighbours->append(adjacentNode);
        }
    }

    // Node to the north
    if(currentID > columns){
        adjacentNode = nodeHash->value(currentID-columns);

        if(!adjacentNode->isWall() && !adjacentNode->hasBeenVisited()){
            neighbours->append(adjacentNode);
        }
    }

    // Node to the east
    if(((currentID+1) % columns) != 0){ // Check if the node isn't at the very right
        adjacentNode = nodeHash->value(currentID+1);
        if(!adjacentNode->isWall() && (currentID+1) <= totalNumberOfNodes && !adjacentNode->hasBeenVisited()){
            neighbours->append(adjacentNode);
        }
    }
    // Node to the west
    if(currentID != 0 && ((currentID) % columns) != 0){ // Check if the node isn't at the very left
        adjacentNode = nodeHash->value(currentID-1);
        if(!adjacentNode->isWall() && !adjacentNode->hasBeenVisited()){
            neighbours->append(adjacentNode);
        }
    }

    return neighbours;
}

/**
 * @brief One step in the DFS search algorithm
 */
void MSolver::takeFromStack()
{
    // Check if the search has been interrupted
    if(interruptSearch){
        qDebug() << "Interrupted";
        tickerDFS->blockSignals(true);
        tickerDFS->stop();
        interruptSearch = false;
        stopSearch(0); // Stop with the search, pass in 0 as the exit hasn't been found
        return;
    }

    int currentID;
    MazeNode *currentNode;
    MazeNode *nextNode;
    QList<MazeNode*> *neighbours;
    int validNodeCounter = 0;
    bool exitFound = false;

    // This is the DFS search algorithm. Pop a node off of the stack
    // Get adjacent nodes and iterate over them at random and check if they're exits
    // It not, push them onto the stack
    if(!stack->isEmpty()){
        currentNode = stack->pop();
        currentNode->setActive(true);
        currentID = currentNode->getId();
        neighbours = getAdjacentUnvisitedNodes(currentID);
        while(!neighbours->isEmpty()){
            nextNode = neighbours->takeAt(rand() % neighbours->size());
            // Ignore Node it is a wall or has been visited already
            if(nextNode->isWall() || nextNode->hasBeenVisited()){
                continue;
            }
            validNodeCounter++;
            nextNode->setPreviousNode(currentNode);
            stack->push(nextNode);
            if(nextNode->isExit()){
                exitFound = true;
                break;
            }
        }
        currentNode->setVisited(true);

        if(exitFound){
            stopSearch(nextNode); // Exit found
        }
    }
    else{
        stopSearch(0);// No exit found
        return;
    }

    // No Node has been updated this tick
    if(validNodeCounter == 0){
        validNodeCounter = 0;
        tickerDFS->stop();
        takeFromStack(); // rerun the
        tickerDFS->start(tickInterval);
    }

}
/**
 * @brief One step in the BFS search algorithm
 */
void MSolver::takeFromQueue(){
    // Check if the search has been interrupted
    if(interruptSearch){
        tickerBFS->blockSignals(true);
        tickerBFS->stop();
        interruptSearch = false;
        stopSearch(0); // Stop with the search, pass in 0 as the exit hasn't been found
        return;
    }

    int currentID;
    MazeNode *currentNode;
    MazeNode *nextNode;
    QList<MazeNode*> *neighbours;
    int validNodeCounter = 0;
    bool exitFound = false;

    // This is the BFS search algorithm. Dequeue a node from the queue
    // Get adjacent nodes and iterate over them at random and check if they're exits
    // It not, enqueue them on the queue
    if(!queue->isEmpty()){
        currentNode = queue->dequeue();

        if(currentNode->hasBeenVisited()){
            return;
        }

        currentNode->setActive(true);
        currentID = currentNode->getId();
        neighbours = getAdjacentUnvisitedNodes(currentID);

        while(!neighbours->isEmpty()){
            nextNode = neighbours->takeFirst();

            // Ignore Node it is a wall or has been visited already
            if(nextNode->isWall() || nextNode->hasBeenVisited()){
                continue;
            }
            validNodeCounter++;
            nextNode->setPreviousNode(currentNode);
            queue->enqueue(nextNode);
            if(nextNode->isExit()){
                exitFound = true;
                break;
            }


        }

        currentNode->setVisited(true);

        if(exitFound){
            stopSearch(nextNode); // Exit found
        }
    }
    else{
        stopSearch(0);// No exit found
        return;
    }

    // No Node has been updated this tick
    if(validNodeCounter == 0){
        validNodeCounter = 0;
        takeFromQueue();
    }


}

