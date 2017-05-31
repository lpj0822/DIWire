#pragma execution_character_set("utf-8")
#include "bezier.h"
#include<QMessageBox>
#include<QDebug>

Bezier::Bezier(QObject *parent) : QObject(parent)
{

}

Bezier::~Bezier()
{

}

//三次贝塞尔曲线算法
QPoint Bezier::Bezier3(QPoint startPoint, QPoint ctrlPoint1, QPoint ctrlPoint2, QPoint endPoint, double t)
{
    QPoint point;
    double a1 = pow((1-t)*1.0,3);
    double a2 = pow((1-t)*1.0,2)*3.0*t;
    double a3 = 3.0*t*t*(1-t);
    double a4 = t*t*t;

    float x=a1*startPoint.x()+a2*ctrlPoint1.x()+a3*ctrlPoint2.x()+a4*endPoint.x();
    float y=a1*startPoint.y()+a2*ctrlPoint1.y()+a3*ctrlPoint2.y()+a4*endPoint.y();

    point.setX(x);
    point.setY(y);
    return point;
}

//二次贝塞尔曲线算法
QPoint Bezier::Bezier2(QPoint startPoint, QPoint ctrlPoint, QPoint endPoint, double t)
{
    // 计算曲线点坐标，此为2次算法，改变此处可以实现多次曲线
    QPoint point;
    float x = (double)startPoint.x()* pow(1 - t, 2)   +
                (double)ctrlPoint.x() * t * (1 - t) * 2 +
                  (double)endPoint.x() * pow(t, 2);
    float y = (double)startPoint.y() * pow(1 - t, 2)   +
                  (double)ctrlPoint.y() * t * (1 - t) * 2 +
                  (double)endPoint.y() * pow(t, 2);
    point.setX(x);
    point.setX(y);
    return point;
}

//一次贝塞尔曲线算法
QPoint Bezier::Bezier1(QPoint startPoint, QPoint endPoint, double t)
{
    QPoint point;
    float x=(1.0-t)*1.0*startPoint.x()+t*1.0*endPoint.x();
    float y=(1.0-t)*1.0*startPoint.y()+t*1.0*endPoint.y();
    point.setX(x);
    point.setY(y);
    return point;
}

//生成三次贝塞尔曲线点
QList<QPoint> Bezier::pointBezier3(QPoint startPoint, QPoint ctrlPoint1, QPoint ctrlPoint2, QPoint endPoint, int numPoint)
{
    // t的增量， 可以通过setp大小确定需要保存的曲线上点的个数
    double step=1.0/numPoint;
    QPoint point;
    QList<QPoint> list;
    double t=0.0;
    for (t = 0.0; t<1.0+0.001;t+=step)
    {
        point=Bezier3(startPoint,ctrlPoint1,ctrlPoint2,endPoint,t);
        list.append(point);
    }
    //qDebug()<<"三次贝塞尔曲线:"<<list;
    return list;
}

//生成二次贝塞尔曲线点
QList<QPoint> Bezier::pointBezier2(QPoint startPoint, QPoint ctrlPoint, QPoint endPoint, int numPoint)
{
    // t的增量， 可以通过setp大小确定需要保存的曲线上点的个数
    double step = 1.0/numPoint;
    QList<QPoint> list;
    QPoint point;
    double t=0.0;
    for (t = 0.0; t < 1.0+0.001; t += step)
    {
         point=Bezier2(startPoint,ctrlPoint,endPoint,t);
         list.append(point);
    }
    //qDebug()<<"二次贝塞尔曲线:"<<list;
    return list;
}

QList<QPoint> Bezier::pointBezier1(QPoint startPoint, QPoint endPoint, int numPoint)
{
    // t的增量， 可以通过setp大小确定需要保存的曲线上点的个数
    double step = 1.0/numPoint;
    QList<QPoint> list;
    QPoint point;
    double t=0.0;
    for (t = 0.0; t < 1.0+0.001; t += step)
    {
         point=Bezier1(startPoint,endPoint,t);
         list.append(point);
    }
    //qDebug()<<"一次贝塞尔曲线:"<<list;
    return list;
}

