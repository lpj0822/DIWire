#include "pathdata.h"

PathData::PathData()
{

}

PathData::~PathData()
{

}

void PathData::setTypeData(QString type)
{
    this->typeData=type;
}

void PathData::setStartPoint(QPoint point)
{
    this->startPoint=point;
}

void PathData::setCtrl1Point(QPoint point)
{
    this->ctrl1Point=point;
}

void PathData::setCtrl2Point(QPoint point)
{
    this->ctrl2Point=point;
}

void PathData::setEndPoint(QPoint point)
{
    this->endPoint=point;
}

void PathData::setRx(float x)
{
    this->rx=x;
}

void PathData::setRy(float y)
{
    this->ry=y;
}

void PathData::setCenterPoint(QPoint point)
{
    this->centerPoint=point;
}

void PathData::setStartAngle(double angle)
{
    this->startAngle=angle;
}

void PathData::setDeltaAngle(double angle)
{
    this->deltaAngle=angle;
}

QString PathData::getTypeData()
{
    return typeData;
}

QPoint PathData::getStartPoint()
{
    return startPoint;
}

QPoint PathData::getCtrl1Point()
{
    return ctrl1Point;
}

QPoint PathData::getCtrl2Point()
{
    return ctrl2Point;
}

QPoint PathData::getEndPoint()
{
    return endPoint;
}

float PathData::getRx()
{
    return rx;
}

float PathData::getRy()
{
    return ry;
}

QPoint PathData::getCenterPoint()
{
    return centerPoint;
}

double PathData::getStartAngle()
{
    return startAngle;
}

double PathData::getDeltaAngle()
{
    return deltaAngle;
}

