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
        m_renderParameters.addParameter( "aTextureUniform",  m_textureManager.getOrLoadTexture(m_textureManager.addTexture(this->getPath(),0,0,nullptr)));
    }
    std::string getPath(){
        return m_path;
    }
    void setOrComputeTheParameterValues(std::string path) {
        // client side computation of the parameters, e.g.
        m_colorParameter  = Ra::Core::Utils::Color::Red();
        m_scalarParameter = .1_ra;
        m_path = path;
    }

  private:
    Ra::Core::Utils::Color m_colorParameter {Ra::Core::Utils::Color::Green()};
    Scalar m_scalarParameter {1};
    Ra::Engine::TextureManager m_textureManager{};
    std::string m_path {""};
};















