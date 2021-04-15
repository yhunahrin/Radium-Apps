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
        m_textureRaw = m_textureManagerRaw.getOrLoadTexture(m_textureManagerRaw.addTexture(this->getPathRaw(),0,0,nullptr));
        m_textureRaw->bind();
        m_textureUnet = (m_textureManagerUnet.getOrLoadTexture(m_textureManagerUnet.addTexture(this->getPathUnet(),0,0,nullptr)));
        m_textureUnet->bind();
        m_renderParameters.addParameter( "aRawTextureUniform",m_textureRaw );
        m_renderParameters.addParameter( "aUnetTextureUniform",m_textureUnet);
       // this->getMouse();
        m_renderParameters.addParameter( "aMousePos",m_mouse);
        m_renderParameters.addParameter( "aOn",m_on);
    }

    std::string getPathRaw(){
           return m_pathRaw;
    }

    std::string getPathUnet(){
           return m_pathUnet;
    }

    Ra::Core::Vector2 getMouse(){
        return m_mouse;
    }

    void setMouse( Ra::Core::Vector2 mouse){
        m_mouse = mouse;
        m_on = true;
    }
    void setPathRaw(std::string pathRaw){
        m_pathRaw = pathRaw;
    }

    void setPathUnet(std::string pathUnet){
        m_pathUnet = pathUnet;

    }

    Ra::Engine::TextureManager getTextureMangerRaw(){
        return m_textureManagerRaw;
    }

    Ra::Engine::TextureManager getTextureMangerUnet(){
        return m_textureManagerUnet;
    }

    void setTextureMangerRaw(Ra::Engine::TextureManager textureManagerRaw){
        m_textureManagerRaw = textureManagerRaw;
        m_textureRaw->bind();
    }

    void setTextureMangerUnet(Ra::Engine::TextureManager textureManagerUnet){
        m_textureManagerUnet = textureManagerUnet;
        m_textureUnet->bind();
    }

    Ra::Engine::Texture* getTextureRaw(){
        return m_textureRaw;
    }

    Ra::Engine::Texture* getTextureUnet(){
        return m_textureUnet;
    }

    void setTextureRaw(Ra::Engine::Texture* textureRaw){
        m_textureRaw = textureRaw;
        m_textureRaw->bind();
    }

    void setTextureUnet(Ra::Engine::Texture*textureUnet){
        m_textureUnet = textureUnet;
        m_textureUnet->bind();
    }
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
    bool m_on {false};
    Ra::Engine::TextureManager m_textureManagerRaw{};
    Ra::Engine::TextureManager m_textureManagerUnet{};
    Ra::Engine::Texture * m_textureRaw{};
    Ra::Engine::Texture * m_textureUnet{};
    std::string m_pathRaw {""};
    std::string m_pathUnet {""};
    Ra::Core::Vector2 m_mouse{{0,0}};
};















