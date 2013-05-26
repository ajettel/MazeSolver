#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGridLayout>
#include <QFormLayout>
#include <QTransform>
#include <QHash>
#include <QRadioButton>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QElapsedTimer>
#include <QGroupBox>
#include <QTextBrowser>
#include <QSpinBox>

#include "mazenode.h"
#include "msolver.h"

/**
 * @brief This class sets up the user interface for the maze
 */
class MazeUi :  public QGraphicsView
{
    Q_OBJECT
public:
    explicit MazeUi(QWidget* parent = 0);
public slots:
    void mousePressEvent(QMouseEvent *me);


private:
    // UI related objects
    QWidget* currentTab; ///< Currently selected tab
    QTextBrowser *log; ///< Log display
    QGridLayout *mazeTabLayout; ///< Layout that holds maze, log and  buttons
    QGraphicsScene *scene; ///< The graphics scene which is being painted on

    // Buttons
    QPushButton *changeGridSize;
    QPushButton *resetMazeButton;
    QPushButton *startSearchButton;
    QPushButton *clearMazeButton;
    QPushButton *stopSearchButton;

    QComboBox *gridSizeSelection; ///< Selector for the grid size
    QSpinBox *tickIntervalSelector;
    QHash<QGraphicsItem*,MazeNode*> *listOfRectangles; ///< A hashlist that lets us look up the nodes by their drawn rectangle
    QHash<int,MazeNode*> *listOfIds; ///< A hashlist that lets us look up the nodes by ID
    QComboBox *searchSelection; ///< Selector for the type of search algorithm
    MSolver *solver;
    int nodeDescriptionThreshold; ///< The threshold under which a mazenode should not display the description anymore
    int sceneHeight;
    int sceneWidth;
    int rectSize;  ///< The current rectangle sizes
    int **rectSizeList; ///< The list of available rectangle sizes


    ~MazeUi();
    void addItemsToMazeGroupBox(QGroupBox *mazeGroupBox);  ///< Adds visual elements to the group boxes in the UI
    void addItemsToToolsGroupBox(QGroupBox *toolsGroupBox); ///< Adds visual elements to the group boxes in the UI
    void addItemsToLogGroupBox(QGroupBox *logGroupBox); ///< Adds visual elements to the group boxes in the UI
    void addItemsToScene(); ///< Builds the empty maze
    void createGridSizeArray(); ///< Creates an array of the different possible grid sizes
    void drawMaze(); ///< draws the empty maze
    void fillLogWithPath(QStack<int>* path); ///< Writes the found path to the log
    void initializeMazeSolver();
    void setDefaultSelections();
    void setEntranceAndExit();
    void setupUI();
    void switchUiState(); ///< Switches UI elements on an off while searching
    int tracePath(MazeNode* lastNode,QStack<int>* nodeStack); ///< Displays a found path visually

private slots:
    void clearMaze();
    void createRandomMaze();
    void displayResult(MazeNode *lastNode);
    void resetMaze();
    void setNewGridSize();
    void startSearch();
    void stopSearch();

};

#endif // MAZESOLVER_H
