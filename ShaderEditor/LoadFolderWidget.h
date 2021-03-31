#ifndef LOADFOLDERIMAGE_H
#define LOADFOLDERIMAGE_H
#pragma once
#include <QWidget>
#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include "MyParameterProvider.hpp"
#include "ui_LoadFolderWidget.h"
#include "CameraManipulator.hpp"
#include <Core/Geometry/RayCast.hpp>
#include <GuiBase/Viewer/Viewer.hpp>
#define LOADDATA 1
#define SHOWIMAGE 2
#define SAVEJSON 3
#define INVERSEPIXEL 4
enum CNN { Unet, CNN2, CNN3 };
namespace Ui {
class LoadFolderWidget;
}

namespace Ra{
namespace Engine{
    class RenderObject;
    class Renderer;
    class ShaderParameterProvider;
    class TextureManager;
}
}

///
/// \todo Add automatic UI generation from parameter provider

class LoadFolderWidget: public QWidget
{
    Q_OBJECT

public:
    explicit LoadFolderWidget( std::shared_ptr< Ra::Engine::RenderObject > ro,
                               Ra::Engine::Renderer * renderer,
                               std::string pathRaw,
                               std::string pathUnet,
                               std::shared_ptr< MyParameterProvider > paramProvider,
                               Ra::Gui::Viewer *_viewer,
                               QWidget *parent = nullptr);
    ~LoadFolderWidget();
//protected:
 //   void inverseColor();
private slots:
    void enableLoadData();
    void enableShowImage();
    void enableSaveJson();
    void enableInversePixel();
    void checkUnet();
    void checkCNN2();
    void checkCNN3();
    void runImage();
    void runUnet();
protected:
    void mousePressEvent(QMouseEvent *event);
private:
    Ui::LoadFolderWidget *ui;
    std::shared_ptr< Ra::Engine::RenderObject > _ro;
    Ra::Engine::Renderer * _renderer;
    std::string _pathRaw;
    std::string _pathUnet;
    std::shared_ptr< MyParameterProvider  > _paramProvider;
    int choisTypeRun =0;
    CNN typeCNN;
    QStringList pathListRawFull;
    QStringList pathListUnetFull;
    QDir directoryRaw;
    QDir directoryCNN;
    CameraManipulator2D * _camera;
    Ra::Gui::Viewer *_viewer;
    Ra::Core::Ray r;
    Scalar x =-0.5;
    Scalar y =-0.5;
};


#endif // LOADFOLDERIMAGE_H



