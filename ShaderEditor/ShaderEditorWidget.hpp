#pragma once

#include <QWidget>
#include <QString>
#include <Engine/Renderer/Texture/TextureManager.hpp>
namespace Ui {
class ShaderEditorWidget;
}

namespace Ra{
namespace Engine{
    class RenderObject;
    class Renderer;
    class ShaderParameterProvider;
}
}

///
/// \todo Add automatic UI generation from parameter provider
class ShaderEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShaderEditorWidget(const std::string& v,
                                const std::string& f,
                                std::shared_ptr< Ra::Engine::RenderObject > ro,
                                Ra::Engine::Renderer * renderer,
                                std::shared_ptr< Ra::Engine::ShaderParameterProvider > paramProvider,
                                Ra::Engine::TextureManager _textureManager,
                                QWidget *parent = nullptr);
    ~ShaderEditorWidget();

private slots:
    void updateShadersFromUI();
    void loadData();
    void showImage();

private:
    Ui::ShaderEditorWidget *ui;
    std::shared_ptr< Ra::Engine::RenderObject > _ro;
    Ra::Engine::Renderer * _renderer;
    std::shared_ptr< Ra::Engine::ShaderParameterProvider > _paramProvider;
    Ra::Engine::TextureManager _textureManager;
    std::string _path;
};

