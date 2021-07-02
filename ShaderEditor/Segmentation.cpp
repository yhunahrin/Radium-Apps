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

Segmentation::Segmentation(QString pathProgram, QWidget *parent) : QWidget(parent),ui( new Ui::Segmentation),_pathProg(pathProgram)

{
    ui->setupUi(this);
   // ui->segmentation->setEnabled(true);
   // connect( ui->run, &QPushButton::clicked, this, & Segmentation::runSegmentation );
    ui->zoneTrainning->setEnabled(false);
    ui->zoneTest->setEnabled(false);
    ui->textEdit->setEnabled(false);
    ui->buttonLoad->setEnabled(false);
    ui->buttonSave->setEnabled(false);
    ui->modelCheck->setEnabled(false);
    ui->modelTrained->setEnabled(false);
    ui->buttonModelCheck->setEnabled(false);
    ui->buttonModelTrained->setEnabled(false);
    connect(ui->scriptTest, &QCheckBox::clicked,this, &Segmentation::checkScriptTest);
    connect(ui->scriptTraining, &QCheckBox::clicked,this, &Segmentation::checkScriptTrain);
    connect(ui->preTrained, &QCheckBox::clicked,this, &Segmentation::checkPreTrained);
    connect(ui->buttonImageRaw, &QToolButton::clicked,this, &Segmentation::pathListImageRaw);
    connect(ui->buttonMask, &QToolButton::clicked,this, &Segmentation::pathListMask);
    connect(ui->buttonModelCheck, &QToolButton::clicked,this, &Segmentation::pathModelCheckPoint);
    connect(ui->buttonModelTrained, &QToolButton::clicked,this, &Segmentation::pathModelTrained);
    connect(ui->buttonImageTest, &QToolButton::clicked,this, &Segmentation::pathImageTest);
    connect(ui->buttonImageOut, &QToolButton::clicked,this, &Segmentation::pathImageOut);
    connect(ui->unet, &QRadioButton::clicked, this, & Segmentation::checkUnet);
    connect(ui->cnn2, &QRadioButton::clicked, this, & Segmentation::checkCNN2);
    connect(ui->cnn3, &QRadioButton::clicked, this, & Segmentation::checkCNN3);
    connect(ui->cnn4, &QRadioButton::clicked, this, & Segmentation::checkCNN4);
    connect(ui->buttonLoad, &QPushButton::clicked, this, & Segmentation::runLoad);
    connect(ui->buttonSave, &QPushButton::clicked, this, & Segmentation::runSave);
    connect(ui->buttonExcute, &QPushButton::clicked, this, & Segmentation::runExecute);
}

Segmentation::~Segmentation()
{
    delete ui;
}

