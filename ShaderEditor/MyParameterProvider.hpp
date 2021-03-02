#pragma once

#include <Engine/Renderer/RenderTechnique/RenderParameters.hpp>
#include <Engine/Renderer/Texture/Texture.hpp>
#include <Engine/Renderer/Texture/TextureManager.hpp>
#include <QImageReader>
#include <QSettings>
#include <QString>

using ShaderConfigType = std::vector<std::pair<Ra::Engine::ShaderType, std::string>> ;

class MyParameterProvider :public Ra::Engine::ShaderParameterProvider
{
public:
    MyParameterProvider() {}
    ~MyParameterProvider() {}
    void updateGL() override {
        // Method called before drawing each frame in Renderer::updateRenderObjectsInternal.
        // The name of the parameter corresponds to the shader's uniform name.
        m_renderParameters.addParameter( "aColorUniform", m_colorParameter );
        m_renderParameters.addParameter( "aScalarUniform", m_scalarParameter );
        //Loading a texture (meant to be moved in an other class enabling multiple loading)
       // auto texManager = Ra::Engine::TextureManager{};
     /*   auto texParam = texManager.addTexture("C:/Users/aduongng/Desktop/project/ex.bmp",800,800,nullptr);
        auto tex = texManager.getOrLoadTexture(texParam);
        tex->initializeGL();
        tex->bind(0_ra);
        m_renderParameters.addParameter(  "aTextureUniform", tex);*/
        m_renderParameters.addParameter( "aTextureUniform",  m_textureManager.getOrLoadTexture(m_textureManager.addTexture(this->getPath(),0,0,nullptr)));
    }
    std::string getPath(){
           return m_path;
       }
    void setOrComputeTheParameterValues(std::string path) {
        // client side computation of the parameters, e.g.
        m_colorParameter  = Ra::Core::Utils::Color::Red();
        m_scalarParameter = .5_ra;
      //  m_textunit = 0_ra;
         m_path = path;
    }

  private:
    Ra::Core::Utils::Color m_colorParameter {Ra::Core::Utils::Color::Green()};
    Scalar m_scalarParameter {1};
    //Ra::Engine::TextureManager texManager;
    Ra::Engine::TextureManager m_textureManager{};
    std::string m_path {""};
};















