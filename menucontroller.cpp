#include "menucontroller.h"
#include "QDebug"

template<typename T>
T* FindChild(MainWindow* w, QString name)
{
   T* obj = w->findChild<T*>(name);
   if (obj == NULL)
   {
        qDebug() << "No object with such name was found. (" << name << ").";
   }
   return obj;
}

MenuController::MenuController(MainWindow* w) : QObject(w)
{
    dataManager = new DataManager();

    //поиск элементов
    _startButton = FindChild<QPushButton>(w, "StartButton");
    _finishButton = FindChild<QPushButton>(w, "FinishButton");
    _newPolygonButton = FindChild<QPushButton>(w, "NewPolygonButton");
    _Create = FindChild<QPushButton>(w, "CreatePolygon");
    _DeletePolygonButton = FindChild<QPushButton>(w, "DeletePolygonButton");
    _CreateARoute = FindChild<QPushButton>(w, "Create");
    _label1 = FindChild<QLabel>(w, "LogLabel_1");
    _label2 = FindChild<QLabel>(w, "LogLabel_2");
    _label3 = FindChild<QLabel>(w, "LogLabel_3");
    _mapFrame = FindChild<MapQFrame>(w, "MapFrame");
    _TraversabilitySpinBox = FindChild<QSpinBox>(w, "SpinBox");

    //привязка ивентов
    connect(_startButton, &QPushButton::released, _mapFrame, &MapQFrame::changeStartMode);
    connect(_finishButton, &QPushButton::released, _mapFrame, &MapQFrame::changeFinishMode);
    connect(_newPolygonButton, &QPushButton::released, _mapFrame, &MapQFrame::changeNewPolygonMode);
    connect(_DeletePolygonButton, &QPushButton::released, _mapFrame, &MapQFrame::changeDeletePolygonMode);
    connect(_Create, &QPushButton::released, [=]{ MenuController::Create ();});
    connect(_CreateARoute, &QPushButton::released, _mapFrame, &MapQFrame::FindPath);

    _TraversabilitySpinBox ->hide();
    _Create ->hide();
}

//создание препятсвия
void MenuController::Create (){
    _mapFrame->trace_ability = _TraversabilitySpinBox->value();
    _mapFrame->createNewPolygonMode();
}

MapQFrame* MenuController::GetMapFrame()
{
    return _mapFrame;
}

//ищем путь
/*
void MenuController::FindPath()
{
    qDebug() << "Trying to Find Path";
    QElapsedTimer timer;
    timer.start();

    QVector<Node*> path;
    //Node* startNode = dataManager->startNode;
    //Node* endNode = dataManager->endNode;

    Node* startNode = dataManager->startNode;
    Node* endNode = dataManager->endNode;


    if (startNode == nullptr || endNode == nullptr)
    {
        qDebug() << "Cannot find path.";
        return;
    }
    qDebug() << "Sx " << startNode->x;
    qDebug() << "Sy " << startNode->y;
    qDebug() << "Fx " << endNode->x;
    qDebug() << "Fy " << endNode->y;
    //qDebug() << "M " << dataManager->map;
    //dataManager ->startNode = startNode;
    //dataManager -> endNode = endNode;
    dataManager->BuildMap();
    dataManager->lastFoundPath = Pathfinder::FindPath(startNode, endNode, dataManager->map);
    path = dataManager->lastFoundPath;
    qDebug() << "Found Path(" << dataManager->lastFoundPath.count() << ") in " << timer.elapsed() << "ms.";

    _mapFrame->Repaint(dataManager);

}
*/
//режимы
void MenuController::ButtonsColor(bool stateS, bool stateF, bool stateNP, bool stateDP)
{
    _label1->setWordWrap(true);
    _label2->setWordWrap(true);
    _label3->setWordWrap(true);

    _startButton->setStyleSheet("background-color:grey");
    _finishButton->setStyleSheet("background-color:grey");
    _DeletePolygonButton->setStyleSheet("background-color:grey");
    _newPolygonButton->setStyleSheet("background-color:grey");
    _DeletePolygonButton->setStyleSheet("background-color:grey");
    _label1->setText (" ");
    _label2->setText (" ");
    _label3->setText (" ");

    _TraversabilitySpinBox ->hide();
    _Create ->hide();

    if (stateS){
        _startButton->setStyleSheet("background-color:green");
        _label2->setText("РЕЖИМ УСТАНОВКИ СТАРТА");
        _label3->setText("Для постновки начала маршрута кликните по нужному месту на карте.");
    }


    if (stateF){
        _finishButton->setStyleSheet("background-color:red");
        _label2->setText("РЕЖИМ УСТАНОВКИ ФИНИША");
        _label3->setText("Для постновки конечной точки маршрута кликните по нужному месту на карте.");
    }

    if (stateNP) {
        _newPolygonButton->setStyleSheet("background-color:blue");
        _label2->setText("РЕЖИМ СОЗДАНИЯ ПРЕПЯТСТВИЯ");
        _label3->setText("Для создания препятствия отметьте крайние точки препятсвия и уажите проходимость.");
        _label1->setText ("По умолчанию проходимость устанавливается равной 0 (пройти через объект невозможно).");
        _TraversabilitySpinBox ->show();
        _Create ->show();
    }

    if (stateDP) {
        _DeletePolygonButton->setStyleSheet("background-color:blue");
        _label2->setText("РЕЖИМ УДАЛЕНИЯ ПРЕПЯТСВИЯ");
        _label3->setText("Кликните по препятсвию, которое необходимо удалить.");
    }

    _CreateARoute->setStyleSheet("background-color:grey");
}


