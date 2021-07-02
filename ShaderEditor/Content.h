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
enum TypeSlice { SliceXY, SliceXZ, SaveSliceZY };
namespace Ra{
namespace Engine{
struct Dataset{
    std::string path;
    std::string name;
    std::string type;
    Ra::Engine::TextureParameters textureParametres;
    QImage image;
};
}
};

class Content{
private:
    std::shared_ptr< Ra::Engine::RenderObject > _ro;
    Ra::Engine::Renderer * _renderer;
    std::shared_ptr< MyParameterProvider  > _paramProvider;
    std::string _pathRaw;
    std::string _pathCNN;
    int _count = 0;
public:
    Content(std::shared_ptr< Ra::Engine::RenderObject > ro,Ra::Engine::Renderer * renderer, std::shared_ptr< MyParameterProvider  > paramProvider,const std::string& pathRaw,
    const std::string &pathCNN, int count );
    const std::shared_ptr< Ra::Engine::RenderObject >& getRenderObject() const;
     Ra::Engine::Renderer * getRenderer();
    std::shared_ptr< MyParameterProvider  > getParameterProvider();
    const std::string&  getPathRaw() const;
    const std::string&  getPathCNN() const;
    void setCount(const int &count);

    const int& getCount() const;
    void setRenderObject(std::shared_ptr< Ra::Engine::RenderObject > ro);
    void setRenderer(Ra::Engine::Renderer * renderer);
    void setParameterProvider(std::shared_ptr< MyParameterProvider  > paramProvider);
    void setPathRaw(const std::string &pathRaw);
    void setPathCNN(const std::string &pathCNN);
    void enableButtonRun(QGroupBox *boxWork, QPushButton *buttonWork);
    void checkButtonCNN(QTextEdit * textMethode, QGroupBox *boxWork, QPushButton *buttonWork, QString nameType);
    int messBoxYesNo(QString text);
    void messBoxInfo(QString Info);
    std::string textDataRaw(QStringList * pathFull, QDir directoryRaw, QStringList nameList);
    void listPathFull(QStringList * pathFull, QDir directory,QStringList nameList);
    std::string methodeUnet(QDir directoryRaw, QDir directoryCNN, QStringList nameList);
    Ra::Engine::TextureParameters getTextureParameterFromPath(std::string path);
    QImage getImageFromPath(std::string path);
    int findIndex(QString str, QStringList str1);
     QStringList listFiltre(QStringList nameList);
     QString strMax(QStringList list);
    ~Content();
};
#endif // CONTENT_H
