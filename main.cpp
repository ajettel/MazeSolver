/**\mainpage
 *
 * This application allows the user to create and solve a maze with Breadth First Search and
 * Depth First Search algorithms (BFS DFS). It includes a GUI which allows for manual maze
 * changes.
 *
 * To set a wall, left click on a square. To unset, left click again.
 * While the maze is being solved, the red squares denote the visited
 * nodes.When finished, the path found by the algorithms are
 * denoted in green. Also, some stats are being output in the log pane
 *
 * The rest of the app should be self-explanatory.
 *
 *
 * This application is relased under the LGPL and written in QT C++.
 * If you find this software useful or are using it I'd be happy about an email.
 */
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
