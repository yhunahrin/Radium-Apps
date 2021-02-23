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
#include <iostream>
#include <fstream>
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
    color=Qt::black;
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
     color = Qt::black;
}
void Segmentation::colorWhite()
{
     color = Qt::white;
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
            pt1=event -> pos();
           // pt2 =pt1;
        }
}
void  Segmentation::mouseMoveEvent(QMouseEvent *event)
{
   /* if ((event->buttons() == QEvent::MouseMove)&&(drawing ==true))
        {
             pt2.pop_back();
             pt2.push_back(event->pos());
             update ();
        }*/
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
    //ui->listView->resize(image.width(),image.height());
    //ui->label_2->resize(image.width(),image.height());
   // image = QPixmap::fromImage(ui->label_2->pixmap()->toImage());
    QPainter tmpPainter(&image);
    pen.setColor(color);
    pen.setWidth(5);
    tmpPainter.setPen(pen);
    tmpPainter.drawPoint(pt1);
    ui->label_2->setPixmap(image);
      // pt1=pt2;
    }


}
void  Segmentation::draw( QPainter &paint)
{/*
    for(int i=0;i<pt1.size();i++)
    {
       paint.drawPoint(pt1[i]);
      // pt1=pt2;
    }*/
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
    QFileInfo pathDirModel(ui->model->toPlainText());
    QFileInfo pathDirTest(ui->test_image->toPlainText());
    QFileInfo pathDirOut(ui->output->toPlainText());
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
             std::ofstream model ("modelUnet.py");
             model<<"from keras.models import load_model\n";
             model<<"from keras import models\n";
             model<<"import numpy as np\n";
             model<<"import glob2\n";
             model<<"import glob\n";
             model<<"import tensorflow as tf\n";
             model<<"import cv2\n";
             model<<"from tensorflow.keras.optimizers import Adam\n";
             model<<"from tensorflow.keras.callbacks import EarlyStopping\n";
             model<<"from sklearn.model_selection import train_test_split\n";
             model<<"import matplotlib.pyplot as plt\n";
             model<<"from keras.callbacks import ModelCheckpoint\n";
             model<<"from keras.models import *\n";
             model<<"from keras.layers import *\n";
             model<<"from keras.optimizers import *\n";
             model<<"from keras.callbacks import ModelCheckpoint, LearningRateScheduler\n";
             model<<"from keras import backend as keras\n";
             model<<"from keras.utils import to_categorical\n";
             model<<"import skimage.io as io\n";
             model<<"INPUT_SHAPE = 512\n";
             model<<"OUTPUT_SHAPE = 512 \n";
             model<<"image_paths = glob2.glob('";
             model<<pathDirImage.path().toStdString();
             model<<"/*')\n";
             model<<"label_paths = glob2.glob('";
             model<<pathDirMask.path().toStdString();
             model<<"/*')\n";
             model<<"train_img_paths, val_img_paths, train_label_paths, val_label_paths = train_test_split(image_paths, label_paths, test_size = 0.2)\n";
             model<<"def _image_read_paths(train_img_paths, train_label_paths):\n";
             model<<"\tX, Y = [], []\n";
             model<<"\tfor image_path, label_path in zip(train_img_paths, train_label_paths):\n";
             model<<"\t\timage = cv2.imread(image_path)\n";
             model<<"\t\timage_resize = cv2.resize(image, (INPUT_SHAPE, INPUT_SHAPE), cv2.INTER_LINEAR)\n";
             model<<"\t\tlabel = cv2.imread(label_path)\n";
             model<<"\t\tlabel_gray = cv2.cvtColor(label, cv2.COLOR_BGR2GRAY)\n";
             model<<"\t\tlabel_resize = cv2.resize(label_gray, (OUTPUT_SHAPE, OUTPUT_SHAPE), cv2.INTER_LINEAR)\n";
             model<<"\t\tlabel_binary = np.array(label_resize == 255).astype('float32')\n";
             model<<"\t\tlabel_binary = label_binary[..., np.newaxis]\n";
             model<<"\t\tX.append(image_resize)\n";
             model<<"\t\tY.append(label_binary)\n";
             model<<"\tX = np.stack(X)\n";
             model<<"\tY = np.stack(Y)\n";
             model<<"\treturn X, Y\n";
             model<<"X_train, Y_train = _image_read_paths(train_img_paths, train_label_paths)\n";
             model<<"X_val, Y_val = _image_read_paths(val_img_paths, val_label_paths)\n";
             model<<"def _downsample_cnn_block(block_input, channel):\n";
             model<<"\tmaxpool = MaxPooling2D(pool_size=(2,2))(block_input)\n";
             model<<"\tconv1 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(maxpool)\n";
             model<<"\tconv2 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(conv1)\n";
             model<<"\treturn [maxpool, conv1, conv2]\n";
             model<<"def _upsample_cnn_block(block_input, block_counterpart, channel):\n";
             model<<"\tup = Conv2D(filters=channel, kernel_size=2, strides=1, padding='same')(UpSampling2D(size=(2,2))(block_input))\n";
             model<<"\tconcat = Concatenate(axis=-1)([block_counterpart, up])\n";
             model<<"\tconv1 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(concat)\n";
             model<<"\tconv2 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(conv1)\n";
             model<<"\treturn [up, concat, conv1, conv2]\n";
             model<<"def unet():\n";
             model<<"\tinput = Input(shape=(INPUT_SHAPE, INPUT_SHAPE, 3))\n";
             model<<"\tconv1_ds_block1 = Conv2D(filters=64,kernel_size=3, strides=1, padding='same')(input)\n";
             model<<"\tconv2_ds_block1 = Conv2D(filters=64,kernel_size=3, strides=1, padding='same')(conv1_ds_block1)\n";
             model<<"\tds_block2 = _downsample_cnn_block(conv2_ds_block1, channel=128)\n";
             model<<"\tds_block3 = _downsample_cnn_block(ds_block2[-1], channel=256)\n";
             model<<"\tds_block4 = _downsample_cnn_block(ds_block3[-1], channel=512)\n";
             model<<"\tdrop_ds_block4 = Dropout(0.5)(ds_block4[-1])\n";
             model<<"\tds_block5 = _downsample_cnn_block(drop_ds_block4, channel=1024)\n";
             model<<"\tdrop_ds_block5 = Dropout(0.5)(ds_block5[-1])\n";
             model<<"\tus_block4 = _upsample_cnn_block(drop_ds_block5, drop_ds_block4, channel=512)\n";
             model<<"\tus_block3 = _upsample_cnn_block(us_block4[-1], ds_block3[-1], channel=256)\n";
             model<<"\tus_block2 = _upsample_cnn_block(us_block3[-1], ds_block2[-1], channel=128)\n";
             model<<"\tus_block1 = _upsample_cnn_block(us_block2[-1], conv2_ds_block1, channel=64)\n";
             model<<"\tconv3_us_block1 = Conv2D(filters=2,kernel_size=3, strides=1, padding='same')(us_block1[-1])\n";
             model<<"\tconv4_us_block1 = Conv2D(filters=1, kernel_size=1, strides=1, padding='same', activation='sigmoid')(conv3_us_block1)\n";
             model<<"\tmodel = tf.keras.models.Model(inputs = input , outputs = conv4_us_block1)\n";
             model<<"\tmodel.compile(optimizer=Adam(lr = 1e-4), loss='binary_crossentropy', metrics='accuracy')\n";
             model<<"\treturn model\n";
             model<<"model2 = unet()\n";
             model<<"model_checkpoint = ModelCheckpoint('";
             model<<pathDirModel.absoluteFilePath().toStdString();
             model<<"',monitor = 'loss', verbose=1,save_best_only = True)\n";
             model<<"model2.fit(X_train, Y_train,validation_data = (X_val, Y_val),batch_size = 8, epochs = 1 ,callbacks = [model_checkpoint])\n";
             model<<"def _predict_path(path,path_out, figsize = (16, 8)):\n";
             model<<"\timg_tmp = cv2.imread(path)\n";
             model<<"\th, w, _ = img_tmp.shape\n";
             model<<"\timg = cv2.resize(img_tmp, (512, 512), cv2.INTER_LINEAR)\n";
             model<<"\timg_expand = img[np.newaxis, ...]\n";
             model<<"\timg_pred = model2.predict(img_expand).reshape(512, 512)\n";
             model<<"\timg_pred[img_pred < 0.5] = 0\n";
             model<<"\timg_pred[img_pred >= 0.5] = 1\n";
             model<<"\timg_pred = cv2.resize(img_pred,(w,h))\n";
             model<<"\tio.imsave(path_out,img_pred)\n";
             model<<"_predict_path('";
             model<<pathDirTest.absoluteFilePath().toStdString();
             model<<"','";
             model<<pathDirOut.absoluteFilePath().toStdString();
             model<<"')\n";
             model.close();
             msgBox.setText("Your File Model is Successly!!");
             msgBox.exec();
         }

    }
}
