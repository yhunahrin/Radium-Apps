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
                                                            Ra::Gui::Viewer *viewer,
                                                            QWidget *parent) :
                                QWidget( parent ),
                                ui( new Ui::LoadFolderWidget),
                                _ro( ro ),
                                _renderer( renderer ),
                                _pathRaw(pathRaw),
                                _pathUnet(pathUnet),
                                _paramProvider(paramProvider),
                                _viewer(viewer)
{
    ui->setupUi(this);
    ui->imageOrgine->setEnabled(false);
    ui->loadUnet->setEnabled(false);
    ui->textMethodeJson->setEnabled(false);
    ui->buttonRunUnet->setEnabled(false);
   // _camera = new CameraManipulator2D(*(_viewer->getCameraManipulator()),_ro);
    _camera = new CameraManipulator2D(*(_viewer->getCameraManipulator()));
    _viewer->setCameraManipulator(_camera);
    ui->groupCoordinate->setEnabled(false);
    ui->inversePixel->setEnabled(false);
    connect( ui->buttonLoad, &QPushButton::clicked, this, &LoadFolderWidget::enableLoadData );
    connect( ui->buttonShow, &QPushButton::clicked, this, &LoadFolderWidget::enableShowImage);
    connect( ui->buttonJson, &QPushButton::clicked, this, &LoadFolderWidget::enableSaveJson);
    connect( ui->inversePixel, &QPushButton::clicked, this, &LoadFolderWidget::enableInversePixel);
    connect( ui->checkUnet, &QPushButton::clicked, this, &LoadFolderWidget::checkUnet );
    connect( ui->checkCNN2, &QPushButton::clicked, this, &LoadFolderWidget::checkCNN2);
    connect( ui->checkCNN3,&QPushButton::clicked, this, &LoadFolderWidget::checkCNN3);
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
   // ui->loadUnet->setEnabled(true);
    ui->buttonRunImage->setEnabled(true);
    ui->groupCoordinate->setEnabled(false);
}

void LoadFolderWidget::enableShowImage()
{
    choisTypeRun = SHOWIMAGE;
    ui->imageOrgine->setEnabled(true);
   // ui->loadUnet->setEnabled(true);
     ui->buttonRunImage->setEnabled(true);
     ui->groupCoordinate->setEnabled(false);
     ui->inversePixel->setEnabled(false);
}

