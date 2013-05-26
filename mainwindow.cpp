#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mazenode.h"
#include <QDebug>

/**
 *
 * @brief This class sets up the UI and initializes the Mazesolver class
 * @param parent A widget as a parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Fixed Window Height
    windowWidth = 1024;
    windowHeight = 700;

    setupUI();
}
/**
 * @brief The destructor of the UI
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief This function sets up the UI and initializes the visible MazeWidget
 */
void MainWindow::setupUI(){
    // Load layout from form
    ui->setupUi(this);

    // Setting the size of the window
    this->setMinimumHeight(windowHeight);
    this->setMinimumWidth(windowWidth);

    // Setting the size of the tabwidget within the window
    ui->tabWidget->setMinimumHeight(windowHeight - 100);
    ui->tabWidget->setMinimumWidth(windowWidth - 100);

    // Setting Mousetracking. Needed to track user clicks within the widget
    this->setMouseTracking(true);
    ui->tabWidget->setMouseTracking(true);

    maze = new MazeUi(ui->tabWidget->widget(0));

    this->showMaximized();
}

