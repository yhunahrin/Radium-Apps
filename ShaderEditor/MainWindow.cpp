#include "MainWindow.h"
#include <Core/Geometry/MeshPrimitives.hpp>
#include <Core/Geometry/MeshPrimitives.hpp>
#include <Engine/Component/GeometryComponent.hpp>
#include <Engine/Managers/EntityManager/EntityManager.hpp>
#include <Engine/Renderer/RenderObject/RenderObjectManager.hpp>
#include <Engine/System/GeometrySystem.hpp>
// include the custom material definition
#include <Engine/Renderer/Material/RawShaderMaterial.hpp>
#include <QDockWidget>
#include <Engine/Renderer/RenderTechnique/ShaderProgram.hpp>
#include <Engine/Renderer/RenderTechnique/ShaderProgramManager.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
 #include <stb/stb_image_write.h>
using Ra::Engine::ItemEntry;

using namespace Ra::Core::Utils; // log
const std::string _vertexShaderSource1 {"#include \"TransformStructs.glsl\"\n"
                                       "layout (location = 0) in vec3 in_position;\n"
                                       "layout (location = 0) out vec3 out_pos;\n"
                                       "layout (location = 1) in vec2 texCoordIn;\n"
                                       "layout (location = 1) out vec2 texCoordOut;\n"
                                       "uniform Transform transform;\n"
                                       "void main(void)\n"
                                       "{\n"
                                       "    mat4 mvp    = transform.proj * transform.view;\n"
                                       "    out_pos     = in_position;\n"
                                       "    gl_Position = mvp*vec4(out_pos, 1.0);\n"
                                       "    texCoordOut = texCoordIn ;\n"
                                       "}\n"};
// Fragment shader source code
const std::string _fragmentShaderSource1 {
    "layout (location = 0) in  vec3 out_pos;\n"
    "layout (location = 0) out vec4 out_color;\n"
    "layout (location = 1) in vec2 texCoordOut;\n"
    "uniform vec4 aColorUniform;\n"
    "uniform float aMixUniform;\n"
    "uniform sampler2D aRawTextureUniform;\n"
    "uniform sampler2D aUnetTextureUniform;\n"
    "uniform vec2 aMousePos;\n"
    "uniform bool aOn;\n"
    "ivec2 size;\n"
    "void main(void)\n"
    "{\n"
    "    size = textureSize(aUnetTextureUniform,0);\n"
    "    vec2 pixel_size = 1.0/size;\n"
    "    float distance  = length( out_pos.xy - aMousePos);"
    "    float mixK      = pow( distance / aMixUniform, 2.0 ); "
    "    vec4 imageRaw = texture(aRawTextureUniform,  texCoordOut)\n;"
    "    vec4 imageUnet = texture(aUnetTextureUniform,  texCoordOut)\n;"
    "    vec4 whiteColor  = vec4(1.0,1.0,1.0,1.0);\n"
    "    vec4 blackColor  = vec4(0.0,0.0,0.0,1.0);"
    "    if(aOn){\n"
    "    imageUnet = mix(whiteColor,imageUnet, step(0.001, mixK ));\n"
    "    out_color =aMixUniform*(imageRaw + imageUnet);}\n"
    "    else\n"
    "    out_color  =aMixUniform*(imageRaw + imageUnet);\n"
    "}\n"};
const ShaderConfigType defaultConfig1 {
    {Ra::Engine::ShaderType::ShaderType_VERTEX, _vertexShaderSource1},
    {Ra::Engine::ShaderType::ShaderType_FRAGMENT, _fragmentShaderSource1},};

