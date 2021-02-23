#ifndef SEGEMENTATION_H
#define SEGEMENTATION_H
#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QString>
#include <Engine/Renderer/Texture/TextureManager.hpp>
#include <QLabel>
#include <QPainter>
#include <QLine>
#include <QMouseEvent>
#include <QPainterPath>
namespace Ui {
class Segmentation;
}

namespace Ra{
namespace Engine{
    class RenderObject;
    class Renderer;
    class ShaderParameterProvider;
}
}

///
/// \todo Add automatic UI generation from parameter provider

class Segmentation : public QMainWindow
{
    Q_OBJECT

public:
    explicit Segmentation(QWidget *parent = nullptr);
    ~Segmentation();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
    void draw( QPainter &paint);
private slots:
    void loadImageTest();
    void saveImage();
    void saveAsImage();
    void loadData();
    void showImage();
    void quit();
    void colorBlack();
    void colorWhite();
    void zoom(int value);
    void zoomLarge();
    void zoomSmall();
    void drawPen();
    void choosePaint();
    void chooseSegemtation();
    void runSegmentation();
private:
    Ui::Segmentation *ui;
    QString path;
    QPixmap image;
    QColor color;
    bool haveImage = false;
    bool drawing=false;
    QVector<QPoint> pt1;                         // point array
    QVector<QPoint> pt2;
    QPainterPath *p;

};
#endif // SEGEMENTATION_H
