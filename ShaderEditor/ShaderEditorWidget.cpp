#include "LoadFolderWidget.h"
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
#include <fstream>

LoadFolderWidget::LoadFolderWidget( std::shared_ptr< Ra::Engine::RenderObject > ro,
                                                            Ra::Engine::Renderer * renderer,
                                                            std::string pathRaw,
                                                            std::string pathUnet,
                                                            std::shared_ptr< MyParameterProvider > paramProvider,
                                                            QWidget *parent) :
                                QWidget( parent ),
                                ui( new Ui::LoadFolderWidget),
                                _ro( ro ),
                                _renderer( renderer ),
                                _pathRaw(pathRaw),
                                _pathUnet(pathUnet),
                                _paramProvider(paramProvider)
{
    ui->setupUi(this);
    ui->imageOrgine->setEnabled(false);
    ui->loadUnet->setEnabled(false);

    connect( ui->buttonLoad, &QPushButton::clicked, this, &LoadFolderWidget::enableLoadData );
    connect( ui->buttonShow, &QPushButton::clicked, this, &LoadFolderWidget::enableShowImage);
    connect( ui->buttonJson, &QPushButton::clicked, this, &LoadFolderWidget::enableSaveJson);
    connect( ui->buttonRunImage, &QPushButton::clicked, this, &LoadFolderWidget::runImage);
    connect( ui->buttonRunUnet, &QPushButton::clicked, this, &LoadFolderWidget::runUnet);
}

LoadFolderWidget::~LoadFolderWidget()
{
    delete ui;
}

void LoadFolderWidget::enableLoadData()
{
    choisTypeRun = LOADDATA;
    ui->imageOrgine->setEnabled(true);
    ui->loadUnet->setEnabled(true);
}

void LoadFolderWidget::enableShowImage()
{
    choisTypeRun = SHOWIMAGE;
    ui->imageOrgine->setEnabled(true);
    ui->loadUnet->setEnabled(true);
}

