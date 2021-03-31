#include "Content.h"
Content::Content(LoadFolderWidget *widget,std::shared_ptr< Ra::Engine::RenderObject > ro,Ra::Engine::Renderer * renderer, std::shared_ptr< MyParameterProvider  > paramProvider, Ra::Gui::Viewer *viewer):
    _widget( widget ),
    _ro (ro),
    _renderer(renderer),
    _paramProvider(paramProvider),
    _viewer(viewer){
     _camera = new CameraManipulator2D(*(_viewer->getCameraManipulator()));
    viewer->setCameraManipulator(_camera);
    QMouseEvent *event;
    if(!_camera->handleMousePressEvent(event,Qt::LeftButton, Qt::ShiftModifier,1)){
         Ra::Core::Vector2 tmp({-1,-1});
    //    _camera->handleMousePressEvent(event,Qt::LeftButton, Qt::ShiftModifier,1);
    //    tmp = _camera->getMouse();
       std::cout<<"Click ne"<<tmp.x()<<std::endl;
    }
    else{
        std::cout<<"ko click"<<std::endl;
    }

}
LoadFolderWidget* Content::getLoadFolderWidget(){
    return _widget;
}
void Content::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
         Ra::Core::Vector2 tmp({-1,-1});
    //    _camera->handleMousePressEvent(event,Qt::LeftButton, Qt::ShiftModifier,1);
    //    tmp = _camera->getMouse();
       std::cout<<"Click ne"<<tmp.x()<<std::endl;
    }
    else{
        std::cout<<"ko click"<<std::endl;
    }
}
