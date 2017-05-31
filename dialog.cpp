#pragma execution_character_set("utf-8")
#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    init();
    setupUI();
    setupPix();
    LoadSettings();
    this->setMaximumSize(890,625);
    this->setMinimumSize(890,625);
    this->setWindowTitle(tr("DIWire"));
}

Dialog::~Dialog()
{
    if(bezier!=NULL)
        delete bezier;
    SaveSettings();
    delete ui;
}

void Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    case QEvent::Close:
        if(bezier!=NULL)
            delete bezier;
        SaveSettings();
        break;
    default:
        break;
    }
}

//加载UI
void Dialog::setupUI()
{
    group1=new QGroupBox(tr("打开文件"));
    QVBoxLayout *openLayout=new QVBoxLayout();
    openButton= new QPushButton(tr("打开文件"));
    pointButton= new QPushButton(tr("曲线描点"));
    openLayout->addWidget(openButton);
    openLayout->addWidget(pointButton);
    group1->setLayout(openLayout);

    group2=new QGroupBox(tr("图像缩放比例"));
    QVBoxLayout *sizeLayout=new QVBoxLayout();
    QHBoxLayout *layout1=new QHBoxLayout();
    labelSize=new QLabel(tr("缩放比例："));
    editSize=new QLineEdit();
    sliderSize=new QSlider(Qt::Horizontal);
    layout1->addWidget(labelSize);
    layout1->addWidget(editSize);
    sizeLayout->addLayout(layout1);
    sizeLayout->addWidget(sliderSize);
    group2->setLayout(sizeLayout);

    group3=new QGroupBox(tr("描点间隔大小"));
    QVBoxLayout *pointLayout=new QVBoxLayout();
    QHBoxLayout *layout2=new QHBoxLayout();
    labelPoint=new QLabel(tr("描点间隔："));
    editPoint=new QLineEdit();
    sliderPoint=new QSlider(Qt::Horizontal);
    layout2->addWidget(labelPoint);
    layout2->addWidget(editPoint);
    pointLayout->addLayout(layout2);
    pointLayout->addWidget(sliderPoint);
    group3->setLayout(pointLayout);

    QVBoxLayout *rightLayout=new QVBoxLayout();
    rightLayout->addWidget(group1);
    rightLayout->addWidget(group2);
    rightLayout->addWidget(group3);

    areaImage=new QScrollArea();
    //areaImage->setBackgroundRole(QPalette::Dark);
    labelImage = new QLabel();
    //labelImage->setScaledContents(true);
    labelImage->resize(imageWidth,imageHeight);
    labelImage->setAlignment(Qt::AlignCenter);
    labelImage->setPixmap(pix);
    areaImage->setWidget(labelImage);

    mainLayout=new QHBoxLayout();

    mainLayout->addWidget(areaImage);
    mainLayout->addLayout(rightLayout);
    mainLayout->setStretch(0,3);
    mainLayout->setStretch(1,1);

    this->setLayout(mainLayout);

    sliderSize->setTickInterval(10);//设置步进值
    sliderSize->setSingleStep(10);
    sliderSize->setMinimum(MINSIZE);//设置滑动条控件的最小值
    sliderSize->setMaximum(MAXSIZE);//设置滑动条控件的最大值
    sliderSize->setValue(100);//设置滑动条控件的值
    editSize->setText(tr("100"));

    sliderPoint->setTickInterval(10);//设置步进值
    sliderPoint->setSingleStep(10);
    sliderPoint->setMinimum(MINPOINT);//设置滑动条控件的最小值
    sliderPoint->setMaximum(MAXPOINT);//设置滑动条控件的最大值
    sliderPoint->setValue(10);//设置滑动条控件的值
    editPoint->setText(tr("10"));

    connect(openButton,SIGNAL(clicked()),this,SLOT(openSlot()));
    connect(pointButton,SIGNAL(clicked()),this,SLOT(pointSlot()));
    connect(sliderPoint,SIGNAL(valueChanged(int)),this,SLOT(setPointSlot()));
    connect(editPoint,SIGNAL(editingFinished()),this,SLOT(changePointSlot()));
    connect(sliderSize,SIGNAL(valueChanged(int)),this,SLOT(SetSizeSlot()));
    connect(editSize,SIGNAL(editingFinished()),this,SLOT(changeSizeSlot()));
}

//初始化
void Dialog::init()
{
    imageSize=100;//图像比例
    interval=10;//点之间的间隔

    imageWidth=WIDTH;//图片宽度
    imageHeight=HEIGHT;//图片宽度
    pix = QPixmap(imageWidth,imageHeight);

    haveRect=false;
    haveCircle=false;
    haveEllipse=false;
    haveLine=false;
    havePolyline=false;
    havePolygon=false;
    havePath=false;

    bezier=new Bezier();//贝塞尔曲线
    numPoint=10;//生成的贝塞尔曲线点数
}

//加载图片
void Dialog::setupPix()
{
    int loop1=0;
    pix.fill(Qt::white);

    QPainter paint(&pix);

    pen.setStyle(Qt::DotLine);
    pen.setWidth(1);
    pen.setColor(Qt::gray);
    paint.setPen(pen);

    for(loop1=0;loop1<imageWidth;loop1++)//绘制纵线
    {
        if(loop1%50==0)
        {
             paint.drawLine(loop1,0,loop1,imageHeight);
        }
     }
     for(loop1=0;loop1<imageHeight;loop1++)//绘制横线
     {
         if(loop1%50==0)
         {
             paint.drawLine(0,loop1,imageWidth,loop1);
         }
    }

    labelImage->resize(imageWidth,imageHeight);
    labelImage->setPixmap(pix);
}

//清除原来加载图片的数据
void Dialog::clearImageData()
{
    haveRect=false;
    haveCircle=false;
    haveEllipse=false;
    haveLine=false;
    havePolyline=false;
    havePolygon=false;
    havePath=false;

    listRect.clear();
    listCircle.clear();
    listEllipse.clear();
    listLine.clear();
    listPolyline.clear();
    listPolygon.clear();
    listPathData.clear();
    listPath.clear();

    initListRect.clear();
    initListCircle.clear();
    initListEllipse.clear();
    initListLine.clear();
    initListPolyline.clear();
    initListPolygon.clear();
    initListPathData.clear();

    listRectLength.clear();//矩形周长
    listCircleLenght.clear();//圆周长
    listEllipseLength.clear();//椭圆周长
    listLineLenght.clear();//直线长度
    listPolylineLength.clear();//折线长度
    listPolygonLength.clear();//多变形周长
    listPathLength.clear();//路径长度

    listPointRect.clear();
    listPointCircle.clear();
    listPointEllipse.clear();
    listPointLine.clear();
    listPointPolyline.clear();
    listPointPolygon.clear();
    listPointPath.clear();

    listPointLen.clear();
    listPointAngle.clear();

    //sliderPoint->setValue(10);
    sliderSize->setValue(100);
}

//打开文件
void Dialog::openSlot()
{
    QString name=QFileDialog::getOpenFileName(this,tr("open file dialog"),curDir,"svg files(*.svg)");
    if(name.trimmed().isEmpty())
        return;
    //返回包含文件名的绝对路径
    //curDir = QFileInfo(name).absoluteFilePath();
    //返回文件的绝对路径，不包含文件名
    curDir=QFileInfo(name).absolutePath();
    QFile file(name);
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this,tr("DOM SVG"),tr("Cannot read file %1:\n%2.").arg(name).arg(file.errorString()));
        return;
    }
    readSvg(name);
}

//曲线描点
void Dialog::pointSlot()
{
    setupPix();
    showSvg();
    getCurvePoint();
    processCurvePoint();
    showPoints();
    //getLenAndAngle();
    //qDebug()<<"listPointLen:"<<listPointLen;
    //qDebug()<<"listPointAngle:"<<listPointAngle;
}

//改变图像比列
void Dialog::SetSizeSlot()
{
    imageSize=sliderSize->value();
    QString str = QString("%1").arg(imageSize);
    editSize->setText(str);
    imageWidth=WIDTH*imageSize*1.0/100.0;
    imageHeight=HEIGHT*imageSize*1.0/100.0;
    pix=QPixmap(imageWidth,imageHeight);
    setupPix();
    scaleSvg();
    showSvg();
}

void Dialog::changeSizeSlot()
{
  bool ok;
  QString str=editSize->text();
  int dec=str.toInt(&ok,10);
  if(ok)
  {
      if(dec<MINSIZE||dec>MAXSIZE)
      {
          QMessageBox::warning(this,tr("图像比例"),tr("输入的图像比例应该在10～200之间（单位：%）"));
          imageSize=sliderSize->value();
          QString str1 = QString("%1").arg(imageSize);
          editSize->setText(str1);
          return;
      }
      else
      {
        sliderSize->setValue(dec);
      }
  }
  else
  {
      QMessageBox::warning(this,tr("图像比例"),tr("输入的图像比例参数不是整数!\n请输入整数!（单位：%）"));
      imageSize=sliderSize->value();
      QString str1 = QString("%1").arg(imageSize);
      editSize->setText(str1);
  }
}

//改变点间隔大小
void Dialog::setPointSlot()
{
    interval=sliderPoint->value();
    QString str=QString("%1").arg(interval);
    editPoint->setText(str);
    numPoint=interval;
    setupPix();
    showSvg();
    getCurvePoint();
    processCurvePoint();
    showPoints();
}

void Dialog::changePointSlot()
{
    bool ok;
    QString str=editPoint->text();
    int dec=str.toInt(&ok,10);
    if(ok)
    {
        if(dec<MINPOINT||dec>MAXPOINT)
        {
            QMessageBox::warning(this,tr("描点间隔大小"),tr("输入的点间隔大小应该在10～500之间"));
            interval=sliderPoint->value();
            QString str=QString("%1").arg(interval);
            editPoint->setText(str);
            return;
        }
        else
        {
            interval=dec;
            numPoint=interval;
            setupPix();
            showSvg();
            getCurvePoint();
            processCurvePoint();
            showPoints();
            sliderPoint->setValue(dec);
        }
    }
    else
    {
        QMessageBox::warning(this,tr("描点间隔大小"),tr("输入的点间隔大小不是整数!\n请输入整数!"));
        interval=sliderPoint->value();
        QString str=QString("%1").arg(interval);
        editPoint->setText(str);
    }
}

