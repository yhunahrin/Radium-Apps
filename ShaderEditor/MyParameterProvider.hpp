#pragma once

#include <Engine/Renderer/RenderTechnique/RenderParameters.hpp>
#include <Engine/Renderer/Texture/Texture.hpp>
#include <Engine/Renderer/Texture/TextureManager.hpp>
#include <QImageReader>
#include <QSettings>
#include <QString>
#include "CameraManipulator.hpp"
using ShaderConfigType = std::vector<std::pair<Ra::Engine::ShaderType, std::string>> ;

class MyParameterProvider :public Ra::Engine::ShaderParameterProvider
{
public:
    MyParameterProvider() {}
    ~MyParameterProvider() {}
    void updateGL() override {
        // Method called before drawing each frame in Renderer::updateRenderObjectsInternal.
        // The name of the parameter corresponds to the shader's uniform name.
       // m_renderParameters.addParameter( "aColorUniform", m_colorParameter );
        m_renderParameters.addParameter( "aMixUniform", m_MixParameter );
      /*  QImageReader reader;
        QSettings settings;
        reader.setFileName(QString::fromStdString("C:\\Learn\\Test2-20210404T211623Z-001\\Test2\\image61.png"));
        QImage image = reader.read();
       QImage tmp =  image.convertToFormat(QImage::Format_RGBA8888);*/
        m_textureRaw=(m_textureManager.getOrLoadTexture(m_textureP));
        m_textureRaw->bind();
        m_textureUnet=(m_textureManager.getOrLoadTexture(m_texturePU));
        m_textureUnet->bind();
        m_renderParameters.addParameter( "aRawTextureUniform",m_textureRaw );
        m_renderParameters.addParameter( "aUnetTextureUniform",m_textureUnet);
    }

    const std::string &getPathRaw() const{
           return m_pathRaw;
    }

    const std::string &getPathUnet() const{
           return m_pathUnet;
    }

    Ra::Core::Vector2 getMouse(){
        return m_mouse;
    }

    const int &getCount() const{
           return m_count;
    }
    void setMouse( Ra::Core::Vector2 mouse){
        m_mouse = mouse;
        //m_on = true;
    }
    void setPathRaw(const std::string &pathRaw){
        m_pathRaw =pathRaw;
        m_textureP = m_textureManager.addTexture(pathRaw,0,0,nullptr);
    }

    void setPathUnet(const std::string &pathUnet){
        m_pathUnet = pathUnet;
        m_texturePU = m_textureManager.addTexture(pathUnet,0,0,nullptr);
    }

    void setIndexRaw(int &indexRaw){
        m_indexRaw =indexRaw;
    }

    void setIndexCNN(int &indexCNN){
        m_indexCNN =indexCNN;
    }

    const int &getIndexRaw() const{
           return m_indexRaw;
    }

    const int &getIndexCNN() const{
           return m_indexCNN;
    }

    void setTextureParameterRaw(const Ra::Engine::TextureParameters & textureParaRaw){
        m_textureP = textureParaRaw;
    }
    void setTextureParameterCNN(const Ra::Engine::TextureParameters & textureParaCNN){
        m_texturePU = textureParaCNN;
    }

    const Ra::Engine::TextureParameters  &getTextureParaRaw() const{
           return m_textureP;
    }

    const Ra::Engine::TextureParameters  &getTextureParaUnet() const{
           return  m_texturePU;
    }

    void setCount(const int &count){
        m_count = count;
    }

    const Ra::Engine::TextureManager & getTextureManger() const{
        return m_textureManager;
    }
 /*   void setTextureManger(const std::string& texture, void* content ){
        m_textureManager.updateTextureContent(texture,content);
    }
*/
    const Ra::Engine::Texture* getTexture() const{
        return m_textureUnet;
    }

 /*  void setTexture(const Ra::Engine::TextureParameters& textureParameter){
        m_textureUnet->updateData(&textureParameter);
        m_textureUnet->bind();
    }*/
   void setSave(const bool &save){
       m_save = save;
   }

   const bool & getSave() const{
       return m_save;
   }
  /*  void setTextureUnet(Ra::Engine::Texture*textureUnet){
        m_textureUnet = textureUnet;
        m_textureUnet->bind();
    }*/
    /*void setOrComputeTheParameterValues(std::string path) {
        // client side computation of the parameters, e.g.
        m_colorParameter  = Ra::Core::Utils::Color::Red();
        m_scalarParameter = .5_ra;
        m_path = path;
    }*/
  //  void clickMouse(QMouseEvent *event){
    //    _camera->handleMousePressEvent(event,Qt::LeftButton,Qt::ShiftModifier,1);
    //    m_mouse=_camera->getMouse();
   // }
  private:
  //  Ra::Core::Utils::Color m_colorParameter {Ra::Core::Utils::Color::Green()};
    Scalar m_MixParameter {0.5_ra};
    bool m_save {false};
    Ra::Engine::TextureManager m_textureManager;
    Ra::Engine::Texture * m_textureRaw;
    Ra::Engine::Texture * m_textureUnet;
    Ra::Engine::TextureParameters m_textureP;
    Ra::Engine::TextureParameters m_texturePU;
    std::string m_pathRaw;
    std::string m_pathUnet;
    Ra::Core::Vector2 m_mouse{{0,0}};
    int m_indexRaw{-1};
    int m_indexCNN{-1};
    int m_count{0};
};















