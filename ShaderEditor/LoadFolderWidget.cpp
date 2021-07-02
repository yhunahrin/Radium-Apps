#include "LoadFolderWidget.h"


LoadFolderWidget::LoadFolderWidget(Content content, QWidget *parent) :
                                QWidget( parent ),
                                _content(content),
                                ui( new Ui::LoadFolderWidget)

{
    ui->setupUi(this);
    ui->imageOrgine->setEnabled(true);
    ui->loadUnet->setEnabled(false);
  //  ui->textMethodeJson->setEnabled(false);
    ui->buttonRunUnet->setEnabled(false);
 //   ui->saveChanged->setEnabled(false);
  // ui->saveAllSegment->setEnabled(false);
/*    connect( ui->buttonLoad, &QRadioButton::clicked, this, &LoadFolderWidget::enableLoadData );
    connect( ui->buttonShow, &QRadioButton::clicked, this, &LoadFolderWidget::enableShowImage);
    connect( ui->buttonJson, &QRadioButton::clicked, this, &LoadFolderWidget::enableSaveJson);
 //   connect( ui->checkUnet, &QRadioButton::clicked, this, &LoadFolderWidget::checkUnet );*/
   // connect( ui->checkCNN2, &QRadioButton::clicked, this, &LoadFolderWidget::checkCNN2);
  //  connect( ui->checkCNN3,&QRadioButton::clicked, this, &LoadFolderWidget::checkCNN3);
    connect( ui->buttonRunImage, &QPushButton::clicked, this, &LoadFolderWidget::loadUtrasound);
    connect( ui->buttonRunUnet, &QPushButton::clicked, this, &LoadFolderWidget::loadSegmented);

     //connect( ui->boxImageUnet,SIGNAL(activated(int)), this, SLOT(&LoadFolderWidget::touchListUtralSegemented(int)));
   /* connect( ui->saveChanged, &QPushButton::clicked, this, &LoadFolderWidget::saveImageChanged);
    connect( ui->saveAllSegment, &QPushButton::clicked, this, &LoadFolderWidget::saveAll);*/
    model   =     new QStandardItemModel(this);
    proxyModel	= new QSortFilterProxyModel(this);
  //  QCoreApplication::setOrganizationName(QString("Radium_App"));
}

const Content &LoadFolderWidget::getContent() const{
    return _content;
}


void LoadFolderWidget::setContent(Content content){
    _content = content;
}

LoadFolderWidget::~LoadFolderWidget()
{
    delete ui;
}