std::string pathRaw1 = "C:\\Users\\aduongng\\Desktop\\TP_VO\\20703106.jpg";
std::string pathUnet1 = "C:\\Users\\aduongng\\Desktop\\TP_VO\\20703106.jpg";
auto paramProvider1 = std::make_shared<MyParameterProvider>();
MainWindow::MainWindow( QWidget* parent ) : MainWindowInterface( parent ) {
    // Note : at this point most of the components (including the Engine) are
    // not initialized. Listen to the "started" signal.

   setupUi( this );
   qApp->installEventFilter(this);
   Ra::Engine::RawShaderMaterial::registerMaterial();
   auto quad1 = Ra::Core::Geometry::makeZNormalQuad({1_ra, 1_ra});
 //Adding texture coordinates to the geometry
  // Ra::Core::Vector2Array texcoords{{0_ra,1_ra},{0_ra,0_ra},{1_ra,1_ra},{1_ra,0_ra}};
   Ra::Core::Vector2Array texcoords{{0_ra,0_ra},{1_ra,0_ra},{0_ra,1_ra},{1_ra,1_ra}};
 //  Ra::Core::Vector2Array texcoords{{0_ra,1_ra},{.5_ra,.5_ra},{0_ra,1_ra},{1_ra,1_ra}};
//   Ra::Core::Vector2Array texcoords{{1_ra,0_ra},{1_ra,1_ra},{0_ra,0_ra},{1_ra,0_ra},{1_ra,1_ra},{0_ra,0_ra}};
//        Ra::Core::Vector2Array texcoords{{0_ra,1_ra},{1_ra,1_ra},{0_ra,0_ra},{1_ra,0_ra},{1_ra,0_ra},{0_ra,1_ra}};
 quad1.addAttrib("texCoordIn",texcoords);
// auto tex= mainApp->m_engine->getTextureManager();

 //tex->updatePendingTextures();
 //! [Create the engine entity for the quad]
 auto e = mainApp->m_engine->getEntityManager()->createEntity( "Quad Entity" );

 //! [Create Parameter provider for the shader]
 paramProvider1->setPathRaw(pathRaw1);
 paramProvider1->setPathUnet(pathUnet1);
//  paramProvider->setMouse({0,0});
 //! [Create the shader material]
 Ra::Core::Asset::RawShaderMaterialData mat {"Quad Material", defaultConfig1, paramProvider1};
 //! [Create a geometry component using the custom material]
 auto c = new Ra::Engine::TriangleMeshComponent( "Quad Mesh", e, std::move( quad1 ), &mat);
 //! [Register the entity/component association to the geometry system ]
 auto system = mainApp->m_engine->getSystem( "GeometrySystem" );
 system->addComponent( e, c );

 //![get the renderobject for further edition]
  auto m_ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
    c->m_renderObjects[0] );
 // Initialize all OpenGL state for the scene content
  this->postLoadFile( "Cube" );
  //  m_viewer =  mainApp->m_mainWindow->getViewer();
   m_viewer = new Ra::Gui::Viewer();
   m_viewerwidget= QWidget::createWindowContainer( m_viewer );
  // viewerwidget->setMinimumSize( QSize( 800, 600 ) );
   m_viewerwidget->setAutoFillBackground( false );

   setCentralWidget( m_viewerwidget );
      QDockWidget* dock = new QDockWidget("Load Dataset");
     // std::cout<<projetMatrix<<std::endl;
      Content ContentLoadFolder(m_ro,m_viewer->getRenderer(),paramProvider1,pathRaw1,pathUnet1);

      m_LoadFolder = new LoadFolderWidget(ContentLoadFolder,dock);
      dock->setWidget(m_LoadFolder);

     // app.askForUpdate();
    this->addDockWidget(Qt::LeftDockWidgetArea, dock);
      setWindowTitle( QString( "Radium Engine Segementation" ) );
      QStringList headers;
      headers << tr( "Entities -> Components" );
      m_itemModel = new Ra::GuiBase::ItemModel( mainApp->getEngine(), this );
      m_selectionManager = new Ra::GuiBase::SelectionManager( m_itemModel, this );
   //   connect( m_viewer, & Ra::Gui::Viewer::rightClickPicking, this, &MainWindow::handlePicking );
     connect(actionSave, &QAction::triggered, this, &MainWindow::saveTexture );
     connect(actionSaveAs, &QAction::triggered, this, &MainWindow::saveAsTexture);
}

MainWindow::~MainWindow() {
    // Child QObjects will automatically be deleted
}

/// Access the viewer, i.e. the rendering widget.
Ra::Gui::Viewer* MainWindow::getViewer(){
       return  m_viewer;
}

/// Access the selection manager.
Ra::GuiBase::SelectionManager* MainWindow::getSelectionManager(){
        return  m_selectionManager;
}

/// Access the timeline.
Ra::GuiBase::Timeline* MainWindow::getTimeline(){
    Ra::GuiBase::Timeline* tmp;
    return tmp;
}

/// Update the ui from the plugins loaded.
void MainWindow::updateUi( Ra::Plugins::RadiumPluginInterface* plugin ){}

/// Update the UI ( most importantly gizmos ) to the modifications of the engine
void MainWindow::onFrameComplete(){

}

