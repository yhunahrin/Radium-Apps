#include "Segmentation.h"
#include "ui_Segmentation.h"

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
Segmentation::Segmentation(QWidget *parent) : QMainWindow(parent),ui( new Ui::Segmentation)

{
    ui->setupUi(this);
    ui->label_3->setText("100");
    ui->size->setRange(12.5, 600);
    ui->size->setValue(100);
    ui->saveImage->setEnabled(false);
    ui->saveAsImage->setEnabled(false);
    QPixmap pen("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\pencil.png");
    QPixmap colorBlack("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\black.png");
    QPixmap colorWhite("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\white.png");
    QPixmap large("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\large.png");
    QPixmap petit("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\small.png");
    QToolBar *toolbarDraw = addToolBar("Draw");
    QAction * pencil = toolbarDraw->addAction(QIcon(pen), "pencil");
    QAction * black = toolbarDraw->addAction(QIcon(colorBlack), "black");
    QAction * white = toolbarDraw->addAction(QIcon(colorWhite), "white");
    toolbarDraw->addSeparator();
    QAction *_toolbarDraw = toolbarDraw->toggleViewAction();
    ui->menuEdit->addAction(_toolbarDraw);
    QToolBar *toolbarZoom = addToolBar("Zoom");
    QAction * zoomLarge = toolbarZoom->addAction(QIcon(large), "zoomLarge");
    QAction * zoomSmall = toolbarZoom->addAction(QIcon(petit), "zoomSmall");
    toolbarZoom->addSeparator();
    QAction *_toolbarZoom = toolbarZoom->toggleViewAction();
    ui->menuView->addAction(_toolbarZoom);
    connect( ui->openImageTest, &QAction::triggered, this, & Segmentation::loadImageTest);
    connect( ui->saveImage, &QAction::triggered, this, & Segmentation::saveImage);
    connect( ui->saveAsImage, &QAction::triggered, this, & Segmentation::saveAsImage);
    connect( ui->quitApp, &QAction::triggered, this, & Segmentation::quit);
    connect( ui->pushButton, &QPushButton::clicked, this, & Segmentation::showImage );

}

Segmentation::~Segmentation()
{
    delete ui;
}
void Segmentation::loadImageTest()
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
        ui->comboBox->addItems(pathList);
    }
    else if (res == QMessageBox::No){
        QDir directory = QFileDialog::getExistingDirectory(this, "Select Directory");
        QStringList pathList = directory.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP",QDir::Files);
        QStringList pathListFull;
        for (int i=0; i<pathList.size();i++){
            pathListFull.append(directory.path()+"/"+pathList.at(i));
        }
        ui->comboBox->addItems(pathListFull);
    }
}
void Segmentation::saveImage()
{
    image.save(path);
}
void Segmentation::saveAsImage()
{
    path = QFileDialog::getSaveFileName(this, "Save Image", "",
                                                           "BMP(*.bmp);;PNG(*.png);;JPEG(*.jpg *.jpeg);;All Files(*.*) ");
    if(path == ""){
      QMessageBox msgBox;
      msgBox.setText("No directory");
     }
     else
         image.save(path);
}
void Segmentation::loadData()
{

}
void Segmentation::showImage()
{
    QImageReader reader;
    reader.setFileName(ui->comboBox->currentText());
    QImage _image = reader.read();
    Ra::Engine::TextureParameters tmp1;
    path = ui->comboBox->currentText();
    image = QPixmap::fromImage(_image);
    ui->listView->resize(image.width(),image.height());
    ui->label_2->resize(image.width(),image.height());
    ui->label_2->setPixmap(image);
    ui->saveImage->setEnabled(true);
    ui->saveAsImage->setEnabled(true);

}
void Segmentation::quit()
{
    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to quit?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int res = msgBox.exec();
    if (res==QMessageBox::Yes)
        qApp->quit();
}

void Segmentation::draw()
{

}
void Segmentation::colorBlack()
{

}
void Segmentation::colorWhite()
{

}
void Segmentation::zoom()
{

}