//四个控制点的贝塞尔曲线 即三次Bezier曲线
QPoint Bezier::drawBezier3(QPoint ctrlPoint[], double t)
{
    double ax, bx, cx;
    double ay, by, cy;
    double tSquared, tCubed;
    QPoint result;
    /* 计算多项式系数 */
    cx = 3.0 * (ctrlPoint[1].x() - ctrlPoint[0].x());
    bx = 3.0 * (ctrlPoint[2].x() - ctrlPoint[1].x()) - cx;
    ax = ctrlPoint[3].x() - ctrlPoint[0].x() - cx - bx;
    cy = 3.0 * (ctrlPoint[1].y() - ctrlPoint[0].y());
    by = 3.0 * (ctrlPoint[2].y() - ctrlPoint[1].y()) - cy;
    ay = ctrlPoint[3].y() - ctrlPoint[0].y() - cy - by;
    /* 计算t位置的点值 */
    tSquared = t * t;
    tCubed = tSquared * t;
    result.setX((ax * tCubed) + (bx * tSquared) + (cx * t) + ctrlPoint[0].x());
    result.setY((ay * tCubed) + (by * tSquared) + (cy * t) + ctrlPoint[0].y());
    return result;
}

//四个控制点生成三次贝塞尔曲线点
QList<QPoint> Bezier::pointDrawBezier3(QPoint ctrlPoint[], int numPoint)
{

    double dt=0;
    int loop=0;
    QPoint point;
    QList<QPoint> list;
    dt = 1.0 / (numPoint - 1 );
    for( loop = 0; loop< numPoint; loop++)
    {
        point = drawBezier3(ctrlPoint, loop*dt );
        list.append(point);
    }
    return list;
}

//生成二次以上贝塞尔曲线点
QList<QPoint> Bezier::pointDrawBezier(QPoint ctrlPoint[],int n, int numPoint)
{
    QList<QPoint> list;
    QPointF *point;
    if(n<2)
    {
        QMessageBox::warning(NULL,tr("贝塞尔曲线参数"),tr("贝塞尔曲线参数应大于等于二"));
        return list;
    }
    point=new QPointF[n-1];

    for(double t = 0.0; t <=1; t += 0.05 / n) // 调整参数t,计算贝塞尔曲线上的点的坐标,t即为上述u
    {
          for(int i = 1; i < n; ++i)
          {
               for(int j = 0; j < n - i; ++j)
               {

                   if(i == 1) // i==1时,第一次迭代,由已知控制点计算
                   {
                       point[j].setX(ctrlPoint[j].x() * (1 - t) + ctrlPoint[j + 1].x() * t);
                       point[j].setY(ctrlPoint[j].y()* (1 - t) + ctrlPoint[j + 1].y() * t);
                       continue;
                   }
                   // i != 1时,通过上一次迭代的结果计算
                   point[j].setX(point[j].x() * (1 - t) + point[j + 1].x() * t);
                   point[j].setY(point[j].y() * (1 - t) + point[j + 1].y() * t);
               }
           }
          list.append(point[0].toPoint());
    }
    return list;
}