//读取svg文件
void Dialog::readSvg(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return;
    if(!doc.setContent(&file))
    {
        QMessageBox::warning(this,tr("Doc"),tr("doc error!"));
        file.close();
        return;
    }
    file.close();
    svgContent();
}

//解析svg文件的内容
void Dialog::svgContent()
{
    QDomElement root=doc.documentElement();
//    imageWidth=root.attribute(tr("width")).toDouble();
//    imageHeight=root.attribute(tr("height")).toDouble();
//    if(imageWidth>600||imageHeight>480)
//    {
//        if(imageWidth>600&&imageHeight<=480)
//        {
//            imageHeight=480;
//        }
//        if(imageWidth<=600&&imageHeight>480)
//        {
//            imageWidth=600;
//        }
//        pix=QPixmap(imageWidth,imageHeight);
//        setupPix();
//    }
//    else
//    {
//        imageWidth=600;
//        imageHeight=480;
//    }

    setupPix();
    clearImageData();

    QDomElement child=root.firstChildElement();
    while(!child.isNull())
    {
        if(child.tagName()==tr("rect"))
        {
            int x=child.attribute(tr("x")).toDouble();
            int y=child.attribute(tr("y")).toDouble();
            int width=child.attribute(tr("width")).toDouble();
            int height=child.attribute(tr("height")).toDouble();
            parseSvgRect(x,y,width,height);
            haveRect=true;
        }
        else if(child.tagName()==tr("circle"))
        {
            int cx=child.attribute(tr("cx")).toDouble();
            int cy=child.attribute(tr("cy")).toDouble();
            int r=child.attribute(tr("r")).toDouble();
            parseSvgCircle(cx,cy,r);
            haveCircle=true;
        }
        else if(child.tagName()==tr("ellipse"))
        {
            int cx=child.attribute(tr("cx")).toDouble();
            int cy=child.attribute(tr("cy")).toDouble();
            int rx=child.attribute(tr("rx")).toDouble();
            int ry=child.attribute(tr("ry")).toDouble();
            parseSvgEllipse(cx,cy,rx,ry);
            haveEllipse=true;
        }
        else if(child.tagName()==tr("line"))
        {
            int x1=child.attribute(tr("x1")).toDouble();
            int y1=child.attribute(tr("y1")).toDouble();
            int x2=child.attribute(tr("x2")).toDouble();
            int y2=child.attribute(tr("y2")).toDouble();
            parseSvgLine(x1,y1,x2,y2);
            haveLine=true;
        }
        else if(child.tagName()==tr("polygon"))
        {
            QString points=child.attribute(tr("points"));
            parseSvgPolygon(points.trimmed());
            havePolygon=true;
        }
        else if(child.tagName()==tr("polyline"))
        {
            QString points=child.attribute(tr("points"));
            parseSvgPolyline(points.trimmed());
            havePolyline=true;
        }
        else if(child.tagName()==tr("path"))
        {
            QString path=child.attribute(tr("d"));
            qDebug()<<"d1:"<<path.trimmed();
            parseSvgPath(path.trimmed());
            havePath=true;
        }
        else if(child.tagName()==tr("g"))
        {
            QDomElement child1=child.firstChildElement();
            while(!child1.isNull())
            {
                if(child1.tagName()==tr("path"))
                {
                    QString path=child1.attribute(tr("d"));
                    qDebug()<<"d2:"<<path.trimmed();
                    parseSvgPath(path.trimmed());
                    havePath=true;
                }
                child1=child1.nextSiblingElement();
            }
        }
        child=child.nextSiblingElement();
    }
    initListRect.append(listRect);
    initListCircle.append(listCircle);
    initListEllipse.append(listEllipse);
    initListLine.append(listLine);
    initListPolyline.append(listPolyline);
    initListPolygon.append(listPolygon);
    initListPathData.append(listPathData);
    showSvg();
}

void Dialog::showSvg()
{
    QPainter paint(&pix);

    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(Qt::black);
    //打开抗锯齿性能，使图像看的更加流畅
    paint.setRenderHint(QPainter::Antialiasing);
    paint.setPen(pen);

    if(haveRect)
    {
        QListIterator<QRect> iterator1(listRect);
        for(;iterator1.hasNext();)
        {
            paint.drawRect(iterator1.next());
        }
    }
    if(haveCircle)
    {
        QListIterator<QRect> iterator2(listCircle);
        for(;iterator2.hasNext();)
        {
            paint.drawEllipse(iterator2.next());
        }
    }
    if(haveEllipse)
    {
        QListIterator<QRect> iterator3(listEllipse);
        for(;iterator3.hasNext();)
        {
            paint.drawEllipse(iterator3.next());
        }
    }
    if(haveLine)
    {
        QListIterator<QPoint> iterator4(listLine);
        for(;iterator4.hasNext();)
        {
            QPoint point1=iterator4.next();
            QPoint point2=iterator4.next();
            paint.drawLine(point1.x(),point1.y(),point2.x(),point2.y());
        }
    }
    if(havePolyline)
    {
        QListIterator< QList<QPoint> > iterator5(listPolyline);
        QPoint pointArray[1000];
        int loop=0;
        for(;iterator5.hasNext();)
        {
            QList<QPoint> list1=iterator5.next();
            for(loop=0;loop<list1.count();loop++)
            {
                pointArray[loop]=list1[loop];
            }
            paint.drawPolyline(pointArray,loop);
        }
    }
    if(havePolygon)
    {
        QListIterator< QList<QPoint> > iterator6(listPolygon);
        QPoint pointArray[1000];
        int loop=0;
        for(;iterator6.hasNext();)
        {
            QList<QPoint> list2=iterator6.next();
            for(loop=0;loop<list2.count();loop++)
            {
                pointArray[loop]=list2[loop];
            }
            paint.drawPolygon(pointArray,loop);
        }
    }
    if(havePath)
    {
        QListIterator<QPainterPath> iterator7(listPath);
        for(;iterator7.hasNext();)
        {
            paint.drawPath(iterator7.next());
        }
    }
    labelImage->setPixmap(pix);
}

