#ifndef BEZIER_H
#define BEZIER_H

#include <QObject>
#include<QPoint>
#include<QPointF>
#include<QRect>
#include<QList>

#include"dataDef.h"

class Bezier : public QObject
{
    Q_OBJECT
public:
    explicit Bezier(QObject *parent = 0);
    ~Bezier();

    QList<QPoint> pointBezier3(QPoint startPoint,QPoint ctrlPoint1,QPoint ctrlPoint2,QPoint endPoint,int numPoint);//生成三次贝塞尔曲线点
    QList<QPoint> pointBezier2(QPoint startPoint,QPoint ctrlPoint,QPoint endPoint,int numPoint);//生成二次贝塞尔曲线点
    QList<QPoint> pointBezier1(QPoint startPoint,QPoint endPoint,int numPoint);//生成一次贝塞尔曲线点
    //四个控制点生成三次贝塞尔曲线点
    QList<QPoint> pointDrawBezier3(QPoint ctrlPoint[4],int numPoint);
    //生成二次以上贝塞尔曲线点
    QList<QPoint> pointDrawBezier(QPoint ctrlPoint[10],int n,int numPoint);
    //通过贝塞尔曲线算法生成椭圆弧上的点
    QList<QPoint> pointArcToBezier(QPoint centerPoint,float rx,float ry,double startAngle,double sweepAngle,int numPoint);
    //通过贝塞尔曲线算法生成椭圆上的点
    QList<QPoint> pointEllipseToBezier(QPoint centerPoint,float rx,float ry,int numPoint);

    //将一个椭圆转换为4段三次贝塞尔曲线
    QList<QPoint> ellipseToBezier(QPoint centerPoint,float rx, float ry);

    //将一个圆角矩形转换为4段三次贝塞尔曲线
    QList<QPoint> roundRectToBezier(QRect rect,float rx, float ry);

    //将一个椭圆弧转换为多段三次贝塞尔曲线
    int tAngleArcToBezier(QPoint points[16],QPoint centerPoint,float rx, float ry,double startAngle, double sweepAngle);

signals:

public slots:

private:
    QPoint Bezier3(QPoint startPoint,QPoint ctrlPoint1,QPoint ctrlPoint2,QPoint endPoint,double t);//三次贝塞尔曲线算法
    QPoint Bezier2(QPoint startPoint,QPoint ctrlPoint,QPoint endPoint,double t);//二次贝塞尔曲线算法
    QPoint Bezier1(QPoint startPoint,QPoint endPoint,double t);//一次贝塞尔曲线算法

    QPoint drawBezier3(QPoint ctrlPoint[4],double t);//四个控制点的贝塞尔曲线 即三次Bezier曲线

    //将一个椭圆弧转换为多段三次贝塞尔曲线
    void angleArcToBezier(QPoint points[4],QPoint centerPoint,float rx, float ry,double startAngle, double sweepAngle);
    void arcToBezier(QPoint points[4],QPoint centerPoint, float rx, float ry, double start_angle, double sweep_angle);
    int angleArcToBezierPlusSweep(QPoint points[16],QPoint centerPoint,float rx, float ry,double startAngle, double sweepAngle);
    //将数转换到数值范围[tmin, tmax)内
    double toRange(double value, double tmin, double tmax);

signals:

public slots:
};

#endif // BEZIER_H
