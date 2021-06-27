#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <mainwindow.h>
#include <mapqframe.h>
#include <QString>
#include <QSpinBox>
#include <pathfinder.h>

class MenuController;

class MenuController : public QObject
{
    Q_OBJECT
public:
    explicit MenuController(MainWindow*);
    MapQFrame* GetMapFrame();

    void PrintCursorCoords(int x, int y);
    void Create();

public slots:
    void ButtonsColor(bool s, bool s2, bool s3, bool s4);

signals:
    void OnRepaintRequested();

private:
    QPushButton* _startButton;
    QPushButton* _finishButton;
    QPushButton* _newPolygonButton;
    QPushButton* _Create;
    QPushButton* _DeletePolygonButton;
    QPushButton* _CreateARoute;

    QLabel* _label1;
    QLabel* _label2;
    QLabel* _label3;
    QLineEdit* _lineEdit;

    QSpinBox* _TraversabilitySpinBox;

    MapQFrame* _mapFrame;
    DataManager* dataManager;

    void FindPath();
};

#endif // MENUCONTROLLER_H