void LoadFolderWidget::enableSaveJson()
{
    choisTypeRun = SAVEJSON;
    ui->imageOrgine->setEnabled(true);
    ui->loadUnet->setEnabled(true);
}
void LoadFolderWidget::runImage()
{
    QMessageBox msgBox;
    switch(choisTypeRun){
    case LOADDATA:{
        msgBox.setText("Selection Folder Of Original Image?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No); 
        int res = msgBox.exec();
        if (res==QMessageBox::Yes){
            ui->boxListImageOrigine->clear();
            QDir directory = QFileDialog::getExistingDirectory(this, "Select Directory");
            QStringList nameList = directory.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP",QDir::Files);
            std::string textDataset="{ \"datasetinfo\" :\n  {\n       \"name\" : \"";
            textDataset+=directory.dirName().toStdString();
            textDataset+="\",\n";
            textDataset+="       \"location\" : \"";
            textDataset+=directory.path().toStdString();
            textDataset+="\",\n";
            textDataset+="       \"contains\" : \"";
            textDataset+=std::to_string(nameList.size());
            textDataset+=" files\",\n";
            textDataset+="       \"images\" : [\n";
            for (int i=0; i<nameList.size();i++){
                   pathListRawFull.append(directory.path()+"/"+nameList.at(i));
                   textDataset+="       {\n";
                   textDataset+="            \"name\" : \"";
                   char *p;
                   std::string tmp = nameList.at(i).toStdString();
                   char *tmp1 = const_cast<char*>(tmp.c_str());
                   p = strtok(tmp1, ".");
                   textDataset+=p;
                   textDataset+="\",\n";
                   textDataset+="            \"itemtype\" : \"";
                   p = strtok(NULL, "\eof");
                   textDataset+=p;
                   textDataset+="\",\n";
                   QImageReader reader;
                   QSettings settings;
                   reader.setFileName(directory.path()+"/"+nameList.at(i));
                   QImage image = reader.read();
                   textDataset+="            \"size\" : \"";
                   textDataset+=std::to_string(image.sizeInBytes()/1024);
                   textDataset+=" KB\",\n";
                   textDataset+="            \"dimensiones\" : \"";
                   textDataset+=std::to_string(image.width());
                   textDataset+=" x ";
                   textDataset+=std::to_string(image.height());
                   textDataset+="\"\n";
                   if(i!=nameList.size()-1){
                        textDataset+="       },\n";
                        textDataset+="\n";
                    }
                   else
                       textDataset+="       }\n";
             }
            textDataset+="               ]\n";
            textDataset+="  }\n";
            textDataset+="}";
            ui->textDatasetJson->setText(QString::fromStdString(textDataset));
            ui->boxListImageOrigine->addItems(nameList);
        }
        else if (res == QMessageBox::No){
            QMessageBox msgBox1;
            msgBox1.setText("Nothing Selected !!!");
            msgBox1.exec();
        }
    }
        break;
    case SHOWIMAGE:{
        int tmp;
        tmp = ui->boxListImageOrigine->currentIndex();
        _pathRaw = pathListRawFull.at(tmp).toStdString();
       // _pathRaw = ui->boxListImageOrigine->currentText().toStdString();
        tmp = ui->boxImageUnet->findText(ui->boxListImageOrigine->currentText());
        if(tmp==-1){
            QMessageBox msgBox2;
            msgBox2.setText("Have no data of image Unet");
            msgBox2.exec();
        }
        else {
         //   ui->boxImageUnet->setCurrentIndex(tmp);
           // _pathUnet = ui->boxImageUnet->currentText().toStdString();
            _pathUnet = pathListUnetFull.at(tmp).toStdString();
            _paramProvider->setPathRaw(_pathRaw);
            _paramProvider->setPathUnet(_pathUnet);
            }
        break;
    }
   case SAVEJSON:
        if(ui->textDatasetJson->document()->isEmpty()){
            QMessageBox msgBox3;
            msgBox3.setText("Your file Dataset.json is empty !!!");
            msgBox3.exec();
        }
        else{
            QString path = QFileDialog::getSaveFileName(this, "Save Image", "",
                                                                   "Json(*.json) ");
            if(path == ""){
              QMessageBox msgBox4;
              msgBox4.setText("No directory");
              msgBox4.exec();
             }
             else{
                std::ofstream dataset(path.toStdString());
                dataset<<ui->textDatasetJson->toPlainText().toStdString();
                dataset.close();
            }

        }
        break;
 }
}

void LoadFolderWidget::runUnet()
{
    QMessageBox msgBox;
    switch(choisTypeRun){
    case LOADDATA:{
        msgBox.setText("Selection Folder Of Image after Segemation by Unet?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int res = msgBox.exec();
        if (res==QMessageBox::Yes){
            ui->boxImageUnet->clear();
            QDir directory = QFileDialog::getExistingDirectory(this, "Select Directory");
            QStringList nameList = directory.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP",QDir::Files);
            for (int i=0; i<nameList.size();i++){
                pathListUnetFull.append(directory.path()+"/"+nameList.at(i));
            }
            ui->boxImageUnet->addItems(nameList);
        }
        else if (res == QMessageBox::No){
            QMessageBox msgBox1;
            msgBox1.setText("Nothing Selected !!!");
            msgBox1.exec();
        }
    }
        break;
    case SHOWIMAGE:{
        int tmp;
        tmp = ui->boxImageUnet->currentIndex();
        _pathUnet = pathListUnetFull.at(tmp).toStdString();
        //_pathUnet = ui->boxImageUnet->currentText().toStdString();
        tmp = ui->boxListImageOrigine->findText(ui->boxImageUnet->currentText());
        if(tmp==-1){
            QMessageBox msgBox2;
            msgBox2.setText("Have no data of image Raw");
            msgBox2.exec();
        }
        else{
           // ui->boxListImageOrigine->setCurrentIndex(tmp);
            //_pathRaw = ui->boxListImageOrigine->currentText().toStdString();
            _pathRaw = pathListRawFull.at(tmp).toStdString();
            _paramProvider->setPathRaw(_pathRaw);
            _paramProvider->setPathUnet(_pathUnet);
        }
        break;
    }
    case SAVEJSON:
         break;
    }

}
