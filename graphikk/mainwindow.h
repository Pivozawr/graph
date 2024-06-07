#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QPushButton* button;
    ~MainWindow();

private slots:
    void Clicked();

private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
