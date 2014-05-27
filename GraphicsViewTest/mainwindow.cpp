#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGLWidget>
#include <QGraphicsSimpleTextItem>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    ui->graphicsView->setScene(&_scene);

    _texts.append(QString("Hello World"));
    _texts.append(QString("Testing this"));
    _texts.append(QString("Ålesund"));
    _texts.append(QString("Tromsøgata"));
    _texts.append(QString("Fornebuveien 31"));
    _texts.append(QString("Trondhjems gata"));
    _texts.append(QString("Vøyenenga"));

    _fonts.append(QFont("Arial", 12, 2, false));
    _fonts.append(QFont("Times New Roman", 11, 2, true));
    _fonts.append(QFont("Calibri", 10, 2, false));
    _fonts.append(QFont("Courier New", 21, 2, false));
    _fonts.append(QFont("Britannic Bold", 7, 2, false));
    _fonts.append(QFont("Broadway Regular", 16, 2, false));
    _fonts.append(QFont("Onyx Regular", 14, 2, false));
    _fonts.append(QFont("Sitka Text", 8, 2, false));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionAdd_Items_triggered()
{
    qreal w = ui->graphicsView->width();
    qreal h = ui->graphicsView->height();
    _scene.addRect(0, 0, w, h);
    for(int i = 0; i < 500; i++) {
        addTextItem(w, h);
    }
}

void MainWindow::addTextItem(double w, double h)
{
    double x = rand()%(int)w;
    double y = rand()%(int)h;
    MySimpleTextItem* item = new MySimpleTextItem(getRandomText());
    item->setFont(getRandomFont());
    item->setPos(x, y);
    _scene.addItem(item);
    _textItems.append(item);
}

QFont MainWindow::getRandomFont()
{
    int idx = rand()%_fonts.size();
    return _fonts[idx];
}

QString MainWindow::getRandomText()
{
    int idx = rand()%_texts.size();
    return _texts[idx];
}

void MainWindow::on_actionRemove_All_triggered()
{
    qDeleteAll(_scene.items());
    _scene.setSceneRect(_scene.itemsBoundingRect());
}

void MainWindow::on_actionStart_Simulation_triggered()
{
    int w = (int)_scene.width();
    int h = (int)_scene.height();

    for(int i = 0; i < _textItems.size(); i++) {
        MySimpleTextItem* item = _textItems[i];
        QPropertyAnimation* anRotation = new QPropertyAnimation(item, "rotation");
        anRotation->setStartValue(0.0);
        anRotation->setEndValue(360.0);
        int ms = rand()%4000 + 1000;
        anRotation->setDuration(ms);
        _group.addAnimation(anRotation);

        QPropertyAnimation* anX = new QPropertyAnimation(item, "x");
        anX->setStartValue(item->x());
        qreal endW = rand()%w;
        anX->setEndValue(endW);
        ms = rand()%4000 + 1000;
        anX->setDuration(ms);
        _group.addAnimation(anX);

        QPropertyAnimation* anY = new QPropertyAnimation(item, "rotation");
        anY->setStartValue(item->y());
        qreal endH = rand()%h;
        anY->setEndValue(endH);
        ms = rand()%4000 + 1000;
        anY->setDuration(ms);
        _group.addAnimation(anY);
    }
    _group.setLoopCount(2147000000);
    _group.start();
}