//得到曲线坐标点
void Dialog::getCurvePoint()
{
    //矩形
    if(haveRect)
    {
        listPointRect.clear();
        QListIterator<QRect> iterator1(listRect);
        QListIterator<float> iterLen1(listRectLength);
        for(;iterator1.hasNext()&&iterLen1.hasNext();)
        {
            QList<QPoint> list;
            //QList<QPoint> tempList;
            QRect rect=iterator1.next();
            float len=iterLen1.next();
            int x=rect.x();
            int y=rect.y();
            int width=rect.width();
            int height=rect.height();
            //int numW=0;
            //int numH=0;
            QPoint point1(x,y);
            QPoint point2(x+width,y);
            QPoint point3(x+width,y+height);
            QPoint point4(x,y+height);

//            if(numPoint%2!=0)
//            {
//                numW=width*1.0/len*((numPoint+1)/2.0);
//                numH=height*1.0/len*((numPoint+1)/2.0);
//            }
//            else
//            {
//                numW=width*1.0/len*(numPoint/2.0);
//                numH=height*1.0/len*(numPoint/2.0);
//            }

//            tempList=bezier->pointBezier1(point1,point2,numW);
//            list.append(tempList);
//            tempList.clear();
//            tempList=bezier->pointBezier1(point2,point3,numH);
//            list.append(tempList);
//            tempList.clear();
//            tempList=bezier->pointBezier1(point3,point4,numW);
//            list.append(tempList);
//            tempList.clear();
//            tempList=bezier->pointBezier1(point4,point1,numH);
//            list.append(tempList);
//            tempList.clear();
            list.append(point1);
            list.append(point2);
            list.append(point3);
            list.append(point4);
            listPointRect.append(list);
        }
    }
    //圆形
    if(haveCircle)
    {
        listPointCircle.clear();
        QListIterator<QRect> iterator2(listCircle);
        for(;iterator2.hasNext();)
        {
            QList<QPoint> list;
            QRect rect=iterator2.next();
            int rx=rect.width()/2.0;
            int ry=rect.height()/2.0;
            QPoint centerPoint(rect.x()+rx,rect.y()+ry);
            //qDebug()<<"circle:"<<centerPoint<<" "<<rx<<" "<<ry;
            list=bezier->pointEllipseToBezier(centerPoint,rx,ry,numPoint);
            listPointCircle.append(list);
        }
    }
    //椭圆形
    if(haveEllipse)
    {
        listPointEllipse.clear();
        QListIterator<QRect> iterator3(listEllipse);
        for(;iterator3.hasNext();)
        {
            QList<QPoint> list;
            QRect rect=iterator3.next();
            int rx=rect.width()/2.0;
            int ry=rect.height()/2.0;
            QPoint centerPoint(rect.x()+rx,rect.y()+ry);
            qDebug()<<"ellipse:"<<centerPoint<<" "<<rx<<" "<<ry;
            list=bezier->pointEllipseToBezier(centerPoint,rx,ry,numPoint);
            listPointEllipse.append(list);
        }
    }
    //直线
    if(haveLine)
    {
        listPointLine.clear();
        QListIterator<QPoint> iterator4(listLine);
        for(;iterator4.hasNext();)
        {
            QList<QPoint> list;
            QPoint point1=iterator4.next();
            QPoint point2=iterator4.next();
            //list=bezier->pointBezier1(point1,point2,numPoint);
            list.append(point1);
            list.append(point2);
            listPointLine.append(list);
        }
    }
    //折线
    if(havePolyline)
    {
        listPointPolyline.clear();
        QListIterator< QList<QPoint> > iterator5(listPolyline);
        QListIterator<float> iter5(listPolylineLength);
        int loop=0;
        int num=0;
        QList<QPoint> list;
        QList<QPoint> tempList;
        for(;iterator5.hasNext()&&iter5.hasNext();)
        {
            QList<QPoint> list1=iterator5.next();
            float len=iter5.next();
            for(loop=0;loop<list1.count()-1;loop++)
            {
                num=distancePoint(list1[loop],list1[loop+1])/len*numPoint;
                tempList=bezier->pointBezier1(list1[loop],list1[loop+1],num);
                list.append(tempList);
                tempList.clear();
            }
            listPointPolyline.append(list);
        }
    }
    //多边形
    if(havePolygon)
    {
        listPointPolygon.clear();
        QListIterator< QList<QPoint> > iterator6(listPolygon);
        QListIterator<float> iter6(listPolygonLength);
        int loop=0;
        int num=0;
        QList<QPoint> list;
        QList<QPoint> tempList;
        for(;iterator6.hasNext()&&iter6.hasNext();)
        {
            QList<QPoint> list2=iterator6.next();
            float len=iter6.next();
            for(loop=0;loop<list2.count()-1;loop++)
            {
                num=distancePoint(list2[loop],list2[loop+1])/len*numPoint;
                tempList=bezier->pointBezier1(list2[loop],list2[loop+1],num);
                list.append(tempList);
                tempList.clear();
            }
            num=distancePoint(list2[loop],list2[0])/len*numPoint;
            tempList=bezier->pointBezier1(list2[loop],list2[0],num);
            list.append(tempList);
            tempList.clear();
            listPointPolygon.append(list);
        }
    }
    //路径
    if(havePath)
    {
        listPointPath.clear();
        QListIterator< QList<PathData> > iterator7(listPathData);
        QListIterator<float> iter7(listPathLength);
        for(;iterator7.hasNext()&&iter7.hasNext();)
        {
            QList<PathData> list3=iterator7.next();
            float len=iter7.next();
            QListIterator<PathData> iter(list3);
            QPainterPath paintPath;
            QPoint prePoint;
            int num=0;
            QList<QPoint> list;
            QList<QPoint> tempList;
            float preLength=0.0;
            for(;iter.hasNext();)
            {
                PathData pData=iter.next();
                if(pData.getTypeData().contains("M")||pData.getTypeData().contains("m"))
                {
                    //移动
                    prePoint=pData.getStartPoint();
                    list.clear();
                    tempList.clear();
                    preLength=0.0;
                    paintPath.moveTo(prePoint);
                    qDebug()<<"pointM:"<<prePoint;
                }
                else if(pData.getTypeData().contains("L")||pData.getTypeData().contains("l"))
                {
                    //画线
                    QPoint p;
                    float length=0.0;
                    p=pData.getEndPoint();
                    paintPath.lineTo(p);
                    length=paintPath.length()-preLength;
                    num=length/len*numPoint;
                    tempList=bezier->pointBezier1(prePoint,p,num);
                    list.append(tempList);
                    tempList.clear();
                    prePoint=p;
                    preLength+=length;
                }
                else if(pData.getTypeData().contains("H")||pData.getTypeData().contains("h"))
                {
                    //水平线
                    float length=0.0;
                    QPoint p;
                    p=pData.getEndPoint();
                    paintPath.lineTo(p);
                    length=paintPath.length()-preLength;
                    num=length/len*numPoint;
                    tempList=bezier->pointBezier1(prePoint,p,num);
                    list.append(tempList);
                    tempList.clear();
                    prePoint=p;
                    preLength+=length;
                }
                else if(pData.getTypeData().contains("V")||pData.getTypeData().contains("v"))
                {
                    //垂直线
                    float length=0.0;
                    QPoint p;
                    p=pData.getEndPoint();
                    paintPath.lineTo(p);
                    length=paintPath.length()-preLength;
                    num=length/len*numPoint;
                    tempList=bezier->pointBezier1(prePoint,p,num);
                    list.append(tempList);
                    tempList.clear();
                    prePoint=p;
                    preLength+=length;
                }
                else if(pData.getTypeData().contains("C")||pData.getTypeData().contains("c"))
                {
                    //三次贝塞尔曲线
                    float length=0.0;

                    QPoint p1;
                    QPoint p2;
                    QPoint p3;
                    p1=pData.getCtrl1Point();
                    p2=pData.getCtrl2Point();
                    p3=pData.getEndPoint();

                    paintPath.cubicTo(p1,p2,p3);
                    length=paintPath.length()-preLength;
                    num=length/len*numPoint;
                    //qDebug()<<"C p1"<<p1<<p2<<p3<<length;

                    tempList=bezier->pointBezier3(prePoint,p1,p2,p3,num);
                    list.append(tempList);
                    tempList.clear();
                    prePoint=p3;
                    preLength+=length;
                }
                else if(pData.getTypeData().contains("S")||pData.getTypeData().contains("s"))
                {
                    //光滑三次贝塞尔曲线
                    float length=0.0;
                    QPoint p1;
                    QPoint p2;
                    QPoint p3;
                    p1=pData.getCtrl1Point();
                    p2=pData.getCtrl2Point();
                    p3=pData.getEndPoint();

                    paintPath.cubicTo(p1,p2,p3);
                    length=paintPath.length()-preLength;
                    num=length/len*numPoint;

                    tempList=bezier->pointBezier3(prePoint,p1,p2,p3,num);
                    list.append(tempList);
                    tempList.clear();
                    prePoint=p3;
                    preLength+=length;
                }
                else if(pData.getTypeData().contains("Q")||pData.getTypeData().contains("q"))
                {
                    //二次贝塞尔曲线
                    float length=0.0;
                    QPoint p1;
                    QPoint p2;
                    p1=pData.getCtrl1Point();
                    p2=pData.getEndPoint();
                    //paintPath.cubicTo(p1,p1,p2);
                    paintPath.quadTo(p1,p2);
                    length=paintPath.length()-preLength;
                    num=length/len*numPoint;

                    tempList=bezier->pointBezier2(prePoint,p1,p2,num);
                    list.append(tempList);
                    tempList.clear();
                    prePoint=p2;
                    preLength+=length;
                }
                else if(pData.getTypeData().contains("T")||pData.getTypeData().contains("t"))
                {
                    //光滑二次贝塞尔曲线
                    float length=0.0;
                    QPoint p1;
                    QPoint p2;
                    p1=pData.getCtrl1Point();
                    p2=pData.getEndPoint();
                    //paintPath.cubicTo(cPoint,cPoint,p);
                    paintPath.quadTo(p1,p2);
                    length=paintPath.length()-preLength;
                    num=length/len*numPoint;

                    tempList=bezier->pointBezier2(prePoint,p1,p2,num);
                    list.append(tempList);
                    tempList.clear();
                    prePoint=p2;
                    preLength+=length;
                }
                else if(pData.getTypeData().contains("A")||pData.getTypeData().contains("a"))
                {
                    //椭圆弧
                    float length=0.0;
                    float rx=pData.getRx();//弧的半长轴长度
                    float ry=pData.getRy();//弧的半短轴长度
                    //此段弧的中心坐标点
                    QPoint centerPoint=pData.getCenterPoint();
                    //此段弧的起始角度
                    double startAngle=pData.getStartAngle();
                    //此段弧的扫描角度
                    double deltaAngle=pData.getDeltaAngle();
                    //弧终端坐标
                    QPoint p=pData.getEndPoint();

                    qDebug()<<"arc:"<<centerPoint<<" "<<startAngle<<" "<<deltaAngle;
                    paintPath.arcTo(centerPoint.x()-rx,centerPoint.y()-ry,rx*2,ry*2,-startAngle*RAD,-deltaAngle*RAD);
                    length=paintPath.length()-preLength;
                    num=length/len*numPoint;

                    tempList=bezier->pointArcToBezier(centerPoint,rx,ry,startAngle,deltaAngle,num);
                    list.append(tempList);
                    tempList.clear();
                    prePoint=p;
                    preLength+=length;
                }
                else if(pData.getTypeData().contains("Z")||pData.getTypeData().contains("z"))
                {
                    //关闭路径
                    float length=0.0;
                    QPoint p=pData.getEndPoint();
                    paintPath.lineTo(p);
                    length=paintPath.length()-preLength;
                    num=length/len*numPoint;

                    tempList=bezier->pointBezier1(prePoint,p,num);
                    list.append(tempList);
                    tempList.clear();
                    preLength+=length;
                }
                else
                {
                    QMessageBox::warning(this,tr("svg显示"),tr("path路径有误！"));
                    break;
                }
            }
            listPointPath.append(list);
        }
    }
}

//处理得到的曲线坐标点
void Dialog::processCurvePoint()
{
    if(haveRect)
    {
        listPointRect=distinctData(listPointRect);
    }
    if(haveCircle)
    {
        listPointCircle=distinctData(listPointCircle);
    }
    if(haveEllipse)
    {
        listPointEllipse=distinctData(listPointEllipse);
    }
    if(haveLine)
    {
        listPointLine=distinctData(listPointLine);
    }
    if(havePolyline)
    {
        listPointPolyline=distinctData(listPointPolyline);
    }
    if(havePolygon)
    {
        listPointPolygon=distinctData(listPointPolygon);
    }
    if(havePath)
    {
        listPointPath=distinctData(listPointPath);
    }
}

