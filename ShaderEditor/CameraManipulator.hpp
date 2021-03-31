#pragma once

#include <GuiBase/Viewer/TrackballCameraManipulator.hpp>
#include <Engine/Renderer/Camera/ViewingParameters.hpp>
#include <Engine/Renderer/Camera/Camera.hpp>
#include <QString>
#include <QToolTip>
#include <Core/Geometry/RayCast.hpp>
#include <Engine/Renderer/RenderTechnique/RenderParameters.hpp>
#include <Engine/Renderer/RenderObject/RenderObject.hpp>

class CameraManipulator2D : public Ra::Gui::TrackballCameraManipulator
{
private:
    std::shared_ptr< Ra::Engine::RenderObject > _tmp;
    int x;
    int y;
public:
    /// Default constructor
    inline CameraManipulator2D( uint width, uint height )
    : Ra::Gui::TrackballCameraManipulator( width, height ) {}

    /// Copy constructor used when switching camera manipulator
    /// Requires that m_target is on the line of sight of the camera.
    //inline explicit CameraManipulator2D( const CameraManipulator& other, std::shared_ptr< Ra::Engine::RenderObject > tmp)
      //  : Ra::Gui::TrackballCameraManipulator( other ), _tmp(tmp){}
    inline explicit CameraManipulator2D( const CameraManipulator& other)
        : Ra::Gui::TrackballCameraManipulator( other ){}
    inline 
    bool handleMousePressEvent( QMouseEvent* event,
                                const Qt::MouseButtons& buttons,
                                const Qt::KeyboardModifiers& modifiers,
                                int key ) {
    bool handled = false;
    m_lastMouseX = event->pos().x();
    m_lastMouseY = event->pos().y();

    auto action = Ra::Gui::KeyMappingManager::getInstance()->getAction(
        Ra::Gui::KeyMappingManageable<TrackballCameraManipulator>::
        getContext(), buttons, modifiers, key, false );


    if ( action == TRACKBALLCAMERA_PAN )
    {
        m_cameraPanMode = true;
        handled         = true;
    }
    if ( action == TRACKBALLCAMERA_ZOOM )
    {
        m_cameraZoomMode  = true;
        handled          = true;
    }   
    std::cout<<m_camera->getPosition()<<std::endl;
   /* if (event->button() == Qt::LeftButton){
       m_lastMouseX = event->pos().x();
       m_lastMouseY = event->pos().y();
       std::cout<<"Coordinate of pixel: (x;y): ("<<m_lastMouseX<<";"<<m_lastMouseY <<")"<<std::endl;
    }*/
    if( event->button() ==  Qt::LeftButton )
     {
        std::cout<<"yoooooooo"<<std::endl;
        x = event->pos().x();
        y = event->pos().y();
               /* QString toolTipStr( "Mouse is at x:%1, y:%2" );
                toolTipStr.arg(event->pos().x(), event->pos().y());*/
               const auto r = m_camera->getRayFromScreen({event->x(),event->y()});
               // std::cout<<"camera"<<m_camera->getPosition()<<std::endl;
                std::vector<Scalar> t;
                    // {0,0,0} a point and {0,1,0} the normal

                    if ( Ra::Core::Geometry::RayCastPlane(r, {(float)(event->x()),(float)(event->y()),0}, {0, 1, 0}, t ) )
                    {
                        // vsPlane return at most one intersection t
                        //std::cout<<"jojo"<<r.<<std::endl;
                        auto worldPos = r.pointAt( t[1]);

                       // std::cout<<"im pick"<<event->x()<<std::endl;
                      //  auto papePos = _tmp->getTransform().inverse() * worldPos;
                      //  std::cout<<papePos.x()<<std::endl;
                      //   auto paperPos = _tmp->getCamera();
                      //  auto tmp = worldPos*m_camera->getViewMatrix();
                    }
                    else{
             //             std::cout<<"faux"<<std::endl;
                    }
              /*  Core::Ray getRayFromScreen( const Core::Vector2& pix ) const;

                /// Return the screen coordinates of the given point p (in world coordinates).
                inline Core::Vector2 project( const Core::Vector3& p ) const;

                /// Return the point on the screen plane (near plane) represented by screen coordinates pix.
                inline Core::Vector3 unProject( const Core::Vector2& pix ) const;

                */
          //      QToolTip::showText( event->screenPos().toPoint(),"(x,y): "+QString::number(tmp2.x()) + "," + QString::number( tmp2.y()));

           //     m_camera->getProjMatrix();
              //  Engine::Renderer::Camera::ViewingParameters tmp;
    }
    return handled;

}
    inline
    bool handleMouseMoveEvent( QMouseEvent* event,
                                const Qt::MouseButtons& buttons,
                                const Qt::KeyboardModifiers& modifiers,
                                int key ) {
    bool handled = false;
    m_lastMouseX = event->pos().x();
    m_lastMouseY = event->pos().y();

    auto action = Ra::Gui::KeyMappingManager::getInstance()->getAction(
        Ra::Gui::KeyMappingManageable<TrackballCameraManipulator>::
        getContext(), buttons, modifiers, key, false );

 //   std::cout<<"hihi"<<m_camera->getPosition().z()<<std::endl;
    if ( action == TRACKBALLCAMERA_PAN )
    {
        m_cameraPanMode = true;
        handled         = true;
    }
    if ( action == TRACKBALLCAMERA_ZOOM )
    {
        m_cameraZoomMode  = true;
        handled          = true;
    }
   /* if (event->button() == Qt::LeftButton){
       m_lastMouseX = event->pos().x();
       m_lastMouseY = event->pos().y();
       std::cout<<"Coordinate of pixel: (x;y): ("<<m_lastMouseX<<";"<<m_lastMouseY <<")"<<std::endl;
    }*/
    if( event->type() ==  QEvent::MouseMove )
     {
               /* QString toolTipStr( "Mouse is at x:%1, y:%2" );
                toolTipStr.arg(event->pos().x(), event->pos().y());*/
        if(((event->pos().x()-19)*512/390)<0|| (( event->pos().y()-122)*512/390)<0|| (( event->pos().x()-19)*512/390)>512|| (( event->pos().y()-122)*512/390)>512)
        {

        }
        else{
       // QToolTip::showText( event->screenPos().toPoint(),"(x,y): "+QString::number((event->pos().x()-19)*512/390) + "," + QString::number(( event->pos().y()-122)*512/390));
         //      _projetMatrix =  m_camera->getProjMatrix();
           //    _viewMatrix = m_camera->getViewMatrix();
        auto r = m_camera->getRayFromScreen({ event->pos().x(),event->pos().y()});
        std::vector<Scalar> t;
        // {0,0,0} a point and {0,1,0} the normal
        if ( Ra::Core::Geometry::RayCastPlane( r, {0, 0, 0}, {0, 1, 0}, t ) )
        {
            // vsPlane return at most one intersection t
            auto worldPos = r.pointAt(t[0]);
          //  QToolTip::showText( event->screenPos().toPoint(),"(x,y): "+QString::number( worldPos.x()) + "," + QString::number( worldPos.y()));
        }


           //     m_camera->getProjMatrix();
              //  Engine::Renderer::Camera::ViewingParameters tmp;
    }
    }
    return handled;
}


    Ra::Core::Vector2 getMouse(){
        std::cout<<"tao click ne"<<std::endl;
        Ra::Core::Vector2 tmp({x,y});
        return tmp;
    }
};

