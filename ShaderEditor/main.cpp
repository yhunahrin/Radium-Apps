// Include Radium base application and its simple Gui
#include <GuiBase/BaseApplication.hpp>
#include <GuiBase/RadiumWindow/SimpleWindowFactory.hpp>

// include the Engine/entity/component interface
// include the Engine/entity/component interface
#include <Core/Geometry/MeshPrimitives.hpp>
#include <Engine/Component/GeometryComponent.hpp>
#include <Engine/Managers/EntityManager/EntityManager.hpp>
#include <Engine/Renderer/RenderObject/RenderObjectManager.hpp>
#include <Engine/System/GeometrySystem.hpp>
// include the custom material definition
#include <Engine/Renderer/Material/RawShaderMaterial.hpp>

// include the Viewer to demonstrate dynamic edition of materials
#include <GuiBase/Viewer/Viewer.hpp>

#include "CameraManipulator.hpp"
#include "ShaderEditorWidget.hpp"
#include "MyParameterProvider.hpp"
#include "MeshPaintUI.h"
#include "Segmentation.h"
#include "LoadFolderWidget.h"
#include "Content.h"
#include "MainWindow.h"
#include <string>

// Qt
#include <QTimer>
#include <QDockWidget>
#include <QImage>
#include <QImageReader>
#include <QToolBar>
#include <QVBoxLayout>

class MainWindowFactory : public Ra::GuiBase::BaseApplication::WindowFactory
{
  public:
   using Ra::GuiBase::BaseApplication::WindowFactory::WindowFactory;
    Ra::GuiBase::MainWindowInterface* createMainWindow() const override {
        return new MainWindow();
    }
};

 int main( int argc, char* argv[] ) {

     Ra::MainApplication app( argc, argv, MainWindowFactory() );
     app.setContinuousUpdate( false );
    // return 0;


  //  Ra::GuiBase::BaseApplication app( argc, argv, Ra::GuiBase::SimpleWindowFactory{} );
    //! [add the custom material to the material system]
 //   Ra::Engine::RawShaderMaterial::registerMaterial();
 //   int w,h;
 //   auto ro = initQuad( app);
  //  app.askForUpdate();
 //   std::cout<<w<<std::endl;
 //   auto viewer = app.m_mainWindow->getViewer();
  //  auto camera = new CameraManipulator2D( *( viewer->getCameraManipulator() ));
  //  viewer->setCameraManipulator(camera);

 //   for(int i = 0; i<20; i++){
  //  const auto r  =  viewer->getCameraManipulator()->getCamera()->getRayFromScreen({10+i,25+i});
   // const auto tmp =   camera->getCamera()->unProject({25,122});
   // std::cout<<"yo"<<tmp<<std::endl;

   //    std::cout<<r.pointAt(1)<<std::endl;
//     std::vector<Scalar> t;
    // {0,0,0} a point and {0,1,0} the normal
  /*  if ( Ra::Core::Geometry::RayCastPlane( r, {0, 0, 0}, {0, 1, 0}, t ) )
    {
        // vsPlane return at most one intersection t
   //     std::cout<<"hehe"<<t[1]<<std::endl;
        auto worldPos = r.pointAt( t[0] );
        std::cout<<"nehe "<<i<<worldPos<<std::endl;
        // if the brush /*is touching the paper;
        auto paperPos = ro->getTransform().inverse()*worldPos;
       // std::cout<<ro->getTransform().inverse().matrix()<<std::endl;
       std::cout<<"hihi"<<i<<";"<<paperPos.y()<<std::endl;
       std::cout<<"hoho"<<i<<";"<<paperPos.x()<<std::endl;
       std::cout<<"haha"<<i<<";"<<paperPos.z()<<std::endl;
    }*/
//}



 //   QDockWidget* dock = new QDockWidget("Load Dataset");
   // std::cout<<projetMatrix<<std::endl;
  //  auto widget = new LoadFolderWidget(ro, viewer->getRenderer(),pathRaw,pathUnet, paramProvider,viewer,dock);
    //Content tmp(widget,ro, viewer->getRenderer(), paramProvider,viewer);
  //  dock->setWidget(widget);
   // app.askForUpdate();
  //  app.m_mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dock);

    /*QDockWidget* dock2 = new QDockWidget("Mesh Paint");
    dock2->setWidget( new Segmentation(dock2));
    app.m_mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dock2);*/



  //  Ra::GuiBase::BaseApplication app( argc, argv,Ra::GuiBase::SimpleWindowFactory{} );
    /*auto viewer = app.m_mainWindow->getViewer();
    viewer->setCameraManipulator(
        new CameraManipulator2D( *( viewer->getCameraManipulator() ) ) );
      */
   // QDockWidget* dock = new QDockWidget("Segmentation");
    //QMainWindow* a = new QMainWindow(app)
   // dock->setWidget(new Segmentation(dock) );
    //app.m_mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dock);
    /* QApplication app(argc, argv);

      Segmentation window;

      window.setGeometry(300, 300,800, 600);
      window.setWindowIcon(QIcon("C:/Users/aduongng/Desktop/project/App/Radium-Apps/ShaderEditor/image131.bmp"));
      window.setWindowTitle("Segmentation");
      window.show();
*/


    /*Ra::GuiBase::BaseApplication app( argc, argv, Ra::GuiBase::SimpleWindowFactory{} );
    app.setContinuousUpdate( false );
    QDockWidget* dock = new QDockWidget("Segmentation");*/
    // dock->setWidget(new Segmentation(dock) );
     //app.m_mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dock);
    return app.exec();
}