//给曲线描点
void Dialog::showPoints()
{
    QPainter paint(&pix);

    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);
    pen.setColor(Qt::red);
    //打开抗锯齿性能，使图像看的更加流畅
    paint.setRenderHint(QPainter::Antialiasing);
    paint.setPen(pen);

    if(haveRect)
    {
        QListIterator< QList<QPoint> > iterator1(listPointRect);
        //qDebug()<<"listPointRect:"<<listPointRect;
        for(;iterator1.hasNext();)
        {
            QListIterator<QPoint> iter(iterator1.next());
            for(;iter.hasNext();)
            {
                paint.drawPoint(iter.next());
            }
        }
    }
    if(haveCircle)
    {
        QListIterator< QList<QPoint> > iterator2(listPointCircle);
        //qDebug()<<"listPointCircle:"<<listPointCircle;
        for(;iterator2.hasNext();)
        {
            QListIterator<QPoint> iter(iterator2.next());
            for(;iter.hasNext();)
            {
                paint.drawPoint(iter.next());
            }
        }
    }
    if(haveEllipse)
    {
        QListIterator< QList<QPoint> > iterator3(listPointEllipse);
        //qDebug()<<"listPointEllipse:"<<listPointEllipse;
        for(;iterator3.hasNext();)
        {
            QListIterator<QPoint> iter(iterator3.next());
            for(;iter.hasNext();)
            {
                paint.drawPoint(iter.next());
            }
        }
    }
    if(haveLine)
    {
        QListIterator< QList<QPoint> > iterator4(listPointLine);
        //qDebug()<<"listPointLine:"<<listPointLine;
        for(;iterator4.hasNext();)
        {
            QListIterator<QPoint> iter(iterator4.next());
            for(;iter.hasNext();)
            {
                paint.drawPoint(iter.next());
            }
        }
    }
    if(havePolyline)
    {
        QListIterator< QList<QPoint> > iterator5(listPointPolyline);
        //qDebug()<<"listPointPolyline:"<<listPointPolyline;
        for(;iterator5.hasNext();)
        {
            QListIterator<QPoint> iter(iterator5.next());
            for(;iter.hasNext();)
            {
                paint.drawPoint(iter.next());
            }
        }
    }
    if(havePolygon)
    {
        QListIterator< QList<QPoint> > iterator6(listPointPolygon);
        //qDebug()<<"listPointPolygon:"<<listPointPolygon;
        for(;iterator6.hasNext();)
        {
            QListIterator<QPoint> iter(iterator6.next());
            for(;iter.hasNext();)
            {
                paint.drawPoint(iter.next());
            }
        }
    }
    if(havePath)
    {
        QListIterator< QList<QPoint> > iterator7(listPointPath);
        //qDebug()<<"listPointPath:"<<listPointPath;
        for(;iterator7.hasNext();)
        {
            QListIterator<QPoint> iter(iterator7.next());
            for(;iter.hasNext();)
            {
                paint.drawPoint(iter.next());
            }
        }
    }

    labelImage->setPixmap(pix);
}

//缩放图像
void Dialog::scaleSvg()
{
    if(haveRect)
    {
        listRect=scaleListRect(initListRect);
    }
    if(haveCircle)
    {
        listCircle=scaleListRect(initListCircle);
    }
    if(haveEllipse)
    {
        listEllipse=scaleListRect(initListEllipse);
    }
    if(haveLine)
    {
        listLine=scaleListLine(initListLine);
    }
    if(havePolyline)
    {
        listPolyline=scaleListPoly(initListPolyline);
    }
    if(havePolygon)
    {
        listPolygon=scaleListPoly(initListPolygon);
    }
    if(havePath)
    {
        listPathData=scaleListPath(initListPathData);
    }
}

//解析rect参数
void Dialog::parseSvgRect(int x,int y,int width,int height)
{
    QRect rect;
    rect.setRect(x,y,width,height);
    //rect=adjustRect(rect);//归一化坐标
    listRectLength.append(rect.width()+rect.height());
    listRect.append(rect);
}

//解析圆参数
void Dialog::parseSvgCircle(int cx,int cy,int r)
{
    QRect rect;
    rect.setRect(cx-r,cy-r,2*r,2*r);
    //rect=adjustRect(rect);//归一化坐标
    listCircleLenght.append(PI*rect.width());
    listCircle.append(rect);
}

//解析椭圆参数
void Dialog::parseSvgEllipse(int cx,int cy,int rx,int ry)
{
    QRect rect;
    rect.setRect(cx-rx,cy-ry,2*rx,2*ry);
    //rect=adjustRect(rect);//归一化坐标
    if(rect.width()>=rect.height())
    {
        listEllipseLength.append(PI*rect.height()+2*(rect.width()-rect.height()));
    }
    else
    {
        listEllipseLength.append(PI*rect.width()+2*(rect.height()-rect.height()));
    }
    listEllipse.append(rect);
}

//解析直线参数
void Dialog::parseSvgLine(int x1,int y1,int x2,int y2)
{
    QPoint points[2];
    points[0].setX(x1);
    points[0].setY(y1);
    points[1].setX(x2);
    points[1].setY(y2);
    //adjustLine(points);//归一化坐标
    float len=distancePoint(points[0],points[1]);
    listLineLenght.append(len);
    listLine.append(points[0]);
    listLine.append(points[1]);
}

//解析多边形参数
void Dialog::parseSvgPolygon(QString points)
{
    bool isRight=true;
    QRegExp rx(tr("[ \t\r\n,]+"));
    QStringList list=points.split(rx);
    qDebug()<<"Polygon:"<<list;
    QList<QPoint> poly1;
    float pointX=0.0;
    float pointY=0.0;
    QPoint tempPoint;
    float len=0.0;
    int loop=0;

    QListIterator<QString> iter(list);
    for(;iter.hasNext();)
    {
       QString strX=iter.next().trimmed();
       if(!strX.isEmpty())
       {
            QString strY=iter.next().trimmed();
            if(!strY.isEmpty())
            {
                pointX=strX.toDouble(&isRight);
                pointY=strY.toDouble(&isRight);
                if(!isRight)
                {
                    break;
                }
                tempPoint.setX(pointX);
                tempPoint.setY(pointY);
                poly1.append(tempPoint);
            }
       }
    }
    if(isRight)
    {
        listPolygon.append(poly1);

        for(loop=0;loop<poly1.count()-1;loop++)
        {
            len+=distancePoint(poly1[loop],poly1[loop+1]);
        }
        len+=distancePoint(poly1[loop],poly1[0]);
        listPolygonLength.append(len);
    }
}

//解析折线参数
void Dialog::parseSvgPolyline(QString points)
{
    bool isRight=true;
    QRegExp rx(tr("[ \t\r\n,]+"));
    QStringList list=points.split(rx);
    qDebug()<<"Polyline:"<<list;
    QList<QPoint> poly2;
    QPoint tempPoint;
    float pointX;
    float pointY;
    float len=0.0;
    int loop=0;

    QListIterator<QString> iter(list);
    for(;iter.hasNext();)
    {
       QString strX=iter.next().trimmed();
       if(!strX.isEmpty())
       {
            QString strY=iter.next().trimmed();
            if(!strY.isEmpty())
            {
                pointX=strX.toDouble(&isRight);
                pointY=strY.toDouble(&isRight);
                if(!isRight)
                {
                    break;
                }
                tempPoint.setX(pointX);
                tempPoint.setY(pointY);
                poly2.append(tempPoint);
            }
       }
    }
    if(isRight)
    {
        listPolyline.append(poly2);

        for(loop=0;loop<poly2.count()-1;loop++)
        {
            len+=distancePoint(poly2[loop],poly2[loop+1]);
        }
        listPolylineLength.append(len);
    }
}

