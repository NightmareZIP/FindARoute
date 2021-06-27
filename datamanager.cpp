#include "datamanager.h"
#include <QDebug>

DataManager::DataManager()
{
    map = new Map();
}

PolygonStruct* DataManager::TryCreateNewPolygon(QVector<QPoint> pointList, bool &result)
{
    if (pointList.count() > 2)
    {
        QPolygon* newPolygon = new QPolygon(pointList);
        PolygonStruct* ps = new PolygonStruct(newPolygon);
        PolygonList.append(ps);
        qDebug() << "Created new polygon.";
        result = true;
        return ps;
    }
    result = false;
    return nullptr;
}

void DataManager::TryDeletePolygon(int ind){
    PolygonStruct *polygon = PolygonList.value(ind);
     PolygonList.remove(ind,1);
    delete polygon;
}

QVector<PolygonStruct*> DataManager::GetAllPolygons()
{
    return PolygonList;
}

Map *DataManager::GetMap()
{
    return map;
}

void DataManager::BuildMap()
{
    QElapsedTimer timer;
    timer.start();

    //максимально не оптимизированно, но у меня нет других идей
    for (int x = 0; x < map->GetSize(); x++)
    {
        for (int y = 0; y < map->GetSize(); y++)
        {
            QPoint* point = new QPoint(x, y);
            map->NodeAt(x, y)->traversability = 100;
            for (int i = PolygonList.count() - 1; i >= 0; i--)
            {
                PolygonStruct* ps = PolygonList.value(i);
                if (ps->GetPolygon()->containsPoint(*point, Qt::WindingFill))
                {
                    map->NodeAt(x, y)->traversability = ps->GetTraversability();
                    break;
                }
            }
            delete point;
        }
    }
}
