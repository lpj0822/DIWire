#ifndef DIALOG_H
#define DIALOG_H

#include <QtWidgets>
#include <QtXml>
#include <QDialog>
#include <QDir>
#include <QFile>
#include"bezier.h"
#include"pathdata.h"
#include"ellipticarc.h"
#include"dataDef.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void setupUI();//加载UI
    void setupPix();//加载图片
    void init();//初始化数据

    void readSvg(QString fileName);//读取svg文件
    void svgContent();//解析svg文件的内容
    void showSvg();//显示svg
    void getCurvePoint();//得到曲线坐标点
    void processCurvePoint();//处理得到的曲线坐标点
    void showPoints();//给曲线描点

    void clearImageData();//清除原来加载图片的数据

    void scaleSvg();//缩放图像

    //对list矩形区域进行缩放
    QList<QRect> scaleListRect(QList<QRect> list);
    //对list直线进行缩放
    QList<QPoint> scaleListLine(QList<QPoint> list);
    //对list折线进行缩放
    QList< QList<QPoint> > scaleListPoly(QList< QList<QPoint> > list);
    //对list路径进行缩放
    QList< QList<PathData> > scaleListPath(QList< QList<PathData> > list);

    //调整矩形坐标,使之适应显示区域
    QRect adjustRect(QRect rect);
    //调整直线坐标，使之适应显示区域
    void adjustLine(QPoint points[2]);
    //调整折线坐标，使之适应显示区域
    QList<QPoint> adjustPolyline(QList<QPoint> list);

protected:
    void changeEvent(QEvent *e);
    //void paintEvent(QPaintEvent *event);

signals:

public slots:
    void openSlot();
    void pointSlot();
    void setPointSlot();
    void SetSizeSlot();
    void changePointSlot();
    void changeSizeSlot();

private:
    Ui::Dialog *ui;

    QPushButton *openButton;
    QPushButton *pointButton;

    QLabel *labelPoint;
    QLabel *labelSize;
    QLineEdit *editPoint;
    QLineEdit *editSize;
    QSlider *sliderPoint;
    QSlider *sliderSize;

    QGroupBox *group1;
    QGroupBox *group2;
    QGroupBox *group3;

    QScrollArea *areaImage;
    QLabel *labelImage;

    QHBoxLayout *mainLayout;

    QPixmap pix;//生成图片
    QPen pen;//画笔

    int imageWidth;//图片宽度
    int imageHeight;//图片宽度

    QDomDocument doc;//svg内容
    bool haveRect;//有矩形
    bool haveCircle;//有圆形
    bool haveEllipse;//有椭圆
    bool haveLine;//有线条
    bool havePolyline;//有折线
    bool havePolygon;//有多变形
    bool havePath;//有路径

    //绘图数据
    QList<QRect> listRect;
    QList<QRect> listCircle;
    QList<QRect> listEllipse;
    QList<QPoint> listLine;
    QList< QList<QPoint> > listPolyline;
    QList< QList<QPoint> > listPolygon;
    QList< QList<PathData> > listPathData;
    QList<QPainterPath> listPath;

    //初始绘图数据
    QList<QRect> initListRect;
    QList<QRect> initListCircle;
    QList<QRect> initListEllipse;
    QList<QPoint> initListLine;
    QList< QList<QPoint> > initListPolyline;
    QList< QList<QPoint> > initListPolygon;
    QList< QList<PathData> > initListPathData;

    QList<float> listRectLength;//矩形周长
    QList<float> listCircleLenght;//圆周长
    QList<float> listEllipseLength;//椭圆周长
    QList<float> listLineLenght;//直线长度
    QList<float> listPolylineLength;//折线长度
    QList<float> listPolygonLength;//多变形周长
    QList<float> listPathLength;//路径长度

    //曲线坐标点
    QList< QList<QPoint> > listPointRect;
    QList< QList<QPoint> > listPointCircle;
    QList< QList<QPoint> > listPointEllipse;
    QList< QList<QPoint> > listPointLine;
    QList< QList<QPoint> > listPointPolyline;
    QList< QList<QPoint> > listPointPolygon;
    QList< QList<QPoint> > listPointPath;

    //描点坐标之间的长度
    QList<float> listPointLen;
    //三点之间的线段夹角
    QList<float> listPointAngle;

    int imageSize;//图像比例
    int interval;//点之间的间隔

    Bezier *bezier;//贝塞尔曲线
    int numPoint;//生成的贝塞尔曲线点数

    QString curDir;//包含文件名的绝对路径

    void parseSvgRect(int x,int y,int width,int height);//解析rect参数
    void parseSvgCircle(int cx,int cy,int r);//解析圆参数
    void parseSvgEllipse(int cx,int cy,int rx,int ry);//解析椭圆参数
    void parseSvgLine(int x1,int y1,int x2,int y2);//解析直线参数
    void parseSvgPolygon(QString points);//解析多边形参数
    void parseSvgPolyline(QString points);//解析折线参数
    void parseSvgPath(QString dStr);//解析path参数

    double distancePoint(QPoint p1,QPoint p2);//求两点之间的距离
    QPoint stringPoint(QString point);//将字符串类型的点转换成QPoint

    //去除list中重复元素
    QList< QList<QPoint> > distinctData(QList< QList<QPoint> > list);

    //对矩形缩放
    QRect scaleRect(QRect rect,float size);
    //缩放直线
    void scaleLine(QPoint points[2],float size);
    //缩放折线
    QList<QPoint> scalePolyline(QList<QPoint> list,float size);
    //缩放路径
    QList<PathData> scalePath(QList<PathData> list,QPainterPath &paintPath,float size);

    //计算点与点之间的夹角与距离
    void getLenAndAngle();

    //保存信息到Settings表中
    void LoadSettings();
    //保存信息到Settings表中
    void SaveSettings();
};

#endif // DIALOG_H