//解析path参数
void Dialog::parseSvgPath(QString dStr)
{
    QStringList tempList;
    QStringList list;
    QRegExp rx1(QString("[ \t\r\n,]+"));
    QRegExp rx2(QString("[MLHVCSQTAZmlhvcsqtaz]"));
//  QRegExp rx3(QString("(([-+]?[0-9]+(\\.[0-9]*)?|[-+]?\\.[0-9]+)([eE][-+]?[0-9]+)?)"));
//    rx1.setMinimal(true);//最小匹配模式
//    rx2.setMinimal(true);
//    rx3.setMinimal(true);
//    int offset=0;
      int p=0;
//    int len=dStr.length();
//    while(1)
//    {
//      p=rx1.indexIn(dStr,offset);
//      if(p>0)
//      {
//          offset=p+rx1.matchedLength();
//      }
//      if(offset>=len)
//          break;
//      p=dStr.indexOf(rx2,offset);
//      if(p>=0)
//      {
//        list.append(dStr.mid(p,rx2.matchedLength()));
//        offset=p+rx2.matchedLength();
//        continue;
//      }
//      p=dStr.indexOf(rx3,offset);
//      if(p>=0)
//      {
//          list.append(dStr.mid(p,rx3.matchedLength()));
//          offset=p+rx3.matchedLength();
//          continue;
//      }
//      else
//      {
//        QMessageBox::warning(this,tr("svg显示"),tr("path路径有误！"));
//        return;
//      }
//    }

    tempList=dStr.split(rx1);
    QListIterator<QString> tempIter(tempList);
    QString temp;
    for(;tempIter.hasNext();)
    {
        temp=tempIter.next();
        p=rx2.indexIn(temp,0);
        if(p>=0)
        {
            if(p!=0)
            {
                list.append(temp.mid(0,p));
                list.append(temp.mid(p,rx2.matchedLength()));
                if(!((temp.mid(p+rx2.matchedLength())).trimmed().isEmpty()))
                {
                    list.append(temp.mid(p+rx2.matchedLength()));
                }
            }
            else
            {
                list.append(temp.mid(0,rx2.matchedLength()));
                if(!((temp.mid(p+rx2.matchedLength())).trimmed().isEmpty()))
                {
                    list.append(temp.mid(p+rx2.matchedLength()));
                }
            }
        }
        else
        {
            if(!temp.isEmpty())
                list.append(temp);
        }
    }
    qDebug()<<"path:"<<list;
    if(!list.at(0).contains("M")&&!list.at(0).contains("m"))
    {
        QMessageBox::warning(this,tr("svg file error"),tr("svg文件格式有误！"));
        return;
    }

    float firstPointX=list.at(1).toDouble();
    float firstPointY=list.at(2).toDouble();

    QListIterator<QString> iter(list);
    QPainterPath paintPath;
    QPoint firstPoint(firstPointX,firstPointY);
    QPoint prePoint;
    QPoint cPoint;
    QList<PathData> pList;
    QString preCommand=list.at(0).trimmed();
    //处理path字符串
    for(;iter.hasNext();)
    {
        QString strP=iter.peekNext().trimmed();//返回下一个列表项，但不移动迭代点
        p=rx2.indexIn(strP,0);
        if(p>=0)
        {
            preCommand=iter.next().trimmed();
        }
        if(preCommand.contains("M")||preCommand.contains("m"))
        {
            //移动到
            PathData pData;
            float pointX=iter.next().toDouble();
            float pointY=iter.next().toDouble();
            prePoint.setX(pointX);
            prePoint.setY(pointY);

            pData.setTypeData("M");
            pData.setStartPoint(prePoint);
            pList.append(pData);

            paintPath.moveTo(prePoint.x(),prePoint.y());
            //qDebug()<<"pointM:"<<prePoint;
        }
        else if(preCommand.contains("L")||preCommand.contains("l"))
        {
            //画线到
            PathData pData;
            QPoint p;
            float pointX=iter.next().toDouble();
            float pointY=iter.next().toDouble();
            if(preCommand.contains("l"))
            {
                pointX=pointY+prePoint.x();
                pointY=pointY+prePoint.y();
            }
            p.setX(pointX);
            p.setY(pointY);
            pData.setTypeData("L");
            pData.setStartPoint(prePoint);
            pData.setEndPoint(p);
            pList.append(pData);

            paintPath.lineTo(p.x(),p.y());
            prePoint=p;
         }
         else if(preCommand.contains("H")||preCommand.contains("h"))
         {
             //水平线到
             PathData pData;
             float pointX=iter.next().toDouble();
             QPoint p;
             if(preCommand.contains("h"))
             {
                 pointX=pointX+prePoint.x();
             }
             p.setX(pointX);
             p.setY(prePoint.y());
             pData.setTypeData("H");
             pData.setStartPoint(prePoint);
             pData.setEndPoint(p);
             pList.append(pData);

             paintPath.lineTo(p.x(),p.y());
             prePoint=p;
         }
         else if(preCommand.contains("V")||preCommand.contains("v"))
         {
             //垂直线到
             PathData pData;
             float pointY=iter.next().toDouble();
             QPoint p;
             if(preCommand.contains("v"))
             {
                 pointY=pointY+prePoint.y();
             }
             p.setX(prePoint.x());
             p.setY(pointY);
             pData.setTypeData("V");
             pData.setStartPoint(prePoint);
             pData.setEndPoint(p);
             pList.append(pData);

             paintPath.lineTo(p.x(),p.y());
             prePoint=p;
         }
         else if(preCommand.contains("C")||preCommand.contains("c"))
         {
             //三次贝塞尔曲线到
             PathData pData;
             float point1X=iter.next().toDouble();
             float point1Y=iter.next().toDouble();
             float point2X=iter.next().toDouble();
             float point2Y=iter.next().toDouble();
             float point3X=iter.next().toDouble();
             float point3Y=iter.next().toDouble();
             QPoint p1;
             QPoint p2;
             QPoint p3;
             if(preCommand.contains("c"))
             {
                 point1X+=prePoint.x();
                 point1Y+=prePoint.y();
                 point2X+=prePoint.x();
                 point2Y+=prePoint.y();
                 point3X+=prePoint.x();
                 point3Y+=prePoint.y();
             }
             p1.setX(point1X);
             p1.setY(point1Y);
             p2.setX(point2X);
             p2.setY(point2Y);
             p3.setX(point3X);
             p3.setY(point3Y);
             //qDebug()<<"C p1"<<p1<<p2<<p3;

             pData.setTypeData("C");
             pData.setStartPoint(prePoint);
             pData.setCtrl1Point(p1);
             pData.setCtrl2Point(p2);
             pData.setEndPoint(p3);
             pList.append(pData);

             paintPath.cubicTo(p1.x(),p1.y(),p2.x(),p2.y(),p3.x(),p3.y());
             cPoint=p2;
             prePoint=p3;
         }
         else if(preCommand.contains("S")||preCommand.contains("s"))
         {
             //光滑三次贝塞尔曲线到
             PathData pData;
             int rx=prePoint.x()-cPoint.x();
                int ry=prePoint.y()-cPoint.y();
                QPoint p1(cPoint.x()+2*rx,cPoint.y()+2*ry);
                float point2X=iter.next().toDouble();
                float point2Y=iter.next().toDouble();
                float point3X=iter.next().toDouble();
                float point3Y=iter.next().toDouble();
                QPoint p2;
                QPoint p3;
                if(preCommand.contains("s"))
                {
                    point2X+=prePoint.x();
                    point2Y+=prePoint.y();
                    point3X+=prePoint.x();
                    point3Y+=prePoint.y();
                }
                p2.setX(point2X);
                p2.setY(point2Y);
                p3.setX(point3X);
                p3.setY(point3Y);
                pData.setTypeData("S");
                pData.setStartPoint(prePoint);
                pData.setCtrl1Point(p1);
                pData.setCtrl2Point(p2);
                pData.setEndPoint(p3);
                pList.append(pData);

                paintPath.cubicTo(p1.x(),p1.y(),p2.x(),p2.y(),p3.x(),p3.y());
                cPoint=p2;
                prePoint=p3;
            }
            else if(preCommand.contains("Q")||preCommand.contains("q"))
            {
                //二次贝塞尔曲线到
                PathData pData;
                float point1X=iter.next().toDouble();
                float point1Y=iter.next().toDouble();
                float point2X=iter.next().toDouble();
                float point2Y=iter.next().toDouble();
                QPoint p1;
                QPoint p2;
                if(preCommand.contains("q"))
                {
                    point1X+=prePoint.x();
                    point1Y+=prePoint.y();
                    point2X+=prePoint.x();
                    point2Y+=prePoint.y();
                }
                p1.setX(point1X);
                p1.setY(point1Y);
                p2.setX(point2X);
                p2.setY(point2Y);
                //paintPath.cubicTo(cPoint,cPoint,prePoint);

                pData.setTypeData("Q");
                pData.setStartPoint(prePoint);
                pData.setCtrl1Point(p1);
                pData.setEndPoint(p2);
                pList.append(pData);

                paintPath.quadTo(p1,p2);
                cPoint=p1;
                prePoint=p2;
            }
            else if(preCommand.contains("T")||preCommand.contains("t"))
            {
                //光滑二次贝塞尔曲线到
                PathData pData;
                int rx=prePoint.x()-cPoint.x();
                int ry=prePoint.y()-cPoint.y();
                int x=cPoint.x()+2*rx;
                int y=cPoint.y()+2*ry;
                QPoint p1(x,y);
                float point2X=iter.next().toDouble();
                float point2Y=iter.next().toDouble();
                QPoint p2;
                if(preCommand.contains("t"))
                {
                    point2X+=prePoint.x();
                    point2Y+=prePoint.y();
                }
                p2.setX(point2X);
                p2.setY(point2Y);
                //paintPath.cubicTo(cPoint,cPoint,prePoint);

                pData.setTypeData("T");
                pData.setStartPoint(prePoint);
                pData.setCtrl1Point(p1);
                pData.setEndPoint(p2);
                pList.append(pData);

                paintPath.quadTo(p1,p2);
                cPoint=p1;
                prePoint=p2;
            }
            else if(preCommand.contains("A")||preCommand.contains("a"))
            {
                 //椭圆弧
                 PathData pData;
                 EllipticArc arc;
                 float rx=iter.next().toDouble();//弧的半长轴长度
                 float ry=iter.next().toDouble();//弧的半短轴长度
                 //此段弧所在的x轴与水平方向的夹角，即x轴的逆时针旋转角度，负数代表顺时针转动的角度
                 double phi=iter.next().toDouble();
                 //为1 表示大角度弧线，0 代表小角度弧线
                 int fA=iter.next().toInt();
                 //为1代表从起点到终点弧线绕中心顺时针方向，0 代表逆时针方向
                 int fS=iter.next().toInt();
                 //弧终端坐标
                 float pointX=iter.next().toDouble();
                 float pointY=iter.next().toDouble();
                 QPoint p;
                 if(preCommand.contains("a"))
                 {
                     pointX+=prePoint.x();
                     pointY+=prePoint.y();
                 }
                 p.setX(pointX);
                 p.setY(pointY);
                 //qDebug()<<"A:"<<rx<<" "<<ry<<" "<<phi<<" "<<fA<<" "<<fS<<" "<<p;

                 //计算椭圆弧的圆心和起始角度、扫描角度
                 arc.Aconvert(prePoint.x(),prePoint.y(),p.x(),p.y(),fA,fS,rx,ry,phi);
                 qDebug()<<"A arc:"<<arc.getCenterPoint()<<" "<<arc.getStartAngle()<<" "<<arc.getDeltaAngle();

                 pData.setTypeData("A");
                 pData.setStartPoint(prePoint);
                 pData.setEndPoint(p);
                 pData.setRx(rx);
                 pData.setRy(ry);
                 pData.setCenterPoint(arc.getCenterPoint());
                 pData.setStartAngle(arc.getStartAngle());
                 pData.setDeltaAngle(arc.getDeltaAngle());
                 pList.append(pData);

                 paintPath.arcTo(arc.getCenterPoint().x()-rx,arc.getCenterPoint().y()-ry,rx*2,ry*2,-arc.getStartAngle()*RAD,-arc.getDeltaAngle()*RAD);
                 //qDebug()<<"RAD:"<<arc.getStartAngle()*RAD<<" "<<arc.getDeltaAngle()*RAD;
                 prePoint=p;
            }
            else if(preCommand.contains("Z")||preCommand.contains("z"))
            {
                //关闭路径
                PathData pData;
                pData.setTypeData("Z");
                pData.setStartPoint(prePoint);
                pData.setEndPoint(firstPoint);
                pList.append(pData);

                paintPath.lineTo(firstPoint.x(),firstPoint.y());
            }
            else
            {
                qDebug()<<"strP:"<<strP;
                QMessageBox::warning(this,tr("svg显示"),tr("path路径有误！"));
                return;
            }
        }
        //qDebug()<<"path:"<<paintPath.length();
        listPathLength.append(paintPath.length());

        listPath.append(paintPath);
        listPathData.append(pList);
}

