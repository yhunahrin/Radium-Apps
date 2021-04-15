#ifndef CONTENT_H
#define CONTENT_H
#include "MyParameterProvider.hpp"
//Qt library
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
#include <QGroupBox>
#include <QTextEdit>
#include <QMessageBox>
namespace Ra{
namespace Engine{
    class RenderObject;
    class Renderer;
    class ShaderParameterProvider;
    class TextureManager;
}
}
enum TypeRun { Loadata, ShowImage, SaveJson };
enum CNN { Unet, CNN2, CNN3 };
class Content{
private:
    std::shared_ptr< Ra::Engine::RenderObject > _ro;
    Ra::Engine::Renderer * _renderer;
    std::shared_ptr< MyParameterProvider  > _paramProvider;
    std::string _pathRaw;
    std::string _pathCNN;
public:
    Content(std::shared_ptr< Ra::Engine::RenderObject > ro,Ra::Engine::Renderer * renderer, std::shared_ptr< MyParameterProvider  > paramProvider,std::string pathRaw,
    std::string pathCNN );
    std::shared_ptr< Ra::Engine::RenderObject > getRenderObject();
    Ra::Engine::Renderer * getRenderer();
    std::shared_ptr< MyParameterProvider  > getParameterProvider();
    std::string getPathRaw();
    std::string getPathCNN();
    void setRenderObject(std::shared_ptr< Ra::Engine::RenderObject > ro);
    void setRenderer(Ra::Engine::Renderer * renderer);
    void setParameterProvider(std::shared_ptr< MyParameterProvider  > paramProvider);
    void setPathRaw(std::string pathRaw);
    void setPathCNN(std::string pathCNN);
    void enableButtonRun(QGroupBox *boxWork, QPushButton *buttonWork);
    void checkButtonCNN(QTextEdit * textMethode, QGroupBox *boxWork, QPushButton *buttonWork, QString nameType);
    int messBoxYesNo(QString text);
    void messBoxInfo(QString Info);
    std::string textDataRaw(QStringList * pathFull, QDir directoryRaw, QStringList nameList);
    void listPathFull(QStringList * pathFull, QDir directory,QStringList nameList);
    std::string methodeUnet(QDir directoryRaw, QDir directoryCNN, QStringList nameList);
    ~Content();
};
#endif // CONTENT_H
