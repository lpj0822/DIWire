#ifndef PATHDATA_H
#define PATHDATA_H

#include <QObject>
#include<QPoint>

class PathData
{

public:
    PathData();
    ~PathData();

private:
    QString typeData;//指令类型
    QPoint startPoint;//起始坐标
    QPoint ctrl1Point;//控制点坐标1
    QPoint ctrl2Point;//控制点坐标2
    QPoint endPoint;//结束点坐标
    float rx;//弧的半长轴长度
    float ry;//弧的半短轴长度
    QPoint centerPoint;//中心点
    double startAngle;//起始角度
    double deltaAngle;//起始角度与终止角度差

public:
    //设置数据，获取数据
    void setTypeData(QString type);
    void setStartPoint(QPoint point);
    void setCtrl1Point(QPoint point);
    void setCtrl2Point(QPoint point);
    void setEndPoint(QPoint point);
    void setRx(float x);
    void setRy(float y);
    void setCenterPoint(QPoint point);
    void setStartAngle(double angle);
    void setDeltaAngle(double angle);

    QString getTypeData();
    QPoint getStartPoint();
    QPoint getCtrl1Point();
    QPoint getCtrl2Point();
    QPoint getEndPoint();
    float getRx();
    float getRy();
    QPoint getCenterPoint();
    double getStartAngle();
    double getDeltaAngle();
};

#endif // PATHDATA_H