/*void LoadFolderWidget::enableLoadData()
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
 //   ui->loadUnet->setEnabled(false);
  //  ui->textMethodeJson->setEnabled(false);
   // ui->buttonRunUnet->setEnabled(false);
}

void LoadFolderWidget::enableSaveJson()
{
    typeRun = SaveJson;
    _content.enableButtonRun(ui->imageOrgine, ui->buttonRunImage);
    ui->buttonLoad->setChecked(false);
    ui->buttonShow->setChecked(false);
    ui->buttonJson->setChecked(true);
}*/
int findIndex(QString str1, QStringList str2){
    for(int i =0; i<str2.size();i++){
        if(str1==str2.at(i))
            return i;
    }
    return -1;
}
void  LoadFolderWidget::showImage(){
    int tmp;
    int tmp1;
    if((ui->boxImageUnet->currentIndex()==-1)&&(!creatDataCNN)){
        QString folderCNN =directoryRaw.path();
        folderCNN+="_Result";
        if(!QDir(folderCNN).exists()){
            pathListUnetFull.clear();
            _dataCNN.clear();
            QDir().mkdir(folderCNN);
            for(int i = 0;i<_dataRaw.size();i++){
                auto pathRaw = QFileInfo(QString::fromStdString(_dataRaw.at(i).path));
                QString pathCNN=folderCNN;
                pathCNN+="/";
                pathCNN+=pathRaw.fileName();
                QImage imageCNN = _dataRaw.at(i).image;
                for(int j=0;j<imageCNN.width();j++)
                    for(int k=0;k<imageCNN.height();k++)
                        imageCNN.setPixel(j,k,Qt::black);
                imageCNN.save(pathCNN);
                Ra::Engine::TextureParameters texPara = _content.getTextureParameterFromPath(pathCNN.toStdString());
                Ra::Engine::Dataset data;
                data.path=pathCNN.toStdString();
                data.image=imageCNN;
                data.textureParametres=texPara;
                pathListUnetFull.push_back(pathCNN);
                _dataCNN.push_back(data);
            }
            directoryCNN = QDir(folderCNN);
            creatDataCNN = true;
        }
        else {
            pathListUnetFull.clear();
            _dataCNN.clear();
            for(int i = 0;i<_dataRaw.size();i++){
                auto pathRaw = QFileInfo(QString::fromStdString(_dataRaw.at(i).path));
                QString pathCNN=folderCNN;
                pathCNN+="/";
                pathCNN+=pathRaw.fileName();
                QImage imageCNN = _dataRaw.at(i).image;
                for(int j=0;j<imageCNN.width();j++)
                    for(int k=0;k<imageCNN.height();k++)
                        imageCNN.setPixel(j,k,Qt::black);
                imageCNN.save(pathCNN);
                Ra::Engine::TextureParameters texPara = _content.getTextureParameterFromPath(pathCNN.toStdString());
                Ra::Engine::Dataset data;
                data.path=pathCNN.toStdString();
                data.image=imageCNN;
                data.textureParametres=texPara;
                pathListUnetFull.push_back(pathCNN);
                _dataCNN.push_back(data);

                }
             creatDataCNN = true;
             directoryCNN = QDir(folderCNN);
        }
    }
    if((_content.getParameterProvider()->getCount()!=0)&&(_content.getParameterProvider()->getSave()==false)){
         _content.getParameterProvider()->setSave(true);
        QModelIndex index = ui->listView->currentIndex();
        QString itemText = index.data(Qt::DisplayRole).toString();
        QString nameItemRaw = directoryRaw.path();
        nameItemRaw+="/";
        nameItemRaw+=itemText;
      //  std::cout<<"hihi"<<nameItemRaw.toStdString()<<std::endl;
        QString nameItemCNN = directoryCNN.path();
        nameItemCNN+="/";
        nameItemCNN+=itemText;
        int res = _content.messBoxYesNo("Save modified segmentation?");
        if(res == QMessageBox::No){
         //   tmp =  findIndex(nameItemRaw,pathListRawFull);
         //   _content.setPathRaw(pathListRawFull.at(tmp).toStdString());
           //  _content.setPathRaw(nameItemRaw.toStdString());
           // _pathRaw = ui->boxListImageOrigine->currentText().toStdString();
            tmp=_content.findIndex(nameItemRaw,pathListRawFull);
            tmp1 = ui->boxImageUnet->findText(itemText);
            if(tmp1==-1){
               // _content.messBoxInfo("Have no data of CNN!!!");
                if((loadSeg==false)&&(etatShow==false)){
                int res = _content.messBoxYesNo("No segemented image load. Load segmentation?");
                if(res == QMessageBox::No){
                //   _content.getParameterProvider()->setPathRaw(_content.getPathRaw());
                  // _content.getParameterProvider()->setPathUnet(_content.getPathRaw());
                    _content.getParameterProvider()->setIndexRaw(tmp);
                    _content.getParameterProvider()->setIndexCNN(tmp);
                    _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
                    _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
                    this->setIndexRaw(tmp);
                    this->setIndexCNN(tmp);
                    ui->boxListImageOrigine->setCurrentIndex(tmp);
                   emit needUpdate();
                   _content.getParameterProvider()->setCount(0);
                   etatShow=true;
                }
                else {
                  //  Segmentation *nw = new Segmentation();
                   // nw->setPathIn(QString::fromStdString(_content.getPathRaw()));
                   // nw->show();

                    pathOut.clear();
                  //  _content.getParameterProvider()->setPathRaw(_content.getPathRaw());
                    _content.getParameterProvider()->setIndexRaw(tmp);
                    _content.getParameterProvider()->setIndexCNN(tmp);
                    _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
                    _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
                    etatShow = true;
                    this->setIndexRaw(tmp);
                    this->setIndexCNN(tmp);
                    ui->boxListImageOrigine->setCurrentIndex(tmp);

                   // _content.getParameterProvider()->setTextureParameterCNN(_textureParaRaw.at(tmp));
                 /*   QString textScript="from keras.models import load_model\n";
                    textScript+="from keras import models\n";
                    textScript+="import numpy as np\n";
                    textScript+="import cv2\n";
                    textScript+="import matplotlib.pyplot as plt\n";
                    textScript+="import skimage.io as io\n";
                    textScript+="INPUT_SHAPE = 512\n";
                    textScript+="OUTPUT_SHAPE = 512 \n";
                    textScript+="model = load_model('";
                    textScript+= "UNET_b_160_IOU.h5";
                    textScript+="',compile= True)\n";
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
                    textScript+="_predict_path('";
                    textScript+=QString::fromStdString(_content.getPathRaw());
                    textScript+="','";
                    QFileInfo pathDirOut(QString::fromStdString(_content.getPathRaw()));
                   // _content.getParameterProvider()->setPathUnet(_content.getPathRaw());
                    pathOut = pathDirOut.fileName();
                    textScript+=pathOut;
                    textScript+="')";
                   // std::cout<<this->getPathProg().toStdString()<<std::endl;
                    // std::ofstream model ("settingPythonProgram.ini");
                    // model<<textScript.toStdString();*/
                    Segmentation *nw = new Segmentation(this->getPathProg());
                    nw->show();

                  /*  QProcess p;
                    QStringList params;
                     params <<"unet.py";
                     p.start(this->getPathProg(), params);
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
                     msgBox1.exec();}*/
                   // _content.getParameterProvider()->setPathUnet(nw->getPathOut().toStdString());
                   // std::cout<<"haha"<<nw->getPathOut().toStdString()<<std::endl;
                 //   _content.getParameterProvider()->setTextureParameterCNN(_dataRaw.at(tmp).textureParametres);
                    _content.getParameterProvider()->setCount(0);
        }
            }
                else {
                    _content.getParameterProvider()->setIndexRaw(tmp);
                    _content.getParameterProvider()->setIndexCNN(tmp);
                    _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
                    _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
                    this->setIndexRaw(tmp);
                    this->setIndexCNN(tmp);
                    ui->boxListImageOrigine->setCurrentIndex(tmp);
                   emit needUpdate();
                   _content.getParameterProvider()->setCount(0);
                   etatShow=true;
                }
    }
            else{
                _content.getParameterProvider()->setIndexRaw(tmp);
                _content.getParameterProvider()->setIndexCNN(tmp);
                _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
                _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
                this->setIndexRaw(tmp);
                this->setIndexCNN(tmp);
                ui->boxListImageOrigine->setCurrentIndex(tmp);
               emit needUpdate();
               _content.getParameterProvider()->setCount(0);
               etatShow=true;
            }
  }
        else if(res == QMessageBox::Yes){
            etatShow=true;
        }

    }else{
          _content.getParameterProvider()->setSave(true);
        QModelIndex index = ui->listView->currentIndex();
        QString itemText = index.data(Qt::DisplayRole).toString();

        QString nameItemRaw = directoryRaw.path();
        nameItemRaw+="/";
        nameItemRaw+=itemText;
    //    std::cout<<"hihi"<<nameItemRaw.toStdString()<<std::endl;
        QString nameItemCNN = directoryCNN.path();
        nameItemCNN+="/";
        nameItemCNN+=itemText;
     //   std::cout<<"haha"<<nameItemCNN.toStdString()<<std::endl;
    //tmp = findIndex(nameItemRaw,pathListRawFull);
   // _content.setPathRaw(pathListRawFull.at(tmp).toStdString());
     //    _content.setPathRaw(nameItemRaw.toStdString());
   // _pathRaw = ui->boxListImageOrigine->currentText().toStdString();
    tmp= _content.findIndex(nameItemRaw,pathListRawFull);
    tmp1 = ui->boxImageUnet->findText(itemText);
    _content.getParameterProvider()->setSave(false);
  //  std::cout<<"yes"<<nameItemCNN.toStdString()<<std::endl;
  //  std::cout<<"no"<<ui->boxImageUnet->currentIndex()<<std::endl;
    if(tmp1==-1){
       // _content.messBoxInfo("Have no data of CNN!!!");
            _content.getParameterProvider()->setSave(true);
           if((loadSeg==false)&&(etatShow==false)){

        int res = _content.messBoxYesNo("No segemented image load. Load segmentation?");
        if(res == QMessageBox::No){
     //      _content.getParameterProvider()->setPathRaw(_content.getPathRaw());
       //    _content.getParameterProvider()->setPathUnet(_content.getPathRaw());
            _content.getParameterProvider()->setIndexRaw(tmp);
            _content.getParameterProvider()->setIndexCNN(tmp);
            _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
            //std::cout<<index.<<std::endl;
            _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
            ui->boxListImageOrigine->setCurrentIndex(tmp);
            this->setIndexRaw(tmp);
            this->setIndexCNN(tmp);
           emit needUpdate();
           _content.getParameterProvider()->setCount(0);
           etatShow =true;
        }
        else {
          //  Segmentation *nw = new Segmentation();
           // nw->setPathIn(QString::fromStdString(_content.getPathRaw()));
           // nw->show();
          //  remove(pathOut.toStdString().c_str());
            pathOut.clear();
        //    _content.getParameterProvider()->setPathRaw(_content.getPathRaw());
            _content.getParameterProvider()->setIndexRaw(tmp);
            _content.getParameterProvider()->setIndexCNN(tmp);
            _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
            _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
            this->setIndexRaw(tmp);
            this->setIndexCNN(tmp);
            ui->boxListImageOrigine->setCurrentIndex(tmp);
      /*      QString textScript="from keras.models import load_model\n";
            textScript+="from keras import models\n";
            textScript+="import numpy as np\n";
            textScript+="import cv2\n";
            textScript+="import matplotlib.pyplot as plt\n";
            textScript+="import skimage.io as io\n";
            textScript+="INPUT_SHAPE = 512\n";
            textScript+="OUTPUT_SHAPE = 512 \n";
            textScript+="model = load_model('";
            textScript+= "UNET_b_160_IOU.h5";
            textScript+="',compile= True)\n";
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
            textScript+="_predict_path('";
            textScript+=QString::fromStdString(_content.getPathRaw());
            textScript+="','";
            QFileInfo pathDirOut(QString::fromStdString(_content.getPathRaw()));
            pathOut = pathDirOut.fileName();
            textScript+=pathOut;
            textScript+="')";
         //   std::cout<<this->getPathProg().toStdString()<<std::endl;
            std::ofstream model ("unet.py");
            model<<textScript.toStdString();*/
            Segmentation *nw = new Segmentation(this->getPathProg());
            nw->show();
           /* QProcess p;
             QStringList params;
             params <<"unet.py";
             p.start(this->getPathProg(), params);
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
             msgBox1.exec();}*/
         //   _content.getParameterProvider()->setPathUnet(_content.getPathRaw());
           // _content.getParameterProvider()->setPathUnet(nw->getPathOut().toStdString());
           // std::cout<<"haha"<<nw->getPathOut().toStdString()<<std::endl;
           emit needUpdate();
            _content.getParameterProvider()->setCount(0);
            etatShow =true;
        }
           }
           else{
               _content.getParameterProvider()->setIndexRaw(tmp);
               _content.getParameterProvider()->setIndexCNN(tmp);
               _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
               //std::cout<<index.<<std::endl;
               _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
               ui->boxListImageOrigine->setCurrentIndex(tmp);
               this->setIndexRaw(tmp);
               this->setIndexCNN(tmp);
              emit needUpdate();
              _content.getParameterProvider()->setCount(0);
              etatShow =true;
           }
    }
    else {
     //   ui->boxImageUnet->setCurrentIndex(tmp);
       // _pathUnet = ui->boxImageUnet->currentText().toStdString();
      //  _content.setPathCNN(pathListUnetFull.at(tmp).toStdString());
      //   _content.setPathCNN(nameItemCNN.toStdString());
       //  ui->boxImageUnet->setCurrentIndex(tmp);
     //   _content.getParameterProvider()->setPathRaw(_content.getPathRaw());
       // _content.getParameterProvider()->setPathUnet(_content.getPathCNN());
            _content.getParameterProvider()->setSave(true);
        _content.getParameterProvider()->setIndexRaw(tmp);
        _content.getParameterProvider()->setIndexCNN(tmp1);
         _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
         _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp1).textureParametres);
         ui->boxListImageOrigine->setCurrentIndex(tmp);
         ui->boxImageUnet->setCurrentIndex(tmp1);
         this->setIndexRaw(tmp);
         this->setIndexCNN(tmp1);
        emit needUpdate();
        _content.getParameterProvider()->setCount(0);
        etatShow = true;

//            _content.setParameterProvider( _content.getParameterProvider());


      //  std::cout<<_pathRaw<<std::endl;
       // std::cout<<_pathUnet<<std::endl;
    //    ui->inversePixel->setEnabled(true);
        }
    }
}
void LoadFolderWidget::loadUtrasound(){
    if(_drawing == true){
        int res1 = _content.messBoxYesNo("Save modified segmentation?");
         if (res1==QMessageBox::Yes){
             this->saveAll();
             _drawing =false;
             int res = _content.messBoxYesNo("Select ultrasound image folder?");
             showed = false;
             etatShow =false;
             if (res==QMessageBox::Yes){
                 ui->loadUnet->setEnabled(true);
                 directoryRaw = QFileDialog::getExistingDirectory(this, "Select Directory",QDir::homePath());
                 if(directoryRaw.exists()){
                  directoryRaw.setSorting(QDir::SortFlag::DirsFirst | QDir::SortFlag::Name);
                 QStringList nameList = directoryRaw.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP"<<"*.jpeg"<<"*.jfif"<<"*.pjpeg"<<"*.pjp"<<"*.JPEG",QDir::Files,QDir::Name|QDir::IgnoreCase);
               //  ui->textDatasetJson->setText(QString::fromStdString());
                 infoImage = QString::fromStdString(_content.textDataRaw(&pathListRawFull,directoryRaw,nameList));
                 if(pathListRawFull.size()==0){
                     _content.messBoxInfo("Nothing Files Images !!!");
                     _dataRaw.clear();
                     ui->boxListImageOrigine->clear();
                     model->clear();
                     ui->listView->clearPropertyFlags();
                 }
                 else{
                 _dataRaw.clear();
                 ui->boxListImageOrigine->clear();
                 model->clear();
                 ui->listView->clearPropertyFlags();
                 Ra::Engine::Dataset dataset;
                 for(int i=0; i<pathListRawFull.size();i++){
                     dataset.path=pathListRawFull.at(i).toStdString();
                     QFileInfo f(pathListRawFull.at(i));
                     dataset.name=f.fileName().toStdString();
                     char *p;
                     std::string tmp3 = pathListRawFull.at(i).toStdString();
                     char *tmp4 = const_cast<char*>(tmp3.c_str());
                     p = strtok(tmp4, ".");
                     p = strtok(NULL, "\\eof");
                     dataset.type = p;
                     dataset.textureParametres=_content.getTextureParameterFromPath(pathListRawFull.at(i).toStdString());
                     dataset.image=_content.getImageFromPath(pathListRawFull.at(i).toStdString());
                     _dataRaw.push_back(dataset);
                     etatShow=false;
                 }
                // std::cout<<pathListRawFull.size();
                 ui->boxListImageOrigine->addItems(nameList);
                 disconnect(ui->listView, &QListView::doubleClicked, this, &LoadFolderWidget::showImage);
                 disconnect( ui->boxListImageOrigine,SIGNAL(activated(int)), this, SLOT(touchListUltrasound()));
               //  ui->textMethodeJson->setEnabled(false);
                 ui->buttonRunUnet->setEnabled(true);
                 const int numRows = pathListRawFull.size();

                 model->insertRows(0,numRows);
                 for(int i=0;i<numRows ;++i)
                 {
                 QFileInfo nameImage(pathListRawFull.at(i));
                // model->setData(model->index(i,0),QPixmap(thumbnails.at(i)),Qt::DecorationRole);
                // model->setData(model->index(i,1),nameImage.fileName());
                 QPixmap p(pathListRawFull.at(i));
                 QIcon *icon = new QIcon(p);
                 QStandardItem *item = new QStandardItem(*icon, nameImage.fileName());
                 model->appendRow(item);
                 }
                 proxyModel->setSourceModel(model);
                 proxyModel->setFilterKeyColumn(0);
                 ui->listView->setViewMode(QListView::IconMode);
                 ui->listView->setModel(model);
                 ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked);
                 connect(ui->listView, &QListView::doubleClicked, this, &LoadFolderWidget::showImage);
                 char *p;
                 std::string tmp3 = pathListRawFull.at(0).toStdString();
                 char *tmp4 = const_cast<char*>(tmp3.c_str());
                 p = strtok(tmp4, ".");
                 p = strtok(NULL, "\\eof");
                 QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "Radium_App","DataEchographie");
                 settings.setValue("NameFolder", directoryRaw.dirName());
                 settings.setValue("Path", directoryRaw.path());
                 settings.setValue("SizeOfFiles", pathListRawFull.size());
                 settings.setValue("Type", p);
                 //ui->saveChanged->setEnabled(true);
                 //ui->saveAllSegment->setEnabled(true);
                 creatDataCNN=false;
                 connect( ui->boxListImageOrigine,SIGNAL(activated(int)), this, SLOT(touchListUltrasound()));
                 }
                 }
                 else
                     _content.messBoxInfo("Nothing Selected !!!");
             }
             else if (res == QMessageBox::No){
                 _content.messBoxInfo("Nothing Selected !!!");
             }
         }
         else{
             int res = _content.messBoxYesNo("Select ultrasound image folder?");
             showed = false;
             etatShow =false;
             if (res==QMessageBox::Yes){
                 ui->loadUnet->setEnabled(true);
                 directoryRaw = QFileDialog::getExistingDirectory(this, "Select Directory",QDir::homePath());
                 if(directoryRaw.exists()){
                  directoryRaw.setSorting(QDir::SortFlag::DirsFirst | QDir::SortFlag::Name);
                 QStringList nameList = directoryRaw.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP"<<"*.jpeg"<<"*.jfif"<<"*.pjpeg"<<"*.pjp"<<"*.JPEG",QDir::Files,QDir::Name|QDir::IgnoreCase);
               //  ui->textDatasetJson->setText(QString::fromStdString());
                 infoImage = QString::fromStdString(_content.textDataRaw(&pathListRawFull,directoryRaw,nameList));
                 if(pathListRawFull.size()==0){
                     _content.messBoxInfo("Nothing Files Images !!!");
                     _dataRaw.clear();
                     ui->boxListImageOrigine->clear();
                     model->clear();
                     ui->listView->clearPropertyFlags();
                 }
                 else{
                 _dataRaw.clear();
                 ui->boxListImageOrigine->clear();
                 model->clear();
                 ui->listView->clearPropertyFlags();
                 Ra::Engine::Dataset dataset;
                 for(int i=0; i<pathListRawFull.size();i++){
                     dataset.path=pathListRawFull.at(i).toStdString();
                     dataset.textureParametres=_content.getTextureParameterFromPath(pathListRawFull.at(i).toStdString());
                     dataset.image=_content.getImageFromPath(pathListRawFull.at(i).toStdString());
                     _dataRaw.push_back(dataset);
                     etatShow=false;
                 }
                // std::cout<<pathListRawFull.size();
                 ui->boxListImageOrigine->addItems(nameList);
                 disconnect(ui->listView, &QListView::doubleClicked, this, &LoadFolderWidget::showImage);
                 disconnect( ui->boxListImageOrigine,SIGNAL(activated(int)), this, SLOT(touchListUltrasound()));
               //  ui->textMethodeJson->setEnabled(false);
                 ui->buttonRunUnet->setEnabled(true);
                 const int numRows = pathListRawFull.size();
                 _drawing = false;
                 model->insertRows(0,numRows);
                 for(int i=0;i<numRows ;++i)
                 {
                 QFileInfo nameImage(pathListRawFull.at(i));
                // model->setData(model->index(i,0),QPixmap(thumbnails.at(i)),Qt::DecorationRole);
                // model->setData(model->index(i,1),nameImage.fileName());
                 QPixmap p(pathListRawFull.at(i));
                 QIcon *icon = new QIcon(p);
                 QStandardItem *item = new QStandardItem(*icon, nameImage.fileName());
                 model->appendRow(item);
                 }
                 proxyModel->setSourceModel(model);
                 proxyModel->setFilterKeyColumn(0);
                 ui->listView->setViewMode(QListView::IconMode);
                 ui->listView->setModel(model);
                 ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked);
                 connect(ui->listView, &QListView::doubleClicked, this, &LoadFolderWidget::showImage);
                 char *p;
                 std::string tmp3 = pathListRawFull.at(0).toStdString();
                 char *tmp4 = const_cast<char*>(tmp3.c_str());
                 p = strtok(tmp4, ".");
                 p = strtok(NULL, "\\eof");
                 QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "Radium_App","DataEchographie");
                 settings.setValue("NameFolder", directoryRaw.dirName());
                 settings.setValue("Path", directoryRaw.path());
                 settings.setValue("SizeOfFiles", pathListRawFull.size());
                 settings.setValue("Type", p);
                 //ui->saveChanged->setEnabled(true);
                 //ui->saveAllSegment->setEnabled(true);
                 creatDataCNN=false;
                 connect( ui->boxListImageOrigine,SIGNAL(activated(int)), this, SLOT(touchListUltrasound()));
                 }
                 }
                 else
                     _content.messBoxInfo("Nothing Selected !!!");
             }
             else if (res == QMessageBox::No){
                 _content.messBoxInfo("Nothing Selected !!!");
             }
         }
    }
    else{
    int res = _content.messBoxYesNo("Select ultrasound image folder?");
    showed = false;
    etatShow =false;
    if (res==QMessageBox::Yes){
        ui->loadUnet->setEnabled(true);
        directoryRaw = QFileDialog::getExistingDirectory(this, "Select Directory",QDir::homePath());
        if(directoryRaw.exists()){
         directoryRaw.setSorting(QDir::SortFlag::DirsFirst | QDir::SortFlag::Name);
        QStringList nameList = directoryRaw.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP"<<"*.jpeg"<<"*.jfif"<<"*.pjpeg"<<"*.pjp"<<"*.JPEG",QDir::Files,QDir::Name|QDir::IgnoreCase);
      //  ui->textDatasetJson->setText(QString::fromStdString());
        infoImage = QString::fromStdString(_content.textDataRaw(&pathListRawFull,directoryRaw,nameList));
        if(pathListRawFull.size()==0){
            _content.messBoxInfo("Nothing Files Images !!!");
            _dataRaw.clear();
            ui->boxListImageOrigine->clear();
            model->clear();
            ui->listView->clearPropertyFlags();
        }
        else{
        _dataRaw.clear();
        ui->boxListImageOrigine->clear();
        model->clear();
        ui->listView->clearPropertyFlags();
        Ra::Engine::Dataset dataset;
        for(int i=0; i<pathListRawFull.size();i++){
            dataset.path=pathListRawFull.at(i).toStdString();
            dataset.textureParametres=_content.getTextureParameterFromPath(pathListRawFull.at(i).toStdString());
            dataset.image=_content.getImageFromPath(pathListRawFull.at(i).toStdString());
            _dataRaw.push_back(dataset);
            etatShow=false;
        }
       // std::cout<<pathListRawFull.size();
        ui->boxListImageOrigine->addItems(nameList);
        disconnect(ui->listView, &QListView::doubleClicked, this, &LoadFolderWidget::showImage);
        disconnect( ui->boxListImageOrigine,SIGNAL(activated(int)), this, SLOT(touchListUltrasound()));
      //  ui->textMethodeJson->setEnabled(false);
        ui->buttonRunUnet->setEnabled(true);
        const int numRows = pathListRawFull.size();
        _drawing = false;
        model->insertRows(0,numRows);
        for(int i=0;i<numRows ;++i)
        {
        QFileInfo nameImage(pathListRawFull.at(i));
       // model->setData(model->index(i,0),QPixmap(thumbnails.at(i)),Qt::DecorationRole);
       // model->setData(model->index(i,1),nameImage.fileName());
        QPixmap p(pathListRawFull.at(i));
        QIcon *icon = new QIcon(p);
        QStandardItem *item = new QStandardItem(*icon, nameImage.fileName());
        model->appendRow(item);
        }
        proxyModel->setSourceModel(model);
        proxyModel->setFilterKeyColumn(0);
        ui->listView->setViewMode(QListView::IconMode);
        ui->listView->setModel(model);
        ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked);
        connect(ui->listView, &QListView::doubleClicked, this, &LoadFolderWidget::showImage);
        char *p;
        std::string tmp3 = pathListRawFull.at(0).toStdString();
        char *tmp4 = const_cast<char*>(tmp3.c_str());
        p = strtok(tmp4, ".");
        p = strtok(NULL, "\\eof");
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "Radium_App","DataEchographie");
        settings.setValue("NameFolder", directoryRaw.dirName());
        settings.setValue("Path", directoryRaw.path());
        settings.setValue("SizeOfFiles", pathListRawFull.size());
        settings.setValue("Type", p);
        //ui->saveChanged->setEnabled(true);
        //ui->saveAllSegment->setEnabled(true);
        creatDataCNN=false;
        connect( ui->boxListImageOrigine,SIGNAL(activated(int)), this, SLOT(touchListUltrasound()));
        }
        }
        else
            _content.messBoxInfo("Nothing Selected !!!");
    }
    else if (res == QMessageBox::No){
        _content.messBoxInfo("Nothing Selected !!!");
    }
    }
}
void LoadFolderWidget::touchListUltrasound(){
    if(ui->boxListImageOrigine->currentIndex()==-1){
        _content.messBoxInfo("Have no data of image ultrasound");
    }
    else{
    int tmp;
    int tmp1;
    if((ui->boxImageUnet->currentIndex()==-1)&&(!creatDataCNN)){
        QString folderCNN =directoryRaw.path();
        folderCNN+="_Result";
        if(!QDir(folderCNN).exists()){
            pathListUnetFull.clear();
            _dataCNN.clear();
            QDir().mkdir(folderCNN);
            for(int i = 0;i<_dataRaw.size();i++){
                auto pathRaw = QFileInfo(QString::fromStdString(_dataRaw.at(i).path));
                QString pathCNN=folderCNN;
                pathCNN+="/";
                pathCNN+=pathRaw.fileName();
                QImage imageCNN = _dataRaw.at(i).image;
                for(int j=0;j<imageCNN.width();j++)
                    for(int k=0;k<imageCNN.height();k++)
                        imageCNN.setPixel(j,k,Qt::black);
                imageCNN.save(pathCNN);
                Ra::Engine::TextureParameters texPara = _content.getTextureParameterFromPath(pathCNN.toStdString());
                Ra::Engine::Dataset data;
                data.path=pathCNN.toStdString();
                data.image=imageCNN;
                data.textureParametres=texPara;
                pathListUnetFull.push_back(pathCNN);
                _dataCNN.push_back(data);
            }
            directoryCNN = QDir(folderCNN);
            creatDataCNN = true;
        }
        else {
            pathListUnetFull.clear();
            _dataCNN.clear();
            for(int i = 0;i<_dataRaw.size();i++){
                auto pathRaw = QFileInfo(QString::fromStdString(_dataRaw.at(i).path));
                QString pathCNN=folderCNN;
                pathCNN+="/";
                pathCNN+=pathRaw.fileName();
                QImage imageCNN = _dataRaw.at(i).image;
                for(int j=0;j<imageCNN.width();j++)
                    for(int k=0;k<imageCNN.height();k++)
                        imageCNN.setPixel(j,k,Qt::black);
                imageCNN.save(pathCNN);
                Ra::Engine::TextureParameters texPara = _content.getTextureParameterFromPath(pathCNN.toStdString());
                Ra::Engine::Dataset data;
                data.path=pathCNN.toStdString();
                data.image=imageCNN;
                data.textureParametres=texPara;
                pathListUnetFull.push_back(pathCNN);
                _dataCNN.push_back(data);
                creatDataCNN = true;
                }
        }
    }
    tmp = ui->boxListImageOrigine->currentIndex();
    _content.setPathRaw(pathListRawFull.at(tmp).toStdString());
   // _pathRaw = ui->boxListImageOrigine->currentText().toStdString();
    tmp1 = ui->boxImageUnet->findText(ui->boxListImageOrigine->currentText());
//    std::cout<<"hehe___"<<ui->boxListImageOrigine->currentText().toStdString()<<std::endl;
    _content.getParameterProvider()->setSave(false);
    if(tmp1==-1){
       // _content.messBoxInfo("Have no data of CNN!!!");
            if((loadSeg==false)&&(etatShow==false)){
        int res = _content.messBoxYesNo("Have no mask. Do you want to come to segmenttation?");
        if(res == QMessageBox::No){
        //   _content.getParameterProvider()->setPathRaw(_content.getPathRaw());
          // _content.getParameterProvider()->setPathUnet(_content.getPathRaw());
            _content.getParameterProvider()->setIndexRaw(tmp);
            _content.getParameterProvider()->setIndexCNN(tmp);
            _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
            _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
            this->setIndexRaw(tmp);
            this->setIndexCNN(tmp);
             ui->listView->selectionModel()->reset();
             ui->listView->update();
           emit needUpdate();
           _content.getParameterProvider()->setCount(0);
           etatShow = true;

        }
        else {
          //  Segmentation *nw = new Segmentation();
           // nw->setPathIn(QString::fromStdString(_content.getPathRaw()));
           // nw->show();
          //  remove(pathOut.toStdString().c_str());
            pathOut.clear();
         //   _content.getParameterProvider()->setPathRaw(_content.getPathRaw());
            QString textScript="from keras.models import load_model\n";
            textScript+="from keras import models\n";
            textScript+="import numpy as np\n";
            textScript+="import cv2\n";
            textScript+="import matplotlib.pyplot as plt\n";
            textScript+="import skimage.io as io\n";
            textScript+="INPUT_SHAPE = 512\n";
            textScript+="OUTPUT_SHAPE = 512 \n";
            textScript+="model = load_model('";
            textScript+= "UNET_b_160_IOU.h5";
            textScript+="',compile= True)\n";
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
            textScript+="_predict_path('";
            textScript+=QString::fromStdString(_content.getPathRaw());
            textScript+="','";
            QFileInfo pathDirOut(QString::fromStdString(_content.getPathRaw()));
            pathOut = pathDirOut.fileName();
            textScript+=pathOut;
            textScript+="')";
        //    std::cout<<this->getPathProg().toStdString()<<std::endl;
         //   std::ofstream model ("unet.py");
           // model<<textScript.toStdString();
            Segmentation *nw = new Segmentation(this->getPathProg());
            nw->show();
          /*  QProcess p;
             QStringList params;
             params <<"C:/Users/aduongng/Desktop/project/App/Radium-2021-01-26-Windows/Radium-2021-01-26-Windows/bin/unet.py";
             p.start(this->getPathProg(), params);
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
             msgBox1.exec();}*/
          //  _content.getParameterProvider()->setPathUnet(_content.getPathRaw());
            _content.getParameterProvider()->setIndexRaw(tmp);
            _content.getParameterProvider()->setIndexCNN(tmp1);
            _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
            _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp1).textureParametres);
            this->setIndexRaw(tmp);
            this->setIndexCNN(tmp1);
              ui->listView->selectionModel()->reset();
              ui->listView->update();
            emit needUpdate();
           // _content.getParameterProvider()->setPathUnet(nw->getPathOut().toStdString());
           // std::cout<<"haha"<<nw->getPathOut().toStdString()<<std::endl;

            _content.getParameterProvider()->setCount(0);
            etatShow = true;

        }
    }
            else {
                _content.getParameterProvider()->setIndexRaw(tmp);
                _content.getParameterProvider()->setIndexCNN(tmp);
                _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
                _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
                this->setIndexRaw(tmp);
                this->setIndexCNN(tmp);
                 ui->listView->selectionModel()->reset();
                 ui->listView->update();
               emit needUpdate();
               _content.getParameterProvider()->setCount(0);
               etatShow = true;
            }
    }
    else {
     //   ui->boxImageUnet->setCurrentIndex(tmp);
       // _pathUnet = ui->boxImageUnet->currentText().toStdString();
      //  _content.setPathCNN(pathListUnetFull.at(tmp).toStdString());
         ui->boxImageUnet->setCurrentIndex(tmp1);
     //   _content.getParameterProvider()->setPathRaw(_content.getPathRaw());
       // _content.getParameterProvider()->setPathUnet(_content.getPathCNN());
         _content.getParameterProvider()->setIndexRaw(tmp);
         _content.getParameterProvider()->setIndexCNN(tmp1);
         _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
         _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp1).textureParametres);
         showed = true;
         this->setIndexRaw(tmp);
         this->setIndexCNN(tmp1);
          ui->listView->selectionModel()->reset();
          ui->listView->update();
        emit needUpdate();
        _content.getParameterProvider()->setCount(0);
        etatShow = true;

