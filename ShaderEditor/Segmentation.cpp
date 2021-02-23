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
#include <QPainterPath>
QAction * zLarge;
QAction * zSmall ;
QAction * pencil;
int w;
int h;
Segmentation::Segmentation(QWidget *parent) : QMainWindow(parent),ui( new Ui::Segmentation)

{
    ui->setupUi(this);
    ui->label_3->setText("100");
    ui->size->setRange(50, 300);
    ui->size->setValue(100);
    ui->saveImage->setEnabled(false);
    ui->saveAsImage->setEnabled(false);
    ui->segmentation->setEnabled(false);
    ui->editImage->setEnabled(false);
    color = QColor(0,0,0);
    QPixmap pen("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\pencil.png");
    QPixmap colorBlack("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\black.png");
    QPixmap colorWhite("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\white.png");
    QPixmap large("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\large.png");
    QPixmap petit("C:\\Users\\aduongng\\Desktop\\project\\App\\Radium-Apps\\ShaderEditor\\small.png");
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
    zSmall = toolbarZoom->addAction(QIcon(petit), "zoomSmall");
    toolbarZoom->addSeparator();
    QAction *_toolbarZoom = toolbarZoom->toggleViewAction();
    ui->menuView->addAction(_toolbarZoom);
    w = ui->listView->width();
    h = ui->listView->height();
    connect( ui->openImageTest, &QAction::triggered, this, & Segmentation::loadImageTest);
    connect( ui->saveImage, &QAction::triggered, this, & Segmentation::saveImage);
    connect( ui->saveAsImage, &QAction::triggered, this, & Segmentation::saveAsImage);
    connect( ui->quitApp, &QAction::triggered, this, & Segmentation::quit);
    connect( ui->pushButton, &QPushButton::clicked, this, & Segmentation::showImage );
    connect( ui->paint, &QAction::triggered, this, & Segmentation::choosePaint );
    connect( ui->segmentation_2D, &QAction::triggered, this, & Segmentation::chooseSegemtation );
    connect(ui->size, SIGNAL(valueChanged(int)), this, SLOT(zoom(int)));
    connect(zLarge, &QAction::triggered, this, & Segmentation::zoomLarge);
    connect(zSmall, &QAction::triggered, this, & Segmentation::zoomSmall);
    connect(pencil, &QAction::triggered, this, & Segmentation::drawPen);
    connect(black, &QAction::triggered, this, & Segmentation::colorBlack);
    connect(white, &QAction::triggered, this, & Segmentation::colorWhite);
    connect( ui->run, &QPushButton::clicked, this, & Segmentation::runSegmentation );

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
void Segmentation::drawPen()
{
 drawing = true;
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
    w = image.width();
    h = image.height();
    ui->listView->resize(w,h);
    ui->label_2->resize(w,h);
    ui->label_2->setPixmap(image);
    ui->saveImage->setEnabled(true);
    ui->saveAsImage->setEnabled(true);
    pencil->setEnabled(true);
    haveImage = true;
    ui->label_3->setText("100");
    ui->size->setValue(100);


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

void Segmentation::colorBlack()
{
   color = QColor(0,0,0);
}
void Segmentation::colorWhite()
{
    color = QColor(255,255,255);
}
void Segmentation::zoom(int k)
{
   drawing = false;
  /* float tmp_width = (ui->listView->width()*ui->label_3->text().toInt()/100);
   float tmp_height = (ui->listView->height()*ui->label_3->text().toInt()/100);
   ui->label_3->setText(QString::number(k));
   float tmp1_width = (ui->listView->width()*k/100);
   float tmp1_height = ui->listView->height()*k/100;
   float width = ui->listView->width()+ (tmp1_width- tmp_width);
   float height = ui->listView->height()+ (tmp1_height- tmp_height);*/
   int width = w*k/100;
   int height = h*k/100;
   ui->listView->resize(width,height);
   ui->label_2->resize(width,height);
   ui->label_3->setText(QString::number(k));
   if(haveImage){
       //QImageReader reader;
       //reader.setFileName(path);
      // QImage _image = reader.read();
       QImage _image = image.toImage();
       QImage img2 = _image.scaled(width, height, Qt::IgnoreAspectRatio);
       ui->label_2->setPixmap(QPixmap::fromImage(img2));
   }
   if(k==300)
      zLarge->setEnabled(false);

}
void Segmentation::zoomLarge()
{
    drawing = false;
    zSmall->setEnabled(true);
    if(ui->label_3->text().toInt()<=590){
        float k = ui->label_3->text().toInt() +10;
        ui->label_3->setText(QString::number(k));
        int width = w*k/100;
        int height = h*k/100;
        ui->listView->resize(width,height);
        ui->label_2->resize(width,height);
        ui->size->setValue(k);
        if(haveImage){
           // QImageReader reader;
            //reader.setFileName(path);
            //QImage _image = reader.read();
            QImage _image = image.toImage();
            QImage img2 = _image.scaled(width, height, Qt::IgnoreAspectRatio);
            ui->label_2->setPixmap(QPixmap::fromImage(img2));

        }
        if(k==300)
           zLarge->setEnabled(false);
        if(k==50)
           zSmall->setEnabled(false);
    }
}
void Segmentation::zoomSmall()
{
    drawing = false;
    zLarge->setEnabled(true);
    if(ui->label_3->text().toInt()>=60){
        float k = ui->label_3->text().toInt() -10;
        ui->label_3->setText(QString::number(k));
        int width = w*k/100;
        int height = h*k/100;
        ui->listView->resize(width,height);
        ui->label_2->resize(width,height);
        ui->size->setValue(k);
        if(haveImage){
           // QImageReader reader;
            //reader.setFileName(path);
            QImage _image = image.toImage();
            QImage img2 = _image.scaled(width, height, Qt::IgnoreAspectRatio);
            ui->label_2->setPixmap(QPixmap::fromImage(img2));

        }
        if(k==50)
           zSmall->setEnabled(false);
    }

}
void  Segmentation::mousePressEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton)&&(drawing ==true))
        {
            pt1.push_back(event -> pos());
            pt2 =pt1;
        }
}
void  Segmentation::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() == QEvent::MouseMove)&&(drawing ==true))
        {
             pt2.pop_back();
             pt2.push_back(event->pos());
             update ();
        }
}
void  Segmentation::mouseReleaseEvent(QMouseEvent *event)
{

   /*if (drawing ==true)
    {
    pt2.pop_back();
    pt2.push_back(event->pos());
    update ();
   }*/

}
void  Segmentation::paintEvent(QPaintEvent *)
{
    if(drawing==true){
    QPen pen;
    ui->listView->resize(image.width(),image.height());
    ui->label_2->resize(image.width(),image.height());
    QPainter tmpPainter(&image);
    pen.setColor(color);
    pen.setWidth(5);
    tmpPainter.setPen(pen);
    draw(tmpPainter);
    ui->label_2->setPixmap(image);
    }
}
void  Segmentation::draw( QPainter &paint)
{
    for(int i=0;i<pt1.size();i++)
    {
       paint.drawPoint(pt1[i]);
      // pt1=pt2;
    }
}

