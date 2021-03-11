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

#include <string>

// Qt
#include <QTimer>
#include <QDockWidget>
#include <QImage>
#include <QImageReader>
#include <QToolBar>
#include <QVBoxLayout>
/**
 * Demonstrate the usage of RawShaderMaterial functionalities
 */
// Vertex shader source code
const std::string _vertexShaderSource {"#include \"TransformStructs.glsl\"\n"
                                       "layout (location = 0) in vec3 in_position;\n"
                                       "layout (location = 0) out vec3 out_pos;\n"
                                       "layout (location = 1) in vec2 texCoordIn;\n"
                                       "layout (location = 1) out vec2 texCoordOut;\n"
                                       "uniform Transform transform;\n"
                                       "void main(void)\n"
                                       "{\n"
                                       "    mat4 mvp    = transform.proj * transform.view;\n"
                                       "    out_pos     = in_position;\n"
                                       "    gl_Position = mvp*vec4(in_position.xyz, 1.0);\n"
                                       "    texCoordOut = texCoordIn;\n"
                                       "}\n"};
// Fragment shader source code
const std::string _fragmentShaderSource {
    "layout (location = 0) in  vec3 in_pos;\n"
    "layout (location = 0) out vec4 out_color;\n"
    "layout (location = 1) in vec2 texCoordOut;\n"
    "uniform vec4 aColorUniform;\n"
    "uniform float aMixUniform;\n"
    "uniform sampler2D aRawTextureUniform;\n"
    "uniform sampler2D aUnetTextureUniform;\n"
    "void main(void)\n"
    "{\n"
    "    vec4 imageRaw = texture(aRawTextureUniform,  texCoordOut)\n;"
    "    vec4 imageUnet = texture(aUnetTextureUniform,  texCoordOut)\n;"
    "    out_color = aMixUniform*(imageRaw + imageUnet);\n"
    "}\n"};

const ShaderConfigType defaultConfig {
    {Ra::Engine::ShaderType::ShaderType_VERTEX, _vertexShaderSource},
    {Ra::Engine::ShaderType::ShaderType_FRAGMENT, _fragmentShaderSource}};

std::string pathRaw = "C:\\Users\\aduongng\\Desktop\\TP_VO\\20703106.jpg";
std::string pathUnet = "C:\\Users\\aduongng\\Desktop\\TP_VO\\20703106.jpg";
auto paramProvider = std::make_shared<MyParameterProvider>();
//Ra::Gui::PickingManager* pickingManager;
//Ra::Gui::PickingManager* pickingManager;
/**
 * Generate a quad with a ShaderMaterial attached
 * @param app
 * @return The renderObject associated to the created component.
 *
 */
std::shared_ptr<Ra::Engine::RenderObject> initQuad( Ra::GuiBase::BaseApplication& app ) {

        //! [Creating the quad]
        auto quad = Ra::Core::Geometry::makeZNormalQuad( {1_ra, 1_ra} );
        //Adding texture coordinates to the geometry
         // Ra::Core::Vector2Array texcoords{{0_ra,1_ra},{0_ra,0_ra},{1_ra,1_ra},{1_ra,0_ra}};
          Ra::Core::Vector2Array texcoords{{0_ra,0_ra},{1_ra,0_ra},{0_ra,1_ra},{1_ra,1_ra}};
     //   Ra::Core::Vector2Array texcoords{{1_ra,0_ra},{1_ra,1_ra},{0_ra,0_ra},{1_ra,0_ra},{1_ra,1_ra},{0_ra,0_ra}};
//        Ra::Core::Vector2Array texcoords{{0_ra,1_ra},{1_ra,1_ra},{0_ra,0_ra},{1_ra,0_ra},{1_ra,0_ra},{0_ra,1_ra}};
        quad.addAttrib("texCoordIn",texcoords);
        //! [Create the engine entity for the quad]
        auto e = app.m_engine->getEntityManager()->createEntity( "Quad Entity" );

        //! [Create Parameter provider for the shader]
        paramProvider->setPathRaw(pathRaw);
        paramProvider->setPathUnet(pathUnet);
        //! [Create the shader material]
        Ra::Core::Asset::RawShaderMaterialData mat {"Quad Material", defaultConfig, paramProvider};

        //! [Create a geometry component using the custom material]
        auto c = new Ra::Engine::TriangleMeshComponent( "Quad Mesh", e, std::move( quad ), &mat);
        //! [Register the entity/component association to the geometry system ]
        auto system = app.m_engine->getSystem( "GeometrySystem" );
        system->addComponent( e, c );

        //![get the renderobject for further edition]
        auto ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
            c->m_renderObjects[0] );

        // Initialize all OpenGL state for the scene content
        app.m_mainWindow->postLoadFile( "Cube" );
        return ro;
}

 int main( int argc, char* argv[] ) {
    Ra::GuiBase::BaseApplication app( argc, argv, Ra::GuiBase::SimpleWindowFactory{} );
    //! [add the custom material to the material system]
    Ra::Engine::RawShaderMaterial::registerMaterial();

    auto ro = initQuad( app );
    auto viewer = app.m_mainWindow->getViewer();
    viewer->setCameraManipulator(
        new CameraManipulator2D( *( viewer->getCameraManipulator() ) ) );

    QDockWidget* dock = new QDockWidget("Load Dataset");
    dock->setWidget( new LoadFolderWidget(ro, viewer->getRenderer(),pathRaw,pathUnet, paramProvider, dock) );
    app.m_mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dock);
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
