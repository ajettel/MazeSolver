#include "mazeui.h"
/**
 * @brief Creates a new maze solver ui with solving capabilities on a given tab
 * @param QWidget the widget on which to create the UI
 */
MazeUi::MazeUi(QWidget *newCurrentTab)
{
    currentTab = newCurrentTab;

    // Fixed height and width
    sceneHeight = 600;
    sceneWidth = 750;

    // Create the different selectable sizes for the maze
    createGridSizeArray();

    // Selects a few default values
    rectSize = rectSizeList[1][0];
    nodeDescriptionThreshold = rectSizeList[1][2];

    // Create the data structures that hold pointers to all the nodes
    listOfRectangles = new QHash<QGraphicsItem *,MazeNode *>(); // one by the graphics items
    listOfIds = new QHash<int,MazeNode*>(); // one by the ID

    setupUI();
    setDefaultSelections();
    drawMaze();
    initializeMazeSolver();
}
/**
 * @brief Destructor
 */
MazeUi::~MazeUi(){
    delete listOfRectangles;
    delete listOfIds;
    delete view;
}
/**
 * @brief Creates an array for the different sizes of the arrays
 */
void MazeUi::createGridSizeArray(){
    // 2D array. First row is the user-visible number of squares
    // The second row is the size of squares (used for internal purposes)
    // that correspond to the first row
    rectSizeList = new int*[2];
    for(int ii = 0; ii < 2; ii++)
        rectSizeList[ii] = new int[6];

    rectSizeList[0][0] = 20;
    rectSizeList[0][1] = 80;
    rectSizeList[0][2] = 500;
    rectSizeList[0][3] = 2000;
    rectSizeList[0][4] = 4500;
    rectSizeList[0][5] = 18000;

    rectSizeList[1][0] = 150;
    rectSizeList[1][1] = 75;
    rectSizeList[1][2] = 30;
    rectSizeList[1][3] = 15;
    rectSizeList[1][4] = 10;
    rectSizeList[1][5] = 5;

}
/**
 * @brief Initializes all the UI items and arranges them in a Gridlayout
 */
void MazeUi::setupUI(){

    QGridLayout *gridLayout = new QGridLayout();
    currentTab->setLayout(gridLayout);

    // Setting up the graphics view for the rectangles
    view = new QGraphicsView();
    view->setGeometry(0,0,sceneWidth,sceneHeight);
    scene = new QGraphicsScene();
    view->setScene(scene);
    gridLayout->addWidget(view,0,0,3,1);

    // Setting up Spacers
    QSpacerItem *viewSpacer = new QSpacerItem(10,0);
    gridLayout->addItem(viewSpacer,0,1);

    // Setting up control boxes
    QGroupBox *mazeGroupBox = new QGroupBox("Maze",this);
    gridLayout->addWidget(mazeGroupBox,0,2);
    addItemsToMazeGroupBox(mazeGroupBox);

    QGroupBox *toolsGroupBox = new QGroupBox("Tools",this);
    gridLayout->addWidget(toolsGroupBox,1,2);
    addItemsToToolsGroupBox(toolsGroupBox);

    QGroupBox *logGroupBox = new QGroupBox("Log",this);
    gridLayout->addWidget(logGroupBox,2,2);
    addItemsToLogGroupBox(logGroupBox);

}

/**
 * @brief Sets up the maze group box
 * @param mazeGroupBox The group box that holds everything related to the maze
 */
void MazeUi::addItemsToMazeGroupBox(QGroupBox *mazeGroupBox){
    // Settings layouts
    QFormLayout *controlLayout = new QFormLayout();
    mazeGroupBox->setLayout(controlLayout);

    QLabel *searchDescription = new QLabel("Search algorithm");
    searchSelection = new QComboBox();
    searchSelection->addItem("DFS");
    searchSelection->addItem("BFS");
    controlLayout->addRow(searchDescription,searchSelection);

    // Get the possible sizes of the maze from the created array
    QLabel *gridSizeDescription = new QLabel("Select Grid size");
    gridSizeSelection = new QComboBox();
    gridSizeSelection->addItem(QString::number(rectSizeList[0][0]));
    gridSizeSelection->addItem(QString::number(rectSizeList[0][1]));
    gridSizeSelection->addItem(QString::number(rectSizeList[0][2]));
    gridSizeSelection->addItem(QString::number(rectSizeList[0][3]));
    gridSizeSelection->addItem(QString::number(rectSizeList[0][4]));
    gridSizeSelection->addItem(QString::number(rectSizeList[0][5]));
    controlLayout->addRow(gridSizeDescription,gridSizeSelection);

    changeGridSize= new QPushButton("Change Grid Size");
    controlLayout->addRow(changeGridSize);

    resetMazeButton = new QPushButton("Restart this Maze");
    controlLayout->addRow(resetMazeButton);

    clearMazeButton = new QPushButton("Clear Grid");
    controlLayout->addRow(clearMazeButton);

    startSearchButton = new QPushButton("Start Search");
    controlLayout->addRow(startSearchButton);

    stopSearchButton = new QPushButton("Stop Search");
    controlLayout->addRow(stopSearchButton);
    stopSearchButton->setVisible(false);

    // Connect the buttons to slots
    connect(changeGridSize,SIGNAL(clicked()),this,SLOT(setNewGridSize()));
    connect(startSearchButton,SIGNAL(clicked()),this,SLOT(startSearch()));
    connect(stopSearchButton,SIGNAL(clicked()),this,SLOT(stopSearch()));
    connect(resetMazeButton,SIGNAL(clicked()),this,SLOT(resetMaze()));
    connect(clearMazeButton,SIGNAL(clicked()),this,SLOT(clearMaze()));

}
/**
 * @brief Sets up the tools group box
 * @param toolsGroupBox The group box that holds everything related to the tools
 */
