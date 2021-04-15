// Include Radium base application and its simple Gui
#include <GuiBase/BaseApplication.hpp>
#include <GuiBase/RadiumWindow/SimpleWindowFactory.hpp>

// include the Engine/entity/component interface
// include the Engine/entity/component interface
#include <Core/Geometry/MeshPrimitives.hpp>
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
     return app.exec();
}