//通过贝塞尔曲线算法生成椭圆弧上的点
QList<QPoint> Bezier::pointArcToBezier(QPoint centerPoint, float rx, float ry, double startAngle, double sweepAngle, int numPoint)
{
    QList<QPoint> list;
    QList<QPoint> blist;
    QPoint points[16];
    int n=0;
    int loop=0;
    int num1=0;
    int num2=0;
    int num3=0;
    int num4=0;
    double endAngle=0.0;
    n=tAngleArcToBezier(points,centerPoint,rx,ry,startAngle,sweepAngle);
    // 计算第一段椭圆弧的终止角度
    if (startAngle < MPI2)
    {
        // +Y
        endAngle = MPI2;
    }
    else if (startAngle < MPI)
    {
        // -X
        endAngle = MPI;
    }
    else if (startAngle < 3.0*MPI2)
    {
        // -Y
        endAngle = 3.0*MPI2;
    }
    else
    {
        // +X
        endAngle = M2PI;
    }
    if(n<4)
    {
        QMessageBox::warning(NULL,tr("椭圆弧参数"),tr("椭圆弧参数有误!"));
        return list;
    }
    if(n==4)
    {
        num1=numPoint;
    }
    else if(n==7)
    {
        num1=(endAngle-startAngle)/fabs(sweepAngle)*numPoint;
        num2=numPoint-num1;
    }
    else if(n==10)
    {
        num1=(endAngle-startAngle)/fabs(sweepAngle)*numPoint;
        num2=MPI2/fabs(sweepAngle)*numPoint;
        num3=numPoint-num1-num2;
    }
    else if(n==13)
    {
        num1=(endAngle-startAngle)/fabs(sweepAngle)*numPoint;
        num2=MPI2/fabs(sweepAngle)*numPoint;
        num3=MPI2/fabs(sweepAngle)*numPoint;
        num4=numPoint-num1-num2-num3;
    }
    else
    {
        QMessageBox::warning(NULL,tr("椭圆弧坐标点"),tr("图圆弧坐标点有误!"));
        return list;
    }
    loop=1;
    while(loop<n)
    {
        if(loop<4)
        {
             blist=pointBezier3(points[loop-1],points[loop],points[loop+1],points[loop+2],num1);
        }
        else if(loop<7)
        {
            blist=pointBezier3(points[loop-1],points[loop],points[loop+1],points[loop+2],num2);
        }
        else if(loop<10)
        {
             blist=pointBezier3(points[loop-1],points[loop],points[loop+1],points[loop+2],num3);
        }
        else if(loop<13)
        {
             blist=pointBezier3(points[loop-1],points[loop],points[loop+1],points[loop+2],num4);
        }
        for(int loop1=0;loop1<blist.size();loop1++)
        {
            list.append(blist[loop1]);
        }
        blist.clear();
        loop=loop+3;
    }
    return list;
}

//通过贝塞尔曲线算法生成椭圆上的点
QList<QPoint> Bezier::pointEllipseToBezier(QPoint centerPoint, float rx, float ry,int numPoint)
{
    QList<QPoint> list;
    QList<QPoint> ctrlList;
    QList<QPoint> blist;
    int loop=0;
    int num=0;
    ctrlList=ellipseToBezier(centerPoint,rx,ry);
    if(numPoint%4!=0)
    {
        num=(numPoint+numPoint%4)*1.0/4.0;
    }
    else
    {
        num=numPoint*1.0/4.0;
    }
    if(ctrlList.size()<13)
    {
        QMessageBox::warning(NULL,tr("椭圆参数"),tr("椭圆参数有误!"));
        return list;
    }
    blist=pointBezier3(ctrlList[0],ctrlList[1],ctrlList[2],ctrlList[3],num);
    for(loop=0;loop<blist.size();loop++)
    {
        list.append(blist[loop]);
    }
    blist.clear();
    blist=pointBezier3(ctrlList[3],ctrlList[4],ctrlList[5],ctrlList[6],num);
    for(loop=0;loop<blist.size();loop++)
    {
        list.append(blist[loop]);
    }
    blist.clear();
    blist=pointBezier3(ctrlList[6],ctrlList[7],ctrlList[8],ctrlList[9],num);
    for(loop=0;loop<blist.size();loop++)
    {
        list.append(blist[loop]);
    }
    blist.clear();
    blist=pointBezier3(ctrlList[9],ctrlList[10],ctrlList[11],ctrlList[12],num);
    for(loop=0;loop<blist.size();loop++)
    {
        list.append(blist[loop]);
    }
    blist.clear();
    return list;
}