//            _content.setParameterProvider( _content.getParameterProvider());


      //  std::cout<<_pathRaw<<std::endl;
       // std::cout<<_pathUnet<<std::endl;
    //    ui->inversePixel->setEnabled(true);
        }
    }

}
void LoadFolderWidget::touchListSegmented(){
    if(ui->boxImageUnet->currentIndex()==-1){
        _content.messBoxInfo("Have no data of image Unet");
    }
    else{
    int tmp;
    int tmp1;
        _content.getParameterProvider()->setSave(true);
    tmp = ui->boxImageUnet->currentIndex();
   // _content.setPathCNN(pathListUnetFull.at(tmp).toStdString());
    //_pathUnet = ui->boxImageUnet->currentText().toStdString();
    tmp1 = ui->boxListImageOrigine->findText(ui->boxImageUnet->currentText());
    if(tmp1==-1){
        _content.messBoxInfo("Have no data of image Raw");
    }
    else{
       // ui->boxListImageOrigine->setCurrentIndex(tmp);
        //_pathRaw = ui->boxListImageOrigine->currentText().toStdString();
        //_content.setPathRaw(pathListRawFull.at(tmp).toStdString());

        ui->boxListImageOrigine->setCurrentIndex(tmp1);
        //_content.getParameterProvider()->setPathRaw( _content.getPathRaw());
        //_content.getParameterProvider()->setPathUnet( _content.getPathCNN());
        _content.getParameterProvider()->setIndexRaw(tmp1);
        _content.getParameterProvider()->setIndexCNN(tmp);
        _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp1).textureParametres);
        _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
        showed = true;
        this->setIndexRaw(tmp1);
        this->setIndexCNN(tmp);
        ui->listView->selectionModel()->reset();
        ui->listView->update();
        emit needUpdate();
        etatShow = true;
        _content.getParameterProvider()->setCount(0);

     //   _content.setParameterProvider( _content.getParameterProvider());
       // std::cout<<_pathRaw<<std::endl;
       // std::cout<<_pathUnet<<std::endl;
      //  ui->inversePixel->setEnabled(true);
    }
    }
}
void LoadFolderWidget::loadSegmented(){
    if(_drawing == true){
        int res1 = _content.messBoxYesNo("Save modified segmentation?");
         if (res1==QMessageBox::Yes){
             this->saveAll();
             _drawing =false;
             int res = _content.messBoxYesNo("Select segmented image folder?");
             showed = false;
            /* if(pathListUnetFull.size()!=0)
                 this->saveAll();*/
             if (res==QMessageBox::Yes){

                 directoryCNN = QFileDialog::getExistingDirectory(this, "Select Directory",QDir::homePath());

               //  directoryCNN.refresh();
                 if(directoryCNN.exists()){
                 QStringList nameList = directoryCNN.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP"<<"*.jpeg"<<"*.jfif"<<"*.pjpeg"<<"*.pjp"<<"*.JPEG",QDir::Files,QDir::Name|QDir::IgnoreCase);
                 QStringList nameListFiltre= _content.listFiltre(nameList);
                 _content.listPathFull(&pathListUnetFull,directoryCNN,nameListFiltre);
                 if(pathListUnetFull.size()==0){
                     _content.messBoxInfo("Nothing Files Images !!!");
                     ui->boxImageUnet->clear();
                     loadSeg = false;
                   _dataCNN.clear();
                   }
                 else{
                   ui->boxImageUnet->clear();
                 _dataCNN.clear();
                 Ra::Engine::Dataset dataset;
                 for(int i=0; i<pathListUnetFull.size();i++){
                     dataset.path=pathListUnetFull.at(i).toStdString();
                     dataset.textureParametres=_content.getTextureParameterFromPath(pathListUnetFull.at(i).toStdString());
                     dataset.image=_content.getImageFromPath(pathListUnetFull.at(i).toStdString());
                     QFileInfo f(pathListUnetFull.at(i));
                     char *p1;
                     std::string tmp6 = f.fileName().toStdString();
                     if(strchr(tmp6.c_str(),'_')==NULL){
                         char *tmp7 = const_cast<char*>(tmp6.c_str());
                         p1 = strtok(tmp7, "_");
                         dataset.name=p1;}
                     else{
                     char *tmp7 = const_cast<char*>(tmp6.c_str());
                     p1 = strtok(tmp7, "_");
                     dataset.name=p1;}
                     char *p;
                     std::string tmp3 = pathListUnetFull.at(i).toStdString();
                     char *tmp4 = const_cast<char*>(tmp3.c_str());
                     p = strtok(tmp4, ".");
                     p = strtok(NULL, "\\eof");
                     dataset.type = p;
                     _dataCNN.push_back(dataset);
                     loadSeg = true;
                 }
                 QStringList nameList1;
                 for(int i=0;i<_dataCNN.size();i++){
                     std::string strName =_dataCNN.at(i).name;
                     strName+=".";
                     strName+=_dataCNN.at(i).type;
                     nameList1.append(QString::fromStdString(strName));
                 }
                 ui->boxImageUnet->addItems(nameList1);
                 ui->boxImageUnet->setCurrentIndex(ui->boxListImageOrigine->currentIndex());
                 disconnect( ui->boxImageUnet,SIGNAL(activated(int)), this, SLOT(touchListSegmented()));
                 //ui->saveChanged->setEnabled(true);
                 //1ui->saveAllSegment->setEnabled(true);
                 char *p;
                 std::string tmp3 = pathListUnetFull.at(0).toStdString();
                 char *tmp4 = const_cast<char*>(tmp3.c_str());
                 p = strtok(tmp4, ".");
                 p = strtok(NULL, "\\eof");
                QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "Radium_App","DataSegmented");
                 settings.setValue("NameFolder", directoryCNN.dirName());
                 settings.setValue("Path", directoryCNN.path());
                 settings.setValue("SizeOfFiles", pathListUnetFull.size());
                 settings.setValue("Type", p);
             //    connect(ui->listView, &QListView::doubleClicked, this, &LoadFolderWidget::showImage);
                /* switch(typeCNN){
                 case Unet:{
                     ui->textMethodeJson->setText(QString::fromStdString(_content.methodeUnet(directoryRaw,directoryCNN,nameList)));
                 }
                     break;
                 case CNN2:
                     break;
                 case CNN3:
                     break;
                 }*/
                 connect( ui->boxImageUnet,SIGNAL(activated(int)), this, SLOT(touchListSegmented()));
                 }
         }
                 else{
                      _content.messBoxInfo("Nothing Selected !!!");
                      loadSeg = false;
                 }
             }
             else if (res == QMessageBox::No){
                  _content.messBoxInfo("Nothing Selected !!!");
                  loadSeg = false;
             }
         }
         else {
             int res = _content.messBoxYesNo("Select segmented image folder?");
             showed = false;
            /* if(pathListUnetFull.size()!=0)
                 this->saveAll();*/
             if (res==QMessageBox::Yes){

                 directoryCNN = QFileDialog::getExistingDirectory(this, "Select Directory",QDir::homePath());

               //  directoryCNN.refresh();
                 if(directoryCNN.exists()){
                 QStringList nameList = directoryCNN.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP"<<"*.jpeg"<<"*.jfif"<<"*.pjpeg"<<"*.pjp"<<"*.JPEG",QDir::Files,QDir::Name|QDir::IgnoreCase);
                 QStringList nameListFiltre= _content.listFiltre(nameList);
                 _content.listPathFull(&pathListUnetFull,directoryCNN,nameListFiltre);
                 if(pathListUnetFull.size()==0){
                     _content.messBoxInfo("Nothing Files Images !!!");
                     ui->boxImageUnet->clear();
                     loadSeg = false;
                   _dataCNN.clear();
                   }
                 else{
                   ui->boxImageUnet->clear();
                 _dataCNN.clear();
                 Ra::Engine::Dataset dataset;
                 for(int i=0; i<pathListUnetFull.size();i++){
                     dataset.path=pathListUnetFull.at(i).toStdString();
                     dataset.textureParametres=_content.getTextureParameterFromPath(pathListUnetFull.at(i).toStdString());
                     dataset.image=_content.getImageFromPath(pathListUnetFull.at(i).toStdString());
                     QFileInfo f(pathListUnetFull.at(i));
                     char *p1;
                     std::string tmp6 = f.fileName().toStdString();
                     if(strchr(tmp6.c_str(),'_')==NULL){
                         char *tmp7 = const_cast<char*>(tmp6.c_str());
                         p1 = strtok(tmp7, "_");
                         dataset.name=p1;}
                     else{
                     char *tmp7 = const_cast<char*>(tmp6.c_str());
                     p1 = strtok(tmp7, "_");
                     dataset.name=p1;}
                     char *p;
                     std::string tmp3 = pathListUnetFull.at(i).toStdString();
                     char *tmp4 = const_cast<char*>(tmp3.c_str());
                     p = strtok(tmp4, ".");
                     p = strtok(NULL, "\\eof");
                     dataset.type = p;
                     _dataCNN.push_back(dataset);
                     loadSeg = true;
                 }
                 QStringList nameList1;
                 for(int i=0;i<_dataCNN.size();i++){
                     std::string strName =_dataCNN.at(i).name;
                     strName+=".";
                     strName+=_dataCNN.at(i).type;
                     nameList1.append(QString::fromStdString(strName));
                 }
                 ui->boxImageUnet->addItems(nameList1);
                 ui->boxImageUnet->setCurrentIndex(ui->boxListImageOrigine->currentIndex());
                 disconnect( ui->boxImageUnet,SIGNAL(activated(int)), this, SLOT(touchListSegmented()));
                 _drawing =false;
                 //ui->saveChanged->setEnabled(true);
                 //1ui->saveAllSegment->setEnabled(true);
                 char *p;
                 std::string tmp3 = pathListUnetFull.at(0).toStdString();
                 char *tmp4 = const_cast<char*>(tmp3.c_str());
                 p = strtok(tmp4, ".");
                 p = strtok(NULL, "\\eof");
                QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "Radium_App","DataSegmented");
                 settings.setValue("NameFolder", directoryCNN.dirName());
                 settings.setValue("Path", directoryCNN.path());
                 settings.setValue("SizeOfFiles", pathListUnetFull.size());
                 settings.setValue("Type", p);
             //    connect(ui->listView, &QListView::doubleClicked, this, &LoadFolderWidget::showImage);
                /* switch(typeCNN){
                 case Unet:{
                     ui->textMethodeJson->setText(QString::fromStdString(_content.methodeUnet(directoryRaw,directoryCNN,nameList)));
                 }
                     break;
                 case CNN2:
                     break;
                 case CNN3:
                     break;
                 }*/
                 connect( ui->boxImageUnet,SIGNAL(activated(int)), this, SLOT(touchListSegmented()));
                 }
         }
                 else{
                      _content.messBoxInfo("Nothing Selected !!!");
                      loadSeg = false;
                 }
             }
             else if (res == QMessageBox::No){
                  _content.messBoxInfo("Nothing Selected !!!");
                  loadSeg = false;
             }
         }

         }


     else  {
    int res = _content.messBoxYesNo("Select segmented image folder?");
    showed = false;
  /*  if(pathListUnetFull.size()!=0)
        this->saveAll();*/
    if (res==QMessageBox::Yes){

        directoryCNN = QFileDialog::getExistingDirectory(this, "Select Directory",QDir::homePath());

      //  directoryCNN.refresh();
        if(directoryCNN.exists()){
        QStringList nameList = directoryCNN.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP"<<"*.jpeg"<<"*.jfif"<<"*.pjpeg"<<"*.pjp"<<"*.JPEG",QDir::Files,QDir::Name|QDir::IgnoreCase);
        QStringList nameListFiltre= _content.listFiltre(nameList);
        _content.listPathFull(&pathListUnetFull,directoryCNN,nameListFiltre);
        if(pathListUnetFull.size()==0){
            _content.messBoxInfo("Nothing Files Images !!!");
            ui->boxImageUnet->clear();
            loadSeg = false;
          _dataCNN.clear();
          }
        else{
          ui->boxImageUnet->clear();
        _dataCNN.clear();
        Ra::Engine::Dataset dataset;
        for(int i=0; i<pathListUnetFull.size();i++){
            dataset.path=pathListUnetFull.at(i).toStdString();
            dataset.textureParametres=_content.getTextureParameterFromPath(pathListUnetFull.at(i).toStdString());
            dataset.image=_content.getImageFromPath(pathListUnetFull.at(i).toStdString());
            QFileInfo f(pathListUnetFull.at(i));
            char *p1;
            std::string tmp6 = f.fileName().toStdString();
            if(strchr(tmp6.c_str(),'_')==NULL){
                char *tmp7 = const_cast<char*>(tmp6.c_str());
                p1 = strtok(tmp7, ".");
                dataset.name=p1;}
            else{
            char *tmp7 = const_cast<char*>(tmp6.c_str());
            p1 = strtok(tmp7, "_");
            dataset.name=p1;}
            char *p;
            std::string tmp3 = pathListUnetFull.at(i).toStdString();
            char *tmp4 = const_cast<char*>(tmp3.c_str());
            p = strtok(tmp4, ".");
            p = strtok(NULL, "\\eof");
            dataset.type = p;
            _dataCNN.push_back(dataset);
            loadSeg = true;
        }
        QStringList nameList1;
        for(int i=0;i<_dataCNN.size();i++){
            std::string strName =_dataCNN.at(i).name;
            strName+=".";
            strName+=_dataCNN.at(i).type;
            nameList1.append(QString::fromStdString(strName));
        }
        ui->boxImageUnet->addItems(nameList1);
        ui->boxImageUnet->setCurrentIndex(ui->boxListImageOrigine->currentIndex());
        disconnect( ui->boxImageUnet,SIGNAL(activated(int)), this, SLOT(touchListSegmented()));
        //ui->saveChanged->setEnabled(true);
        //1ui->saveAllSegment->setEnabled(true);
        char *p;
        std::string tmp3 = pathListUnetFull.at(0).toStdString();
        char *tmp4 = const_cast<char*>(tmp3.c_str());
        p = strtok(tmp4, ".");
        p = strtok(NULL, "\\eof");
        _drawing =false;
       QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "Radium_App","DataSegmented");
        settings.setValue("NameFolder", directoryCNN.dirName());
        settings.setValue("Path", directoryCNN.path());
        settings.setValue("SizeOfFiles", pathListUnetFull.size());
        settings.setValue("Type", p);
    //    connect(ui->listView, &QListView::doubleClicked, this, &LoadFolderWidget::showImage);
       /* switch(typeCNN){
        case Unet:{
            ui->textMethodeJson->setText(QString::fromStdString(_content.methodeUnet(directoryRaw,directoryCNN,nameList)));
        }
            break;
        case CNN2:
            break;
        case CNN3:
            break;
        }*/
        connect( ui->boxImageUnet,SIGNAL(activated(int)), this, SLOT(touchListSegmented()));
        }
}
        else{
             _content.messBoxInfo("Nothing Selected !!!");
             loadSeg = false;
        }
    }
    else if (res == QMessageBox::No){
         _content.messBoxInfo("Nothing Selected !!!");
         loadSeg = false;
    }
    }
}
void LoadFolderWidget::runImage()
{
    switch(typeRun){
    case Loadata:{
        int res = _content.messBoxYesNo("Select ultrasound image folder?");
        showed = false;
        if (res==QMessageBox::Yes){
            ui->loadUnet->setEnabled(true);
            directoryRaw = QFileDialog::getExistingDirectory(this, "Select Directory",QDir::homePath());          
            if(directoryRaw.exists()){
            QStringList nameList = directoryRaw.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP",QDir::Files);           
          //  ui->textDatasetJson->setText(QString::fromStdString());
            infoImage = QString::fromStdString(_content.textDataRaw(&pathListRawFull,directoryRaw,nameList));
            if(pathListRawFull.size()==0){
                _content.messBoxInfo("Nothing Files Images !!!");
                _dataRaw.clear();
                ui->boxListImageOrigine->clear();
                model->clear();
                ui->listView->clearPropertyFlags();
            }
            else{
            _dataRaw.clear();
            ui->boxListImageOrigine->clear();
            model->clear();
            ui->listView->clearPropertyFlags();
            Ra::Engine::Dataset dataset;
            for(int i=0; i<pathListRawFull.size();i++){
                dataset.path=pathListRawFull.at(i).toStdString();
                dataset.textureParametres=_content.getTextureParameterFromPath(pathListRawFull.at(i).toStdString());
                dataset.image=_content.getImageFromPath(pathListRawFull.at(i).toStdString());
                QFileInfo f(pathListUnetFull.at(i));
                char *p1;
                std::string tmp6 = f.fileName().toStdString();
                if(strchr(tmp6.c_str(),'_')==NULL)
                    dataset.name=p1;
                else{
                char *tmp7 = const_cast<char*>(tmp6.c_str());
                p1 = strtok(tmp7, "_");
                dataset.name=p1;}
                char *p;
                std::string tmp3 = pathListUnetFull.at(i).toStdString();
                char *tmp4 = const_cast<char*>(tmp3.c_str());
                p = strtok(tmp4, ".");
                p = strtok(NULL, "\\eof");
                dataset.type = p;
                _dataRaw.push_back(dataset);
            }
           // std::cout<<pathListRawFull.size();
            ui->boxListImageOrigine->addItems(nameList);
          //  ui->textMethodeJson->setEnabled(false);
            ui->buttonRunUnet->setEnabled(true);
            const int numRows = pathListRawFull.size();

            model->insertRows(0,numRows);
            for(int i=0;i<numRows ;++i)
            {
            QFileInfo nameImage(pathListRawFull.at(i));
           // model->setData(model->index(i,0),QPixmap(thumbnails.at(i)),Qt::DecorationRole);
           // model->setData(model->index(i,1),nameImage.fileName());
            QPixmap p(pathListRawFull.at(i));
            QIcon *icon = new QIcon(p);
            QStandardItem *item = new QStandardItem(*icon, nameImage.fileName());
            model->appendRow(item);
            }
            proxyModel->setSourceModel(model);
            proxyModel->setFilterKeyColumn(0);
            ui->listView->setViewMode(QListView::IconMode);
            ui->listView->setModel(model);
            ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked);
            connect(ui->listView, &QListView::doubleClicked, this, &LoadFolderWidget::showImage);
            char *p;
            std::string tmp3 = pathListRawFull.at(0).toStdString();
            char *tmp4 = const_cast<char*>(tmp3.c_str());
            p = strtok(tmp4, ".");
            p = strtok(NULL, "\\eof");
            QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "Radium_App","DataEchographie");
            settings.setValue("NameFolder", directoryRaw.dirName());
            settings.setValue("Path", directoryRaw.path());
            settings.setValue("SizeOfFiles", pathListRawFull.size());
            settings.setValue("Type", p);
            //ui->saveChanged->setEnabled(true);
            //ui->saveAllSegment->setEnabled(true);
            creatDataCNN=false;
            }
            }
            else
                _content.messBoxInfo("Nothing Selected !!!");
        }
        else if (res == QMessageBox::No){
            _content.messBoxInfo("Nothing Selected !!!");
        }
    }
        break;
    case ShowImage:{

        if(ui->boxListImageOrigine->currentIndex()==-1){
            _content.messBoxInfo("Have no data of image ultrasound");
        }
        else{
        int tmp;
        int tmp1;
        if((ui->boxImageUnet->currentIndex()==-1)&&(!creatDataCNN)){
            QString folderCNN =directoryRaw.path();
            folderCNN+="_Result";
            if(!QDir(folderCNN).exists()){
                pathListUnetFull.clear();
                _dataCNN.clear();
                QDir().mkdir(folderCNN);
                for(int i = 0;i<_dataRaw.size();i++){
                    auto pathRaw = QFileInfo(QString::fromStdString(_dataRaw.at(i).path));
                    QString pathCNN=folderCNN;
                    pathCNN+="/";
                    pathCNN+=pathRaw.fileName();
                    QImage imageCNN = _dataRaw.at(i).image;
                    for(int j=0;j<imageCNN.width();j++)
                        for(int k=0;k<imageCNN.height();k++)
                            imageCNN.setPixel(j,k,Qt::black);
                    imageCNN.save(pathCNN);
                    Ra::Engine::TextureParameters texPara = _content.getTextureParameterFromPath(pathCNN.toStdString());
                    Ra::Engine::Dataset data;
                    data.path=pathCNN.toStdString();
                    data.image=imageCNN;
                    data.textureParametres=texPara;
                    pathListUnetFull.push_back(pathCNN);
                    _dataCNN.push_back(data);
                }
                directoryCNN = QDir(folderCNN);
                creatDataCNN = true;
            }
            else {
                pathListUnetFull.clear();
                _dataCNN.clear();
                for(int i = 0;i<_dataRaw.size();i++){
                    auto pathRaw = QFileInfo(QString::fromStdString(_dataRaw.at(i).path));
                    QString pathCNN=folderCNN;
                    pathCNN+="/";
                    pathCNN+=pathRaw.fileName();
                    QImage imageCNN = _dataRaw.at(i).image;
                    for(int j=0;j<imageCNN.width();j++)
                        for(int k=0;k<imageCNN.height();k++)
                            imageCNN.setPixel(j,k,Qt::black);
                    imageCNN.save(pathCNN);
                    Ra::Engine::TextureParameters texPara = _content.getTextureParameterFromPath(pathCNN.toStdString());
                    Ra::Engine::Dataset data;
                    data.path=pathCNN.toStdString();
                    data.image=imageCNN;
                    data.textureParametres=texPara;
                    pathListUnetFull.push_back(pathCNN);
                    _dataCNN.push_back(data);
                    creatDataCNN = true;
                    }
            }
        }
        if((_content.getParameterProvider()->getCount()!=0)&&(_content.getParameterProvider()->getSave()==false)){
            int res = _content.messBoxYesNo("Do you want stay to save image?");
            if(res == QMessageBox::No){
                tmp = ui->boxListImageOrigine->currentIndex();
              //  _content.setPathRaw(pathListRawFull.at(tmp).toStdString());
               // _pathRaw = ui->boxListImageOrigine->currentText().toStdString();
                tmp1 = ui->boxImageUnet->findText(ui->boxListImageOrigine->currentText());
             //   std::cout<<"hehe___"<<ui->boxListImageOrigine->currentText().toStdString()<<std::endl;
                if(tmp1==-1){
                   // _content.messBoxInfo("Have no data of CNN!!!");
                    int res = _content.messBoxYesNo("Have no mask. Do you want to come to segmenttation?");
                    if(res == QMessageBox::No){
                     //  _content.getParameterProvider()->setPathRaw(_content.getPathRaw());
                      // _content.getParameterProvider()->setPathUnet(_content.getPathRaw());
                        _content.getParameterProvider()->setIndexRaw(tmp);
                        _content.getParameterProvider()->setIndexCNN(tmp);
                        _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
                        _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
                        this->setIndexRaw(tmp);
                        this->setIndexCNN(tmp);
                       _content.getParameterProvider()->setCount(0);
                       showed = true;
                    }
                    else {
                      //  Segmentation *nw = new Segmentation();
                       // nw->setPathIn(QString::fromStdString(_content.getPathRaw()));
                       // nw->show();

                        pathOut.clear();
                    //    _content.getParameterProvider()->setPathRaw(_content.getPathRaw());
                    /*    QString textScript="from keras.models import load_model\n";
                        textScript+="from keras import models\n";
                        textScript+="import numpy as np\n";
                        textScript+="import cv2\n";
                        textScript+="import matplotlib.pyplot as plt\n";
                        textScript+="import skimage.io as io\n";
                        textScript+="INPUT_SHAPE = 512\n";
                        textScript+="OUTPUT_SHAPE = 512 \n";
                        textScript+="model = load_model('";
                        textScript+= "UNET_b_160_IOU.h5";
                        textScript+="',compile= True)\n";
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
                        textScript+="_predict_path('";
                        textScript+=QString::fromStdString(_content.getPathRaw());
                        textScript+="','";
                        QFileInfo pathDirOut(QString::fromStdString(_content.getPathRaw()));
                        pathOut = pathDirOut.fileName();
                        textScript+=pathOut;
                        textScript+="')";*/
                    //    std::cout<<this->getPathProg().toStdString()<<std::endl;
                      //  std::ofstream model ("unet.py");
                       // model<<textScript.toStdString();
                        Segmentation *nw = new Segmentation(this->getPathProg());
                        nw->show();
                       /* QProcess p;
                        QStringList params;
                         params <<"C:/Users/aduongng/Desktop/project/App/Radium-2021-01-26-Windows/Radium-2021-01-26-Windows/bin/unet.py";
                         p.start(this->getPathProg(), params);
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
                         msgBox1.exec();}*/
                      //  _content.getParameterProvider()->setPathUnet(_content.getPathRaw());
                        _content.getParameterProvider()->setIndexRaw(tmp);
                        _content.getParameterProvider()->setIndexCNN(tmp);
                        _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
                        _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
                        this->setIndexRaw(tmp);
                        this->setIndexCNN(tmp);
                        showed = true;
                       // _content.getParameterProvider()->setPathUnet(nw->getPathOut().toStdString());
                       // std::cout<<"haha"<<nw->getPathOut().toStdString()<<std::endl;

                        _content.getParameterProvider()->setCount(0);
            }
        }
                else{
                    //_content.setPathRaw(nameItemRaw.toStdString());
                   // _content.setPathCNN(nameItemCNN.toStdString());
                  //  ui->boxImageUnet->setCurrentIndex(tmp);
                 //  _content.getParameterProvider()->setPathRaw(_content.getPathRaw());
                   //_content.getParameterProvider()->setPathUnet(_content.getPathCNN());
                    _content.getParameterProvider()->setIndexRaw(tmp);
                    _content.getParameterProvider()->setIndexCNN(tmp1);
                    _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
                    _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp1).textureParametres);
                    ui->boxListImageOrigine->setCurrentIndex(tmp);
                    ui->boxImageUnet->setCurrentIndex(tmp1);
                   showed=true;
                   this->setIndexRaw(tmp);
                   this->setIndexCNN(tmp1);
                   emit needUpdate();
                   _content.getParameterProvider()->setCount(0);
                }
      }
            else if(res == QMessageBox::Yes){
            }

        }else{
        tmp = ui->boxListImageOrigine->currentIndex();
        _content.setPathRaw(pathListRawFull.at(tmp).toStdString());
       // _pathRaw = ui->boxListImageOrigine->currentText().toStdString();
        tmp1 = ui->boxImageUnet->findText(ui->boxListImageOrigine->currentText());
    //    std::cout<<"hehe___"<<ui->boxListImageOrigine->currentText().toStdString()<<std::endl;
        _content.getParameterProvider()->setSave(false);
        if(tmp1==-1){
           // _content.messBoxInfo("Have no data of CNN!!!");
            int res = _content.messBoxYesNo("Have no mask. Do you want to come to segmenttation?");
            if(res == QMessageBox::No){
            //   _content.getParameterProvider()->setPathRaw(_content.getPathRaw());
              // _content.getParameterProvider()->setPathUnet(_content.getPathRaw());
                _content.getParameterProvider()->setIndexRaw(tmp);
                _content.getParameterProvider()->setIndexCNN(tmp);
                _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
                _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
                this->setIndexRaw(tmp);
                this->setIndexCNN(tmp);
               emit needUpdate();
               _content.getParameterProvider()->setCount(0);
            }
            else {
              //  Segmentation *nw = new Segmentation();
               // nw->setPathIn(QString::fromStdString(_content.getPathRaw()));
               // nw->show();
              //  remove(pathOut.toStdString().c_str());
                pathOut.clear();
             //   _content.getParameterProvider()->setPathRaw(_content.getPathRaw());
                QString textScript="from keras.models import load_model\n";
                textScript+="from keras import models\n";
                textScript+="import numpy as np\n";
                textScript+="import cv2\n";
                textScript+="import matplotlib.pyplot as plt\n";
                textScript+="import skimage.io as io\n";
                textScript+="INPUT_SHAPE = 512\n";
                textScript+="OUTPUT_SHAPE = 512 \n";
                textScript+="model = load_model('";
                textScript+= "UNET_b_160_IOU.h5";
                textScript+="',compile= True)\n";
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
                textScript+="_predict_path('";
                textScript+=QString::fromStdString(_content.getPathRaw());
                textScript+="','";
                QFileInfo pathDirOut(QString::fromStdString(_content.getPathRaw()));
                pathOut = pathDirOut.fileName();
                textScript+=pathOut;
                textScript+="')";
            //    std::cout<<this->getPathProg().toStdString()<<std::endl;
                std::ofstream model ("unet.py");
                model<<textScript.toStdString();
                Segmentation *nw = new Segmentation(this->getPathProg());
                nw->show();
              /*  QProcess p;
                 QStringList params;
                 params <<"C:/Users/aduongng/Desktop/project/App/Radium-2021-01-26-Windows/Radium-2021-01-26-Windows/bin/unet.py";
                 p.start(this->getPathProg(), params);
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
                 msgBox1.exec();}*/
              //  _content.getParameterProvider()->setPathUnet(_content.getPathRaw());
                _content.getParameterProvider()->setIndexRaw(tmp);
                _content.getParameterProvider()->setIndexCNN(tmp);
                _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
                _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
                this->setIndexRaw(tmp);
                this->setIndexCNN(tmp1);
                emit needUpdate();
               // _content.getParameterProvider()->setPathUnet(nw->getPathOut().toStdString());
               // std::cout<<"haha"<<nw->getPathOut().toStdString()<<std::endl;

                _content.getParameterProvider()->setCount(0);
            }
        }
        else {
         //   ui->boxImageUnet->setCurrentIndex(tmp);
           // _pathUnet = ui->boxImageUnet->currentText().toStdString();
          //  _content.setPathCNN(pathListUnetFull.at(tmp).toStdString());
             ui->boxImageUnet->setCurrentIndex(tmp1);
         //   _content.getParameterProvider()->setPathRaw(_content.getPathRaw());
           // _content.getParameterProvider()->setPathUnet(_content.getPathCNN());
             _content.getParameterProvider()->setIndexRaw(tmp);
             _content.getParameterProvider()->setIndexCNN(tmp1);
             _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp).textureParametres);
             _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp1).textureParametres);
             showed = true;
             this->setIndexRaw(tmp);
             this->setIndexCNN(tmp1);
            emit needUpdate();
            _content.getParameterProvider()->setCount(0);
