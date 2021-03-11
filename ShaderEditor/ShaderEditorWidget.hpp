#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QString>
#include "MyParameterProvider.hpp"
namespace Ui {
class ShaderEditorWidget;
}

namespace Ra{
namespace Engine{
    class RenderObject;
    class Renderer;
    class ShaderParameterProvider;
    class TextureManager;
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
                                std::string path,
                                std::shared_ptr< MyParameterProvider > paramProvider,
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
    std::string _path;
    std::shared_ptr< MyParameterProvider  > _paramProvider;
};

