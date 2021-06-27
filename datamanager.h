#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QVector>
#include <QPoint>
#include <polygonstruct.h>
#include <map.h>
#include <QPolygon>
#include <qelapsedtimer.h>

class DataManager
{
public:
    explicit DataManager();

    PolygonStruct* TryCreateNewPolygon(QVector<QPoint> pointList, bool& result);
    void TryDeletePolygon(int ind);
    QVector<PolygonStruct*> GetAllPolygons();
    Map* GetMap();
    QVector<PolygonStruct*> PolygonList;
    void BuildMap();

    QVector<Node*> lastFoundPath;
    Node* startNode = nullptr;
    Node* endNode = nullptr;
    Map* map;
};

#endif // DATAMANAGER_H
