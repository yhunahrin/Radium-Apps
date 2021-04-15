#include "LoadFolderWidget.h"


LoadFolderWidget::LoadFolderWidget(Content content, QWidget *parent) :
                                QWidget( parent ),
                                _content(content),
                                ui( new Ui::LoadFolderWidget)

{
    ui->setupUi(this);
    ui->imageOrgine->setEnabled(false);
    ui->loadUnet->setEnabled(false);
    ui->textMethodeJson->setEnabled(false);
    ui->buttonRunUnet->setEnabled(false);
    connect( ui->buttonLoad, &QRadioButton::clicked, this, &LoadFolderWidget::enableLoadData );
    connect( ui->buttonShow, &QRadioButton::clicked, this, &LoadFolderWidget::enableShowImage);
    connect( ui->buttonJson, &QRadioButton::clicked, this, &LoadFolderWidget::enableSaveJson);
    connect( ui->checkUnet, &QRadioButton::clicked, this, &LoadFolderWidget::checkUnet );
    connect( ui->checkCNN2, &QRadioButton::clicked, this, &LoadFolderWidget::checkCNN2);
    connect( ui->checkCNN3,&QRadioButton::clicked, this, &LoadFolderWidget::checkCNN3);
    connect( ui->buttonRunImage, &QPushButton::clicked, this, &LoadFolderWidget::runImage);
    connect( ui->buttonRunUnet, &QPushButton::clicked, this, &LoadFolderWidget::runUnet);
}

Content LoadFolderWidget::getContent(){
    return _content;
}

void LoadFolderWidget::setContent(Content content){
    _content = content;
}

LoadFolderWidget::~LoadFolderWidget()
{
    delete ui;
}

void LoadFolderWidget::enableLoadData()
{
    typeRun = Loadata;
    _content.enableButtonRun(ui->imageOrgine, ui->buttonRunImage);
    ui->buttonLoad->setChecked(true);
    ui->buttonShow->setChecked(false);
    ui->buttonJson->setChecked(false);
}

void LoadFolderWidget::enableShowImage()
{
    typeRun =ShowImage;
    _content.enableButtonRun(ui->imageOrgine, ui->buttonRunImage);
    ui->buttonLoad->setChecked(false);
    ui->buttonShow->setChecked(true);
    ui->buttonJson->setChecked(false);
}