void LoadFolderWidget::enableSaveJson()
{
    choisTypeRun = SAVEJSON;
    ui->imageOrgine->setEnabled(true);
   // ui->loadUnet->setEnabled(true);
     ui->buttonRunImage->setEnabled(true);
     ui->groupCoordinate->setEnabled(false);
     ui->inversePixel->setEnabled(false);
}
void LoadFolderWidget::enableInversePixel()
{
    choisTypeRun = INVERSEPIXEL;
    ui->groupCoordinate->setEnabled(true);
    ui->buttonRunImage->setEnabled(false);
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
            ui->loadUnet->setEnabled(true);
            ui->boxListImageOrigine->clear();
            directoryRaw = QFileDialog::getExistingDirectory(this, "Select Directory");
            QStringList nameList = directoryRaw.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP",QDir::Files);
            std::string textDataset="{ \"datasetinfo\" :\n  {\n       \"name\" : \"";
            textDataset+=directoryRaw.dirName().toStdString();
            textDataset+="\",\n";
            textDataset+="       \"location\" : \"";
            textDataset+=directoryRaw.path().toStdString();
            textDataset+="\",\n";
            textDataset+="       \"contains\" : \"";
            textDataset+=std::to_string(nameList.size());
            textDataset+=" files\",\n";
            textDataset+="       \"images\" : [\n";
           /* int count=1;
            int size = nameList.size();
            while(size>=10)
            {
                size/=10;
                count++;
            }
            std::cout<<count<<std::endl;
            for (int i=0; i<nameList.size();i++)
            {
                std::string tmp_name = "img";
                int count2=1;
                int tmp,tmp2;
                tmp=i;
                while(tmp>=10){
                    count2++;
                }
                tmp2=count-count2;
                for(int j=0;j<tmp2;j++){
                    tmp_name+="0";
                }
                tmp_name+=std::to_string(i);
                std::cout<<tmp_name<<std::endl;
               // nameList[i]= QString::fromStdString(tmp_name);
            }*/
            pathListRawFull.clear();
            for (int i=0; i<nameList.size();i++){
                   pathListRawFull.append(directoryRaw.path()+"/"+nameList.at(i));
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
                   reader.setFileName(directoryRaw.path()+"/"+nameList.at(i));
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
        if(ui->boxListImageOrigine->currentIndex()==-1){
            QMessageBox msgBox2;
            msgBox2.setText("Have no data of image Raw");
            msgBox2.exec();
        }
        else{
        int tmp;
        tmp = ui->boxListImageOrigine->currentIndex();
        _pathRaw = pathListRawFull.at(tmp).toStdString();
       // _pathRaw = ui->boxListImageOrigine->currentText().toStdString();
        tmp = ui->boxImageUnet->findText(ui->boxListImageOrigine->currentText());
        if(tmp==-1){
            QMessageBox msgBox2;
            msgBox2.setText("Have no data of CNN!!!");
            msgBox2.exec();
        }
        else {
         //   ui->boxImageUnet->setCurrentIndex(tmp);
           // _pathUnet = ui->boxImageUnet->currentText().toStdString();
            _pathUnet = pathListUnetFull.at(tmp).toStdString();
            _paramProvider->setPathRaw(_pathRaw);
            _paramProvider->setPathUnet(_pathUnet);
            ui->boxImageUnet->setCurrentIndex(tmp);
          //  std::cout<<_pathRaw<<std::endl;
           // std::cout<<_pathUnet<<std::endl;
            ui->inversePixel->setEnabled(true);
            }
        }
        break;
    }
   case SAVEJSON:
        if(ui->textDatasetJson->document()->isEmpty()){
            QMessageBox msgBox3;
            msgBox3.setText("Dataset.json is empty !!!");
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

void LoadFolderWidget::checkUnet()
{
    typeCNN = Unet;
    ui->textMethodeJson->setEnabled(true);
    ui->buttonRunUnet->setEnabled(true);
      ui->loadUnet->setTitle("Unet");

}
void LoadFolderWidget::checkCNN2()
{
    typeCNN = CNN2;
    ui->textMethodeJson->setEnabled(true);
    ui->buttonRunUnet->setEnabled(true);
    ui->loadUnet->setTitle("CNN2");
}
void LoadFolderWidget::checkCNN3()
{
    typeCNN = CNN3;
    ui->textMethodeJson->setEnabled(true);
    ui->buttonRunUnet->setEnabled(true);
    ui->loadUnet->setTitle("CNN3");
}

void LoadFolderWidget::runUnet()
{
    QMessageBox msgBox;
    switch(choisTypeRun){
    case LOADDATA:{
        msgBox.setText("Selection Folder Of Image after Segemation by CNN?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int res = msgBox.exec();
        if (res==QMessageBox::Yes){
            ui->boxImageUnet->clear();
            directoryCNN = QFileDialog::getExistingDirectory(this, "Select Directory");
            QStringList nameList = directoryCNN.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP",QDir::Files);
            pathListUnetFull.clear();
            for (int i=0; i<nameList.size();i++){
                pathListUnetFull.append(directoryCNN.path()+"/"+nameList.at(i));
            }
            ui->boxImageUnet->addItems(nameList);
            switch(typeCNN){
            case Unet:{
                std::string textMethod="{ \"methodinfo\" :\n  {\n       \"name\" : \"";
                textMethod+="Unet";
                textMethod+="\",\n";
                textMethod+="       \"inputlocation\" : \"";
                textMethod+=directoryRaw.path().toStdString();
                textMethod+="\",\n";
                textMethod+="       \"outputlocation\" : \"";
                textMethod+=directoryCNN.path().toStdString();
                textMethod+="\",\n";
                textMethod+="       \"contains\" : \"";
                textMethod+=std::to_string(nameList.size());
                textMethod+=" files\",\n";
                textMethod+="       \"descrptionpython\" : [\n";
                textMethod+="       {\n";
                textMethod+="            \"def_downsample_cnn_block\" : \n";
                textMethod+="             {\n";
                textMethod+="                \"input\" : \"block_input, channel\",\n";
                textMethod+="                \"output\" : {\"maxpool, conv1, conv2\"},\n";
                textMethod+="                \"content\" :\n";
                textMethod+="                 {\n";
                textMethod+="                    \"maxpool\" : \"MaxPooling2D(pool_size=(2,2))(block_input)\",\n";
                textMethod+="                    \"conv1\" : \"Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(maxpool)\",\n";
                textMethod+="                    \"conv2\" : \"Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(conv1)\"\n";
                textMethod+="                 }\n";
                textMethod+="             }\n";
                textMethod+="       },\n";
                textMethod+="\n";
                textMethod+="       {\n";
                textMethod+="            \"def_upsample_cnn_block\" : \n";
                textMethod+="             {\n";
                textMethod+="                \"input\" : \"block_input, block_counterpart, channel\",\n";
                textMethod+="                \"output\" : \"up, concat, conv1, conv2\",\n";
                textMethod+="                \"content\" :\n";
                textMethod+="                 {\n";
                textMethod+="                    \"up\" : \"Conv2D(filters=channel, kernel_size=2, strides=1, padding='same')(UpSampling2D(size=(2,2))(block_input))\",\n";
                textMethod+="                    \"concat\" : \"Concatenate(axis=-1)([block_counterpart, up])\",\n";
                textMethod+="                    \"conv1\" : \"Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(concat)\",\n";
                textMethod+="                    \"conv2\" : \"Conv2D(filters=channel, kernel_size=3, strides=1, padding='same')(conv1)\"\n";
                textMethod+="                 }\n";
                textMethod+="             }\n";
                textMethod+="       },\n";
                textMethod+="\n";
                textMethod+="       {\n";
                textMethod+="            \"def_Unet\" : \n";
                textMethod+="             {\n";
                textMethod+="                \"output\" : \"model\",\n";
                textMethod+="                \"content\" :\n";
                textMethod+="                 {\n";
                textMethod+="                    \"input\" : \"Input(shape=(512, 512, 3))\",\n";
                textMethod+="                    \"conv1_ds_block1\" : \"Conv2D(filters=64,kernel_size=3, strides=1, padding='same')(input)\",\n";
                textMethod+="                    \"conv2_ds_block1\" : \"Conv2D(filters=64,kernel_size=3, strides=1, padding='same')(conv1_ds_block1)\",\n";
                textMethod+="                    \"ds_block2\" : \"_downsample_cnn_block(conv2_ds_block1, channel=128)\",\n";
                textMethod+="                    \"ds_block3\" : \"_downsample_cnn_block(ds_block2[-1], channel=256)\",\n";
                textMethod+="                    \"ds_block4\" : \"_downsample_cnn_block(ds_block3[-1], channel=512)\",\n";
                textMethod+="                    \"drop_ds_block4\" : \"Dropout(0.5)(ds_block4[-1])\",\n";
                textMethod+="                    \"ds_block5\" : \"_downsample_cnn_block(drop_ds_block4, channel=1024)\",\n";
                textMethod+="                    \"drop_ds_block5\" : \"Dropout(0.5)(ds_block5[-1])\",\n";
                textMethod+="                    \"us_block4\" : \"_upsample_cnn_block(drop_ds_block5, drop_ds_block4, channel=512)\",\n";
                textMethod+="                    \"us_block3\" : \"_upsample_cnn_block(us_block4[-1], ds_block3[-1], channel=256)\",\n";
                textMethod+="                    \"us_block2\" : \"_upsample_cnn_block(us_block3[-1], ds_block2[-1], channel=128)\",\n";
                textMethod+="                    \"us_block1\" : \"_upsample_cnn_block(us_block2[-1], conv2_ds_block1, channel=64)\",\n";
                textMethod+="                    \"conv3_us_block1\" : \"Conv2D(filters=2,kernel_size=3, strides=1, padding='same')(us_block1[-1])\",\n";
                textMethod+="                    \"conv4_us_block1\" : \"Conv2D(filters=1, kernel_size=1, strides=1, padding='same', activation='sigmoid')(conv3_us_block1)\",\n";
                textMethod+="                    \"model\" : \"tf.keras.models.Model(inputs = input , outputs = conv4_us_block1)\",\n";
                textMethod+="                    \"model.compile\" : \n";
                textMethod+="                      {\n";
                textMethod+="                       \"optimizer\" : \"Adam(lr = 1e-4)\",\n";
                textMethod+="                       \"loss\" : \"binary_crossentropy\",\n";
                textMethod+="                       \"metrics\" : \"accuracy\"\n";
                textMethod+="                      }\n";
                textMethod+="                 }\n";
                textMethod+="             }\n";
                textMethod+="       }\n";
                textMethod+="               ]\n";
                textMethod+="  }\n";
                textMethod+="}\n";
                ui->textMethodeJson->setText(QString::fromStdString(textMethod));
            }
                break;
            case CNN2:
                break;
            case CNN3:
                break;
            }

        }
        else if (res == QMessageBox::No){
            QMessageBox msgBox1;
            msgBox1.setText("Nothing Selected !!!");
            msgBox1.exec();
        }
    }
        break;
    case SHOWIMAGE:{
        if(ui->boxImageUnet->currentIndex()==-1){
            QMessageBox msgBox2;
            msgBox2.setText("Have no data of image Unet");
            msgBox2.exec();
        }
        else{
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
            ui->boxListImageOrigine->setCurrentIndex(tmp);
            _paramProvider->setPathRaw(_pathRaw);
            _paramProvider->setPathUnet(_pathUnet);
           // std::cout<<_pathRaw<<std::endl;
           // std::cout<<_pathUnet<<std::endl;
            ui->inversePixel->setEnabled(true);
        }
        }
        break;
    }
    case SAVEJSON:{
        if(ui->textMethodeJson->document()->isEmpty()){
            QMessageBox msgBox3;
            msgBox3.setText("Method.json is empty !!!");
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
                std::ofstream method(path.toStdString());
                method<<ui->textMethodeJson->toPlainText().toStdString();
                method.close();
            }

        }
    }
        break;
    case INVERSEPIXEL:{
        QImageReader reader;
        QSettings settings;
        reader.setFileName(QString::fromStdString(_pathUnet));
        QImage image = reader.read();
        int x =ui->spinX->text().toInt();
        int y =ui->spinY->text().toInt();
      if(x<0||y<0||x>image.width()||y>image.height()){
          QMessageBox msgBox5;
          msgBox5.setText("Coordinate not exist");
          msgBox5.exec();
      }
      else{
          std::cout<<"hihi"<<qRed(image.pixel(x,y))<<" "<<qGreen(image.pixel(x,y))<<qBlue(image.pixel(x,y))<<std::endl;
          if(qRed(image.pixel(x,y))==0&&qGreen(image.pixel(x,y))==0&&qBlue(image.pixel(x,y))==0){
              image.setPixelColor(x,y,Qt::white);
              QMessageBox msgBox6;
              msgBox6.setText("Black->White");
              msgBox6.exec();
          }
          else if(qRed(image.pixel(x,y))==255&&qGreen(image.pixel(x,y))==255&&qBlue(image.pixel(x,y))==255){
              image.setPixelColor(x,y,Qt::black);
              QMessageBox msgBox6;
              msgBox6.setText("White->Black");
              msgBox6.exec();
          }
          std::cout<<"hoho"<<qRed(image.pixel(x,y))<<" "<<qGreen(image.pixel(x,y))<<qBlue(image.pixel(x,y))<<std::endl;
          image.save(QString::fromStdString(_pathUnet));
          int tmp = ui->boxImageUnet->currentIndex();
          pathListUnetFull[tmp]=QString::fromStdString(_pathUnet);
          _paramProvider->setPathUnet(_pathUnet);
          Ra::Engine::Renderer::PickingQuery tmp1;
          Ra::Core::Vector2 coords{x,y};
          tmp1.m_screenCoords = coords;
          tmp1.m_purpose = Ra::Engine::Renderer::SELECTION;
          tmp1.m_mode = Ra::Engine::Renderer::VERTEX;
          _renderer->addPickingRequest(tmp1);

          //auto tex = _renderer->get
        //  _renderer->addPickingRequest(tmp1);
       //   std::cout<<_renderer-><<std::endl;

        //  _renderer->doPickingNow(coords,_ro->
      }
    }
        break;
    }

}


/*void LoadFolderWidget::inverseColor(){
        QImageReader reader;
        QSettings settings;
        reader.setFileName(QString::fromStdString(_pathUnet));
        QImage image = reader.read();
        int x =ui->spinX->text().toInt();
        int y =ui->spinY->text().toInt();
        if(qRed(image.pixel(x,y))==0&&qGreen(image.pixel(x,y))==0&&qBlue(image.pixel(x,y))==0){
            image.setPixelColor(x,y,Qt::white);
        }
        else if(qRed(image.pixel(x,y))==255&&qGreen(image.pixel(x,y))==255&&qBlue(image.pixel(x,y))==255){
            image.setPixelColor(x,y,Qt::black);
        }
        image.save(QString::fromStdString(_pathUnet));
}
*/
void LoadFolderWidget::mousePressEvent(QMouseEvent *event){
         //if(!_camera->handleMousePressEvent(event,Qt::LeftButton, Qt::ShiftModifier,1))
         //   std::cout<<"click roi ne"<<std::endl;
    if(event->button()==Qt::LeftButton){
        x = x+0.1;
        y = y+0.1;
       _paramProvider->setMouse({x,y});
       std::cout<<"hhhhhhhhhhhhhhh"<<x<<std::endl;
    }
}