void Segmentation::choosePaint()
{
 ui->editImage->setEnabled(true);
 ui->segmentation->setEnabled(false);
}
void Segmentation::chooseSegemtation()
{
 ui->segmentation->setEnabled(true);
 ui->editImage->setEnabled(false);
}

void Segmentation::runSegmentation()
{
    QMessageBox msgBox;
    QDir pathDirImage(ui->image_path->toPlainText());
    QDir pathDirMask(ui->mask_path->toPlainText());
    QFile pathDirModel(ui->model->toPlainText());
    QDir pathDirTest(ui->test_image->toPlainText());
    if(ui->image_path->toPlainText().isEmpty()){
         msgBox.setText("Can you add path of images tranining please?");
         msgBox.exec();
    }
    else if(ui->mask_path->toPlainText().isEmpty()){
        msgBox.setText("Can you add path of masks tranining please?");
        msgBox.exec();
    }
    else if(ui->model->toPlainText().isEmpty()){
        msgBox.setText("Can you add path of model check point please?");
        msgBox.exec();
    }
    else if(ui->test_image->toPlainText().isEmpty()){
        msgBox.setText("Can you add path of image test please?");
        msgBox.exec();
    }
    else if(ui->output->toPlainText().isEmpty()){
        msgBox.setText("Can you add path of image segementation please?");\
        msgBox.exec();
    }
    else
    {
         if (!pathDirImage.exists()){
             msgBox.setText("Path of image training is not exist!");
             msgBox.exec();
         }
         else if (!pathDirMask.exists()){
             msgBox.setText("Path of masks tranining is not exist!");
             msgBox.exec();
         }
         else if (!pathDirModel.exists()){
             msgBox.setText("Path of  model check point is not exist!");
             msgBox.exec();
         }
         else if (!pathDirTest.exists()){
             msgBox.setText("Path of image test is not exist!");
             msgBox.exec();
         }
         else {
             msgBox.setText("Ahihi");
             msgBox.exec();
         }

    }
}
