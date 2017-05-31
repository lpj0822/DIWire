#include "ellipticarc.h"

EllipticArc::EllipticArc()
{

}

EllipticArc::~EllipticArc()
{

}

void EllipticArc::setCenterPoint(QPoint point)
{
    this->centerPoint=point;
}

void EllipticArc::setStartAngle(double angle)
{
    this->startAngle=angle;
}

void EllipticArc::setDeltaAngle(double angle)
{
    this->deltaAngle=angle;
}

QPoint EllipticArc::getCenterPoint()
{
    return centerPoint;
}

double EllipticArc::getStartAngle()
{
    return startAngle;
}

double EllipticArc::getDeltaAngle()
{
    return deltaAngle;
}

double EllipticArc::radian(double ux, double uy, double vx, double vy)
{
    double dot = ux * vx + uy * vy;
    double mod = sqrt( ( ux * ux + uy * uy ) * ( vx * vx + vy * vy ) );
    double rad = acos( dot / mod );
    if( ux * vy - uy * vx < 0.0 )
    {
        rad = -rad;
    }
    return  rad;
}

//根据svg中路径A指令参数计算椭圆弧的圆心和起始角度、扫描角度
/***************************************
分别代表的含义是：
(x1 y1)圆弧路径起点
(x2 y2)圆弧路径终点
fA 标记是否大弧段
fS 标记是否顺时针绘制
rx 椭圆弧的X半轴长度
ry 椭圆弧的Y半轴长度
phi 椭圆弧X轴方向的旋转角度
****************************************/
void EllipticArc::Aconvert(double x1, double y1, double x2, double y2, int fA, int fS, double rx, double ry,double phi)
{
    double cx=0.0,cy=0.0;
    double theta1=0.0;
    double delta_theta=0.0;
    QPoint point;

    if( fabs(rx)<ZERO || fabs(ry)<ZERO)
    {
        return;  // invalid arguments
    }

    double  s_phi = sin( phi );
    double  c_phi = cos( phi );
    double  hd_x = ( x1 - x2 ) / 2.0;   // half diff of x
    double  hd_y = ( y1 - y2 ) / 2.0;   // half diff of y
    double  hs_x = ( x1 + x2 ) / 2.0;   // half sum of x
    double  hs_y = ( y1 + y2 ) / 2.0;   // half sum of y

    // F6.5.1
    double  x1_ = c_phi * hd_x + s_phi * hd_y;
    double  y1_ = c_phi * hd_y - s_phi * hd_x;

    double  rxry = rx * ry;
    double  rxy1_ = rx * y1_;
    double  ryx1_ = ry * x1_;
    double  sum_of_sq = rxy1_ * rxy1_ + ryx1_ * ryx1_;   // sum of square
    double  coe =sqrt( ( rxry * rxry - sum_of_sq ) / sum_of_sq );

    if( fA == fS )
    {
        coe = -coe;
    }

    // F6.5.2
    double  cx_ = coe * rxy1_ / ry;
    double  cy_ = -coe * ryx1_ / rx;

    // F6.5.3
    cx = c_phi * cx_ - s_phi * cy_ + hs_x;
    cy = s_phi * cx_ + c_phi * cy_ + hs_y;

    double  xcr1 = ( x1_ - cx_ ) / rx;
    double  xcr2 = ( x1_ + cx_ ) / rx;
    double  ycr1 = ( y1_ - cy_ ) / ry;
    double  ycr2 = ( y1_ + cy_ ) / ry;

    // F6.5.5
    theta1 = radian( 1.0, 0.0, xcr1, ycr1 );

    // F6.5.6
    delta_theta = radian( xcr1, ycr1, -xcr2, -ycr2 );

    double  PI2 = PI * 2.0;
    while( delta_theta > PI2 )
    {
        delta_theta -= PI2;
    }
    while( delta_theta < -PI2 )
    {
        delta_theta += PI2;
    }
    if( fS == 0&&delta_theta>0)
    {
        delta_theta -= PI2;
    }
    else if(fS==1&&delta_theta<0)
    {
        delta_theta += PI2;
    }

    //qDebug()<<"delta:"<<delta_theta;
    //qDebug()<<"theta1:"<<theta1;

    point.setX(cx);
    point.setY(cy);
    this->centerPoint=point;
    this->startAngle=theta1;
    this->deltaAngle=delta_theta;
}