//将一个椭圆转换为4段三次贝塞尔曲线
/*4段三次贝塞尔曲线是按逆时针方向从第一象限到第四象限连接，
第一个点和最后一个点重合于+X轴上点(rx, 0)。
\param[out] 贝塞尔曲线的控制点，13个点
\param[in] centerPoint 椭圆心
\param[in] rx 半长轴的长度
\param[in] ry 半短轴的长度
*/
QList<QPoint> Bezier::ellipseToBezier(QPoint centerPoint, float rx, float ry)
{
    const double M = 0.5522847498307933984022516f; // 4(sqrt(2)-1)/3
    double dx = rx * M;
    double dy = ry * M;
    QList<QPoint> list;
    QPoint point;

    point.setX(centerPoint.x() + rx);
    point.setY(centerPoint.y());
    list.append(point);

    point.setX(centerPoint.x() + rx);
    point.setY(centerPoint.y() + dy);
    list.append(point);

    point.setX(centerPoint.x() + dx);
    point.setY(centerPoint.y() + ry);
    list.append(point);

    point.setX(centerPoint.x());
    point.setY(centerPoint.y() + ry);
    list.append(point);

    point.setX(centerPoint.x() - dx);
    point.setY(centerPoint.y() + ry);
    list.append(point);

    point.setX(centerPoint.x() - rx);
    point.setY(centerPoint.y() + dy);
    list.append(point);

    point.setX(centerPoint.x() - rx);
    point.setY(centerPoint.y());
    list.append(point);

    point.setX(centerPoint.x() - rx);
    point.setY(centerPoint.y() - dy);
    list.append(point);

    point.setX(centerPoint.x() - dx);
    point.setY(centerPoint.y() - ry);
    list.append(point);

    point.setX(centerPoint.x());
    point.setY(centerPoint.y() - ry);
    list.append(point);

    point.setX(centerPoint.x() + dx);
    point.setY(centerPoint.y() - ry);
    list.append(point);

    point.setX(centerPoint.x() + rx);
    point.setY(centerPoint.y() - dy);
    list.append(point);

    point.setX(centerPoint.x() + rx);
    point.setY(centerPoint.y());
    list.append(point);

    return list;
}

//将一个圆角矩形转换为4段三次贝塞尔曲线
/*这4段贝塞尔曲线按逆时针方向从第一象限到第四象限，每段4个点，
第一段的点序号为[0,1,2,3]，其余段递增类推。
\param[out] 贝塞尔曲线的控制点，每段4个点，共16个点
\param[in] rect 矩形外框，规范化矩形
\param[in] rx X方向的圆角半径，非负数
\param[in] ry Y方向的圆角半径，非负数
*/
QList<QPoint> Bezier::roundRectToBezier(QRect rect, float rx, float ry)
{
    QList<QPoint> list;
    QPoint points[16];
    if (2 * rx > rect.width())
        rx = rect.width() / 2;
    if (2 * ry > rect.height())
        ry = rect.height() / 2;

    int i=0, j=0;
    float dx = rect.width() / 2 - rx;
    float dy = rect.height() / 2 - ry;

    list=ellipseToBezier(rect.center(), rx, ry);

    for (i = 3; i >= 1; i--)
    {
        for (j = 3; j >= 0; j--)
            points[4 * i + j] = points[3 * i + j];
    }
    for (i = 0; i < 4; i++)
    {
        float dx1 = (0 == i || 3 == i) ? dx : -dx;
        float dy1 = (0 == i || 1 == i) ? dy : -dy;
        for (j = 0; j < 4; j++)
        {
            points[0].setX(points[0].x()+dx1);
            points[0].setY(points[0].y()+dy1);
        }
    }
    for(int loop=0;loop<16;loop++)
    {
        list[loop]=points[loop];
    }
    return list;
}

//椭圆弧段用贝塞尔曲线去代替
void Bezier::arcToBezier(QPoint points[4],QPoint centerPoint, float rx, float ry, double start_angle, double sweep_angle)
{
    double x0 = cos(sweep_angle / 2.0);
    double y0 = sin(sweep_angle / 2.0);
    double tx = (1.0 - x0) * 4.0 / 3.0;
    double ty = y0 - tx * x0 / y0;
    double px[4];
    double py[4];
    double curve[8];

    px[0] =  x0;
    py[0] = -y0;
    px[1] =  x0 + tx;
    py[1] = -ty;
    px[2] =  x0 + tx;
    py[2] =  ty;
    px[3] =  x0;
    py[3] =  y0;

    double sn = sin(start_angle + sweep_angle / 2.0);
    double cs = cos(start_angle + sweep_angle / 2.0);

    for(int loop = 0; loop < 4; loop++)
    {
         curve[loop * 2]     = centerPoint.x() + rx * (px[loop] * cs - py[loop] * sn);
         curve[loop * 2 + 1] = centerPoint.y() + ry * (px[loop] * sn + py[loop] * cs);
         points[loop].setX(qRound(curve[loop*2]));
         points[loop].setY(qRound(curve[loop*2+1]));
    }
}

