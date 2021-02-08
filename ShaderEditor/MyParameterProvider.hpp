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
        m_renderParameters.addParameter( "aTextureUniform", m_textureParameter);
    }
    void setOrComputeTheParameterValues() {
        // client side computation of the parameters, e.g.
        m_colorParameter  = Ra::Core::Utils::Color::Red();
        m_scalarParameter = .1_ra;
        Ra::Engine::TextureManager tmp;
        Ra::Engine::TextureParameters tmp1;
        QImageReader reader;
        reader.setFileName("C:\\Users\\aduongng\\Desktop\\TP_VO\\20703106.jpg");
        QImage image = reader.read();
        //tmp1.name = "C:\\Users\\aduongng\\Deskto\\TP_VO\\20703106.jpg";
        tmp1.name = "C:\\Users\\aduongng\\Desktop\\TP_VO\\20703106.jpg";
        tmp1.width = image.width();
        tmp1.height = image.height();
        tmp1 = tmp.addTexture("C:\\Users\\aduongng\\Desktop\\TP_VO\\20703106.jpg", tmp1.width, tmp1.height,tmp1.texels);
      //  tmp.loadTextureImage(tmp1);
         m_textureParameter = tmp.loadTexture(tmp1);
      //   m_textureParameter = tmp1.name = "C:\\Users\\aduongng\\Desktop\\TP_VO\\20703106.jpg";
       //tmp.getOrLoadTexture(tmp1);
       /* int w,h;
        unsigned char * texture_data = stbi_load("C:/Users/aduongng/Desktop/TP_VO/20703106.jpg", &w, &h, nullptr, 4);
        tmp1 = tmp.addTexture("C:/Users/aduongng/Desktop/Capture.PNG",w,h,texture_data);
        m_textureParameter = tmp.loadTexture(tmp1);*/
    }

  private:
    Ra::Core::Utils::Color m_colorParameter {Ra::Core::Utils::Color::Green()};
    Scalar m_scalarParameter {1};
    Ra::Engine::Texture * m_textureParameter{};
};






















