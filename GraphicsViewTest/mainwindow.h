#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QParallelAnimationGroup>
#include <QFont>
#include <QVector>
#include "mysimpletextitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
    void on_actionAdd_Items_triggered();
    void on_actionRemove_All_triggered();
    void on_actionStart_Simulation_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene _scene;
    QParallelAnimationGroup _group;
    QVector<QString> _texts;
    QVector<QFont> _fonts;
    QVector<MySimpleTextItem*> _textItems;
    void addTextItem(double w, double h);
    QFont getRandomFont();
    QString getRandomText();
    void addRotateTransform(MySimpleTextItem* item);
    void addTranslateTransform(MySimpleTextItem* item);
    void addScaleTransform(MySimpleTextItem* item);
};

#endif // MAINWINDOW_H
