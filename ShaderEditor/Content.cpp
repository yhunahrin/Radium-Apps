#include "Content.h"
Content::Content(std::shared_ptr< Ra::Engine::RenderObject > ro,Ra::Engine::Renderer * renderer, std::shared_ptr< MyParameterProvider  > paramProvider,std::string pathRaw,
                 std::string pathCNN ):
    _ro (ro),
    _renderer(renderer),
    _paramProvider(paramProvider),
    _pathRaw(pathRaw),
    _pathCNN(pathCNN){}

std::shared_ptr< Ra::Engine::RenderObject > Content::getRenderObject(){
    return _ro;
}

Ra::Engine::Renderer * Content::getRenderer(){
    return _renderer;
}

std::shared_ptr< MyParameterProvider  > Content::getParameterProvider(){
    return _paramProvider;
}

std::string Content::getPathRaw(){
    return _pathRaw;
}

std::string Content::getPathCNN(){
    return _pathCNN;
}

void Content::setRenderObject(std::shared_ptr< Ra::Engine::RenderObject > ro){
    _ro = ro;
}
void Content::setRenderer(Ra::Engine::Renderer * renderer){
    _renderer = renderer;
}

void Content::setParameterProvider(std::shared_ptr< MyParameterProvider  > paramProvider){
    _paramProvider = paramProvider;
}

void Content::setPathRaw(std::string pathRaw){
    _pathRaw = pathRaw;
}

void  Content::setPathCNN(std::string pathCNN){
    _pathCNN = pathCNN;
}

void Content::enableButtonRun( QGroupBox *boxWork, QPushButton *buttonWork){

    boxWork->setEnabled(true);
    buttonWork->setEnabled(true);
}

void Content::checkButtonCNN( QTextEdit * textMethode, QGroupBox *boxWork, QPushButton *buttonWork, QString nameType){
    textMethode->setEnabled(true);
    boxWork->setTitle(nameType);
    buttonWork->setEnabled(true);

}

int Content::messBoxYesNo(QString text){
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    return msgBox.exec();
}

void Content::messBoxInfo(QString Info){
    QMessageBox msgBox;
    msgBox.setText(Info);
    msgBox.exec();
}

std::string Content::textDataRaw(QStringList *pathFull, QDir directoryRaw, QStringList nameList){
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
    pathFull->clear();
    for (int i=0; i<nameList.size();i++){
           pathFull->append(directoryRaw.path()+"/"+nameList.at(i));
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
    std::cout<<"hihi"<<pathFull->size()<<std::endl;
    return textDataset;
}

void Content::listPathFull(QStringList *pathList, QDir directory,QStringList nameList){
    pathList->clear();
    for (int i=0; i<nameList.size();i++){
        pathList->append(directory.path()+"/"+nameList.at(i));
    }
}
std::string Content::methodeUnet(QDir directoryRaw, QDir directoryCNN, QStringList nameList){
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
    return textMethod;
}
Content::~Content(){

}
