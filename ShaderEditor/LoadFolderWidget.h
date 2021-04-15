#ifndef LOADFOLDERIMAGE_H
#define LOADFOLDERIMAGE_H
#pragma once
#include <Engine/Renderer/Renderer.hpp>
#include <Engine/Renderer/RenderObject/RenderObjectManager.hpp>
#include <Engine/Renderer/RenderObject/RenderObject.hpp>

// include the custom material definition
#include <Engine/Renderer/Material/RawShaderMaterial.hpp>

#include <QPushButton>
#include <QString>
#include <QTextEdit>
#include <QMenu>
#include <QImageReader>

#include <QColorDialog>
#include <QComboBox>
#include <QPushButton>
#include <QSettings>
#include <QToolButton>
#include <iostream>

#include <QImage>
#include <QImageReader>
#include <QMenu>
#include <QToolBar>
#include <QSlider>
#include <fstream>
#include <QWidget>
#include <QString>
#include <QFileDialog>
#include "MyParameterProvider.hpp"
#include "ui_LoadFolderWidget.h"
#include "Content.h"

namespace Ui {
class LoadFolderWidget;
}

///
/// \todo Add automatic UI generation from parameter provider

class LoadFolderWidget: public QWidget
{
    Q_OBJECT

public:
    explicit LoadFolderWidget(Content content,
                               QWidget *parent = nullptr);
    Content getContent();
    void setContent(Content content);

    ~LoadFolderWidget();
private slots:
    void enableLoadData();
    void enableShowImage();
    void enableSaveJson();
    void checkUnet();
    void checkCNN2();
    void checkCNN3();
    void runImage();
    void runUnet();
private:
    Content _content;
    Ui::LoadFolderWidget *ui;
    int typeRun =0;
    CNN typeCNN;
    QStringList pathListRawFull;
    QStringList pathListUnetFull;
    QDir directoryRaw;
    QDir directoryCNN;
};


#endif // LOADFOLDERIMAGE_H