void MazeUi::addItemsToToolsGroupBox(QGroupBox *toolsGroupBox)
{
    // Set the layout
    QFormLayout *controlLayout = new QFormLayout();
    toolsGroupBox->setLayout(controlLayout);

    QPushButton *createRandomMazeButton = new QPushButton("Create Random Maze");
    controlLayout->addRow(createRandomMazeButton);

    // Setting the tick intervals
    tickIntervalSelector = new QSpinBox(this);
    tickIntervalSelector->setMinimum(1);
    tickIntervalSelector->setMaximum(1000);
    tickIntervalSelector->setValue(10);
    controlLayout->addRow("Tick Interval",tickIntervalSelector);

    connect(createRandomMazeButton,SIGNAL(clicked()),this,SLOT(createRandomMaze()));

}
/**
 * @brief Sets up the log group box
 * @param toolsGroupBox The group box that holds everything related to the log
 */
void MazeUi::addItemsToLogGroupBox(QGroupBox *logGroupBox)
{
    QGridLayout *controlLayout = new QGridLayout();
    logGroupBox->setLayout(controlLayout);

    log = new QTextBrowser();
    controlLayout->addWidget(log);

}
/**
 * @brief Sets the entrance and exit node
 */
void MazeUi::setEntranceAndExit()
{
    listOfIds->value(0)->setEntrance(true);
    listOfIds->value(listOfIds->size()-1)->setExit(true);

}

/**
 * @brief Resets the maze but leaves the walls intact
 */
void MazeUi::resetMaze()
{
    foreach(MazeNode *item, *listOfRectangles){
        if(!item->isWall()){
            item->unsetWall();
            item->setEntrance(false);
            item->setVisited(false);
            item->setExit(false);
            item->setActive(false);

            if(rectSize > nodeDescriptionThreshold){
                item->setDescripionToId();
            }
        }
    }

    // Reset entrance and exit
    setEntranceAndExit();

    startSearchButton->setEnabled(true);
    log->clear();


}

/**
 * @brief Draws the empty maze
 */
void MazeUi::drawMaze(){
    addItemsToScene();
}
/**
 * @brief Switches UI elements on an off, helpful during the executing of the search
 */
void MazeUi::switchUiState()
{
    startSearchButton->setEnabled(!startSearchButton->isEnabled());
    resetMazeButton->setEnabled(!resetMazeButton->isEnabled());
    changeGridSize->setEnabled(!changeGridSize->isEnabled());
    gridSizeSelection->setEnabled(!gridSizeSelection->isEnabled());
    searchSelection->setEnabled(!searchSelection->isEnabled());
    clearMazeButton->setEnabled(!clearMazeButton->isEnabled());
}
/**
 * @brief Writes the found path to the log
 * @param path The path that is to be output to the log
 */
void MazeUi::fillLogWithPath(QStack<int> *path)
{
    while(!path->isEmpty()){
        log->append(QString::number(path->pop()));
    }
}
/**
 * @brief Sets up the maze solver object
 */
void MazeUi::initializeMazeSolver()
{
    int columns = sceneWidth / rectSize;
    int rows = sceneHeight / rectSize;
    solver = new MSolver(listOfIds,rows,columns,tickIntervalSelector->value());
    connect(solver,SIGNAL(displayExit(MazeNode*)),this,SLOT(displayResult(MazeNode*)));

}
/**
 * @brief Sets the default selections in the UI
 */
void MazeUi::setDefaultSelections()
{
    // Set gridsize to a large value
    gridSizeSelection->setCurrentIndex(3);
    rectSize = rectSizeList[1][gridSizeSelection->currentIndex()];

    // Set BFS first
    searchSelection->setCurrentIndex(1);
}

/**
 * @brief Adds all the rectangles from the nodes to the scene
 */
