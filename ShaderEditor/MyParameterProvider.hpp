#pragma once

#include <Engine/Renderer/RenderTechnique/RenderParameters.hpp>
#include <Engine/Renderer/Texture/Texture.hpp>
#include <Engine/Renderer/Texture/TextureManager.hpp>
#include <QImageReader>
#include <QSettings>
#include <QString>

using ShaderConfigType = std::vector<std::pair<Ra::Engine::ShaderType, std::string>> ;

class MyParameterProvider : public Ra::Engine::ShaderParameterProvider
{
  public:
    MyParameterProvider() {}
    ~MyParameterProvider() {}
    void updateGL() override {
        // Method called before drawing each frame in Renderer::updateRenderObjectsInternal.
        // The name of the parameter corresponds to the shader's uniform name.
        m_renderParameters.addParameter( "aColorUniform", m_colorParameter );
        m_renderParameters.addParameter( "aScalarUniform", m_scalarParameter );
        Ra::Engine::TextureParameters tmp1;

        tmp1.name = m_path;
        m_textureParameter = m_textureManager.loadTexture(tmp1);
        m_renderParameters.addParameter( "aTextureUniform", m_textureParameter);
    }
    std::string getPath(){
        return m_path;
    }
    void setPath(std::string path){
        m_path = path;
    }
    void setOrComputeTheParameterValues() {
        // client side computation of the parameters, e.g.
     //  m_colorParameter  = Ra::Core::Utils::Color::Red();
      //  m_scalarParameter = .1_ra;
        m_path = "C:\\Users\\aduongng\\Desktop\\TP_VO\\20703106.jpg";
        Ra::Engine::TextureParameters tmp1;
        //tmp1.name = "C:\\Users\\aduongng\\Deskto\\TP_VO\\20703106.jpg";
        tmp1.name = m_path;
        //tmp1 = tmp.addTexture(tmp1.name, tmp1.width, tmp1.height,tmp1.texels);
      //  tmp.loadTextureImage(tmp1);
         m_textureParameter = m_textureManager.loadTexture(tmp1);
        // m_textureParameter = tmp.getOrLoadTexture(tmp1);

      //   m_textureParameter = tmp1.name = "C:\\Users\\aduongng\\Desktop\\TP_VO\\20703106.png";
       //tmp.getOrLoadTexture(tmp1);
       /* int w,h;
        unsigned char * texture_data = stbi_load("C:/Users/aduongng/Desktop/TP_VO/20703106.jpg", &w, &h, nullptr, 4);
        tmp1 = tmp.addTexture("C:/Users/aduongng/Desktop/Capture.PNG",w,h,texture_data);
        m_textureParameter = tmp.loadTexture(tmp1);*/
    }

  private:
    Ra::Core::Utils::Color m_colorParameter {Ra::Core::Utils::Color::Green()};
    Scalar m_scalarParameter {1};
    Ra::Engine::TextureManager m_textureManager{};
    Ra::Engine::Texture * m_textureParameter{};
    std::string m_path {""};
};





















