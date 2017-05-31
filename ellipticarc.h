#ifndef ELLIPTICARC_H
#define ELLIPTICARC_H

#include <QObject>
#include<QPoint>

#include"dataDef.h"

class EllipticArc
{

public:
    EllipticArc();
    ~EllipticArc();

signals:

public slots:

private:
    QPoint centerPoint;//中心点
    double startAngle;//起始角度
    double deltaAngle;//起始角度与终止角度差

    double radian(double ux,double uy,double vx,double vy);

public:
    //设置 获取值
    void setCenterPoint(QPoint point);

    void setStartAngle(double angle);

    void setDeltaAngle(double angle);

    QPoint getCenterPoint();

    double getStartAngle();

    double getDeltaAngle();

    //根据svg中路径A指令参数计算椭圆弧的圆心和起始角度、扫描角度
    void Aconvert(double x1,double y1,double x2,double y2,int fA,int fS,double rx,double ry,double phi);
};

#endif // ELLIPTICARC_H
