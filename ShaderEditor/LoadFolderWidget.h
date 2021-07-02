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
#include <QStringListModel>
#include <QListView>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include "MyParameterProvider.hpp"
#include "ui_LoadFolderWidget.h"
#include "Content.h"
#include "Segmentation.h"
#include <QSettings>
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
    const Content &getContent() const;
    void setContent(Content content);
    const QString& getPathProg() const;
    void setPathProg(const QString &pathProg);
    const QDir& getDirCNN() const;
    void setPathTmp(const QString &pathTmp);
    const QString& getPathTmp() const;
    const QSettings& getSetting() const;
    const QStringList& getPathListCNNFull() const;
    const QStringList& getPathListRawFull() const;
    void setIndexRaw(const int &i);
    const int& getIndexRaw() const;
    void setIndexCNN(const int &i);
    const int& getIndexCNN() const;
    bool showedImage();
    void saveAll();
    void saveImageChanged();
    const QVector <Ra::Engine::Dataset>& getDatasetRaw() const;
    const QVector <Ra::Engine::Dataset>& getDatasetCNN() const;
    void setDatasetRaw(QVector <Ra::Engine::Dataset> &dataRaw);
    void setDatasetCNN(QVector <Ra::Engine::Dataset> &dataCNN);
    const QDir& getDirModified() const;
    void setDirModified(const QDir& dirModified);
    const bool& getDraw() const;
    void setDraw(bool draw);
    void setIndexListBox(int idxRaw,int idxCNN);
    ~LoadFolderWidget();
private slots:
  /*  void enableLoadData();
    void enableShowImage();
    void enableSaveJson();*/
    //void checkUnet();
    //void checkCNN2();
     //void checkCNN3();
    void loadUtrasound();
    void loadSegmented();
    void runImage();
    void runUnet();
    void touchListUltrasound();
    void touchListSegmented();
    void showImage();
/*protected:
    bool eventFilter(QObject *obj, QEvent *evt);

    */
signals:
    void needUpdate();
private:
    Content _content;
    Ui::LoadFolderWidget *ui;
    int typeRun =0;
   // CNN typeCNN;
    QStringList pathListRawFull;
    QStringList pathListUnetFull;
    QDir directoryRaw;
    QDir directoryCNN;
    QString pathOut;
    QString infoImage;
    QStandardItemModel *model;
    QSortFilterProxyModel *proxyModel;
    QPixmap pix;
    QString _pathProg;
    QString _pathtmp;
    bool showed =false;
    bool creatDataCNN = false;
    int indexRaw =-1;
    int indexCNN =-1;
    QVector <Ra::Engine::TextureParameters> _textureParaRaw;
    QVector <Ra::Engine::TextureParameters> _textureParaCNN;
    QVector <Ra::Engine::Dataset> _dataRaw;
    QVector <Ra::Engine::Dataset> _dataCNN;
    bool loadSeg = 0;
    bool etatShow =0;
    QDir modifed;
    QString folderSave;
    bool _drawing = 0;
};


#endif // LOADFOLDERIMAGE_H