void Segmentation::runExecute(){
    switch(typeCNN){
    case Unet:{
    QMessageBox msgBox;
    msgBox.setText("Do you want choose a script python from disk?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int res = msgBox.exec();
    if (res==QMessageBox::Yes){
        QProcess p;
         QStringList params;
         QStringList pythonScript = QFileDialog::getOpenFileNames(
                                 this,
                                 "Select one to open",
                                 "",
                                 " (*.py)");
         if( pythonScript.size() ==0){
             QMessageBox msgBox1;
             msgBox1.setText("Nothing script python selected!!!");
             msgBox1.exec();
         }
         else{
         params << pythonScript;
         p.start(this->getPathProgram(), params);
         QMessageBox msgBox2;
         msgBox2.setWindowTitle("Python info");
         msgBox2.setText("Python Running...");
         msgBox2.exec();
         p.waitForFinished(-1);
         QString p_stdout = p.readAll();
         QString p_stderr = p.readAllStandardError();

         if(!p_stderr.isEmpty()){
             QMessageBox msgBox1;
             msgBox1.setWindowTitle("Python error");
             msgBox1.setText(p_stderr);
             msgBox1.exec();
         }
         else{
         QMessageBox msgBox1;
         msgBox1.setWindowTitle("Python Result");
         msgBox1.setText(p_stderr);
         msgBox1.exec();}
         }
    }
    else if (res == QMessageBox::No){
       if(ui->textEdit->toPlainText().isEmpty()){
           QMessageBox msgBox1;
           msgBox1.setText("Have no script to execute");
           msgBox1.exec();}
       }
    else {
        std::ofstream model ("unet.py");
        model<<textScript.toStdString();
        QProcess p;
         QStringList params;
         QString pythonScript = QFileDialog::getOpenFileName(
                                 this,
                                 "Select one to open",
                                 "",
                                 " (*.py)");
         if( pythonScript ==""){
             QMessageBox msgBox1;
             msgBox1.setText("Nothing script python selected!!!");
             msgBox1.exec();
         }
         else{
         params << pythonScript;
         p.start(this->getPathProgram(), params);
         QMessageBox msgBox2;
         msgBox2.setWindowTitle("Python info");
         msgBox2.setText("Python Running...");
         msgBox2.exec();
         p.waitForFinished(-1);
         QString p_stdout = p.readAll();
         QString p_stderr = p.readAllStandardError();

         if(!p_stderr.isEmpty()){
             QMessageBox msgBox1;
             msgBox1.setWindowTitle("Python error");
             msgBox1.setText(p_stderr);
             msgBox1.exec();
         }
         else{
         QMessageBox msgBox1;
         msgBox1.setWindowTitle("Python Result");
         msgBox1.setText(p_stderr);
         msgBox1.exec();}
         }
    }
    }
        break;
}
}


//void Segmentation::runSegmentation()
//{
/*
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
             QString path = QFileDialog::getSaveFileName(this, "Save Image", "",
                                                                    "Python(*.py) ");
             std::ofstream model (path.toStdString());
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
             model<<"from keras.utils.np_utils import to_categorical\n";
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
             model<<"')";
             model.close();
             msgBox.setText("Your File Model is Successly!!");
             msgBox.exec();
         }

    }*/
//}

const QStringList& Segmentation::getPathTest() const{
    return _pathTest;
}
const QString& Segmentation::getPathOut() const{
    return _pathOut;
}
void Segmentation::setPathTest( const QStringList& pathTest){
    _pathTest.clear();
    for(int i=0;i<pathTest.size();i++)
        _pathTest.append(pathTest.at(i));
}
void Segmentation::setPathOut(const QString& pathOut){
    _pathOut=pathOut;
}

void Segmentation::checkUnet(){
   // ui->execute->setEnabled(true);
    typeModel = "UNET_b_160_IOU.h5";
    ui->zoneTrainning->setEnabled(true);
    ui->zoneTest->setEnabled(true);
    ui->textEdit->setEnabled(true);
    ui->buttonLoad->setEnabled(true);
    typeCNN = Unet;

}
void Segmentation::checkCNN2(){
    ui->zoneTrainning->setEnabled(true);
    ui->zoneTest->setEnabled(true);
    ui->textEdit->setEnabled(true);
    ui->buttonLoad->setEnabled(true);
    typeCNN = CNN2;
}
void Segmentation::checkCNN3(){
    ui->zoneTrainning->setEnabled(true);
    ui->zoneTest->setEnabled(true);
    ui->textEdit->setEnabled(true);
    ui->buttonLoad->setEnabled(true);
    typeCNN = CNN3;
}
void Segmentation::checkCNN4(){
    ui->zoneTrainning->setEnabled(true);
    ui->zoneTest->setEnabled(true);
    ui->textEdit->setEnabled(true);
    ui->buttonLoad->setEnabled(true);
    typeCNN = CNN4;
}
/*void Segmentation::checkBox(){
    if(ui->checkScriptPython->checkState()==Qt::Checked){
     ui->scriptPython->setEnabled(true);
     ui->scriptPath->setEnabled(true);}
    else if (ui->checkScriptPython->checkState()==Qt::Unchecked){
        ui->scriptPython->setEnabled(false);
        ui->scriptPath->setEnabled(false);
    }
}
*/
/*void Segmentation::pathProgram(){
    _pathPro = QFileDialog::getOpenFileName(
                            this,
                            "Select one to open",
                            "",
                            "Application (*.exe)");
    if(_pathPro!="")
        ui->pythonProgram->setPlainText(_pathPro);
}
void Segmentation::pathScript(){
    _pathPy = QFileDialog::getOpenFileName(
                            this,
                            "Select one to open",
                            "",
                            "Python (*.py)");
    if(_pathPy!="")
        ui->scriptPython->setPlainText(_pathPy);
}

void Segmentation::runPy(){
        QProcess p;
         QStringList params;
         if(ui->checkScriptPython->checkState()==Qt::Checked){
         QString pythonPath = ui->pythonProgram->toPlainText();
         QString pythonScript = ui->scriptPython->toPlainText();

         params << pythonScript;
         p.start(pythonPath, params);
         QMessageBox msgBox;
         msgBox.setWindowTitle("Python info");
         msgBox.setText("Python Running...");
         msgBox.exec();
         p.waitForFinished(-1);
         QString p_stdout = p.readAll();
         QString p_stderr = p.readAllStandardError();

         if(!p_stderr.isEmpty()){
             QMessageBox msgBox1;
             msgBox1.setWindowTitle("Python error");
             msgBox1.setText(p_stderr);
             msgBox1.exec();
         }
         else{
         QMessageBox msgBox1;
         msgBox1.setWindowTitle("Python Result");
         msgBox1.setText(p_stderr);
         msgBox1.exec();}
         }
         else{
             _pathOut = QFileDialog::getSaveFileName(this, "Save Image", "",
                                                            "BMP(*.bmp);;PNG(*.png);;JPEG(*.jpg *.jpeg);;All Files(*.*) ");
             std::ofstream model ("unet.py");
             model<<"from keras.models import load_model\n";
             model<<"from keras import models\n";
             model<<"import numpy as np\n";
             model<<"import cv2\n";
             model<<"import matplotlib.pyplot as plt\n";
             model<<"import skimage.io as io\n";
             model<<"INPUT_SHAPE = 512\n";
             model<<"OUTPUT_SHAPE = 512 \n";
             model<<"model = load_model('";
             model<<typeModel.toStdString();
             model<<"',compile= True)\n";
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
             model<<_pathIn.toStdString();
             model<<"','";
             model<<_pathOut.toStdString();
             model<<"')";
             model.close();
             QString pythonPath = ui->pythonProgram->toPlainText();
             QString pythonScript ="unet.py";

             params << pythonScript;
             p.start(pythonPath, params);
             QMessageBox msgBox;
             msgBox.setWindowTitle("Python info");
             msgBox.setText("Python Running...");
             msgBox.exec();
             p.waitForFinished(-1);
             QString p_stdout = p.readAll();
             QString p_stderr = p.readAllStandardError();

             if(!p_stderr.isEmpty()){
                 QMessageBox msgBox1;
                 msgBox1.setWindowTitle("Python error");
                 msgBox1.setText(p_stderr);
                 msgBox1.exec();
             }
             else{
             QMessageBox msgBox1;
             msgBox1.setWindowTitle("Python Result");
             msgBox1.setText(p_stderr);
             msgBox1.exec();}
             }

}
*/
void Segmentation::runLoad(){
    QMessageBox msgBox;


    switch(typeCNN){
    case Unet:{
        if((scriptTest)&&(scriptTrain)&&(!pretrained)){
            QDir pathDirImage(ui->imageRaw->toPlainText());
            QDir pathDirMask(ui->mask->toPlainText());
            //QFileInfo pathDirModel(ui->modelTra->toPlainText());
            QFileInfo pathDirOut(ui->imageOut->toPlainText());
            textScript.clear();
            if(ui->imageRaw->toPlainText().isEmpty()){
                 msgBox.setText("Can you add path of images tranining please?");
                 msgBox.exec();
            }
            else if(ui->mask->toPlainText().isEmpty()){
                msgBox.setText("Can you add path of masks tranining please?");
                msgBox.exec();
            }
   /*         else if(ui->modelCheck->toPlainText().isEmpty()){
                msgBox.setText("Can you add path of model check point please?");
                msgBox.exec();
            }*/
            else if(ui->imageTest->toPlainText().isEmpty()){
                msgBox.setText("Can you add path of image test please?");
                msgBox.exec();
            }
            else if(ui->imageOut->toPlainText().isEmpty()){
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
               /*  else if (!pathDirModel.exists()){
                     msgBox.setText("Path of  model check point is not exist!");
                     msgBox.exec();
                 }*/
                 else {
                     //QString path = QFileDialog::getSaveFileName(this, "Save Image", "",
                          //                                                  "Python(*.py) ");

                     textScript+="from keras.models import load_model\n";
                     textScript+="from keras import models\n";
                     textScript+="import numpy as np\n";
                     textScript+="import glob2\n";
                     textScript+="import glob\n";
                     textScript+="import tensorflow as tf\n";
                     textScript+="import cv2\n";
                     textScript+="from tensorflow.keras.optimizers import Adam\n";
                     textScript+="from tensorflow.keras.callbacks import EarlyStopping\n";
                     textScript+="from sklearn.model_selection import train_test_split\n";
                     textScript+="import matplotlib.pyplot as plt\n";
                     textScript+="import matplotlib.pyplot as plt\n";
                     textScript+="import skimage.io as io\n";
                     textScript+="INPUT_SHAPE = 512\n";
                     textScript+="OUTPUT_SHAPE = 512 \n";
                     textScript+="from keras.callbacks import ModelCheckpoint\n";
                     textScript+="from keras.models import *\n";
                     textScript+="from keras.layers import *\n";
                     textScript+="from keras.optimizers import *\n";
                     textScript+="from keras.callbacks import ModelCheckpoint, LearningRateScheduler\n";
                     textScript+="from keras import backend as keras\n";
                     textScript+="from keras.utils.np_utils import to_categorical\n";
                     textScript+="INPUT_SHAPE = 512\n";
                     textScript+="OUTPUT_SHAPE = 512 \n";
                     textScript+="image_paths = glob2.glob('";
                     textScript+=ui->imageRaw->toPlainText();
                     textScript+="/*')\n";
                     textScript+="label_paths = glob2.glob('";
                     textScript+=ui->mask->toPlainText();
                     textScript+="/*')\n";
                     textScript+="train_img_paths, val_img_paths, train_label_paths, val_label_paths = train_test_split(image_paths, label_paths, test_size = 0.2)\n";
                     textScript+="def _image_read_paths(train_img_paths, train_label_paths):\n";
                     textScript+="\tX, Y = [], []\n";
                     textScript+="\tfor image_path, label_path in zip(train_img_paths, train_label_paths):\n";
                     textScript+="\t\timage = cv2.imread(image_path)\n";
                     textScript+="\t\timage_resize = cv2.resize(image, (INPUT_SHAPE, INPUT_SHAPE), cv2.INTER_LINEAR)\n";
                     textScript+="\t\tlabel = cv2.imread(label_path)\n";
                     textScript+="\t\tlabel_gray = cv2.cvtColor(label, cv2.COLOR_BGR2GRAY)\n";
                     textScript+="\t\tlabel_resize = cv2.resize(label_gray, (OUTPUT_SHAPE, OUTPUT_SHAPE), cv2.INTER_LINEAR)\n";
                     textScript+="\t\tlabel_binary = np.array(label_resize == 255).astype('float32')\n";
                     textScript+="\t\tlabel_binary = label_binary[..., np.newaxis]\n";
                     textScript+="\t\tX.append(image_resize)\n";
                     textScript+="\t\tY.append(label_binary)\n";
                     textScript+="\tX = np.stack(X)\n";
                     textScript+="\tY = np.stack(Y)\n";
                     textScript+="\treturn X, Y\n";
                     textScript+="X_train, Y_train = _image_read_paths(train_img_paths, train_label_paths)\n";
                     textScript+="X_val, Y_val = _image_read_paths(val_img_paths, val_label_paths)\n";
                     textScript+="def _downsample_cnn_block(block_input, channel):\n";
                     textScript+="\tmaxpool = MaxPooling2D(pool_size=(2,2))(block_input)\n";
                     textScript+="\tconv1 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(maxpool)\n";
                     textScript+="\tconv2 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(conv1)\n";
                     textScript+="\treturn [maxpool, conv1, conv2]\n";
                     textScript+="def _upsample_cnn_block(block_input, block_counterpart, channel):\n";
                     textScript+="\tup = Conv2D(filters=channel, kernel_size=2, strides=1, padding='same')(UpSampling2D(size=(2,2))(block_input))\n";
                     textScript+="\tconcat = Concatenate(axis=-1)([block_counterpart, up])\n";
                     textScript+="\tconv1 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(concat)\n";
                     textScript+="\tconv2 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(conv1)\n";
                     textScript+="\treturn [up, concat, conv1, conv2]\n";
                     textScript+="def unet(pretrained_weights = None):\n";
                     textScript+="\tinput = Input(shape=(INPUT_SHAPE, INPUT_SHAPE, 3))\n";
                     textScript+="\tconv1_ds_block1 = Conv2D(filters=64,kernel_size=3, strides=1, padding='same')(input)\n";
                     textScript+="\tconv2_ds_block1 = Conv2D(filters=64,kernel_size=3, strides=1, padding='same')(conv1_ds_block1)\n";
                     textScript+="\tds_block2 = _downsample_cnn_block(conv2_ds_block1, channel=128)\n";
                     textScript+="\tds_block3 = _downsample_cnn_block(ds_block2[-1], channel=256)\n";
                     textScript+="\tds_block4 = _downsample_cnn_block(ds_block3[-1], channel=512)\n";
                     textScript+="\tdrop_ds_block4 = Dropout(0.5)(ds_block4[-1])\n";
                     textScript+="\tds_block5 = _downsample_cnn_block(drop_ds_block4, channel=1024)\n";
                     textScript+="\tdrop_ds_block5 = Dropout(0.5)(ds_block5[-1])\n";
                     textScript+="\tus_block4 = _upsample_cnn_block(drop_ds_block5, drop_ds_block4, channel=512)\n";
                     textScript+="\tus_block3 = _upsample_cnn_block(us_block4[-1], ds_block3[-1], channel=256)\n";
                     textScript+="\tus_block2 = _upsample_cnn_block(us_block3[-1], ds_block2[-1], channel=128)\n";
                     textScript+="\tus_block1 = _upsample_cnn_block(us_block2[-1], conv2_ds_block1, channel=64)\n";
                     textScript+="\tconv3_us_block1 = Conv2D(filters=2,kernel_size=3, strides=1, padding='same')(us_block1[-1])\n";
                     textScript+="\tconv4_us_block1 = Conv2D(filters=1, kernel_size=1, strides=1, padding='same', activation='sigmoid')(conv3_us_block1)\n";
                     textScript+="\tmodel = tf.keras.models.Model(inputs = input , outputs = conv4_us_block1)\n";
                     textScript+="\tmodel.compile(optimizer=Adam(lr = 1e-4), loss='binary_crossentropy', metrics='accuracy')\n";
                     textScript+="\tif(pretrained_weights):\n";
                     textScript+="\t\tmodel.load_weights(pretrained_weights)\n";
                     textScript+="\treturn model\n";
                     textScript+="model2 = unet()\n";
                     textScript+="model_checkpoint = ModelCheckpoint('";
                     //textScript+=ui->modelTrained->toPlainText();
                     textScript+="UNET_b_160_IOU.h5";
                     textScript+="',monitor = 'loss', verbose=1,save_best_only = True)\n";
                     textScript+="model2.fit(X_train, Y_train,validation_data = (X_val, Y_val),batch_size = 8, epochs = 1 ,callbacks = [model_checkpoint])\n";
                     textScript+="def _predict_path(path,path_out, figsize = (16, 8)):\n";
                     textScript+="\timg_tmp = cv2.imread(path)\n";
                     textScript+="\th, w, _ = img_tmp.shape\n";
                     textScript+="\timg = cv2.resize(img_tmp, (512, 512), cv2.INTER_LINEAR)\n";
                     textScript+="\timg_expand = img[np.newaxis, ...]\n";
                     textScript+="\timg_pred = model2.predict(img_expand).reshape(512, 512)\n";
                     textScript+="\timg_pred[img_pred < 0.5] = 0\n";
                     textScript+="\timg_pred[img_pred >= 0.5] = 1\n";
                     textScript+="\timg_pred = cv2.resize(img_pred,(w,h))\n";
                     textScript+="\tio.imsave(path_out,img_pred)\n";
                     for(int i =0;i<_pathTest.size();i++){
                     textScript+="_predict_path('";
                     textScript+=_pathTest.at(i);
                     textScript+="','";
                     textScript+=ui->imageOut->toPlainText();
                     textScript+="/";
                      QFileInfo pathDirTest(_pathTest.at(i));
                     textScript+=pathDirTest.fileName();
                     textScript+="')";
                     }
                     msgBox.setText("Your File Script is Successly!!");
                     msgBox.exec();
                     ui->textEdit->setText(textScript);
                     ui->buttonSave->setEnabled(true);
                 }

            }
        }
        if((scriptTest)&&(scriptTrain)&&(pretrained)){
            QDir pathDirImage(ui->imageRaw->toPlainText());
            QDir pathDirMask(ui->mask->toPlainText());
            //QFileInfo pathDirModel(ui->modelTra->toPlainText());
            QFileInfo pathDirOut(ui->imageOut->toPlainText());
            textScript.clear();
            if(ui->imageRaw->toPlainText().isEmpty()){
                 msgBox.setText("Can you add path of images tranining please?");
                 msgBox.exec();
            }
            else if(ui->mask->toPlainText().isEmpty()){
                msgBox.setText("Can you add path of masks tranining please?");
                msgBox.exec();
            }
          else if(ui->modelCheck->toPlainText().isEmpty()){
                msgBox.setText("Can you add path of model check point please?");
                msgBox.exec();
            }
            else if(ui->imageTest->toPlainText().isEmpty()){
                msgBox.setText("Can you add path of image test please?");
                msgBox.exec();
            }
            else if(ui->imageOut->toPlainText().isEmpty()){
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
               /*  else if (!pathDirModel.exists()){
                     msgBox.setText("Path of  model check point is not exist!");
                     msgBox.exec();
                 }*/
                 else {
                     //QString path = QFileDialog::getSaveFileName(this, "Save Image", "",
                          //                                                  "Python(*.py) ");

                     textScript+="from keras.models import load_model\n";
                     textScript+="from keras import models\n";
                     textScript+="import numpy as np\n";
                     textScript+="import glob2\n";
                     textScript+="import glob\n";
                     textScript+="import tensorflow as tf\n";
                     textScript+="import cv2\n";
                     textScript+="from tensorflow.keras.optimizers import Adam\n";
                     textScript+="from tensorflow.keras.callbacks import EarlyStopping\n";
                     textScript+="from sklearn.model_selection import train_test_split\n";
                     textScript+="import matplotlib.pyplot as plt\n";
                     textScript+="import matplotlib.pyplot as plt\n";
                     textScript+="import skimage.io as io\n";
                     textScript+="INPUT_SHAPE = 512\n";
                     textScript+="OUTPUT_SHAPE = 512 \n";
                     textScript+="from keras.callbacks import ModelCheckpoint\n";
                     textScript+="from keras.models import *\n";
                     textScript+="from keras.layers import *\n";
                     textScript+="from keras.optimizers import *\n";
                     textScript+="from keras.callbacks import ModelCheckpoint, LearningRateScheduler\n";
                     textScript+="from keras import backend as keras\n";
                     textScript+="from keras.utils.np_utils import to_categorical\n";
                     textScript+="INPUT_SHAPE = 512\n";
                     textScript+="OUTPUT_SHAPE = 512 \n";
                     textScript+="image_paths = glob2.glob('";
                     textScript+=ui->imageRaw->toPlainText();
                     textScript+="/*')\n";
                     textScript+="label_paths = glob2.glob('";
                     textScript+=ui->mask->toPlainText();
                     textScript+="/*')\n";
                     textScript+="train_img_paths, val_img_paths, train_label_paths, val_label_paths = train_test_split(image_paths, label_paths, test_size = 0.2)\n";
                     textScript+="def _image_read_paths(train_img_paths, train_label_paths):\n";
                     textScript+="\tX, Y = [], []\n";
                     textScript+="\tfor image_path, label_path in zip(train_img_paths, train_label_paths):\n";
                     textScript+="\t\timage = cv2.imread(image_path)\n";
                     textScript+="\t\timage_resize = cv2.resize(image, (INPUT_SHAPE, INPUT_SHAPE), cv2.INTER_LINEAR)\n";
                     textScript+="\t\tlabel = cv2.imread(label_path)\n";
                     textScript+="\t\tlabel_gray = cv2.cvtColor(label, cv2.COLOR_BGR2GRAY)\n";
                     textScript+="\t\tlabel_resize = cv2.resize(label_gray, (OUTPUT_SHAPE, OUTPUT_SHAPE), cv2.INTER_LINEAR)\n";
                     textScript+="\t\tlabel_binary = np.array(label_resize == 255).astype('float32')\n";
                     textScript+="\t\tlabel_binary = label_binary[..., np.newaxis]\n";
                     textScript+="\t\tX.append(image_resize)\n";
                     textScript+="\t\tY.append(label_binary)\n";
                     textScript+="\tX = np.stack(X)\n";
                     textScript+="\tY = np.stack(Y)\n";
                     textScript+="\treturn X, Y\n";
                     textScript+="X_train, Y_train = _image_read_paths(train_img_paths, train_label_paths)\n";
                     textScript+="X_val, Y_val = _image_read_paths(val_img_paths, val_label_paths)\n";
                     textScript+="def _downsample_cnn_block(block_input, channel):\n";
                     textScript+="\tmaxpool = MaxPooling2D(pool_size=(2,2))(block_input)\n";
                     textScript+="\tconv1 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(maxpool)\n";
                     textScript+="\tconv2 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(conv1)\n";
                     textScript+="\treturn [maxpool, conv1, conv2]\n";
                     textScript+="def _upsample_cnn_block(block_input, block_counterpart, channel):\n";
                     textScript+="\tup = Conv2D(filters=channel, kernel_size=2, strides=1, padding='same')(UpSampling2D(size=(2,2))(block_input))\n";
                     textScript+="\tconcat = Concatenate(axis=-1)([block_counterpart, up])\n";
                     textScript+="\tconv1 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(concat)\n";
                     textScript+="\tconv2 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(conv1)\n";
                     textScript+="\treturn [up, concat, conv1, conv2]\n";
                     textScript+="def unet(pretrained_weights = None):\n";
                     textScript+="\tinput = Input(shape=(INPUT_SHAPE, INPUT_SHAPE, 3))\n";
                     textScript+="\tconv1_ds_block1 = Conv2D(filters=64,kernel_size=3, strides=1, padding='same')(input)\n";
                     textScript+="\tconv2_ds_block1 = Conv2D(filters=64,kernel_size=3, strides=1, padding='same')(conv1_ds_block1)\n";
                     textScript+="\tds_block2 = _downsample_cnn_block(conv2_ds_block1, channel=128)\n";
                     textScript+="\tds_block3 = _downsample_cnn_block(ds_block2[-1], channel=256)\n";
                     textScript+="\tds_block4 = _downsample_cnn_block(ds_block3[-1], channel=512)\n";
                     textScript+="\tdrop_ds_block4 = Dropout(0.5)(ds_block4[-1])\n";
                     textScript+="\tds_block5 = _downsample_cnn_block(drop_ds_block4, channel=1024)\n";
                     textScript+="\tdrop_ds_block5 = Dropout(0.5)(ds_block5[-1])\n";
                     textScript+="\tus_block4 = _upsample_cnn_block(drop_ds_block5, drop_ds_block4, channel=512)\n";
                     textScript+="\tus_block3 = _upsample_cnn_block(us_block4[-1], ds_block3[-1], channel=256)\n";
                     textScript+="\tus_block2 = _upsample_cnn_block(us_block3[-1], ds_block2[-1], channel=128)\n";
                     textScript+="\tus_block1 = _upsample_cnn_block(us_block2[-1], conv2_ds_block1, channel=64)\n";
                     textScript+="\tconv3_us_block1 = Conv2D(filters=2,kernel_size=3, strides=1, padding='same')(us_block1[-1])\n";
                     textScript+="\tconv4_us_block1 = Conv2D(filters=1, kernel_size=1, strides=1, padding='same', activation='sigmoid')(conv3_us_block1)\n";
                     textScript+="\tmodel = tf.keras.models.Model(inputs = input , outputs = conv4_us_block1)\n";
                     textScript+="\tmodel.compile(optimizer=Adam(lr = 1e-4), loss='binary_crossentropy', metrics='accuracy')\n";
                     textScript+="\tif(pretrained_weights):\n";
                     textScript+="\t\tmodel.load_weights(pretrained_weights)\n";
                     textScript+="\treturn model\n";
                     textScript+="model2 = unet('";
                     textScript+=ui->modelCheck->toPlainText();
                     textScript+="')\n";
                   //  textScript+="model_checkpoint = ModelCheckpoint('";
                  //   textScript+=ui->modelCheck->toPlainText();
                    // textScript+="UNET_b_160_IOU.h5";
                    // textScript+="',monitor = 'loss', verbose=1,save_best_only = True)\n";
                    // textScript+="model2.fit(X_train, Y_train,validation_data = (X_val, Y_val),batch_size = 8, epochs = 1 ,callbacks = [model_checkpoint])\n";
                     textScript+="def _predict_path(path,path_out, figsize = (16, 8)):\n";
                     textScript+="\timg_tmp = cv2.imread(path)\n";
                     textScript+="\th, w, _ = img_tmp.shape\n";
                     textScript+="\timg = cv2.resize(img_tmp, (512, 512), cv2.INTER_LINEAR)\n";
                     textScript+="\timg_expand = img[np.newaxis, ...]\n";
                     textScript+="\timg_pred = model2.predict(img_expand).reshape(512, 512)\n";
                     textScript+="\timg_pred[img_pred < 0.5] = 0\n";
                     textScript+="\timg_pred[img_pred >= 0.5] = 1\n";
                     textScript+="\timg_pred = cv2.resize(img_pred,(w,h))\n";
                     textScript+="\tio.imsave(path_out,img_pred)\n";
                     for(int i =0;i<_pathTest.size();i++){
                     textScript+="_predict_path('";
                     textScript+=_pathTest.at(i);
                     textScript+="','";
                     textScript+=ui->imageOut->toPlainText();
                     textScript+="/";
                      QFileInfo pathDirTest(_pathTest.at(i));
                     textScript+=pathDirTest.fileName();
                     if(i==_pathTest.size()-1)
                       textScript+="')";
                     else
                         textScript+="')\n";
                     }
                    msgBox.setText("Your File Script is Successly!!");
                    msgBox.exec();
                     ui->textEdit->setText(textScript);
                     ui->buttonSave->setEnabled(true);
                 }

            }
        }
        else if((scriptTrain)&&(!scriptTest)&&(!pretrained)){
            QDir pathDirImage(ui->imageRaw->toPlainText());
            QDir pathDirMask(ui->mask->toPlainText());
            QFileInfo pathDirModel(ui->modelCheck->toPlainText());
                textScript.clear();
                if(ui->imageRaw->toPlainText().isEmpty()){
                     msgBox.setText("Can you add path of images tranining please?");
                     msgBox.exec();
                }
                else if(ui->mask->toPlainText().isEmpty()){
                    msgBox.setText("Can you add path of masks tranining please?");
                    msgBox.exec();
                }
            /*    else if(ui->modelCheck->toPlainText().isEmpty()){
                    msgBox.setText("Can you add path of model check point please?");
                    msgBox.exec();
                }*/
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
                  /*   else if (!pathDirModel.exists()){
                         msgBox.setText("Path of  model check point is not exist!");
                         msgBox.exec();
                     }*/
                     else {
                         //QString path = QFileDialog::getSaveFileName(this, "Save Image", "",
                              //                                                  "Python(*.py) ");

                         textScript+="from keras.models import load_model\n";
                         textScript+="from keras import models\n";
                         textScript+="import numpy as np\n";
                         textScript+="import glob2\n";
                         textScript+="import glob\n";
                         textScript+="import tensorflow as tf\n";
                         textScript+="import cv2\n";
                         textScript+="from tensorflow.keras.optimizers import Adam\n";
                         textScript+="from tensorflow.keras.callbacks import EarlyStopping\n";
                         textScript+="from sklearn.model_selection import train_test_split\n";
                         textScript+="import matplotlib.pyplot as plt\n";
                         textScript+="from keras.callbacks import ModelCheckpoint\n";
                         textScript+="from keras.models import *\n";
                         textScript+="from keras.layers import *\n";
                         textScript+="from keras.optimizers import *\n";
                         textScript+="from keras.callbacks import ModelCheckpoint, LearningRateScheduler\n";
                         textScript+="from keras import backend as keras\n";
                         textScript+="from keras.utils.np_utils import to_categorical\n";
                         textScript+="INPUT_SHAPE = 512\n";
                         textScript+="OUTPUT_SHAPE = 512 \n";
                         textScript+="image_paths = glob2.glob('";
                         textScript+=ui->imageRaw->toPlainText();
                         textScript+="/*')\n";
                         textScript+="label_paths = glob2.glob('";
                         textScript+=ui->mask->toPlainText();
                         textScript+="/*')\n";
                         textScript+="train_img_paths, val_img_paths, train_label_paths, val_label_paths = train_test_split(image_paths, label_paths, test_size = 0.2)\n";
                         textScript+="def _image_read_paths(train_img_paths, train_label_paths):\n";
                         textScript+="\tX, Y = [], []\n";
                         textScript+="\tfor image_path, label_path in zip(train_img_paths, train_label_paths):\n";
                         textScript+="\t\timage = cv2.imread(image_path)\n";
                         textScript+="\t\timage_resize = cv2.resize(image, (INPUT_SHAPE, INPUT_SHAPE), cv2.INTER_LINEAR)\n";
                         textScript+="\t\tlabel = cv2.imread(label_path)\n";
                         textScript+="\t\tlabel_gray = cv2.cvtColor(label, cv2.COLOR_BGR2GRAY)\n";
                         textScript+="\t\tlabel_resize = cv2.resize(label_gray, (OUTPUT_SHAPE, OUTPUT_SHAPE), cv2.INTER_LINEAR)\n";
                         textScript+="\t\tlabel_binary = np.array(label_resize == 255).astype('float32')\n";
                         textScript+="\t\tlabel_binary = label_binary[..., np.newaxis]\n";
                         textScript+="\t\tX.append(image_resize)\n";
                         textScript+="\t\tY.append(label_binary)\n";
                         textScript+="\tX = np.stack(X)\n";
                         textScript+="\tY = np.stack(Y)\n";
                         textScript+="\treturn X, Y\n";
                         textScript+="X_train, Y_train = _image_read_paths(train_img_paths, train_label_paths)\n";
                         textScript+="X_val, Y_val = _image_read_paths(val_img_paths, val_label_paths)\n";
                         textScript+="def _downsample_cnn_block(block_input, channel):\n";
                         textScript+="\tmaxpool = MaxPooling2D(pool_size=(2,2))(block_input)\n";
                         textScript+="\tconv1 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(maxpool)\n";
                         textScript+="\tconv2 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(conv1)\n";
                         textScript+="\treturn [maxpool, conv1, conv2]\n";
                         textScript+="def _upsample_cnn_block(block_input, block_counterpart, channel):\n";
                         textScript+="\tup = Conv2D(filters=channel, kernel_size=2, strides=1, padding='same')(UpSampling2D(size=(2,2))(block_input))\n";
                         textScript+="\tconcat = Concatenate(axis=-1)([block_counterpart, up])\n";
                         textScript+="\tconv1 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(concat)\n";
                         textScript+="\tconv2 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(conv1)\n";
                         textScript+="\treturn [up, concat, conv1, conv2]\n";
                         textScript+="def unet(pretrained_weights = None):\n";
                         textScript+="\tinput = Input(shape=(INPUT_SHAPE, INPUT_SHAPE, 3))\n";
                         textScript+="\tconv1_ds_block1 = Conv2D(filters=64,kernel_size=3, strides=1, padding='same')(input)\n";
                         textScript+="\tconv2_ds_block1 = Conv2D(filters=64,kernel_size=3, strides=1, padding='same')(conv1_ds_block1)\n";
                         textScript+="\tds_block2 = _downsample_cnn_block(conv2_ds_block1, channel=128)\n";
                         textScript+="\tds_block3 = _downsample_cnn_block(ds_block2[-1], channel=256)\n";
                         textScript+="\tds_block4 = _downsample_cnn_block(ds_block3[-1], channel=512)\n";
                         textScript+="\tdrop_ds_block4 = Dropout(0.5)(ds_block4[-1])\n";
                         textScript+="\tds_block5 = _downsample_cnn_block(drop_ds_block4, channel=1024)\n";
                         textScript+="\tdrop_ds_block5 = Dropout(0.5)(ds_block5[-1])\n";
                         textScript+="\tus_block4 = _upsample_cnn_block(drop_ds_block5, drop_ds_block4, channel=512)\n";
                         textScript+="\tus_block3 = _upsample_cnn_block(us_block4[-1], ds_block3[-1], channel=256)\n";
                         textScript+="\tus_block2 = _upsample_cnn_block(us_block3[-1], ds_block2[-1], channel=128)\n";
                         textScript+="\tus_block1 = _upsample_cnn_block(us_block2[-1], conv2_ds_block1, channel=64)\n";
                         textScript+="\tconv3_us_block1 = Conv2D(filters=2,kernel_size=3, strides=1, padding='same')(us_block1[-1])\n";
                         textScript+="\tconv4_us_block1 = Conv2D(filters=1, kernel_size=1, strides=1, padding='same', activation='sigmoid')(conv3_us_block1)\n";
                         textScript+="\tmodel = tf.keras.models.Model(inputs = input , outputs = conv4_us_block1)\n";
                         textScript+="\tmodel.compile(optimizer=Adam(lr = 1e-4), loss='binary_crossentropy', metrics='accuracy')\n";
                         textScript+="\tif(pretrained_weights):\n";
                         textScript+="\t\tmodel.load_weights(pretrained_weights)\n";
                         textScript+="\treturn model\n";
                         textScript+="model2 = unet()\n";
                         textScript+="model_checkpoint = ModelCheckpoint('";
                       //  textScript+=ui->modelCheck->toPlainText();
                         textScript+="UNET_b_160_IOU.h5";
                         textScript+="',monitor = 'loss', verbose=1,save_best_only = True)\n";
                         textScript+="model2.fit(X_train, Y_train,validation_data = (X_val, Y_val),batch_size = 8, epochs = 1 ,callbacks = [model_checkpoint])\n";
                         msgBox.setText("Your File Script is Successly!!");
                         msgBox.exec();
                         ui->textEdit->setText(textScript);
                         ui->buttonSave->setEnabled(true);
        }

    }
}
        if((!scriptTest)&&(scriptTrain)&&(pretrained)){
            QDir pathDirImage(ui->imageRaw->toPlainText());
            QDir pathDirMask(ui->mask->toPlainText());
            //QFileInfo pathDirModel(ui->modelTra->toPlainText());
            QFileInfo pathDirOut(ui->imageOut->toPlainText());
            textScript.clear();
            if(ui->imageRaw->toPlainText().isEmpty()){
                 msgBox.setText("Can you add path of images tranining please?");
                 msgBox.exec();
            }
            else if(ui->mask->toPlainText().isEmpty()){
                msgBox.setText("Can you add path of masks tranining please?");
                msgBox.exec();
            }
          else if(ui->modelCheck->toPlainText().isEmpty()){
                msgBox.setText("Can you add path of model check point please?");
                msgBox.exec();
            }
         /*   else if(ui->imageTest->toPlainText().isEmpty()){
                msgBox.setText("Can you add path of image test please?");
                msgBox.exec();
            }
            else if(ui->imageOut->toPlainText().isEmpty()){
                msgBox.setText("Can you add path of image segementation please?");\
                msgBox.exec();
            }*/
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
               /*  else if (!pathDirModel.exists()){
                     msgBox.setText("Path of  model check point is not exist!");
                     msgBox.exec();
                 }*/
                 else {
                     //QString path = QFileDialog::getSaveFileName(this, "Save Image", "",
                          //                                                  "Python(*.py) ");

                     textScript+="from keras.models import load_model\n";
                     textScript+="from keras import models\n";
                     textScript+="import numpy as np\n";
                     textScript+="import glob2\n";
                     textScript+="import glob\n";
                     textScript+="import tensorflow as tf\n";
                     textScript+="import cv2\n";
                     textScript+="from tensorflow.keras.optimizers import Adam\n";
                     textScript+="from tensorflow.keras.callbacks import EarlyStopping\n";
                     textScript+="from sklearn.model_selection import train_test_split\n";
                     textScript+="import matplotlib.pyplot as plt\n";
                     textScript+="from keras.callbacks import ModelCheckpoint\n";
                     textScript+="from keras.models import *\n";
                     textScript+="from keras.layers import *\n";
                     textScript+="from keras.optimizers import *\n";
                     textScript+="from keras.callbacks import ModelCheckpoint, LearningRateScheduler\n";
                     textScript+="from keras import backend as keras\n";
                     textScript+="from keras.utils.np_utils import to_categorical\n";
                     textScript+="INPUT_SHAPE = 512\n";
                     textScript+="OUTPUT_SHAPE = 512 \n";
                     textScript+="image_paths = glob2.glob('";
                     textScript+=ui->imageRaw->toPlainText();
                     textScript+="/*')\n";
                     textScript+="label_paths = glob2.glob('";
                     textScript+=ui->mask->toPlainText();
                     textScript+="/*')\n";
                     textScript+="train_img_paths, val_img_paths, train_label_paths, val_label_paths = train_test_split(image_paths, label_paths, test_size = 0.2)\n";
                     textScript+="def _image_read_paths(train_img_paths, train_label_paths):\n";
                     textScript+="\tX, Y = [], []\n";
                     textScript+="\tfor image_path, label_path in zip(train_img_paths, train_label_paths):\n";
                     textScript+="\t\timage = cv2.imread(image_path)\n";
                     textScript+="\t\timage_resize = cv2.resize(image, (INPUT_SHAPE, INPUT_SHAPE), cv2.INTER_LINEAR)\n";
                     textScript+="\t\tlabel = cv2.imread(label_path)\n";
                     textScript+="\t\tlabel_gray = cv2.cvtColor(label, cv2.COLOR_BGR2GRAY)\n";
                     textScript+="\t\tlabel_resize = cv2.resize(label_gray, (OUTPUT_SHAPE, OUTPUT_SHAPE), cv2.INTER_LINEAR)\n";
                     textScript+="\t\tlabel_binary = np.array(label_resize == 255).astype('float32')\n";
                     textScript+="\t\tlabel_binary = label_binary[..., np.newaxis]\n";
                     textScript+="\t\tX.append(image_resize)\n";
                     textScript+="\t\tY.append(label_binary)\n";
                     textScript+="\tX = np.stack(X)\n";
                     textScript+="\tY = np.stack(Y)\n";
                     textScript+="\treturn X, Y\n";
                     textScript+="X_train, Y_train = _image_read_paths(train_img_paths, train_label_paths)\n";
                     textScript+="X_val, Y_val = _image_read_paths(val_img_paths, val_label_paths)\n";
                     textScript+="def _downsample_cnn_block(block_input, channel):\n";
                     textScript+="\tmaxpool = MaxPooling2D(pool_size=(2,2))(block_input)\n";
                     textScript+="\tconv1 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(maxpool)\n";
                     textScript+="\tconv2 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(conv1)\n";
                     textScript+="\treturn [maxpool, conv1, conv2]\n";
                     textScript+="def _upsample_cnn_block(block_input, block_counterpart, channel):\n";
                     textScript+="\tup = Conv2D(filters=channel, kernel_size=2, strides=1, padding='same')(UpSampling2D(size=(2,2))(block_input))\n";
                     textScript+="\tconcat = Concatenate(axis=-1)([block_counterpart, up])\n";
                     textScript+="\tconv1 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(concat)\n";
                     textScript+="\tconv2 = Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(conv1)\n";
                     textScript+="\treturn [up, concat, conv1, conv2]\n";
                     textScript+="def unet(pretrained_weights = None):\n";
                     textScript+="\tinput = Input(shape=(INPUT_SHAPE, INPUT_SHAPE, 3))\n";
                     textScript+="\tconv1_ds_block1 = Conv2D(filters=64,kernel_size=3, strides=1, padding='same')(input)\n";
                     textScript+="\tconv2_ds_block1 = Conv2D(filters=64,kernel_size=3, strides=1, padding='same')(conv1_ds_block1)\n";
                     textScript+="\tds_block2 = _downsample_cnn_block(conv2_ds_block1, channel=128)\n";
                     textScript+="\tds_block3 = _downsample_cnn_block(ds_block2[-1], channel=256)\n";
                     textScript+="\tds_block4 = _downsample_cnn_block(ds_block3[-1], channel=512)\n";
                     textScript+="\tdrop_ds_block4 = Dropout(0.5)(ds_block4[-1])\n";
                     textScript+="\tds_block5 = _downsample_cnn_block(drop_ds_block4, channel=1024)\n";
                     textScript+="\tdrop_ds_block5 = Dropout(0.5)(ds_block5[-1])\n";
                     textScript+="\tus_block4 = _upsample_cnn_block(drop_ds_block5, drop_ds_block4, channel=512)\n";
                     textScript+="\tus_block3 = _upsample_cnn_block(us_block4[-1], ds_block3[-1], channel=256)\n";
                     textScript+="\tus_block2 = _upsample_cnn_block(us_block3[-1], ds_block2[-1], channel=128)\n";
                     textScript+="\tus_block1 = _upsample_cnn_block(us_block2[-1], conv2_ds_block1, channel=64)\n";
                     textScript+="\tconv3_us_block1 = Conv2D(filters=2,kernel_size=3, strides=1, padding='same')(us_block1[-1])\n";
                     textScript+="\tconv4_us_block1 = Conv2D(filters=1, kernel_size=1, strides=1, padding='same', activation='sigmoid')(conv3_us_block1)\n";
                     textScript+="\tmodel = tf.keras.models.Model(inputs = input , outputs = conv4_us_block1)\n";
                     textScript+="\tmodel.compile(optimizer=Adam(lr = 1e-4), loss='binary_crossentropy', metrics='accuracy')\n";
                     textScript+="\tif(pretrained_weights):\n";
                     textScript+="\t\tmodel.load_weights(pretrained_weights)\n";
                     textScript+="\treturn model\n";
                     textScript+="model2 = unet('";
                     textScript+=ui->modelCheck->toPlainText();
                     textScript+="')\n";
                     msgBox.setText("Your File Script is Successly!!");
                     msgBox.exec();
                     ui->textEdit->setText(textScript);
                     ui->buttonSave->setEnabled(true);
                 }

            }
        }
        else if((!scriptTrain)&&(scriptTest)){
            QFileInfo pathDirModel(ui->modelTrained->toPlainText());
         //  QFileInfo pathDirTest(ui->imageTest->toPlainText());
            QFileInfo pathDirOut(ui->imageOut->toPlainText());
            textScript.clear();
            if(ui->modelTrained->toPlainText().isEmpty()){
                msgBox.setText("Can you add path of model trained please?");
                msgBox.exec();
            }
            else if(ui->imageTest->toPlainText().isEmpty()){
                msgBox.setText("Can you add path of image test please?");
                msgBox.exec();
            }
            else if(ui->imageOut->toPlainText().isEmpty()){
                msgBox.setText("Can you add path for image segementation please?");\
                msgBox.exec();
            }
            else
            {

                 if (!pathDirModel.exists()){
                     msgBox.setText("Path of  model check point is not exist!");
                     msgBox.exec();
                 }
                 else {
                     //QString path = QFileDialog::getSaveFileName(this, "Save Image", "",
                          //                                                  "Python(*.py) ");

                     textScript+="from keras.models import load_model\n";
                     textScript+="from keras import models\n";
                     textScript+="import numpy as np\n";
                     textScript+="import cv2\n";
                     textScript+="import matplotlib.pyplot as plt\n";
                     textScript+="import skimage.io as io\n";
                     textScript+="INPUT_SHAPE = 512\n";
                     textScript+="OUTPUT_SHAPE = 512 \n";
                     textScript+="model = load_model('";
                     textScript+=ui->modelTrained->toPlainText();
                     textScript+="',compile= True)\n";
                     textScript+="model.load_weights('";
                     textScript+=ui->modelTrained->toPlainText();
                     textScript+="')\n";
                     textScript+="def _predict_path(path,path_out, figsize = (16, 8)):\n";
                     textScript+="\timg_tmp = cv2.imread(path)\n";
                     textScript+="\th, w, _ = img_tmp.shape\n";
                     textScript+="\timg = cv2.resize(img_tmp, (512, 512), cv2.INTER_LINEAR)\n";
                     textScript+="\timg_expand = img[np.newaxis, ...]\n";
                     textScript+="\timg_pred = model.predict(img_expand).reshape(512, 512)\n";
                     textScript+="\timg_pred[img_pred < 0.5] = 0\n";
                     textScript+="\timg_pred[img_pred >= 0.5] = 1\n";
                     textScript+="\timg_pred = cv2.resize(img_pred,(w,h))\n";
                     textScript+="\tio.imsave(path_out,img_pred)\n";
                    /* for(int i =0;i<_pathTest.size();i++){
                     textScript+="_predict_path('";
                     textScript+=ui->imageTest->toPlainText();
                     textScript+="','";
                     textScript+=ui->imageOut->toPlainText();
                     textScript+="/";
                     textScript+=pathDirTest.fileName();
                     textScript+="')";
                     }*/
                      for(int i =0;i<_pathTest.size();i++){
                      textScript+="_predict_path('";
                      textScript+=_pathTest.at(i);
                      textScript+="','";
                      textScript+=ui->imageOut->toPlainText();
                      textScript+="/";
                       QFileInfo pathDirTest(_pathTest.at(i));
                      textScript+=pathDirTest.fileName();
                      if(i==_pathTest.size()-1)
                        textScript+="')";
                      else
                          textScript+="')\n";
                      }
                     msgBox.setText("Your File Script is Successly!!");
                     msgBox.exec();
                     ui->textEdit->setText(textScript);
                     ui->buttonSave->setEnabled(true);
                 }

            }
        }
          else {
            msgBox.setText("Nothing to load");
            msgBox.exec();
        }
    }
        break;
    }
}
void Segmentation::runSave(){
    if(ui->textEdit->toPlainText().isEmpty()){
        QMessageBox msgBox4;
        msgBox4.setText("Nothing to save");
        msgBox4.exec();
    }
    else{

    textScript = ui->textEdit->toPlainText();
    QString path = QFileDialog::getSaveFileName(this, "Save Script Python", "",
                                               "Python(*.py) ");
    if(path == ""){
      QMessageBox msgBox4;
      msgBox4.setText("No directory");
      msgBox4.exec();
     }
     else{
       std::ofstream model (path.toStdString());
       model<<textScript.toStdString();
}
    }
}
void Segmentation::checkScriptTrain(){
    if(ui->scriptTraining->checkState()==Qt::Checked)
        scriptTrain = true;
    else if (ui->scriptTraining->checkState()==Qt::Unchecked)
        scriptTrain = false;
}
void Segmentation::checkScriptTest(){
    if(!scriptTrain){
        ui->modelTrained->setEnabled(true);
        ui->buttonModelTrained->setEnabled(true);
    }
    if(ui->scriptTest->checkState()==Qt::Checked)
        scriptTest = true;
    else if (ui->scriptTest->checkState()==Qt::Unchecked)
        scriptTest = false;
}
void Segmentation::pathListImageRaw(){
    dirListImageRaw = QFileDialog::getExistingDirectory(this, "Select Directory");
  //  QStringList nameList = dirListImageRaw.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP",QDir::Files);
    ui->imageRaw->setPlainText(dirListImageRaw.path());
}
void Segmentation::pathListMask(){
    dirListMask = QFileDialog::getExistingDirectory(this, "Select Directory");
  //  QStringList nameList = dirListMask.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP",QDir::Files);
    ui->mask->setPlainText(dirListMask.path());
}
void Segmentation::pathModelCheckPoint(){
    _pathModelCheck = QFileDialog::getOpenFileName(
                            this,
                            "Select one to open",
                            "",
                            " (*.h5)");
    if(_pathModelCheck!="")
        ui->modelCheck->setPlainText(_pathModelCheck);
}
void Segmentation::pathModelTrained(){
    _pathModelTrained = QFileDialog::getOpenFileName(
                            this,
                            "Select one to open",
                            "",
                            " (*.h5)");
    if(_pathModelTrained !="")
        ui->modelTrained ->setPlainText(_pathModelTrained );
}
void Segmentation::pathImageTest(){
    _pathTest = QFileDialog::getOpenFileNames(
                            this,
                            "Select one to open",
                            "",
                             "BMP(*.bmp);;PNG(*.png);;JPEG(*.jpg *.jpeg);;All Files(*.*) ");
    if(_pathTest.size()!=0){
      QString text ="";
      for(int i =0;i<_pathTest.size();i++){
          text+=_pathTest.at(i);
          text+="\n";
      }
        ui->imageTest->setPlainText(text);
    }
}
void Segmentation::pathImageOut(){
    dirPathOut = QFileDialog::getExistingDirectory(this, "Select Directory");
    ui->imageOut->setPlainText(dirPathOut.path());
}
const QString& Segmentation::getPathModelTrained() const{
    return _pathModelTrained;
}
void Segmentation::setPathModelTrained( const QString& pathModelTrained){
    _pathModelTrained = pathModelTrained;
}

const QString& Segmentation::getPathProgram() const{
    return _pathProg;
}
void Segmentation::setPathProgram( const QString& pathProgram){
    _pathProg = pathProgram;
}
void Segmentation::checkPreTrained(){
    if(ui->preTrained->checkState()==Qt::Checked){
        ui->modelCheck->setEnabled(true);
        ui->modelTrained->setEnabled(true);
        ui->buttonModelCheck->setEnabled(true);
        ui->buttonModelTrained->setEnabled(true);
        pretrained = true;
    }
    else if (ui->preTrained->checkState()==Qt::Unchecked){
        pretrained = false;
        if(!pretrained){
            ui->modelCheck->setEnabled(false);
            ui->modelTrained->setEnabled(false);
            ui->buttonModelCheck->setEnabled(false);
            ui->buttonModelTrained->setEnabled(false);
        }
    }
}