//            _content.setParameterProvider( _content.getParameterProvider());


          //  std::cout<<_pathRaw<<std::endl;
           // std::cout<<_pathUnet<<std::endl;
        //    ui->inversePixel->setEnabled(true);
            }
        }
        }
        break;
    }
   case SaveJson:
    /*    if(ui->textDatasetJson->document()->isEmpty()){
            _content.messBoxInfo("Dataset.json is empty !!!");
        }*/

            QString path = QFileDialog::getSaveFileName(this, "Save Image", "/home",
                                                                   "Json(*.json) ");
            if(path == ""){
              _content.messBoxInfo("No directory");

             }
             else{
                std::ofstream dataset(path.toStdString());
                dataset<<infoImage.toStdString();
                dataset.close();
            }
        break;
 }
}

/*void LoadFolderWidget::checkUnet()
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
*/
void LoadFolderWidget::runUnet()
{
    switch(typeRun){
    case Loadata:{
        int res = _content.messBoxYesNo("Select segmented image folder?");
        showed = false;
        if(pathListUnetFull.size()!=0)
            this->saveAll();
        if (res==QMessageBox::Yes){

            directoryCNN = QFileDialog::getExistingDirectory(this, "Select Directory",QDir::homePath());
          //  directoryCNN.refresh();
            if(directoryCNN.exists()){
            QStringList nameList = directoryCNN.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.bmp" << "*.BMP",QDir::Files);
            _content.listPathFull(&pathListUnetFull,directoryCNN,nameList);
            if(pathListUnetFull.size()==0){
                _content.messBoxInfo("Nothing Files Images !!!");
                ui->boxImageUnet->clear();
              _dataCNN.clear();
              }
            else{
              ui->boxImageUnet->clear();
            _dataCNN.clear();
            Ra::Engine::Dataset dataset;
            for(int i=0; i<pathListUnetFull.size();i++){
                dataset.path=pathListUnetFull.at(i).toStdString();
                dataset.textureParametres=_content.getTextureParameterFromPath(pathListUnetFull.at(i).toStdString());
                dataset.image=_content.getImageFromPath(pathListUnetFull.at(i).toStdString());
                _dataCNN.push_back(dataset);
            }
            ui->boxImageUnet->addItems(nameList);
         //   ui->saveChanged->setEnabled(true);
          //  ui->saveAllSegment->setEnabled(true);
            char *p;
            std::string tmp3 = pathListUnetFull.at(0).toStdString();
            char *tmp4 = const_cast<char*>(tmp3.c_str());
            p = strtok(tmp4, ".");
            p = strtok(NULL, "\\eof");
           QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "Radium_App","DataSegmented");
            settings.setValue("NameFolder", directoryCNN.dirName());
            settings.setValue("Path", directoryCNN.path());
            settings.setValue("SizeOfFiles", pathListUnetFull.size());
            settings.setValue("Type", p);
        //    connect(ui->listView, &QListView::doubleClicked, this, &LoadFolderWidget::showImage);
           /* switch(typeCNN){
            case Unet:{
                ui->textMethodeJson->setText(QString::fromStdString(_content.methodeUnet(directoryRaw,directoryCNN,nameList)));
            }
                break;
            case CNN2:
                break;
            case CNN3:
                break;
            }*/
            }
}
            else{
                 _content.messBoxInfo("Nothing Selected !!!");
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
        int tmp1;
        tmp = ui->boxImageUnet->currentIndex();
       // _content.setPathCNN(pathListUnetFull.at(tmp).toStdString());
        //_pathUnet = ui->boxImageUnet->currentText().toStdString();
        tmp1 = ui->boxListImageOrigine->findText(ui->boxImageUnet->currentText());
        if(tmp1==-1){
            _content.messBoxInfo("Have no data of image Raw");
        }
        else{
           // ui->boxListImageOrigine->setCurrentIndex(tmp);
            //_pathRaw = ui->boxListImageOrigine->currentText().toStdString();
            //_content.setPathRaw(pathListRawFull.at(tmp).toStdString());

            ui->boxListImageOrigine->setCurrentIndex(tmp1);
            //_content.getParameterProvider()->setPathRaw( _content.getPathRaw());
            //_content.getParameterProvider()->setPathUnet( _content.getPathCNN());
            _content.getParameterProvider()->setIndexRaw(tmp1);
            _content.getParameterProvider()->setIndexCNN(tmp);
            _content.getParameterProvider()->setTextureParameterRaw(_dataRaw.at(tmp1).textureParametres);
            _content.getParameterProvider()->setTextureParameterCNN(_dataCNN.at(tmp).textureParametres);
            showed = true;
            this->setIndexRaw(tmp1);
            this->setIndexCNN(tmp);
            emit needUpdate();
            _content.getParameterProvider()->setCount(0);
         //   _content.setParameterProvider( _content.getParameterProvider());
           // std::cout<<_pathRaw<<std::endl;
           // std::cout<<_pathUnet<<std::endl;
          //  ui->inversePixel->setEnabled(true);
        }
        }
        break;
    }
    case SaveJson:{
     /*   if(ui->textMethodeJson->document()->isEmpty()){
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

        }*/
         _content.messBoxInfo("Nothing to save !!!");
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
void LoadFolderWidget::saveImageChanged(){
   /* if(etatShow == true){
    auto dirFix = this->getDirCNN();
    QString folderSave = dirFix.path();
    QDateTime current = QDateTime::currentDateTime();
    folderSave+="_";
    folderSave+=current.date().toString("ddMMyyyy");
    folderSave+="_";
    folderSave+=current.time().toString("hhmm");
    std::cout<<folderSave.toStdString()<<std::endl;
    if(QDir(folderSave).exists()){}
    else {
        QDir().mkdir(folderSave);
        std::cout<<"hehe"<<std::endl;}
        auto pathTexture = QFileInfo(QString::fromStdString(_dataCNN.at(_content.getParameterProvider()->getIndexCNN()).path));
        folderSave+="/";
        folderSave+=pathTexture.fileName();
        QImageReader reader_;
        QSettings settings;
        reader_.setFileName(this->getPathTmp());
        QImage image_ = reader_.read();*/
        Ra::Engine::Dataset data;
        data.path=_dataCNN.at(_content.getParameterProvider()->getIndexCNN()).path;
        data.image = _content.getImageFromPath(this->getPathTmp().toStdString());;
        data.textureParametres = _content.getTextureParameterFromPath(this->getPathTmp().toStdString());
    //    data.image.save(QString::fromStdString(folderSave.toStdString()));
        QVector<Ra::Engine::Dataset> dataset;
        dataset.clear();
        for(int i=0;i<_dataCNN.size();i++){
            if(i!=_content.getParameterProvider()->getIndexCNN())
                dataset.push_back(_dataCNN.at(i));
            else
                dataset.push_back(data);
        }
        this->setDatasetCNN(dataset);
        _content.getParameterProvider()->setSave(true);
}
const QString&  LoadFolderWidget::getPathProg() const{
    return _pathProg;
}
void LoadFolderWidget::setPathProg(const QString &pathProg){
    _pathProg = pathProg;
}
const QDir&  LoadFolderWidget::getDirCNN() const{
    return directoryCNN;
}
void  LoadFolderWidget::setPathTmp(const QString &pathTmp){

    _pathtmp =  pathTmp;
}
const QString&  LoadFolderWidget::getPathTmp() const{
    return _pathtmp;
}
void LoadFolderWidget::saveAll(){
    if(pathListUnetFull.size()!=0){
    auto dirFix = this->getDirCNN();
    QString folderModifided = dirFix.path();
    //folderSave+="_changed";
    QDateTime current = QDateTime::currentDateTime();
    folderModifided+="_";
    folderModifided+=current.date().toString("yyyyMMdd");
    folderModifided+="_";
    folderModifided+=current.time().toString("hhmm");
    folderSave = folderModifided;
    this->setDirModified(folderModifided);
        QDir().mkdir(folderModifided);
        for(int i=0; i<pathListUnetFull.size();i++){
        QString filename ="";
        filename+= folderSave;
        auto pathTexture = QFileInfo(pathListUnetFull.at(i));
        filename+="/";
        filename+=pathTexture.fileName();
      /*  QImageReader reader_;
        QSettings settings;
        reader_.setFileName(pathListUnetFull.at(i));
        QImage image_ = reader_.read();
        image_.save(filename);*/
        _dataCNN.at(i).image.save(filename);
      //  std::cout<<filename.toStdString()<<std::endl;
        }
    }
    QDir dir(this->getDirModified());
    char *p;
    std::string tmp3 = pathListUnetFull.at(0).toStdString();
    char *tmp4 = const_cast<char*>(tmp3.c_str());
    p = strtok(tmp4, ".");
    p = strtok(NULL, "\\eof");
    QSettings setting(QSettings::NativeFormat, QSettings::UserScope, "Radium_App", "DataSegmentModified");
    setting.setValue("NameFolder", dir.dirName());
    setting.setValue("Path", folderSave);
    setting.setValue("SizeOfFiles", pathListUnetFull.size());
    setting.setValue("Type", p);
       // _content.getParameterProvider()->setSave(true);
}

const QStringList& LoadFolderWidget::getPathListCNNFull() const{
    return pathListUnetFull;
}
const QStringList& LoadFolderWidget::getPathListRawFull() const{
    return pathListRawFull;
}
bool LoadFolderWidget::showedImage(){
    return showed;
}
void LoadFolderWidget::setIndexRaw(const int& i){
    indexRaw = i;
}
const int& LoadFolderWidget::getIndexRaw() const{
    return indexRaw;
}
void LoadFolderWidget::setIndexCNN(const int& i){
    indexCNN = i;
}
const int& LoadFolderWidget::getIndexCNN() const{
    return indexCNN;
}
const QVector <Ra::Engine::Dataset>& LoadFolderWidget::getDatasetRaw() const{
    return _dataRaw;
}
const QVector <Ra::Engine::Dataset>& LoadFolderWidget::getDatasetCNN() const{
    return _dataCNN;
}
void LoadFolderWidget::setDatasetRaw(QVector <Ra::Engine::Dataset> &dataRaw){
    _dataRaw.clear();
    for (int i =0;i<dataRaw.size();i++)
        _dataRaw.push_back(dataRaw.at(i));
}
void LoadFolderWidget::setDatasetCNN(QVector <Ra::Engine::Dataset> &dataCNN){
    _dataCNN.clear();
    for (int i =0;i<dataCNN.size();i++)
        _dataCNN.push_back(dataCNN.at(i));
}
/*const QSettings& LoadFolderWidget::getSetting() const{
    return _setting;
}*/
/*bool LoadFolderWidget::eventFilter(QObject *obj, QEvent *evt){
    if((evt->type()== QEvent::MouseButtonPress)&&(obj==ui->boxListImageOrigine)){
        std::cout<<"hihi"<<std::endl;
        this->touchListUtralsound();
        return true;
    }
    if((evt->type()== QEvent::MouseButtonPress)&&(obj==ui->boxImageUnet)){
        this->touchListSegmented();
        return true;
    }
     return eventFilter(obj,evt);
}*/
const QDir& LoadFolderWidget::getDirModified() const{
    return modifed;
}
void LoadFolderWidget::setDirModified(const QDir& dirModified){
    modifed = dirModified;
}
const bool& LoadFolderWidget::getDraw() const{
    return _drawing;
}
void LoadFolderWidget::setDraw(bool draw){
    _drawing = draw;
}

void LoadFolderWidget::setIndexListBox(int idxRaw,int idxCNN){
    ui->boxListImageOrigine->setCurrentIndex(idxRaw);
    if(ui->boxImageUnet!=0)
        ui->boxImageUnet->setCurrentIndex(idxCNN);
}
