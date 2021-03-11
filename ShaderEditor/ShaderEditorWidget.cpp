#include "ShaderEditorWidget.hpp"
#include "ui_ShaderEditorWidget.h"

#include "MyParameterProvider.hpp"

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
#include <QFileDialog>
#include <QPushButton>
#include <QSettings>
#include <QToolButton>
#include <iostream>
#include <QMessageBox>
#include <QImage>
#include <QImageReader>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QSlider>
ShaderEditorWidget::ShaderEditorWidget(
                                const std::string& v,
                                const std::string& f,
                                std::shared_ptr< Ra::Engine::RenderObject > ro,
                                Ra::Engine::Renderer * renderer,
                                std::string path,
                                std::shared_ptr< MyParameterProvider > paramProvider,
                                QWidget *parent) :
    QWidget( parent ),
    ui( new Ui::ShaderEditorWidget ),
    _ro( ro ),
    _renderer( renderer ),
    _path(path),
    _paramProvider(paramProvider)

{
    ui->setupUi(this);
    ui->_vertShaderEdit->setPlainText( QString::fromStdString( v ) );
    ui->_fragShaderEdit->setPlainText( QString::fromStdString( f ) );
    QPixmap pen("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\pencil.png");
    QPixmap colorBlack("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\black.png");
    QPixmap colorWhite("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\white.png");
    QPixmap large("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\large1.png");
    QPixmap petit("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\small1.png");
    /*add
    QToolBar *toolbarDraw = addToolBar("Draw");
    pencil = toolbarDraw->addAction(QIcon(pen), "pencil");
    QAction *black = toolbarDraw->addAction(QIcon(colorBlack), "black");
    QAction *white = toolbarDraw->addAction(QIcon(colorWhite), "white");
    pencil->setEnabled(false);
    toolbarDraw->addSeparator();
    QAction *_toolbarDraw = toolbarDraw->toggleViewAction();
    ui->menuEdit->addAction(_toolbarDraw);
    QToolBar *toolbarZoom = addToolBar("Zoom");
    zLarge = toolbarZoom->addAction(QIcon(large), "zoomLarge");
    zSmall = toolbarZoom->addAction(QIcon(petit), "zoomSmall");*/

    connect( ui->_compileShaders, &QPushButton::clicked, this, &ShaderEditorWidget::updateShadersFromUI );
    connect( ui->_addData, &QPushButton::clicked, this, &ShaderEditorWidget::loadData);
    connect( ui->_showImage, &QPushButton::clicked, this, &ShaderEditorWidget::showImage);
}

ShaderEditorWidget::~ShaderEditorWidget()
{
    delete ui;
}

void ShaderEditorWidget::updateShadersFromUI() 
{
    using ShaderConfigType = std::vector<std::pair<Ra::Engine::ShaderType, std::string>> ;

    const ShaderConfigType config {
       {Ra::Engine::ShaderType::ShaderType_VERTEX,   ui->_vertShaderEdit->toPlainText().toStdString()},
       {Ra::Engine::ShaderType::ShaderType_FRAGMENT, ui->_fragShaderEdit->toPlainText().toStdString()}};
    auto mat           = static_cast<Ra::Engine::RawShaderMaterial*>( _ro->getMaterial().get() );
    mat->updateShaders( config, _paramProvider );
    _renderer->buildRenderTechnique( _ro.get() );
}

void ShaderEditorWidget::loadData()
{
    QImageReader reader;
    QSettings settings;
    QMessageBox msgBox;
    msgBox.setText("Selection Files(Yes) or Folder(No)?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int res = msgBox.exec();
    if (res==QMessageBox::Yes){
        QString path  = settings.value( "files/load", QDir::homePath()).toString();
        QStringList pathList = QFileDialog::getOpenFileNames( this, "Open Files", path,tr("Image files (*.jpg *.png *.bmp)") );
        ui->_listPath->addItems(pathList);
    }
    else if (res == QMessageBox::No){
        QDir directory = QFileDialog::getExistingDirectory(this, "Select Directory");
        QStringList pathList = directory.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP",QDir::Files);
        QStringList pathListFull;
        for (int i=0; i<pathList.size();i++){
            pathListFull.append(directory.path()+"/"+pathList.at(i));
        }
        ui->_listPath->addItems(pathListFull);
    }
}

void ShaderEditorWidget::showImage()
{
    _path = ui->_listPath->currentText().toStdString();
    //_paramProvider->setPath(_path);
}
