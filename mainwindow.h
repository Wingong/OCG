#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chsheet.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow  *ui;
    ChSheet         sheet;
    int             minStroke;
    int             maxStroke;
    int             minIndex;
    int             maxIndex;

    QRegExp rePunc = QRegExp("[\\s -~！￥…（）{}【】、；：‘’“”，《。》？]");
};

#endif // MAINWINDOW_H
