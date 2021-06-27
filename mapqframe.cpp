#include "mapqframe.h"
#include <QPainter>
#include <QPoint>
#include <Qt>
#include <datamanager.h>

MapQFrame::MapQFrame(QWidget* parent) : QFrame(parent){}

void MapQFrame::paintEvent(QPaintEvent *event)
{
    DrawPolygons();
    QPainter painter(this);


    painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::FlatCap));
    if (NewPointsVector.length() != 0 and NewPolygonMode){
        for (int i = 0; i < NewPointsVector.length(); i++)
        {
            painter.drawEllipse(NewPointsVector.value(i).x(), NewPointsVector.value(i).y(), 5, 5);
        }
    }

    //draw path
    for (int i = 0; i < dataManager->lastFoundPath.count(); i++)
    {
        Node* node = dataManager->lastFoundPath.value(i);
        QColor* color = new QColor(223, 234, 33);
        painter.drawEllipse(node->x, node->y, 5, 5);
        //DrawCircle(painter, node->x, node->y, *color, 3);
        delete color;
    }

    painter.setPen(QPen(Qt::green, 3, Qt::SolidLine, Qt::FlatCap));
    painter.drawEllipse(StartX, StartY, 5, 5);

    painter.setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::FlatCap));
    painter.drawEllipse(FinishX, FinishY, 5, 5);

    painter.end();

}

void MapQFrame::Repaint(DataManager *dataManager)
{
    this->dataManager = dataManager;
    repaint();
}

void MapQFrame::DrawPolygons()
{
    QPainter painter(this);
    QVector<PolygonStruct*> polygons = dataManager->GetAllPolygons();
    for (int i = 0; i < polygons.count(); i++)
    {
        PolygonStruct* polygon = polygons.value(i);
        int rgb = polygon->GetTraversability() * 2.55;
        painter.setBrush(*new QBrush(*new QColor(rgb, rgb, rgb)));
        painter.drawPolygon(*polygon->GetPolygon(), Qt::WindingFill);
    }

    painter.end();
}

void MapQFrame::DrawNode(Node *node, QColor* color)
{
    QPainter painter(this);
    painter.setPen(*color);
    painter.drawPoint(node->x, node->y);
        painter.setPen(QPen(Qt::green, 3, Qt::SolidLine, Qt::FlatCap));
        painter.drawEllipse(StartX, StartY, 5, 5);

        painter.setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::FlatCap));
        painter.drawEllipse(FinishX, FinishY, 5, 5);
}

void MapQFrame::mousePressEvent(QMouseEvent* ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        if (StartMode)
        {
            StartX=ev->x();
            StartY=ev->y();
            dataManager->startNode = dataManager->map->NodeAt(StartX, StartY);
            /*if (dataManager->startNode==nullptr) dataManager->startNode = new Node(StartX,  StartY);
            else {
                dataManager->startNode->x=StartX;
                dataManager->startNode->y= StartY;
            }*/
        }
        if (FinishMode)
        {
            FinishX=ev->x();
            FinishY=ev->y();
            dataManager->endNode = dataManager->map->NodeAt(FinishX, FinishY);
            /*if (dataManager->endNode==nullptr) dataManager->endNode = new Node(FinishX, FinishY);
            else {
                dataManager->endNode->x=FinishX;
                dataManager->endNode->y=FinishY;
            }*/
        }
        if (NewPolygonMode)
        {
            //записываем очередную точку полигона
            QPoint* PolygonPoint = new QPoint();
            PolygonPoint->rx()=ev->x();
            PolygonPoint->ry()=ev->y();
            NewPointsVector.append(*PolygonPoint);
        }
        if (DeletePolygonMode){
            QVector<PolygonStruct*> polygons = dataManager->GetAllPolygons();
            for (int i = polygons.length()-1; i >= 0; i--){
                QPolygon* polygon = polygons.value(i)->GetPolygon();
                if (polygon->containsPoint(QPoint(ev->x(), ev->y()), Qt::OddEvenFill) == 1){
                    qDebug() << "Find delete";

                    dataManager->TryDeletePolygon(i);

                    repaint();
                    break;
                }
            }
        }
        repaint();
        emit OnMousePressed(ev->x(), ev->y());

    }
}

void MapQFrame::changeStartMode()
{
    StartMode=!StartMode;
    FinishMode=false;
    if (NewPolygonMode) changeNewPolygonMode();
    emit ChangeButtons(StartMode, FinishMode, NewPolygonMode, DeletePolygonMode);
}

void MapQFrame::changeFinishMode()
{
    FinishMode=!FinishMode;
    StartMode=false;
    if (NewPolygonMode) changeNewPolygonMode();
    DeletePolygonMode = false;

    emit ChangeButtons(StartMode, FinishMode, NewPolygonMode, DeletePolygonMode);
}

void MapQFrame::changeNewPolygonMode()
{
    NewPolygonMode=!NewPolygonMode;
    StartMode=false;
    FinishMode=false;
    DeletePolygonMode = false;
    emit ChangeButtons(StartMode, FinishMode, NewPolygonMode, DeletePolygonMode);

    if (NewPolygonMode) //если только включили режим
    {
        NewPointsVector  = *(new QVector<QPoint>); //создаем вектор/массив для точек полигон
    }
    else repaint();
}

void MapQFrame::createNewPolygonMode(){
    bool addResult;
    PolygonStruct* newPolygon = dataManager->TryCreateNewPolygon(NewPointsVector, addResult);
    newPolygon->SetTraversability(trace_ability);
    repaint();
    changeNewPolygonMode();
}

void MapQFrame::changeDeletePolygonMode(){
    DeletePolygonMode = !DeletePolygonMode;
    StartMode = false;
    FinishMode = false;
    if (NewPolygonMode) changeNewPolygonMode();
    emit ChangeButtons(StartMode, FinishMode, NewPolygonMode, DeletePolygonMode);

}

void MapQFrame::FindPath(){
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

    repaint();

}