void MazeUi::addItemsToScene(){

    MazeNode *node;
    int columns = sceneWidth / rectSize;
    int rows = sceneHeight / rectSize;
    int ii = 0;
    int jj = 0;
    int idCounter = 0;

    // Each row and each column
    while(ii < rows){
        while(jj < columns){
            // Gets a new node
            node = new MazeNode(idCounter,jj*rectSize,ii * rectSize,rectSize);
            scene->addItem(node->getRectangle());

            // Insert the nodes into the hashtables as well later access
            listOfRectangles->insert(node->getRectangle(), node);
            listOfIds->insert(idCounter,node);

            // Only set descriptions if the nodes are big enough
            if(rectSize > nodeDescriptionThreshold){
                node->setDescripionToId();
            }
            jj++;
            idCounter++;
        }
        jj = 0;
        ii++;
    }

    setEntranceAndExit();
}

/**
 * @brief Blanks the entire maze to walkable space
 */
void MazeUi::clearMaze()
{
    foreach(MazeNode *item, *listOfRectangles){
        item->unsetWall();
        item->setEntrance(false);
        item->setVisited(false);
        item->setExit(false);
        item->setActive(false);

        if(rectSize > nodeDescriptionThreshold){
            item->setDescripionToId();
        }
    }
    setEntranceAndExit();
    startSearchButton->setEnabled(true);
    log->clear();

}

/**
 * @brief This function handles mouse press events handed down from the mainwindow
 * @param me the mousepressevent
 */
void MazeUi::mousePressEvent(QMouseEvent *me){
    QGraphicsItem *current = view->itemAt(me->pos().x()-25,me->pos().y()-75); // Transpose them  -- buggy

    // Null check
    if(!current){
        return;
    }

    MazeNode *selectedMazeNode = listOfRectangles->value(current);

    if(selectedMazeNode == 0){
        return;
    }

    // Sets or unsets the wall at the clicked node
    if(me->button() == Qt::LeftButton){
        if(selectedMazeNode->isWall()){
            selectedMazeNode->unsetWall();
        }
        else{
            selectedMazeNode->setWall();
        }
    }


}


/**
 * @brief Sets a new grid size
 */
void MazeUi::setNewGridSize()
{

    if(rectSize == rectSizeList[1][gridSizeSelection->currentIndex()]){
        return;
    }
    rectSize = rectSizeList[1][gridSizeSelection->currentIndex()];
    scene->clear();

    foreach(MazeNode *item, *listOfRectangles){
        delete item;
    }

    listOfRectangles->clear();
    listOfIds->clear();
    addItemsToScene();
    startSearchButton->setEnabled(true);
}

/**
 * @brief Starts the search algorithm
 */
void MazeUi::startSearch()
{
    log->append("Starting search " + searchSelection->currentText());

    switchUiState();
    startSearchButton->setVisible(false);
    stopSearchButton->setVisible(true);

    int columns = sceneWidth / rectSize;
    int rows = sceneHeight / rectSize;

    solver->setParameters(listOfIds,rows,columns,tickIntervalSelector->value());

    if(searchSelection->currentText() == "DFS"){
        solver->startDFS();
    }
    else if(searchSelection->currentText() == "BFS"){
        solver->startBFS();
    }
    else{
        log->append("This algorithm hasn't been implemented yet");
        switchUiState();
    }
}

/**
 * @brief Stops the search
 */
void MazeUi::stopSearch()
{
    stopSearchButton->setVisible(false);
    startSearchButton->setVisible(true);
    solver->triggerStopSearch();
}

/**
 * @brief Traces back a path from the exit (if it exists)
 * @param lastNode the last node that has been visited (the exit)
 * @param nodeStack a stack onto which to push the previously visited nodes
 * @return the number of nodes in the path
 */
int MazeUi::tracePath(MazeNode* lastNode,QStack<int> *nodeStack)
{
    int counter = 0;
    while(lastNode != 0){
        lastNode->tracePath();
        nodeStack->push(lastNode->getId());
        lastNode = lastNode->getPreviousNode();
        counter++;
    }

    return counter;

}

/**
 * @brief Creates a random maze
 */
void MazeUi::createRandomMaze()
{
    clearMaze();
    foreach(MazeNode *item, *listOfRectangles){
        if(!item->isEntrance() || !item->isExit() ){
            int isWallDecider = qrand() % 3; // Pseudorandom remainder decides if it's a wall or not
            if(isWallDecider >= 2){
                item->setWall();
            }
        }
    }
    log->append("Created random maze");
}

/**
 * @brief Displays the result of the search in the log window
 * @param lastNode the last node visited
 */
void MazeUi::displayResult(MazeNode *lastNode)
{
    // First trace the path, then fill the log window
    if(lastNode != 0){
        QStack<int> *path = new QStack<int>;
        int lengthOfPath = tracePath(lastNode,path);
        fillLogWithPath(path);
        log->append("Length of the path: " + QString::number(lengthOfPath));
        log->append("Seconds elapsed: " + QString::number(solver->getTimeElapsed() / 1000.0,'f',3));
        delete path;
    }
    else{
        log->append("No path to the exit found");
    }

    // Switch the UI to disable searching until the maze is reset
    switchUiState();
    startSearchButton->setEnabled(false);
    startSearchButton->setVisible(true);
    stopSearchButton->setVisible(false);
}


