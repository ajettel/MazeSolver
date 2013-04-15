#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QHash>
#include <QPoint>
#include <QMouseEvent>
#include <QGridLayout>

#include "mazenode.h"
#include "mazeui.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief The mainwindow sets up the maze ui
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    // UI related entities
    Ui::MainWindow *ui;
    MazeUi *maze; /// Pointer to the maze GUI item

    /// The main window height
    int windowHeight;
    /// The main window width
    int windowWidth;

    // Functions
    void setupUI();
    void mousePressEvent(QMouseEvent *me);


};

#endif // MAINWINDOW_H
