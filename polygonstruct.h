#ifndef POLYGONSTRUCT_H
#define POLYGONSTRUCT_H

#include <QPolygon>

class PolygonStruct
{
public:
    PolygonStruct(){};
    PolygonStruct(QPolygon*);

    QPolygon* GetPolygon();
    void SetTraversability(int value);
    int GetTraversability();
    int traversability;

private:
    //int traversability;//0-100;
    QPolygon* polygon;

};

#endif // POLYGONSTRUCT_H