//将一个椭圆弧转换为多段三次贝塞尔曲线
void Bezier::angleArcToBezier(QPoint points[4], QPoint centerPoint, float rx, float ry, double startAngle, double sweepAngle)
{
    // Compute bezier curve for arc centered along y axis
    // Anticlockwise: (0,-B), (x,-y), (x,y), (0,B)
    double sy = ry / rx;
    ry = rx;
    double B = ry * sin(sweepAngle / 2.0);
    double C = rx * cos(sweepAngle / 2.0);
    double A = rx - C;
    double X = A * 4.0 / 3.0;
    double Y = B - X * (rx-A)/B;
    points[0].setX(qRound(C));
    points[0].setY(qRound(-B));
    points[1].setX(qRound(C+X));
    points[1].setY(qRound(-Y));
    points[2].setX(qRound(C+X));
    points[2].setY(qRound(Y));
    points[3].setX(qRound(C));
    points[3].setY(qRound(B));
    // rotate to the original angle
    A = startAngle + sweepAngle / 2.0;
    double s = sin(A);
    double c = cos(A);
    for (int i = 0; i < 4; i++)
    {
        points[i].setX(qRound(centerPoint.x() + points[i].x() * c - points[i].y() * s));
        points[i].setY(qRound(centerPoint.y() + points[i].x() * s * sy + points[i].y() * c * sy));
    }
}

int Bezier::angleArcToBezierPlusSweep(QPoint points[16], QPoint centerPoint, float rx, float ry, double startAngle, double sweepAngle)
{
    const double M = 0.5522847498307933984022516;
    double dx = rx * M;
    double dy = ry * M;
    int k, n;
    double endAngle;
    // 计算第一段椭圆弧的终止角度
    if (startAngle < MPI2)
    {
        // +Y
        endAngle = MPI2;
        k = 1;
    }
    else if (startAngle < MPI)
    {
        // -X
        endAngle = MPI;
        k = 2;
    }
    else if (startAngle < 3.0*MPI2)
    {
        // -Y
        endAngle = 3.0*MPI2;
        k = 3;
    }
    else
    {
        // +X
        endAngle = M2PI;
        k = 0;
    }
    if (endAngle - startAngle > 1e-7) // 转换第一段椭圆弧
    {
        arcToBezier(points, centerPoint, rx, ry,startAngle, endAngle - startAngle);
        n = 4;
    }
    else
        n = 1; // 第一点在下边循环内设置

    sweepAngle -= (endAngle - startAngle);
    startAngle = endAngle;

    while (sweepAngle >= MPI2) // 增加整90度弧
    {
        if (k == 0) // 第一象限
        {
            points[n-1].setX(qRound(centerPoint.x() + rx));
            points[n-1].setY(qRound(centerPoint.y()+0.0));
            points[n].setX(qRound(centerPoint.x() + rx));
            points[n].setY(qRound(centerPoint.y() + dy));
            points[n+1].setX(qRound(centerPoint.x() + dx));
            points[n+1].setY(qRound(centerPoint.y() + ry));
            points[n+2].setX(qRound(centerPoint.x()+0.0));
            points[n+2].setY(qRound(centerPoint.y() + ry));
        }
        else if (k == 1) // 第二象限
        {
            points[n-1].setX(qRound(centerPoint.x()+0.0));
            points[n-1].setY(qRound(centerPoint.y() + ry));
            points[n].setX(qRound(centerPoint.x() - dx));
            points[n].setY(qRound(centerPoint.y() + ry));
            points[n+1].setX(qRound(centerPoint.x() - rx));
            points[n+1].setY(qRound(centerPoint.y() + dy));
            points[n+2].setX(qRound(centerPoint.x() - rx));
            points[n+2].setY(qRound(centerPoint.y()+0.0));
        }
        else if (k == 2) // 第三象限
        {
            points[n-1].setX(qRound(centerPoint.x() - rx));
            points[n-1].setY(qRound(centerPoint.y()+0.0));
            points[n].setX(qRound(centerPoint.x() - rx));
            points[n].setY(qRound(centerPoint.y() - dy));
            points[n+1].setX(qRound(centerPoint.x() - dx));
            points[n+1].setY(qRound(centerPoint.y() - ry));
            points[n+2].setX(qRound(centerPoint.x()+0.0));
            points[n+2].setY(qRound(centerPoint.y() - ry));
        }
        else // 第四象限
        {
            points[n-1].setX(qRound(centerPoint.x()+0.0));
            points[n-1].setY(qRound(centerPoint.y() - ry));
            points[n].setX(qRound(centerPoint.x() + dx));
            points[n].setY(qRound(centerPoint.y() - ry));
            points[n+1].setX(qRound(centerPoint.x() + rx));
            points[n+1].setY(qRound(centerPoint.y() - dy));
            points[n+2].setX(qRound(centerPoint.x() + rx));
            points[n+2].setY(qRound(centerPoint.y()+0.0));
        }
        k = (k + 1) % 4;
        n += 3;
        sweepAngle -= MPI2;
        startAngle += MPI2;
    }

    if (sweepAngle > 1e-7) // 增加余下的弧
    {
        arcToBezier(&points[n-1], centerPoint, rx, ry, startAngle, sweepAngle);
        n += 3;
    }
    return n;
}