/// Add a renderer in the application: UI, viewer.
void MainWindow::addRenderer( const std::string& name, std::shared_ptr<Ra::Engine::Renderer> e ){}
/// clear selection, fit camera and update material name in ui
void MainWindow::postLoadFile( const std::string& filename ){}
/// Cleanup resources.
void MainWindow::cleanup(){}
bool MainWindow::eventFilter(QObject *obj, QEvent *event){   
    QImageReader reader;
    QSettings settings;
    reader.setFileName(QString::fromStdString(paramProvider1->getPathUnet()));
    image = reader.read();
    if(event->type() == QEvent::MouseButtonPress){

      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
      auto cam = m_viewer->getCameraManipulator()->getCamera();
    //  auto tmp = mouseEvent->globalPos();
     // auto tmp1 = this->mapFromGlobal(tmp);
      //auto x = mouseEvent->x();
      const auto r   = cam->getRayFromScreen( {mouseEvent->x(),mouseEvent->y()});
          // we compute intersection with the paper plane, which is defined by a and
          //
          std::vector<Scalar> t;
          // {0,0,0} a point and {0,1,0} the normal
          if ( Ra::Core::Geometry::RayCastPlane( r, {0, 0, 0}, {0,0,1}, t ) )
          {
              // vsPlane return at most one intersection t
                auto worldPos = r.pointAt( t[0] );
              // if the brush is touching the paper
                auto QuadEntiny = mainApp->getEngine()->getEntityManager()->getEntity("Quad Entity");
                       CORE_ASSERT( QuadEntiny, "QuadEntiny is null ptr" );
                      auto paperPos = QuadEntiny->getTransform().inverse() * worldPos;
                   //  paramProvider1->setMouse({paperPos.x(),paperPos.y()});
                       paramProvider1->setMouse({paperPos.x(),paperPos.y()});
                       auto tmp1 = (int)((paperPos.x()+1)*paramProvider1->getTextureUnet()->width()/2);
                       auto tmp2 = (int)((paperPos.y()*-1+1)*paramProvider1->getTextureUnet()->height()/2);
                       image.setPixel(tmp1,tmp2,Qt::white);
                       image.save(QString::fromStdString(paramProvider1->getPathUnet()));
                    // mainApp->getEngine()->getInstance()->pushFboAndViewport();
                    // gl45core::glViewport( 0, 0, int(m_viewer->width()), int( m_viewer->height() ));

                     //  std::cout<<"hihi"<<tmp<<std::endl;
                      // image.setPixelColor(tmp.x(),tmp.y(),Qt::white);
                     //  auto tex = m_viewer->getRenderer()->getAvailableTextures();
                       //m_viewer->getRenderer()->render( data );
                       mainApp->askForUpdate();

          }
}
        return false;
  }
    void MainWindow::handlePicking( const Ra::Engine::Renderer::PickingResult& pickingResult ) {
        Ra::Core::Utils::Index roIndex( pickingResult.m_roIdx );
        Ra::Engine::RadiumEngine* engine = Ra::Engine::RadiumEngine::getInstance();
        if ( roIndex.isValid() )
        {
            auto ro = engine->getRenderObjectManager()->getRenderObject( roIndex );
            if ( ro->getType() != Ra::Engine::RenderObjectType::UI )
            {
                Ra::Engine::Component* comp = ro->getComponent();
                Ra::Engine::Entity* ent     = comp->getEntity();

                // For now we don't enable group selection.
                m_selectionManager->setCurrentEntry( ItemEntry( ent, comp, roIndex ),
                                                     QItemSelectionModel::ClearAndSelect |
                                                         QItemSelectionModel::Current );
            }
        }
        else
        { m_selectionManager->clear(); }
    }

void MainWindow::saveTexture(){

}
void MainWindow::saveAsTexture(){
    QString path = QFileDialog::getSaveFileName(this, "Save Image", "",
                                                "BMP(*.bmp);;PNG(*.png);;JPEG(*.jpg *.jpeg);;All Files(*.*) ");
    if(path == ""){
      QMessageBox msgBox4;
      msgBox4.setText("No directory");
      msgBox4.exec();
     }
     else{
        size_t width, height;
       width =  paramProvider1->getTextureUnet()->width();
       height = paramProvider1->getTextureUnet()->height();
      // auto tmp2 = tmp->addTexture(path.toStdString(),width,height, paramProvider1->getTextureUnet()->texels());
      // std::cout<<tmp2.height<<" "<< tmp2.name<< " "<<tmp2.texels<<std::endl;
     //  auto pixels = std::unique_ptr<float[]>( new float[tex->width() *tex->height() * 4] );
      // GL_ASSERT( glGetTexImage( gl45core::GL_TEXTURE_2D, 0, gl45core::GL_RGBA, gl45core::GL_FLOAT, pixels.get() ) );

     //  auto tmp =  m_viewer->getRenderer()->grabFrame(width,height);
       image.save(path);
}
}




//std::shared_ptr<Ra::Engine::RenderObject> initQuad( Ra::GuiBase::BaseApplication& app){

//}
//}
//}
