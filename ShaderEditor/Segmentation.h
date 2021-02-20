#ifndef SEGEMENTATION_H
#define SEGEMENTATION_H
#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QString>
#include <Engine/Renderer/Texture/TextureManager.hpp>
#include <QLabel>
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

private slots:
    void loadImageTest();
    void saveImage();
    void saveAsImage();
    void loadData();
    void showImage();
    void quit();
    void draw();
    void colorBlack();
    void colorWhite();
    void zoom();
private:
    Ui::Segmentation *ui;
    QString path;
    QPixmap image;
    QColor color;

};
#endif // SEGEMENTATION_H
