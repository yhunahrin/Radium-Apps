// Include Radium base application and its simple Gui
#include <GuiBase/BaseApplication.hpp>
#include <GuiBase/RadiumWindow/SimpleWindowFactory.hpp>

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
#include <string>

// Qt
#include <QTimer>
#include <QDockWidget>
#include <QImage>
#include <QImageReader>
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
    "uniform float aScalarUniform;\n"
    "uniform sampler2D aTextureUniform;\n"
    "void main(void)\n"
    "{\n"
    "    out_color =  ( 1 + cos( 20 * ( in_pos.x + aScalarUniform ) ) ) * 0.5 * aColorUniform;\n"
    "    out_color = texture(aTextureUniform,  texCoordOut);\n"
    "}\n"};



const ShaderConfigType defaultConfig {
    {Ra::Engine::ShaderType::ShaderType_VERTEX, _vertexShaderSource},
    {Ra::Engine::ShaderType::ShaderType_FRAGMENT, _fragmentShaderSource}};
std::string path = "C:/Users/aduongng/Desktop/project/ex.bmp";
auto paramProvider = std::make_shared<MyParameterProvider>();
/**
 * Generate a quad with a ShaderMaterial attached
 * @param app
 * @return The renderObject associated to the created component.
 *
 */
std::shared_ptr<Ra::Engine::RenderObject> initQuad( Ra::GuiBase::BaseApplication& app ) {
    //! [Creating the quad]
    auto quad = Ra::Core::Geometry::makeZNormalQuad( {1_ra, 1_ra} );

    //! [Create the engine entity for the quad]
    auto e = app.m_engine->getEntityManager()->createEntity( "Quad Entity" );

    //! [Create Parameter provider for the shader]
    paramProvider->setOrComputeTheParameterValues(path);
    //! [Create the shader material]
    //!

    Ra::Core::Asset::RawShaderMaterialData mat {"Quad Material", defaultConfig, paramProvider};

    //! [Create a geometry component using the custom material]
    auto c = new Ra::Engine::TriangleMeshComponent( "Quad Mesh", e, std::move( quad ), &mat );

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
    QDockWidget* dock = new QDockWidget("Shaders editor");
    dock->setWidget( new ShaderEditorWidget(defaultConfig[0].second, defaultConfig[1].second, ro, viewer->getRenderer(),path, paramProvider, dock) );
    app.m_mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dock);
    return app.exec();
}