//将一个椭圆弧转换为多段三次贝塞尔曲线
/*4段三次贝塞尔曲线是按逆时针方向从第一象限到第四象限连接，每一段4个点，
第一个点和最后一个点重合于+X轴上点(rx, 0)。
\param[out] points 贝塞尔曲线的控制点，16个点
\param[in] center 椭圆心
\param[in] rx 半长轴的长度
\param[in] ry 半短轴的长度，为0则取为rx
\param[in] startAngle 起始角度，弧度，相对于+X轴，逆时针为正
\param[in] sweepAngle 转角，弧度，相对于起始角度，逆时针为正
\return 计算后的控制点数，点数小于4则给定参数有错误
*/
int Bezier::tAngleArcToBezier(QPoint points[16], QPoint centerPoint, float rx, float ry, double startAngle, double sweepAngle)
{
     int n = 0;
     QPoint tempPoint;

    if (fabs(rx)<ZERO||fabs(sweepAngle) < 1e-7)
        return 0;
    if (fabs(ry)<ZERO)
        ry = rx;
    if (sweepAngle > M2PI)
        sweepAngle = M2PI;
    else if (sweepAngle < -M2PI)
        sweepAngle = -M2PI;

    if (fabs(sweepAngle) < MPI2 + 1e-7)
    {
        arcToBezier(points, centerPoint, rx, ry, startAngle, sweepAngle);
        n = 4;
    }
    else if (sweepAngle > 0)
    {
        startAngle = toRange(startAngle, 0.0, M2PI);
        n = angleArcToBezierPlusSweep(points, centerPoint, rx, ry, startAngle, sweepAngle);
    }
    else // sweepAngle < 0
    {
        double endAngle = startAngle + sweepAngle;
        sweepAngle = -sweepAngle;
        startAngle = toRange(endAngle, 0.0, M2PI);
        n = angleArcToBezierPlusSweep(points, centerPoint, rx, ry, startAngle, sweepAngle);
        for (int i = 0; i < n / 2; i++)
        {
            tempPoint=points[i];
            points[i]=points[n - 1 - i];
            points[n - 1 - i]=tempPoint;
        }
    }
    return n;
}

//将数转换到数值范围[tmin, tmax)内
double Bezier::toRange(double value, double tmin, double tmax)
{
    while (value < tmin)
    {
        value += tmax - tmin;
    }
    while (value >= tmax)
    {
        value -= tmax - tmin;
    }
    return value;
}