//对list矩形区域进行缩放
QList<QRect> Dialog::scaleListRect(QList<QRect> list)
{
    QList<QRect> tempList;
    QListIterator<QRect> iterator(list);
    float size=imageSize*1.0/100.0;
    if(haveRect)
    {
        listRectLength.clear();
        for(;iterator.hasNext();)
        {
            QRect rect;
            rect=scaleRect(iterator.next(),size);
            listRectLength.append(rect.width()+rect.height());
            tempList.append(rect);
        }
    }
    else
    {
        for(;iterator.hasNext();)
        {
            QRect rect;
            rect=scaleRect(iterator.next(),size);
            tempList.append(rect);
        }
    }
    return tempList;
}

//对list直线进行缩放
QList<QPoint> Dialog::scaleListLine(QList<QPoint> list)
{
    QList<QPoint> tempList;
    QListIterator<QPoint> iterator(list);
    float size=imageSize*1.0/100.0;
    for(;iterator.hasNext();)
    {
        QPoint points[2];
        points[0]=iterator.next();
        points[1]=iterator.next();
        scaleLine(points,size);
        tempList.append(points[0]);
        tempList.append(points[1]);
    }
    return tempList;
}

//对list折线进行缩放
QList< QList<QPoint> > Dialog::scaleListPoly(QList<QList<QPoint> > list)
{
    QList< QList<QPoint> > tempList;
    QListIterator< QList<QPoint> > iterator(list);
    float size=imageSize*1.0/100.0;
    int loop=0;
    if(havePolyline)
    {
        listPolylineLength.clear();
    }
    else if(havePolygon)
    {
        listPolygonLength.clear();
    }
    for(;iterator.hasNext();)
    {
        QList<QPoint> dataList;
        dataList=scalePolyline(iterator.next(),size);
        tempList.append(dataList);
        if(havePolyline)
        {
            float len=0.0;
            for(loop=0;loop<dataList.count()-1;loop++)
            {
                len+=distancePoint(dataList[loop],dataList[loop+1]);
            }
            listPolylineLength.append(len);
        }
        else if(havePolygon)
        {
            float len=0.0;
            for(loop=0;loop<dataList.count()-1;loop++)
            {
                len+=distancePoint(dataList[loop],dataList[loop+1]);
            }
            len+=distancePoint(dataList[loop],dataList[0]);
            listPolygonLength.append(len);
        }
    }
    return tempList;
}

//对list路径进行缩放
QList< QList<PathData> > Dialog::scaleListPath(QList<QList<PathData> > list)
{
    QList< QList<PathData> > tempList;
    QListIterator< QList<PathData> > iterator(list);
    float size=imageSize*1.0/100.0;
    listPathLength.clear();
    listPath.clear();
    for(;iterator.hasNext();)
    {
        QPainterPath paintPath;
        QList<PathData> dataList;
        dataList=scalePath(iterator.next(),paintPath,size);
        //qDebug()<<"path:"<<paintPath.length();
        listPathLength.append(paintPath.length());

        listPath.append(paintPath);
        tempList.append(dataList);
    }
    return tempList;
}

//对矩形缩放
QRect Dialog::scaleRect(QRect rect, float size)
{
    QRect r;
    int width=rect.width();
    int height=rect.height();
    width=1.0*width*size;
    height=1.0*height*size;
    r.setRect(rect.x(),rect.y(),width,height);
    return r;
}

//缩放直线
void Dialog::scaleLine(QPoint points[2], float size)
{
    float x=0.0;
    float y=0.0;
    x=(points[1].x()-points[0].x())*1.0*size+points[0].x();
    y=(points[1].y()-points[0].y())*1.0*size+points[0].y();
    points[1].setX(x);
    points[1].setY(y);
}

//缩放折线
QList<QPoint> Dialog::scalePolyline(QList<QPoint> list, float size)
{
    QList<QPoint> tempList;
    QPoint point;
    float x=0.0;
    float y=0.0;
    int loop=0;
    tempList.append(list[0]);
    point=list[0];
    for(loop=1;loop<list.count();loop++)
    {
        x=(list[loop].x()-list[loop-1].x())*1.0*size +point.x();
        y=(list[loop].y()-list[loop-1].y())*1.0*size+point.y();
        point.setX(x);
        point.setY(y);
        tempList.append(point);
    }
    return tempList;
}

//缩放路径
QList<PathData> Dialog::scalePath(QList<PathData> list,QPainterPath &paintPath,float size)
{
    QList<PathData> tempList;
    QListIterator<PathData> iterator(list);
    QPoint prePoint;
    float x=0.0;
    float y=0.0;
    for(;iterator.hasNext();)
    {
        PathData pData=iterator.next();
        if(pData.getTypeData().contains("M")||pData.getTypeData().contains("m"))
        {
            //移动
            PathData tempP;
            prePoint=pData.getStartPoint();

            tempP.setTypeData("M");
            tempP.setStartPoint(prePoint);
            tempList.append(tempP);

            paintPath.moveTo(prePoint);
        }
        else if(pData.getTypeData().contains("L")||pData.getTypeData().contains("l"))
        {
            //画线
            PathData tempP;
            QPoint p;
            x=(pData.getEndPoint().x()-pData.getStartPoint().x())*1.0*size +prePoint.x();
            y=(pData.getEndPoint().y()-pData.getStartPoint().y())*1.0*size+prePoint.y();
            p.setX(x);
            p.setY(y);

            tempP.setTypeData("L");
            tempP.setStartPoint(prePoint);
            tempP.setEndPoint(p);
            tempList.append(tempP);

            paintPath.lineTo(p);
            prePoint=p;
        }
        else if(pData.getTypeData().contains("H")||pData.getTypeData().contains("h"))
        {
            //水平线
            PathData tempP;
            QPoint p;
            x=(pData.getEndPoint().x()-pData.getStartPoint().x())*1.0*size +prePoint.x();
            y=(pData.getEndPoint().y()-pData.getStartPoint().y())*1.0*size+prePoint.y();
            p.setX(x);
            p.setY(y);

            tempP.setTypeData("H");
            tempP.setStartPoint(prePoint);
            tempP.setEndPoint(p);
            tempList.append(tempP);

            paintPath.lineTo(p);
            prePoint=p;
        }
        else if(pData.getTypeData().contains("V")||pData.getTypeData().contains("v"))
        {
            //垂直线
            PathData tempP;
            QPoint p;
            x=(pData.getEndPoint().x()-pData.getStartPoint().x())*1.0*size +prePoint.x();
            y=(pData.getEndPoint().y()-pData.getStartPoint().y())*1.0*size+prePoint.y();
            p.setX(x);
            p.setY(y);

            tempP.setTypeData("V");
            tempP.setStartPoint(prePoint);
            tempP.setEndPoint(p);
            tempList.append(tempP);

            paintPath.lineTo(p);
            prePoint=p;
        }
        else if(pData.getTypeData().contains("C")||pData.getTypeData().contains("c"))
        {
            //三次贝塞尔曲线
            PathData tempP;
            QPoint p1;
            QPoint p2;
            QPoint p3;
            x=(pData.getCtrl1Point().x()-pData.getStartPoint().x())*1.0*size +prePoint.x();
            y=(pData.getCtrl1Point().y()-pData.getStartPoint().y())*1.0*size+prePoint.y();
            p1.setX(x);
            p1.setY(y);
            x=(pData.getCtrl2Point().x()-pData.getCtrl1Point().x())*1.0*size +p1.x();
            y=(pData.getCtrl2Point().y()-pData.getCtrl1Point().y())*1.0*size+p1.y();
            p2.setX(x);
            p2.setY(y);
            x=(pData.getEndPoint().x()-pData.getCtrl2Point().x())*1.0*size +p2.x();
            y=(pData.getEndPoint().y()-pData.getCtrl2Point().y())*1.0*size+p2.y();
            p3.setX(x);
            p3.setY(y);

            tempP.setTypeData("C");
            tempP.setStartPoint(prePoint);
            tempP.setCtrl1Point(p1);
            tempP.setCtrl2Point(p2);
            tempP.setEndPoint(p3);
            tempList.append(tempP);

            paintPath.cubicTo(p1,p2,p3);
            prePoint=p3;
        }
        else if(pData.getTypeData().contains("S")||pData.getTypeData().contains("s"))
        {
            //光滑三次贝塞尔曲线
            PathData tempP;
            QPoint p1;
            QPoint p2;
            QPoint p3;
            x=(pData.getCtrl1Point().x()-pData.getStartPoint().x())*1.0*size +prePoint.x();
            y=(pData.getCtrl1Point().y()-pData.getStartPoint().y())*1.0*size+prePoint.y();
            p1.setX(x);
            p1.setY(y);
            x=(pData.getCtrl2Point().x()-pData.getCtrl1Point().x())*1.0*size +p1.x();
            y=(pData.getCtrl2Point().y()-pData.getCtrl1Point().y())*1.0*size+p1.y();
            p2.setX(x);
            p2.setY(y);
            x=(pData.getEndPoint().x()-pData.getCtrl2Point().x())*1.0*size +p2.x();
            y=(pData.getEndPoint().y()-pData.getCtrl2Point().y())*1.0*size+p2.y();
            p3.setX(x);
            p3.setY(y);

            tempP.setTypeData("S");
            tempP.setStartPoint(prePoint);
            tempP.setCtrl1Point(p1);
            tempP.setCtrl2Point(p2);
            tempP.setEndPoint(p3);
            tempList.append(tempP);

            paintPath.cubicTo(p1,p2,p3);
            prePoint=p3;
        }
        else if(pData.getTypeData().contains("Q")||pData.getTypeData().contains("q"))
        {
            //二次贝塞尔曲线
            PathData tempP;
            QPoint p1;
            QPoint p2;
            x=(pData.getCtrl1Point().x()-pData.getStartPoint().x())*1.0*size +prePoint.x();
            y=(pData.getCtrl1Point().y()-pData.getStartPoint().y())*1.0*size+prePoint.y();
            p1.setX(x);
            p1.setY(y);
            x=(pData.getCtrl1Point().x()-pData.getCtrl1Point().x())*1.0*size +p1.x();
            y=(pData.getCtrl1Point().y()-pData.getCtrl1Point().y())*1.0*size+p1.y();
            p2.setX(x);
            p2.setY(y);

            tempP.setTypeData("Q");
            tempP.setStartPoint(prePoint);
            tempP.setCtrl1Point(p1);
            tempP.setEndPoint(p2);
            tempList.append(tempP);

            //paintPath.cubicTo(p1,p1,p2);
            paintPath.quadTo(p1,p2);
            prePoint=p2;
        }
        else if(pData.getTypeData().contains("T")||pData.getTypeData().contains("t"))
        {
            //光滑二次贝塞尔曲线
            PathData tempP;
            QPoint p1;
            QPoint p2;
            x=(pData.getCtrl1Point().x()-pData.getStartPoint().x())*1.0*size +prePoint.x();
            y=(pData.getCtrl1Point().y()-pData.getStartPoint().y())*1.0*size+prePoint.y();
            p1.setX(x);
            p1.setY(y);
            x=(pData.getCtrl1Point().x()-pData.getCtrl1Point().x())*1.0*size +p1.x();
            y=(pData.getCtrl1Point().y()-pData.getCtrl1Point().y())*1.0*size+p1.y();
            p2.setX(x);
            p2.setY(y);

            tempP.setTypeData("Q");
            tempP.setStartPoint(prePoint);
            tempP.setCtrl1Point(p1);
            tempP.setEndPoint(p2);
            tempList.append(tempP);

            //paintPath.cubicTo(p1,p1,p2);
            paintPath.quadTo(p1,p2);
            prePoint=p2;
        }
        else if(pData.getTypeData().contains("A")||pData.getTypeData().contains("a"))
        {
            //椭圆弧
            PathData tempP;
            QPoint cPoint;
            QPoint ePoint;

            float rx=pData.getRx();//弧的半长轴长度
            float ry=pData.getRy();//弧的半短轴长度
            //起点坐标
            QPoint p=pData.getStartPoint();
            //此段弧的中心坐标点
            QPoint centerPoint=pData.getCenterPoint();
            //此段弧的起始角度
            double startAngle=pData.getStartAngle();
            //此段弧的扫描角度
            double deltaAngle=pData.getDeltaAngle();
            //弧终端坐标
            QPoint p1=pData.getEndPoint();

            x=(centerPoint.x()-p.x())*1.0*size+prePoint.x();
            y=(centerPoint.y()-p.y())*1.0*size+prePoint.y();
            cPoint.setX(x);
            cPoint.setY(y);
            x=(p1.x()-centerPoint.x())*1.0*size+cPoint.x();
            y=(p1.y()-centerPoint.y())*1.0*size+cPoint.y();
            ePoint.setX(x);
            ePoint.setY(y);
            rx=rx*size;
            ry=ry*size;

            tempP.setTypeData("A");
            tempP.setStartPoint(prePoint);
            tempP.setEndPoint(ePoint);
            tempP.setRx(rx);
            tempP.setRy(ry);
            tempP.setCenterPoint(cPoint);
            tempP.setStartAngle(startAngle);
            tempP.setDeltaAngle(deltaAngle);
            tempList.append(tempP);

            //qDebug()<<"arc:"<<cPoint<<" "<<startAngle<<" "<<deltaAngle;
            paintPath.arcTo(cPoint.x()-rx,cPoint.y()-ry,rx*2,ry*2,-startAngle*RAD,-deltaAngle*RAD);
            prePoint=ePoint;
        }
        else if(pData.getTypeData().contains("Z")||pData.getTypeData().contains("z"))
        {
            //关闭路径
            PathData tempP;
            QPoint p=pData.getEndPoint();

            tempP.setTypeData("Z");
            tempP.setStartPoint(prePoint);
            tempP.setEndPoint(p);
            tempList.append(pData);

            paintPath.lineTo(p);
        }
        else
        {
            QMessageBox::warning(this,tr("svg显示"),tr("path路径有误！"));
            break;
        }
    }
    return tempList;
}

