#include "mainwindow.h"

#include <menucontroller.h>
#include <QApplication>
#include <mapqframe.h>
#include <datamanager.h>

MenuController* menuController;
MapQFrame* mapFrame;
DataManager* dataManager;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    menuController = new MenuController(&w);
    mapFrame = menuController->GetMapFrame();

    dataManager = new DataManager();
    mapFrame->dataManager = dataManager;


    QObject::connect(mapFrame, &MapQFrame::ChangeButtons, menuController, &MenuController::ButtonsColor);

    return a.exec();
}
