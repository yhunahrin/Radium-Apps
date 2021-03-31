#ifndef CONTENT_H
#define CONTENT_H
#include "LoadFolderWidget.h"
#include "CameraManipulator.hpp"
#include <GuiBase/MainWindowInterface.hpp>
#include "MyParameterProvider.hpp"
#include <GuiBase/Viewer/Viewer.hpp>
#include <GuiBase/BaseApplication.hpp>

class Content : public Ra::GuiBase::MainWindowInterface{
private:
    LoadFolderWidget * _widget;
    std::shared_ptr< Ra::Engine::RenderObject > _ro;
    Ra::Engine::Renderer * _renderer;
    std::shared_ptr< MyParameterProvider  > _paramProvider;
    CameraManipulator2D* _camera;
    Ra::Gui::Viewer *_viewer;
public:
    Content(LoadFolderWidget *widget,std::shared_ptr< Ra::Engine::RenderObject > ro,Ra::Engine::Renderer * renderer, std::shared_ptr< MyParameterProvider  > paramProvider, Ra::Gui::Viewer *viewer);
    LoadFolderWidget* getLoadFolderWidget();
protected:
    void mousePressEvent(QMouseEvent *event);
};


#endif // CONTENT_H