//调整矩形坐标,使之适应显示区域
QRect Dialog::adjustRect(QRect rect)
{
    QRect r;
    int x=rect.x();
    int y=rect.y();
    int width=rect.width();
    int height=rect.height();
    int size1=0;
    int size2=0;
    float scale=0.0;
    if(x<0||x>imageWidth)
    {
        x=1;
    }
    if(y<0||y>imageHeight)
    {
        y=1;
    }
    if(x+width>imageWidth)
    {
        size1=x+width-imageWidth;

    }
    if(y+height>imageHeight)
    {
        size2=y+height-imageHeight;
    }
    if(size1>size2)
    {
        scale=1.0-(1.0*size1/(1.0*(x+width)));
        width=width*scale;
        height=height*scale;
    }
    else
    {
        scale=1.0-(1.0*size2/(1.0*(y+height)));
        width=width*scale;
        height=height*scale;
    }
    r.setRect(x,y,width,height);
    return r;
}

//调整直线坐标，使之适应显示区域
void Dialog::adjustLine(QPoint points[2])
{
    int size=0;
    float x=0.0;
    float y=0.0;
    if((points[0].x()>=0&&points[0].y()>=0&&points[0].x()<=imageWidth&&points[0].y()<=imageHeight)&&(points[1].x()>=0&&points[1].y()>=0&&points[1].x()<=imageWidth&&points[1].y()<=imageHeight))
    {
        return;
    }
    else if((points[0].x()>0&&points[0].y()>0&&points[0].x()<imageWidth&&points[0].y()<imageHeight)||(points[1].x()>0&&points[1].y()>0&&points[1].x()<imageWidth&&points[1].y()<imageHeight))
    {
        if(points[0].x()<=0)
        {
            size=0-points[0].x();
            y=size*1.0/(points[1].x()-points[0].x())*(points[1].y()-points[0].y())+points[0].y();
            points[0].setX(0);
            points[0].setX(y);
        }
        if(points[0].y()<=0)
        {
            size=0-points[0].y();
            x=size*1.0/(points[1].y()-points[0].y())*(points[1].x()-points[0].x())+points[0].x();
            points[0].setX(x);
            points[0].setY(0);
        }
        if(points[0].x()>=imageWidth)
        {
            size=imageWidth-points[0].x();
            y=size*1.0/(points[1].x()-points[0].x())*(points[1].y()-points[0].y())+points[0].y();
            points[0].setX(imageWidth);
            points[0].setX(y);
        }
        if(points[0].y()>=imageHeight)
        {
            size=imageHeight-points[0].y();
            x=size*1.0/(points[1].y()-points[0].y())*(points[1].x()-points[0].x())+points[0].x();
            points[0].setX(x);
            points[0].setY(imageHeight);
        }
        if(points[1].x()<=0)
        {
            size=0-points[0].x();
            y=size*1.0/(points[1].x()-points[0].x())*(points[1].y()-points[0].y())+points[0].y();
            points[1].setX(0);
            points[1].setX(y);
        }
        if(points[1].y()<=0)
        {
            size=0-points[0].y();
            x=size*1.0/(points[1].y()-points[0].y())*(points[1].x()-points[0].x())+points[0].x();
            points[1].setX(x);
            points[1].setY(0);
        }
        if(points[1].x()>=imageWidth)
        {
            size=imageWidth-points[0].x();
            y=size*1.0/(points[1].x()-points[0].x())*(points[1].y()-points[0].y())+points[0].y();
            points[1].setX(imageWidth);
            points[1].setX(y);
        }
        if(points[1].y()>=imageHeight)
        {
            size=imageHeight-points[0].y();
            x=size*1.0/(points[1].y()-points[0].y())*(points[1].x()-points[0].x())+points[0].x();
            points[1].setX(x);
            points[1].setY(imageHeight);
        }
    }
    else
    {
        QMessageBox::warning(this,tr("直线参数"),tr("直线的端点坐标至少有一个坐标在（0，0）～（%1，%2）").arg(imageWidth).arg(imageHeight));
        return;
    }
}

//调整折线坐标，使之适应显示区域
QList<QPoint> Dialog::adjustPolyline(QList<QPoint> list)
{
    QList<QPoint> tempList;
    QPoint point;
    int loop=0;
    int maxSizex=0;
    int maxSizey=0;
    int size=0;
    for(loop=0;loop<list.count();loop++)
    {
        if(list[loop].x()<0)
        {
            size=0-list[loop].x();
            if(size>maxSizex)
                maxSizex=size;
        }
        if(list[loop].y()<0)
        {
            size=0-list[loop].x();
            if(size>maxSizey)
                maxSizey=size;
        }
    }
    if(maxSizex!=0||maxSizey!=0)
    {
        for(loop=0;loop<list.count();loop++)
        {
            point.setX(list[loop].x()+maxSizex);
            point.setY(list[loop].y()+maxSizey);
            tempList.append(point);
        }
        return tempList;
    }
    else
    {
        return list;
    }
}

//求两点之间的距离
double Dialog::distancePoint(QPoint p1, QPoint p2)
{
    double distance=0.0;
    distance=sqrt((p1.x()-p2.x())*(p1.x()-p2.x())+(p1.y()-p2.y())*(p1.y()-p2.y()));
    return distance;
}

//将字符串类型的点转换成QPoint
QPoint Dialog::stringPoint(QString point)
{
    int index=0;
    int x=0;
    int y=0;
    QPoint  returnPoint;
    index=point.indexOf(",");
    x=point.left(index).toDouble();
    y=point.mid(index+1).toDouble();
    returnPoint.setX(x);
    returnPoint.setY(y);
    return returnPoint;
}

