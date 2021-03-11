#ifndef LOADFOLDERIMAGE_H
#define LOADFOLDERIMAGE_H
#pragma once
#include <QWidget>
#include <QMainWindow>
#include <QString>
#include "MyParameterProvider.hpp"
#include "ui_LoadFolderWidget.h"
#define LOADDATA 1
#define SHOWIMAGE 2
#define SAVEJSON 3
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
                               QWidget *parent = nullptr);
    ~LoadFolderWidget();

private slots:
    void enableLoadData();
    void enableShowImage();
    void enableSaveJson();
    void runImage();
    void runUnet();

private:
    Ui::LoadFolderWidget *ui;
    std::shared_ptr< Ra::Engine::RenderObject > _ro;
    Ra::Engine::Renderer * _renderer;
    std::string _pathRaw;
    std::string _pathUnet;
    std::shared_ptr< MyParameterProvider  > _paramProvider;
    int choisTypeRun =0;
    QStringList pathListRawFull;
    QStringList pathListUnetFull;
};


#endif // LOADFOLDERIMAGE_H