void LoadFolderWidget::enableSaveJson()
{
    typeRun = SaveJson;
    _content.enableButtonRun(ui->imageOrgine, ui->buttonRunImage);
    ui->buttonLoad->setChecked(false);
    ui->buttonShow->setChecked(false);
    ui->buttonJson->setChecked(true);
}
void LoadFolderWidget::runImage()
{
    switch(typeRun){
    case Loadata:{
        int res = _content.messBoxYesNo("Selection Folder Of Original Image?");

        if (res==QMessageBox::Yes){
            ui->loadUnet->setEnabled(true);
            ui->boxListImageOrigine->clear();
            directoryRaw = QFileDialog::getExistingDirectory(this, "Select Directory");
            QStringList nameList = directoryRaw.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP",QDir::Files);           
            ui->textDatasetJson->setText(QString::fromStdString(_content.textDataRaw(&pathListRawFull,directoryRaw,nameList)));
            std::cout<<pathListRawFull.size();
            ui->boxListImageOrigine->addItems(nameList);
        }
        else if (res == QMessageBox::No){
            _content.messBoxInfo("Nothing Selected !!!");
        }
    }
        break;
    case ShowImage:{
        if(ui->boxListImageOrigine->currentIndex()==-1){
            _content.messBoxInfo("Have no data of image Raw");
        }
        else{
        int tmp;
        tmp = ui->boxListImageOrigine->currentIndex();
        _content.setPathRaw(pathListRawFull.at(tmp).toStdString());
       // _pathRaw = ui->boxListImageOrigine->currentText().toStdString();
        tmp = ui->boxImageUnet->findText(ui->boxListImageOrigine->currentText());
        if(tmp==-1){
            _content.messBoxInfo("Have no data of CNN!!!");
        }
        else {
         //   ui->boxImageUnet->setCurrentIndex(tmp);
           // _pathUnet = ui->boxImageUnet->currentText().toStdString();
            _content.setPathCNN(pathListUnetFull.at(tmp).toStdString());
            _content.getParameterProvider()->setPathRaw( _content.getPathRaw());
            _content.getParameterProvider()->setPathUnet( _content.getPathCNN());
            _content.setParameterProvider( _content.getParameterProvider());
            ui->boxImageUnet->setCurrentIndex(tmp);
          //  std::cout<<_pathRaw<<std::endl;
           // std::cout<<_pathUnet<<std::endl;
        //    ui->inversePixel->setEnabled(true);
            }
        }
        break;
    }
   case SaveJson:
        if(ui->textDatasetJson->document()->isEmpty()){
            _content.messBoxInfo("Dataset.json is empty !!!");
        }
        else{
            QString path = QFileDialog::getSaveFileName(this, "Save Image", "",
                                                                   "Json(*.json) ");
            if(path == ""){
              _content.messBoxInfo("No directory");

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
    _content.checkButtonCNN(ui->textMethodeJson,ui->loadUnet,ui->buttonRunUnet,"Unet");
}

void LoadFolderWidget::checkCNN2()
{
    typeCNN = CNN2;
    _content.checkButtonCNN(ui->textMethodeJson,ui->loadUnet,ui->buttonRunUnet,"CNN2");
}
void LoadFolderWidget::checkCNN3()
{
    typeCNN = CNN3;
    _content.checkButtonCNN( ui->textMethodeJson,ui->loadUnet,ui->buttonRunUnet,"CNN3");
}

void LoadFolderWidget::runUnet()
{
    switch(typeRun){
    case Loadata:{
        int res = _content.messBoxYesNo("Selection Folder Of Image after Segemation by CNN?");
        if (res==QMessageBox::Yes){
            ui->boxImageUnet->clear();
            directoryCNN = QFileDialog::getExistingDirectory(this, "Select Directory");
            QStringList nameList = directoryCNN.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP",QDir::Files);
            _content.listPathFull(&pathListUnetFull,directoryCNN,nameList);
            ui->boxImageUnet->addItems(nameList);
            switch(typeCNN){
            case Unet:{
                ui->textMethodeJson->setText(QString::fromStdString(_content.methodeUnet(directoryRaw,directoryCNN,nameList)));
            }
                break;
            case CNN2:
                break;
            case CNN3:
                break;
            }

        }
        else if (res == QMessageBox::No){
             _content.messBoxInfo("Nothing Selected !!!");
        }
    }
        break;
    case ShowImage:{
        if(ui->boxImageUnet->currentIndex()==-1){
            _content.messBoxInfo("Have no data of image Unet");
        }
        else{
        int tmp;
        tmp = ui->boxImageUnet->currentIndex();
        _content.setPathCNN(pathListUnetFull.at(tmp).toStdString());
        //_pathUnet = ui->boxImageUnet->currentText().toStdString();
        tmp = ui->boxListImageOrigine->findText(ui->boxImageUnet->currentText());
        if(tmp==-1){
            _content.messBoxInfo("Have no data of image Raw");
        }
        else{
           // ui->boxListImageOrigine->setCurrentIndex(tmp);
            //_pathRaw = ui->boxListImageOrigine->currentText().toStdString();
            _content.setPathRaw(pathListRawFull.at(tmp).toStdString());
            ui->boxListImageOrigine->setCurrentIndex(tmp);
            _content.getParameterProvider()->setPathRaw( _content.getPathRaw());
            _content.getParameterProvider()->setPathUnet( _content.getPathCNN());
            _content.setParameterProvider( _content.getParameterProvider());
           // std::cout<<_pathRaw<<std::endl;
           // std::cout<<_pathUnet<<std::endl;
          //  ui->inversePixel->setEnabled(true);
        }
        }
        break;
    }
    case SaveJson:{
        if(ui->textMethodeJson->document()->isEmpty()){
            _content.messBoxInfo("Method.json is empty !!!");
        }
        else{
            QString path = QFileDialog::getSaveFileName(this, "Save Image", "",
                                                                   "Json(*.json) ");
            if(path == ""){
               _content.messBoxInfo("No directory");
             }
             else{
                std::ofstream method(path.toStdString());
                method<<ui->textMethodeJson->toPlainText().toStdString();
                method.close();
            }

        }
    }
        break;
  /*  case INVERSEPIXEL:{
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
    }*/

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