//去除list中重复元素
QList< QList<QPoint> > Dialog::distinctData(QList<  QList<QPoint> > list)
{
    QList< QList<QPoint> > tempList;
    QListIterator< QList<QPoint> > iterator(list);
    for(;iterator.hasNext();)
    {
        int loop=0;
        int loop1=0;
        QList<QPoint> dataList=iterator.next();
        QList<QPoint> tempDataList;
        //qDebug()<<"data:"<<dataList;
        while(loop<dataList.count())
        {
            tempDataList.append(dataList[loop]);
            for(loop1=loop+1;loop1<dataList.count();loop1++)
            {
                if(dataList[loop]!=dataList[loop1])
                {
                    break;
                }
            }
            if(loop==loop1-1)
            {
                loop++;
            }
            else
            {
                loop=loop1;
            }
        }
        //qDebug()<<"temp:"<<tempDataList;
        tempList.append(tempDataList);
    }
    return tempList;
}

//计算点与点之间的夹角与距离
void Dialog::getLenAndAngle()
{
    QPoint point1;
    QPoint point2;
    QPoint point3;
    float len=0.0;
    float angle=0.0;

    double d1=0.0;
    double d2=0.0;

    if(haveRect)
    {
        QListIterator< QList<QPoint> > iterator1(listPointRect);
        qDebug()<<"listPointRect:"<<listPointRect;
        for(;iterator1.hasNext();)
        {
            QListIterator<QPoint> iter(iterator1.next());
            if(iter.hasNext())
            {
                point1=iter.next();
                if(!iter.hasNext())
                {
                    listPointLen.append(0);
                    listPointAngle.append(-1);
                }
                else
                {
                    point2=iter.next();
                    for(;iter.hasNext();)
                    {
                        point3=iter.next();
                        len=distancePoint(point1,point2);
                        d1=(point1.x()-point2.x())*(point3.x()-point2.x())+(point1.y()-point2.y())*(point3.y()-point2.y());
                        d2=distancePoint(point1,point2)*distancePoint(point3,point2);
                        angle=acos(d1/d2);
                        listPointLen.append(len);
                        listPointAngle.append(angle);
                        point1=point2;
                        point2=point3;
                    }
                    len=distancePoint(point1,point2);
                    listPointLen.append(len);
                    listPointAngle.append(-1);
                }
            }
        }
    }
    if(haveCircle)
    {
        QListIterator< QList<QPoint> > iterator2(listPointCircle);
        qDebug()<<"listPointCircle:"<<listPointCircle;
        for(;iterator2.hasNext();)
        {
            QListIterator<QPoint> iter(iterator2.next());
            if(iter.hasNext())
            {
                point1=iter.next();
                if(!iter.hasNext())
                {
                    listPointLen.append(0);
                    listPointAngle.append(-1);
                }
                else
                {
                    point2=iter.next();
                    for(;iter.hasNext();)
                    {
                        point3=iter.next();
                        len=distancePoint(point1,point2);
                        d1=(point1.x()-point2.x())*(point3.x()-point2.x())+(point1.y()-point2.y())*(point3.y()-point2.y());
                        d2=distancePoint(point1,point2)*distancePoint(point3,point2);
                        angle=acos(d1/d2);
                        listPointLen.append(len);
                        listPointAngle.append(angle);
                        point1=point2;
                        point2=point3;
                    }
                    len=distancePoint(point1,point2);
                    listPointLen.append(len);
                    listPointAngle.append(-1);
                }
            }
        }
    }
    if(haveEllipse)
    {
        QListIterator< QList<QPoint> > iterator3(listPointEllipse);
        qDebug()<<"listPointEllipse:"<<listPointEllipse;
        for(;iterator3.hasNext();)
        {
            QListIterator<QPoint> iter(iterator3.next());
            if(iter.hasNext())
            {
                point1=iter.next();
                if(!iter.hasNext())
                {
                    listPointLen.append(0);
                    listPointAngle.append(-1);
                }
                else
                {
                    point2=iter.next();
                    for(;iter.hasNext();)
                    {
                        point3=iter.next();
                        len=distancePoint(point1,point2);
                        d1=(point1.x()-point2.x())*(point3.x()-point2.x())+(point1.y()-point2.y())*(point3.y()-point2.y());
                        d2=distancePoint(point1,point2)*distancePoint(point3,point2);
                        angle=acos(d1/d2);
                        listPointLen.append(len);
                        listPointAngle.append(angle);
                        point1=point2;
                        point2=point3;
                    }
                    len=distancePoint(point1,point2);
                    listPointLen.append(len);
                    listPointAngle.append(-1);
                }
            }
        }
    }
    if(haveLine)
    {
        QListIterator< QList<QPoint> > iterator4(listPointLine);
        qDebug()<<"listPointLine:"<<listPointLine;
        for(;iterator4.hasNext();)
        {
            QListIterator<QPoint> iter(iterator4.next());
            if(iter.hasNext())
            {
                point1=iter.next();
                if(!iter.hasNext())
                {
                    listPointLen.append(0);
                    listPointAngle.append(-1);
                }
                else
                {
                    point2=iter.next();
                    for(;iter.hasNext();)
                    {
                        point3=iter.next();
                        len=distancePoint(point1,point2);
                        d1=(point1.x()-point2.x())*(point3.x()-point2.x())+(point1.y()-point2.y())*(point3.y()-point2.y());
                        d2=distancePoint(point1,point2)*distancePoint(point3,point2);
                        angle=acos(d1/d2);
                        listPointLen.append(len);
                        listPointAngle.append(angle);
                        point1=point2;
                        point2=point3;
                    }
                    len=distancePoint(point1,point2);
                    listPointLen.append(len);
                    listPointAngle.append(-1);
                }
            }
        }
    }
    if(havePolyline)
    {
        QListIterator< QList<QPoint> > iterator5(listPointPolyline);
        qDebug()<<"listPointPolyline:"<<listPointPolyline;
        for(;iterator5.hasNext();)
        {
            QListIterator<QPoint> iter(iterator5.next());
            if(iter.hasNext())
            {
                point1=iter.next();
                if(!iter.hasNext())
                {
                    listPointLen.append(0);
                    listPointAngle.append(-1);
                }
                else
                {
                    point2=iter.next();
                    for(;iter.hasNext();)
                    {
                        point3=iter.next();
                        len=distancePoint(point1,point2);
                        d1=(point1.x()-point2.x())*(point3.x()-point2.x())+(point1.y()-point2.y())*(point3.y()-point2.y());
                        d2=distancePoint(point1,point2)*distancePoint(point3,point2);
                        angle=acos(d1/d2);
                        listPointLen.append(len);
                        listPointAngle.append(angle);
                        point1=point2;
                        point2=point3;
                    }
                    len=distancePoint(point1,point2);
                    listPointLen.append(len);
                    listPointAngle.append(-1);
                }
            }
        }
    }
    if(havePolygon)
    {
        QListIterator< QList<QPoint> > iterator6(listPointPolygon);
        qDebug()<<"listPointPolygon:"<<listPointPolygon;
        for(;iterator6.hasNext();)
        {
            QListIterator<QPoint> iter(iterator6.next());
            if(iter.hasNext())
            {
                point1=iter.next();
                if(!iter.hasNext())
                {
                    listPointLen.append(0);
                    listPointAngle.append(-1);
                }
                else
                {
                    point2=iter.next();
                    for(;iter.hasNext();)
                    {
                        point3=iter.next();
                        len=distancePoint(point1,point2);
                        d1=(point1.x()-point2.x())*(point3.x()-point2.x())+(point1.y()-point2.y())*(point3.y()-point2.y());
                        d2=distancePoint(point1,point2)*distancePoint(point3,point2);
                        angle=acos(d1/d2);
                        listPointLen.append(len);
                        listPointAngle.append(angle);
                        point1=point2;
                        point2=point3;
                    }
                    len=distancePoint(point1,point2);
                    listPointLen.append(len);
                    listPointAngle.append(-1);
                }
            }
        }
    }
    if(havePath)
    {
        QListIterator< QList<QPoint> > iterator7(listPointPath);
        qDebug()<<"listPointPath:"<<listPointPath;
        for(;iterator7.hasNext();)
        {
            QListIterator<QPoint> iter(iterator7.next());
            if(iter.hasNext())
            {
                point1=iter.next();
                if(!iter.hasNext())
                {
                    listPointLen.append(0);
                    listPointAngle.append(-1);
                }
                else
                {
                    point2=iter.next();
                    for(;iter.hasNext();)
                    {
                        point3=iter.next();
                        len=distancePoint(point1,point2);
                        d1=(point1.x()-point2.x())*(point3.x()-point2.x())+(point1.y()-point2.y())*(point3.y()-point2.y());
                        d2=distancePoint(point1,point2)*distancePoint(point3,point2);
                        angle=acos(d1/d2);
                        listPointLen.append(len);
                        listPointAngle.append(angle);
                        point1=point2;
                        point2=point3;
                    }
                    len=distancePoint(point1,point2);
                    listPointLen.append(len);
                    listPointAngle.append(-1);
                }
            }
        }
    }
}

//保存信息到Settings表中
void Dialog::LoadSettings()
{
    QSettings settings("DIWire","DIWireSettings");
    settings.beginGroup("settings");
    curDir = settings.value("currentDirectory", "").toString();
    QDir d;
    if(!curDir.isEmpty())
    {
         if(d.exists(curDir))
         {
             qDebug()<<"load last Dir\n";
         }
         else
         {
             curDir=tr(".");
         }
     }
    else
    {
        curDir=tr(".");
    }
    qDebug()<<"load: "<<curDir;
    settings.endGroup();
}

//保存信息到Settings表中
void Dialog::SaveSettings()
{
    QSettings settings("DIWire","DIWireSettings");
    settings.beginGroup("settings");
    settings.setValue("currentDirectory",curDir);
    settings.endGroup();
}

