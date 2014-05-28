#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "dataitem.h"
#include "namesviewmodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    NamesViewModel viewModel;
    viewModel.addName(DataItem("Item numero 1", 23));
    viewModel.addName(DataItem("Item Numero 1", 231));
    viewModel.addName(DataItem("Item Numero 3", 18));
    viewModel.addName(DataItem("Item Numero 4", 21));

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("nameModel", &viewModel);
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    return app.exec();
}
