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
#include <queue>
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
    "ivec2 size;\n"
    "void main(void)\n"
    "{\n"
    "    size = textureSize(aUnetTextureUniform,0);\n"
    "    vec2 pixel_size = 1.0/size;\n"
    "    vec4 imageRaw = texture(aRawTextureUniform,  texCoordOut)\n;"
    "    vec4 imageUnet = texture(aUnetTextureUniform,  texCoordOut)\n;"
    "    out_color  =aMixUniform*(imageRaw + imageUnet);\n"
    "}\n"};

const ShaderConfigType defaultConfig1 {
    {Ra::Engine::ShaderType::ShaderType_VERTEX, _vertexShaderSource1},
    {Ra::Engine::ShaderType::ShaderType_FRAGMENT, _fragmentShaderSource1},};

const std::string pathRaw1 = "0.png";
const std::string pathUnet1 = "0.png";
//const std::string pathRaw1 = "C:\\Users\\aduongng\\Desktop\\TP_VO\\hihi.bmp";
//const std::string pathUnet1 = "C:\\Users\\aduongng\\Desktop\\TP_VO\\hihi.bmp";
auto paramProvider1 = std::make_shared<MyParameterProvider>();
QColor color = Qt::white;
std::string pathTexture="";
std::string pathTextureOrigine="";
int count =0;
int sizePixel = 1;
size_t ahihi=0;
bool check=0;
bool drawing = false;
int indexRaw=-1;
int indexCNN=-1;
int comp =0;
int idx =-1;
//Content ContentLoadFolder;
void boundaryFill(QImage *im, int x, int y, QColor fill_color)
{/*
    if(((x>=0)&&((y)>=0)&&((x)<im->width())&&(y)<im->height())&&( im->pixelColor(x, y) != fill_color))
    {
        im->setPixelColor(x,y,fill_color);
        boundaryFill(im,x + 1, y, fill_color);
        boundaryFill(im,x, y + 1, fill_color);
        boundaryFill(im,x - 1, y, fill_color);
        boundaryFill(im,x, y - 1, fill_color);
    }

    */


    std::queue<QPoint> Q;
       QPoint m, Tg;
       if (((x>=0)&&((y)>=0)&&((x)<im->width())&&(y)<im->height())&&( im->pixelColor(x, y) != fill_color))
       {
           m.setX(x);
           m.setY(y);
           im->setPixelColor(m.x(),m.y(),fill_color);
           Q.push(m);  //  Them 1 diem vao queue, queue size tang 1
           while(Q.empty() == false)   //Xet 4 diem xung quanh voi moi diem luu trong queue (neu queue con phan tu)
           {
               Q.pop();//  Xoa 1 diem phia dau queue, queue size giam 1
               //Xet cac diem lan can cua 1 diem
               if(im->pixelColor(m.x()+1, m.y()) != fill_color)
               {
                   im->setPixelColor(m.x()+1, m.y(),fill_color);
                   Tg.setX(m.x()+1);
                   Tg.setY(m.y());
                   Q.push(Tg);// Them 1 diem vao cuoi queue
               }
               if(im->pixelColor(m.x()-1, m.y()) != fill_color)
               {
                   im->setPixelColor(m.x()-1, m.y(),fill_color);
                   Tg.setX(m.x()-1);
                   Tg.setY(m.y());
                   Q.push(Tg);// Them 1 diem vao cuoi queue
               }
               if(im->pixelColor(m.x(), m.y()+1) != fill_color)
               {
                   im->setPixelColor(m.x(), m.y()+1,fill_color);
                   Tg.setX(m.x());
                   Tg.setY(m.y()+1);
                   Q.push(Tg);// Them 1 diem vao cuoi queue
               }
               if(im->pixelColor(m.x(), m.y()-1) != fill_color)
               {
                   im->setPixelColor(m.x(), m.y()-1,fill_color);
                   Tg.setX(m.x());
                   Tg.setY(m.y()-1);
                   Q.push(Tg);// Them 1 diem vao cuoi queue
               }
               if(im->pixelColor(m.x()-1, m.y()-1) != fill_color)
               {
                   im->setPixelColor(m.x()-1, m.y()-1,fill_color);
                   Tg.setX(m.x()-1);
                   Tg.setY(m.y()-1);
                   Q.push(Tg);// Them 1 diem vao cuoi queue
               }
               if(im->pixelColor(m.x()-1, m.y()+1) != fill_color)
               {
                   im->setPixelColor(m.x()-1, m.y()+1,fill_color);
                   Tg.setX(m.x()-1);
                   Tg.setY(m.y()+1);
                   Q.push(Tg);// Them 1 diem vao cuoi queue
               }
               if(im->pixelColor(m.x()+1, m.y()-1) != fill_color)
               {
                   im->setPixelColor(m.x()+1, m.y()-1,fill_color);
                   Tg.setX(m.x()+1);
                   Tg.setY(m.y()-1);
                   Q.push(Tg);// Them 1 diem vao cuoi queue
               }
               if(im->pixelColor(m.x()+1, m.y()+1) != fill_color)
               {
                   im->setPixelColor(m.x()+1, m.y()+1,fill_color);
                   Tg.setX(m.x()+1);
                   Tg.setY(m.y()+1);
                   Q.push(Tg);// Them 1 diem vao cuoi queue
               }
               m = Q.front();// Dua ve gia tri dau tien cho hang doi
           }
       }
}

MainWindow::MainWindow( QWidget* parent ) : MainWindowInterface( parent ) {
    // Note : at this point most of the components (including the Engine) are
    // not initialized. Listen to the "started" signal.

   setupUi( this );
   qApp->installEventFilter(this);
 /*  QSettings setting("Radium","Apllication Segmentation Of Image");
   QSettings ProgramPython("settingPythonProgram.ini", QSettings::IniFormat);
   QFile program(ProgramPython.value("Python/program").toString());
  // program.open(QIODevice::ReadOnly);
 //  std::ifstream file;
 //  file.open(setProgramPython.value("Python/program").toString().toStdString(), std::ios::in);
   static QString file_ = program.fileName();
   pathProgPy = file_;
   std::cout<<"hihi"<<pathProgPy.toStdString()<<std::endl;*/
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
 pathProgPy ="";
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









 // this->postLoadFile( "Cube" );

/*



  Ra::Engine::RawShaderMaterial::registerMaterial();
  std::string vertex = {"#include \"TransformStructs.glsl\"\n"
                        "layout (location = 0) in vec3 in_position;\n"
                        "layout (location = 0) out vec3 out_pos;\n"
                        "layout (location = 1) in vec2 texCoordIn;\n"
                        "layout (location = 1) out vec2 texCoordOut;\n"
                        "uniform Transform transform;\n"
                        "void main(void)\n"
                        "{\n"
                        "    mat4 mvp    = transform.proj * transform.view;\n"
                        "    out_pos     = in_position + vec3("};
  std::string pos ="";
  pos+=std::to_string(2);
  pos+=".0,";
  pos+=std::to_string(2);
  pos+=".0,0.0);\n";
  vertex+=pos;
  std::string vertexAfter=  {"    gl_Position = mvp*vec4(out_pos, 1.0);\n"
                             "    texCoordOut = texCoordIn ;\n"
                               "}\n"};
   vertex+=vertexAfter;
  const std::string _vertexShaderSource2 = vertex;
  const ShaderConfigType defaultConfig2 {
       {Ra::Engine::ShaderType::ShaderType_VERTEX, _vertexShaderSource2},
       {Ra::Engine::ShaderType::ShaderType_FRAGMENT, _fragmentShaderSource1},};
  auto paramProvider2 = std::make_shared<MyParameterProvider>();
  auto quad2 = Ra::Core::Geometry::makeZNormalQuad({1_ra, 1_ra});
//Adding texture coordinates to the geometry
 // Ra::Core::Vector2Array texcoords{{0_ra,1_ra},{0_ra,0_ra},{1_ra,1_ra},{1_ra,0_ra}};
  Ra::Core::Vector2Array texcoords2{{0_ra,0_ra},{1_ra,0_ra},{0_ra,1_ra},{1_ra,1_ra}};
//  Ra::Core::Vector2Array texcoords{{0_ra,1_ra},{.5_ra,.5_ra},{0_ra,1_ra},{1_ra,1_ra}};
//   Ra::Core::Vector2Array texcoords{{1_ra,0_ra},{1_ra,1_ra},{0_ra,0_ra},{1_ra,0_ra},{1_ra,1_ra},{0_ra,0_ra}};
//        Ra::Core::Vector2Array texcoords{{0_ra,1_ra},{1_ra,1_ra},{0_ra,0_ra},{1_ra,0_ra},{1_ra,0_ra},{0_ra,1_ra}};
quad2.addAttrib("texCoordIn",texcoords);
// auto tex= mainApp->m_engine->getTextureManager();

//tex->updatePendingTextures();
//! [Create the engine entity for the quad]
std::string name="Quad Entity";
std::cout<<name<<std::endl;
auto e2 = mainApp->m_engine->getEntityManager()->getEntity( "Quad Entity" );

//! [Create Parameter provider for the shader]

paramProvider2->setPathRaw("C:\\Users\\Rin\\Desktop\\blabla.png");
paramProvider2->setPathUnet("C:\\Users\\Rin\\Desktop\\blabla.png");
//  paramProvider->setMouse({0,0});
//! [Create the shader material]
 std::string name1="Quad Material";
Ra::Core::Asset::RawShaderMaterialData mat2 {name1, defaultConfig2, paramProvider2};
//! [Create a geometry component using the custom material]
 std::string name2="Quad Mesh";
auto c2 = new Ra::Engine::TriangleMeshComponent(name2, e2, std::move( quad2 ), &mat2);
//auto system = mainApp->m_engine->getSystem( "GeometrySystem" );





*/
 // gridLayout(4);








  //  m_viewer =  mainApp->m_mainWindow->getViewer();
   m_viewer = new Ra::Gui::Viewer();
   m_viewerwidget= QWidget::createWindowContainer( m_viewer );
  // viewerwidget->setMinimumSize( QSize( 800, 600 ) );
   m_viewerwidget->setAutoFillBackground( false );

     // std::cout<<projetMatrix<<std::endl;
    //  ContentLoadFolder.setRenderObject(m_ro);
     // ContentLoadFolder.setRenderer(m_viewer->getRenderer());
     // ContentLoadFolder.setParameterProvider(paramProvider1);
     // ContentLoadFolder.setPathRaw(pathRaw1);
     // ContentLoadFolder.setPathCNN(pathUnet1);
     // ContentLoadFolder.setCount(count);
     Content ContentLoadFolder(m_ro,m_viewer->getRenderer(),paramProvider1,pathRaw1,pathUnet1,count);
     QDockWidget* dock = new QDockWidget("Load Dataset");
     QDockWidget* dock2 = new QDockWidget("Paint");

      m_LoadFolder = new LoadFolderWidget(ContentLoadFolder,dock);
      m_Meshpaint = new MeshPaintUI(color,sizePixel,dock2);
      QImageReader reader;
      QSettings settings;
      m_Meshpaint->setSizePixel(1,256);
      dock->setWidget(m_LoadFolder);
      dock2->setWidget(m_Meshpaint);
      QDockWidget* d = new QDockWidget("View");
      QWidget* multiWidget = new QWidget();
      QVBoxLayout* layout = new QVBoxLayout();
      layout->addWidget(m_viewerwidget);
     QPushButton* actionNext = new QPushButton(tr("Next"));
     QPushButton* actionPrevious  = new QPushButton(tr("Previous"));
      QHBoxLayout* layout2 = new QHBoxLayout();
      label = new QLabel();
      label->setSizeIncrement(20,5);
      layout2->addWidget(actionPrevious);
      layout2->addWidget(label);
      layout2->addWidget(actionNext);
      layout->addLayout(layout2);
    //  Segmentation *nw = new Segmentation(pathProgPy);
      //layout->addWidget(nw);
      multiWidget->setLayout(layout);
      d->setWidget(multiWidget);
      setCentralWidget(d);

     // app.askForUpdate();
    this->addDockWidget(Qt::LeftDockWidgetArea, dock);
    this->addDockWidget(Qt::LeftDockWidgetArea, dock2);
      setWindowTitle( QString( "Radium Engine Segementation" ) );
      QStringList headers;
      headers << tr( "Entities -> Components" );
      m_itemModel = new Ra::GuiBase::ItemModel( mainApp->getEngine(), this );
      m_selectionManager = new Ra::GuiBase::SelectionManager( m_itemModel, this );

   //   connect( m_viewer, & Ra::Gui::Viewer::rightClickPicking, this, &MainWindow::handlePicking );
     connect( m_selectionManager, &Ra::GuiBase::SelectionManager::selectionChanged, this,&MainWindow::onSelectionChanged );
     connect( m_viewer, &Ra::Gui::Viewer::rightClickPicking, this, &MainWindow::handlePicking );
  //   connect(actionSave, &QAction::triggered, this, &MainWindow::saveTexture);
    // connect(actionSaveAs, &QAction::triggered, this, &MainWindow::saveAsTexture);
     connect(actionManipulation, &QAction::triggered, this, &MainWindow::manipuSegementation);
     connect(actionProgram, &QAction::triggered, this, &MainWindow::programPython);
     connect(actionNext, &QPushButton::clicked, this, &MainWindow::buttonNext);
     connect(actionPrevious, &QPushButton::clicked, this, &MainWindow::buttonPrev);
     connect(actionChoose, &QAction::triggered, this, &MainWindow::sliceViewer);
     connect(m_LoadFolder,&LoadFolderWidget::needUpdate,mainApp,&Ra::MainApplication::askForUpdate);
     connect(actionSlice_XY, &QAction::triggered, this, &MainWindow::loadSlicesXY);
     connect(actionSlice_XZ, &QAction::triggered, this, &MainWindow::loadSlicesXZ);
     connect(actionSlice_ZY, &QAction::triggered, this, &MainWindow::loadSlicesZY);

     connect(action1x1, &QAction::triggered, this, &MainWindow::grid1);
     connect(action2x2, &QAction::triggered, this, &MainWindow::grid2);
     connect(action4x4, &QAction::triggered, this, &MainWindow::grid4);
     connect(action8x8, &QAction::triggered, this, &MainWindow::grid8);
     menu3D->setEnabled(false);
     actionChoose->setEnabled(false);
     loadSetting();
   //  setCentralWidget( m_Meshpaint);
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
  //  Ra::GuiBase::Timeline* tmp;
    return 0 ;
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
    if(obj==m_viewer){
 //   QImageReader reader;
   // QSettings settings;
    if((event->type() == QEvent::MouseButtonPress)&&(paramProvider1->getCount()==0)){
    //std::cout<<paramProvider1->getTexture()->width()<<std::endl;
   // m_Meshpaint->setSizePixel(1,int((paramProvider1->getTexture()->width()-1)/2));
   // reader.setFileName(QString::fromStdString(paramProvider1->getPathUnet()));
   // pathTextureOrigine = paramProvider1->getPathUnet();
      pathTextureOrigine =  m_LoadFolder->getDatasetCNN().at(paramProvider1->getIndexCNN()).path;
   // m_LoadFolder->setPathTmp(QString::fromStdString(paramProvider1->getPathUnet()));
    //std::cout<<m_LoadFolder->getDatasetCNN().size()<<std::endl;
    image = m_LoadFolder->getDatasetCNN().at(paramProvider1->getIndexCNN()).image;
    paramProvider1->setSave(false);
    //QFileInfo file(QString::fromStdString(pathTextureOrigine));
   // label->setText(file.fileName());
    }


    if((event->type() == QEvent::MouseButtonPress)&&(m_Meshpaint->getPaint()==true)&&(m_Meshpaint->getchoseColor()==true)){
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
          if ( Ra::Core::Geometry::RayCastPlane( r, {0, 0, 0}, {0,0,1}, t ))
          {
              drawing = true;
              // vsPlane return at most one intersection t
                auto worldPos = r.pointAt( t[0] );
              // if the brush is touching the paper
                auto QuadEntiny = mainApp->getEngine()->getEntityManager()->getEntity("Quad Entity");
                       CORE_ASSERT( QuadEntiny, "QuadEntiny is null ptr" );
                      auto paperPos = QuadEntiny->getTransform().inverse() * worldPos;
                   //  paramProvider1->setMouse({paperPos.x(),paperPos.y()});
                    //    paramProvider1->setMouse({paperPos.x(),paperPos.y()});
                      auto tmp1 = (int)((paperPos.x()+1)*paramProvider1->getTexture()->width()/2);
                      auto tmp2 = (int)((paperPos.y()*-1+1)*paramProvider1->getTexture()->height()/2);
                      count++;
                     // std::cout<<paramProvider1->getTexture()->width()<<std::endl;
                      color = m_Meshpaint->getColor();
                      m_Meshpaint->setValueSizePixel(sizePixel);
                      auto sizePixelChange = m_Meshpaint->getSizePixel();
                      int k = -1*(sizePixelChange-1);
                      for(int i = k; i<sizePixelChange; i++)
                          for(int j = k; j<sizePixelChange;j++){
                                if((tmp1+i>=0)&&((tmp2+j)>=0)&&((tmp1+i)<image.width())&&(tmp2+j)<image.height())
                                    image.setPixelColor(tmp1+i,tmp2+j,color);
                          }
                      pathTexture="tmp";
                      pathTexture+=std::to_string(count);
                      char *p;
                      std::string tmp3 = pathTextureOrigine;
                      char *tmp4 = const_cast<char*>(tmp3.c_str());
                      p = strtok(tmp4, ".");
                      p = strtok(NULL, "\\eof");
                      pathTexture+=".";
                      pathTexture+=p;
                      m_LoadFolder->setPathTmp(QString::fromStdString(pathTexture));
                      image.save(QString::fromStdString(pathTexture));
                      paramProvider1->setPathUnet(pathTexture);
                      paramProvider1->setCount(count);
                       mainApp->askForUpdate();
                       if(count>1){
                           int count1 = count-1;
                           std::string path1 = "tmp";
                           path1+=std::to_string(count1);
                           char *p;
                           std::string tmp3 = pathTextureOrigine;
                           char *tmp4 = const_cast<char*>(tmp3.c_str());
                           p = strtok(tmp4, ".");
                           p = strtok(NULL, "\\eof");
                           path1+=".";
                           path1+=p;
                           remove(path1.c_str());
                       }
                        m_LoadFolder->setDraw(true);
                        m_LoadFolder->saveImageChanged();

          }
    }
          if((event->type() == QEvent::MouseMove)&&(m_Meshpaint->getPaint()==true)&&drawing==true){
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
                  if ( Ra::Core::Geometry::RayCastPlane( r, {0, 0, 0}, {0,0,1}, t ))
                  {
                      drawing = true;
                      // vsPlane return at most one intersection t
                        auto worldPos = r.pointAt( t[0] );
                      // if the brush is touching the paper
                        auto QuadEntiny = mainApp->getEngine()->getEntityManager()->getEntity("Quad Entity");
                               CORE_ASSERT( QuadEntiny, "QuadEntiny is null ptr" );
                              auto paperPos = QuadEntiny->getTransform().inverse() * worldPos;
                           //  paramProvider1->setMouse({paperPos.x(),paperPos.y()});
                            //    paramProvider1->setMouse({paperPos.x(),paperPos.y()});
                              auto tmp1 = (int)((paperPos.x()+1)*paramProvider1->getTexture()->width()/2);
                              auto tmp2 = (int)((paperPos.y()*-1+1)*paramProvider1->getTexture()->height()/2);
                              count++;
                             // std::cout<<paramProvider1->getTexture()->width()<<std::endl;
                              color = m_Meshpaint->getColor();
                              m_Meshpaint->setValueSizePixel(sizePixel);
                              auto sizePixelChange = m_Meshpaint->getSizePixel();
                              int k = -1*(sizePixelChange-1);
                              for(int i = k; i<sizePixelChange; i++)
                                  for(int j = k; j<sizePixelChange;j++){
                                        if((tmp1+i>=0)&&((tmp2+j)>=0)&&((tmp1+i)<image.width())&&(tmp2+j)<image.height())
                                            image.setPixelColor(tmp1+i,tmp2+j,color);
                                  }
                              pathTexture="tmp";
                              pathTexture+=std::to_string(count);
                              char *p;
                              std::string tmp3 = pathTextureOrigine;
                              char *tmp4 = const_cast<char*>(tmp3.c_str());
                              p = strtok(tmp4, ".");
                              p = strtok(NULL, "\\eof");
                              pathTexture+=".";
                              pathTexture+=p;
                              m_LoadFolder->setPathTmp(QString::fromStdString(pathTexture));
                              image.save(QString::fromStdString(pathTexture));
                              paramProvider1->setPathUnet(pathTexture);
                              paramProvider1->setCount(count);
                              mainApp->askForUpdate();
                               if(count>1){
                                   int count1 = count-1;
                                   std::string path1 = "tmp";
                                   path1+=std::to_string(count1);
                                   char *p;
                                   std::string tmp3 = pathTextureOrigine;
                                   char *tmp4 = const_cast<char*>(tmp3.c_str());
                                   p = strtok(tmp4, ".");
                                   p = strtok(NULL, "\\eof");
                                   path1+=".";
                                   path1+=p;
                                   remove(path1.c_str());
                               }
                              // this->saveTexture();
                               m_LoadFolder->setDraw(true);
                               m_LoadFolder->saveImageChanged();
          }
}
          if((event->type() == QEvent::MouseButtonRelease)&&(m_Meshpaint->getPaint()==true)){
              drawing = false;
              m_LoadFolder->saveImageChanged();
          }
     if((event->type() == QEvent::MouseButtonPress)&&(m_Meshpaint->getBoundaryFill()==true)&&(m_Meshpaint->getchoseColor()==true)&&(m_Meshpaint->getBoundary()==true)){
                   pt.resize(0);
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
                       if ( Ra::Core::Geometry::RayCastPlane( r, {0, 0, 0}, {0,0,1}, t ))
                       {
                           drawing = true;
                           // vsPlane return at most one intersection t
                             auto worldPos = r.pointAt( t[0] );
                           // if the brush is touching the paper
                             auto QuadEntiny = mainApp->getEngine()->getEntityManager()->getEntity("Quad Entity");
                                    CORE_ASSERT( QuadEntiny, "QuadEntiny is null ptr" );
                                   auto paperPos = QuadEntiny->getTransform().inverse() * worldPos;
                                //  paramProvider1->setMouse({paperPos.x(),paperPos.y()});
                                 //    paramProvider1->setMouse({paperPos.x(),paperPos.y()});
                                   auto tmp1 = (int)((paperPos.x()+1)*paramProvider1->getTexture()->width()/2);
                                   auto tmp2 = (int)((paperPos.y()*-1+1)*paramProvider1->getTexture()->height()/2);
                                   count++;
                                  // std::cout<<paramProvider1->getTexture()->width()<<std::endl;
                                   color = m_Meshpaint->getColor();
                                   auto po =  QPoint(tmp1,tmp2);
                                   pt.push_back(po);
                                   m_Meshpaint->setValueSizePixel(sizePixel);
                                   auto sizePixelChange = m_Meshpaint->getSizePixel();
                                   int k = -1*(sizePixelChange-1);
                                   for(int i = k; i<sizePixelChange; i++)
                                       for(int j = k; j<sizePixelChange;j++){
                                             if((tmp1+i>=0)&&((tmp2+j)>=0)&&((tmp1+i)<image.width())&&(tmp2+j)<image.height())
                                                 image.setPixelColor(tmp1+i,tmp2+j,color);
                                       }
                                   pathTexture="tmp";
                                   pathTexture+=std::to_string(count);
                                   char *p;
                                   std::string tmp3 = pathTextureOrigine;
                                   char *tmp4 = const_cast<char*>(tmp3.c_str());
                                   p = strtok(tmp4, ".");
                                   p = strtok(NULL, "\\eof");
                                   pathTexture+=".";
                                   pathTexture+=p;
                                   m_LoadFolder->setPathTmp(QString::fromStdString(pathTexture));
                                   image.save(QString::fromStdString(pathTexture));
                                   paramProvider1->setPathUnet(pathTexture);
                                   paramProvider1->setCount(count);
                                 // mainApp->getEngine()->getInstance()->pushFboAndViewport();
                                 // gl45core::glViewport( 0, 0, int(m_viewer->width()), int( m_viewer->height() ));

                                  //  std::cout<<"hihi"<<tmp<<std::endl;
                                   // image.setPixelColor(tmp.x(),tmp.y(),Qt::white);
                                  //  auto tex = m_viewer->getRenderer()->getAvailableTextures();
                                    //m_viewer->getRenderer()->render( data );

                                    mainApp->askForUpdate();
                                    if(count>1){
                                        int count1 = count-1;
                                        std::string path1 = "tmp";
                                        path1+=std::to_string(count1);
                                        char *p;
                                        std::string tmp3 = pathTextureOrigine;
                                        char *tmp4 = const_cast<char*>(tmp3.c_str());
                                        p = strtok(tmp4, ".");
                                        p = strtok(NULL, "\\eof");
                                        path1+=".";
                                        path1+=p;
                                        remove(path1.c_str());
                                    }
                                     m_LoadFolder->setDraw(true);
                                     m_LoadFolder->saveImageChanged();
                       }
                 }
                       if((event->type() == QEvent::MouseMove)&&(m_Meshpaint->getBoundaryFill()==true)&&(m_Meshpaint->getBoundary()==true)&&drawing==true){
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
                               if ( Ra::Core::Geometry::RayCastPlane( r, {0, 0, 0}, {0,0,1}, t ))
                               {
                                   drawing = true;
                                   // vsPlane return at most one intersection t
                                     auto worldPos = r.pointAt( t[0] );
                                   // if the brush is touching the paper
                                     auto QuadEntiny = mainApp->getEngine()->getEntityManager()->getEntity("Quad Entity");
                                            CORE_ASSERT( QuadEntiny, "QuadEntiny is null ptr" );
                                           auto paperPos = QuadEntiny->getTransform().inverse() * worldPos;
                                        //  paramProvider1->setMouse({paperPos.x(),paperPos.y()});
                                         //    paramProvider1->setMouse({paperPos.x(),paperPos.y()});
                                           auto tmp1 = (int)((paperPos.x()+1)*paramProvider1->getTexture()->width()/2);
                                           auto tmp2 = (int)((paperPos.y()*-1+1)*paramProvider1->getTexture()->height()/2);
                                           auto po =  QPoint(tmp1,tmp2);
                                           pt.push_back(po);
                                           count++;
                                          // std::cout<<paramProvider1->getTexture()->width()<<std::endl;
                                           color = m_Meshpaint->getColor();
                                           m_Meshpaint->setValueSizePixel(sizePixel);
                                           auto sizePixelChange = m_Meshpaint->getSizePixel();
                                           int k = -1*(sizePixelChange-1);
                                           for(int i = k; i<sizePixelChange; i++)
                                               for(int j = k; j<sizePixelChange;j++){
                                                     if((tmp1+i>=0)&&((tmp2+j)>=0)&&((tmp1+i)<image.width())&&(tmp2+j)<image.height())
                                                         image.setPixelColor(tmp1+i,tmp2+j,color);
                                               }
                                           pathTexture="tmp";
                                           pathTexture+=std::to_string(count);
                                           char *p;
                                           std::string tmp3 = pathTextureOrigine;
                                           char *tmp4 = const_cast<char*>(tmp3.c_str());
                                           p = strtok(tmp4, ".");
                                           p = strtok(NULL, "\\eof");
                                           pathTexture+=".";
                                           pathTexture+=p;
                                           m_LoadFolder->setPathTmp(QString::fromStdString(pathTexture));
                                           image.save(QString::fromStdString(pathTexture));
                                           paramProvider1->setPathUnet(pathTexture);
                                           paramProvider1->setCount(count);
                                         // mainApp->getEngine()->getInstance()->pushFboAndViewport();
                                         // gl45core::glViewport( 0, 0, int(m_viewer->width()), int( m_viewer->height() ));

                                          //  std::cout<<"hihi"<<tmp<<std::endl;
                                           // image.setPixelColor(tmp.x(),tmp.y(),Qt::white);shi
                                            //m_viewer->getRenderer()->render( data );

                                            mainApp->askForUpdate();
                                            if(count>1){
                                                int count1 = count-1;
                                                std::string path1 = "tmp";
                                                path1+=std::to_string(count1);
                                                char *p;
                                                std::string tmp3 = pathTextureOrigine;
                                                char *tmp4 = const_cast<char*>(tmp3.c_str());
                                                p = strtok(tmp4, ".");
                                                p = strtok(NULL, "\\eof");
                                                path1+=".";
                                                path1+=p;
                                                remove(path1.c_str());
                                            }
                                             m_LoadFolder->setDraw(true);
                                             m_LoadFolder->saveImageChanged();
                       }
             }
                       if((event->type() == QEvent::MouseButtonRelease)&&(m_Meshpaint->getBoundaryFill()==true)&&(m_Meshpaint->getBoundary()==true)){
                           count++;
                           drawing = false;
                           pix = QPixmap::fromImage(image);
                           QPainter painter(&image);
                           QPen pen(color,m_Meshpaint->getSizePixel(),Qt::SolidLine);
                           painter.setPen(pen);

                           painter.drawPolygon(pt.data(),pt.size());
                         /*  for (int i =0; i<pt.size()-1;i++){
                              int j = i+1;
                              painter.drawP
                              painter.drawLine(pt.at(i),pt.at(j));
                              std::cout<<i<<" "<<j<<std::endl;
                           }*/
                          // for (int i =0; i<pt.size()-2;i+=3){
                          //    painter.drawLine(pt.at(i),pt.at(i+2));
                           //}
                        /*   for (int i =1; i<pt.size()-1;i+=2){
                              painter.drawLine(pt.at(i),pt.at(i+1));
                              std::cout<<i<<" "<<i+1<<std::endl;
                           }*/
                         //  std::cout<<"hihihi"<<pt.size()<<std::endl;
                          // painter.drawLine(pt.at(pt.size()-1),pt.at(0));
                         //  std::cout<<pt.size()-1<<std::endl;
                          // image = pix.toImage();
                           pathTexture="tmp";
                           pathTexture+=std::to_string(count);
                           char *p;
                           std::string tmp3 = pathTextureOrigine;
                           char *tmp4 = const_cast<char*>(tmp3.c_str());
                           p = strtok(tmp4, ".");
                           p = strtok(NULL, "\\eof");
                           pathTexture+=".";
                           pathTexture+=p;
                           m_LoadFolder->setPathTmp(QString::fromStdString(pathTexture));
                           image.save(QString::fromStdString(pathTexture));
                           paramProvider1->setPathUnet(pathTexture);
                           paramProvider1->setCount(count);
                           if(count>1){
                               int count1 = count-1;
                               std::string path1 = "tmp";
                               path1+=std::to_string(count1);
                               char *p;
                               std::string tmp3 = pathTextureOrigine;
                               char *tmp4 = const_cast<char*>(tmp3.c_str());
                               p = strtok(tmp4, ".");
                               p = strtok(NULL, "\\eof");
                               path1+=".";
                               path1+=p;
                               remove(path1.c_str());
                           }
                            m_LoadFolder->setDraw(true);
                            m_LoadFolder->saveImageChanged();
                       }

     if((event->type() == QEvent::MouseButtonPress)&&(m_Meshpaint->getBoundaryFill()==true)&&(m_Meshpaint->getchoseColor()==true)&&(m_Meshpaint->getFill()==true)){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            auto cam = m_viewer->getCameraManipulator()->getCamera();
            const auto r   = cam->getRayFromScreen( {mouseEvent->x(),mouseEvent->y()});;
                // we compute intersection with the paper plane, which is defined by a and
                //
                std::vector<Scalar> t;
                // {0,0,0} a point and {0,1,0} the normal
                if ( Ra::Core::Geometry::RayCastPlane( r, {0, 0, 0}, {0,0,1}, t ))
                {
                    // vsPlane return at most one intersection t
                      auto worldPos = r.pointAt( t[0] );
                    // if the brush is touching the paper
                      auto QuadEntiny = mainApp->getEngine()->getEntityManager()->getEntity("Quad Entity");
                             CORE_ASSERT( QuadEntiny, "QuadEntiny is null ptr" );
                            auto paperPos = QuadEntiny->getTransform().inverse() * worldPos;
                         //  paramProvider1->setMouse({paperPos.x(),paperPos.y()});
                          //    paramProvider1->setMouse({paperPos.x(),paperPos.y()});
                            auto tmp1 = (int)((paperPos.x()+1)*paramProvider1->getTexture()->width()/2);
                            auto tmp2 = (int)((paperPos.y()*-1+1)*paramProvider1->getTexture()->height()/2);
                            count++;
                           // std::cout<<paramProvider1->getTexture()->width()<<std::endl;
                            color = m_Meshpaint->getColor();
                            boundaryFill(&image,tmp1,tmp2,color);
                            pathTexture="tmp";
                            pathTexture+=std::to_string(count);
                            char *p;
                            std::string tmp3 = pathTextureOrigine;
                            char *tmp4 = const_cast<char*>(tmp3.c_str());
                            p = strtok(tmp4, ".");
                            p = strtok(NULL, "\\eof");
                            pathTexture+=".";
                            pathTexture+=p;
                            m_LoadFolder->setPathTmp(QString::fromStdString(pathTexture));
                            image.save(QString::fromStdString(pathTexture));
                            paramProvider1->setPathUnet(pathTexture);
                            paramProvider1->setCount(count);
                             mainApp->askForUpdate();
                             if(count>1){
                                 int count1 = count-1;
                                 std::string path1 = "tmp";
                                 path1+=std::to_string(count1);
                                 char *p;
                                 std::string tmp3 = pathTextureOrigine;
                                 char *tmp4 = const_cast<char*>(tmp3.c_str());
                                 p = strtok(tmp4, ".");
                                 p = strtok(NULL, "\\eof");
                                 path1+=".";
                                 path1+=p;
                                 remove(path1.c_str());
                             }
                             m_Meshpaint->unCheckedFill();
                              m_LoadFolder->setDraw(true);
                             m_LoadFolder->saveImageChanged();

                }
        }
      if((event->type() == QEvent::MouseButtonPress)&&(m_Meshpaint->getBoundary()==false)&&(m_Meshpaint->getchoseColor()==true)&&(m_Meshpaint->getFill()==false)&&(m_Meshpaint->getBoundaryFill()==true)){
          QMessageBox msgBox4;
          msgBox4.setText("Please choose type to draw");
          msgBox4.exec();
      }
    }
        return false;
  }
    void MainWindow::handlePicking( const Ra::Engine::Renderer::PickingResult& pickingResult ) {
        Ra::Core::Utils::Index roIndex( pickingResult.m_roIdx );
      //  std::cout<<"ahihi"<<roIndex<<std::endl;
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
          //      std::cout<<"ahaha"<<m_selectionManager->currentIndex().column()<<std::endl;
            }
        }
        else
        { m_selectionManager->clear(); }
    }

void MainWindow::saveTexture(){
   auto dirFix = m_LoadFolder->getDirCNN();
   QString folderSave = dirFix.path();
   folderSave+="_changed";
   if(QDir(folderSave).exists()){}
   else {
       QDir().mkdir(folderSave);}
       auto pathTexture = QFileInfo(QString::fromStdString(pathTextureOrigine));
       folderSave+="/";
       folderSave+=pathTexture.fileName();
       image.save(QString::fromStdString(folderSave.toStdString()));
       paramProvider1->setSave(true);
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
        //size_t width, height;
      // width =  paramProvider1->getTextureUnet()->width();
      // height = paramProvider1->getTextureUnet()->height();
      // auto tmp2 = tmp->addTexture(path.toStdString(),width,height, paramProvider1->getTextureUnet()->texels());
      // std::cout<<tmp2.height<<" "<< tmp2.name<< " "<<tmp2.texels<<std::endl;
     //  auto pixels = std::unique_ptr<float[]>( new float[tex->width() *tex->height() * 4] );
    //   GL_ASSERT( glGetTexImage( gl45core::GL_TEXTURE_2D, 0, gl45core::GL_RGBA, gl45core::GL_FLOAT, paramProvider1->getTextureUnet() ) );
     // stbi_write_png(path.toStdString().c_str(), width, height, 4, paramProvider1->getTextureUnet(),width*4*sizeof (uchar));
   //  auto tmp =  m_viewer->getRenderer()->grabFrame(width,height);
       image.save(path);
       paramProvider1->setSave(true);
}
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(m_Meshpaint->getPaint()==true||m_Meshpaint->getBoundaryFill()==true){
        if(m_LoadFolder->getDraw()==true){
            QMessageBox msgBox;
            msgBox.setText("Save modified segmentation?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int res1 = msgBox.exec();
            if(res1 == QMessageBox::No){
    QMessageBox::StandardButton res = QMessageBox::question(this,"Radium Application","Do you want to quit?",QMessageBox::Cancel|QMessageBox::No|QMessageBox::Yes,QMessageBox::Yes);
    if(res!=QMessageBox::Yes)
        event->ignore();
    else {
        if(pathTexture!="")
            remove(pathTexture.c_str());
        event->accept();
        saveSetting();

    }
}
            else if(res1 == QMessageBox::Yes){
                m_LoadFolder->saveAll();
                m_LoadFolder->setDraw(false);
                QMessageBox::StandardButton res = QMessageBox::question(this,"Radium Application","Do you want to quit?",QMessageBox::Cancel|QMessageBox::No|QMessageBox::Yes,QMessageBox::Yes);
                if(res!=QMessageBox::Yes)
                    event->ignore();
                else {
                    if(pathTexture!="")
                        remove(pathTexture.c_str());
                    event->accept();
                    saveSetting();
                }
            }
        }
        else{
            QMessageBox::StandardButton res = QMessageBox::question(this,"Radium Application","Do you want to quit?",QMessageBox::Cancel|QMessageBox::No|QMessageBox::Yes,QMessageBox::Yes);
            if(res!=QMessageBox::Yes)
                event->ignore();
            else {
                if(pathTexture!="")
                    remove(pathTexture.c_str());
                event->accept();
                saveSetting();
            }
        }
    }
    else {
        QMessageBox::StandardButton res = QMessageBox::question(this,"Radium Application","Do you want to quit?",QMessageBox::Cancel|QMessageBox::No|QMessageBox::Yes,QMessageBox::Yes);
        if(res!=QMessageBox::Yes)
            event->ignore();
        else {
            if(pathTexture!="")
                remove(pathTexture.c_str());
            event->accept();
            saveSetting();
        }
    }
}

void MainWindow::onSelectionChanged(){


}
void MainWindow::programPython(){
    pathProgPy = QFileDialog::getOpenFileName(
                            this,
                            "Select one to open",
                            "",
                            " (*.exe)");
    if(pathProgPy==""){
        QMessageBox msgBox1;
        msgBox1.setWindowTitle("Nothing program to selected !!!");
        msgBox1.exec();
    }
    else{
    actionManipulation->setEnabled(true);
  //  actionSave->setEnabled(true);
   // actionSaveAs->setEnabled(true);
    m_LoadFolder->setEnabled(true);
    m_Meshpaint->setEnabled(true);
    m_LoadFolder->setPathProg(pathProgPy);


    //m_LoadFolder->getContent().setPathProg(pathProgPy);
    }
}
void MainWindow::manipuSegementation(){
   /* QDockWidget* dock = new QDockWidget("Segmentation");
     dock->setWidget(new Segmentation(dock) );
     mainApp->m_mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dock);*/
    Segmentation *nw = new Segmentation(pathProgPy);
    nw->show();
    std::cout<<pathProgPy.toStdString()<<std::endl;
}

void  MainWindow::saveSetting(){
    QSettings setting(QSettings::NativeFormat, QSettings::UserScope, "Radium_App", "MainWindow");
    setting.setValue("position",this->geometry());
    QSettings setting2(QSettings::NativeFormat, QSettings::UserScope, "Radium_App", "ProgramPython");
    setting2.setValue("Name", "python");
    setting2.setValue("Path",pathProgPy);
}
void  MainWindow::loadSetting(){
    QSettings setting(QSettings::NativeFormat, QSettings::UserScope, "Radium_App", "MainWindow");
    QRect myrect = setting.value("position").toRect();
    setGeometry(myrect);
    QSettings setting2(QSettings::NativeFormat, QSettings::UserScope, "Radium_App", "ProgramPython");
    pathProgPy = setting2.value("Path").toString();
}

void  MainWindow::buttonNext(){
    if((m_LoadFolder->getDatasetRaw().size()==0)||(m_LoadFolder->getDatasetCNN().size()==0)){
        QMessageBox msgBox;
        msgBox.setText("Nothing to show!!!");
        msgBox.exec();
    }
     else{
           if(grid == 1){
            paramProvider1->setSave(true);
            if(m_LoadFolder->getPathListCNNFull().size()!=0){
            indexRaw = m_LoadFolder->getIndexRaw();
            indexCNN = m_LoadFolder->getIndexCNN();
            if(indexRaw<m_LoadFolder->getPathListCNNFull().size()-1){
             indexRaw++;
             indexCNN++;
             auto Texraw = m_LoadFolder->getDatasetRaw().at(indexRaw).textureParametres;
             auto Texcnn = m_LoadFolder->getDatasetCNN().at(indexCNN).textureParametres;
           //  auto cnn = m_LoadFolder->getPathListCNNFull().at(indexCNN);
             paramProvider1->setTextureParameterRaw(Texraw);
             paramProvider1->setTextureParameterCNN(Texcnn);
             paramProvider1->setIndexRaw(indexRaw);
             paramProvider1->setIndexCNN(indexCNN);
           /*  QFileInfo file(cnn);
             label->setText(file.fileName());
             label->setAlignment(Qt::AlignCenter);*/
             emit m_LoadFolder->needUpdate();
             m_LoadFolder->setIndexRaw(indexRaw);
             m_LoadFolder->setIndexCNN(indexCNN);
             m_LoadFolder->setIndexListBox(indexRaw,indexCNN);
             paramProvider1->setCount(0);
            }
         else {
          /*  auto raw = m_LoadFolder->getPathListRawFull().at(0);
            auto cnn = m_LoadFolder->getPathListCNNFull().at(0);
            paramProvider1->setPathRaw(raw.toStdString());
            paramProvider1->setPathUnet(cnn.toStdString());
            QFileInfo file(cnn);
            label->setText(file.fileName());
             label->setAlignment(Qt::AlignCenter);*/
                auto Texraw = m_LoadFolder->getDatasetRaw().at(0).textureParametres;
                auto Texcnn = m_LoadFolder->getDatasetCNN().at(0).textureParametres;
              //  auto cnn = m_LoadFolder->getPathListCNNFull().at(indexCNN);
                paramProvider1->setTextureParameterRaw(Texraw);
                paramProvider1->setTextureParameterCNN(Texcnn);
              /*  QFileInfo file(cnn);
                label->setText(file.fileName());
                label->setAlignment(Qt::AlignCenter);*/
            emit m_LoadFolder->needUpdate();
            indexRaw =0;
            indexCNN =0;
            paramProvider1->setIndexRaw(indexRaw);
            paramProvider1->setIndexCNN(indexCNN);
            m_LoadFolder->setIndexRaw(indexRaw);
            m_LoadFolder->setIndexCNN(indexCNN);
            m_LoadFolder->setIndexListBox(indexRaw,indexCNN);
            paramProvider1->setCount(0);}
        }
    }
          else if(grid ==2){
               auto dataRaw = m_LoadFolder->getDatasetRaw();
               auto dataCNN = m_LoadFolder->getDatasetCNN();
               if(idx>=dataCNN.size()-1)
                   grid2();
               else{
                   if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                       size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                       for(size_t i =0; i<tmp;i++)
                           Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                   }
                   if((idx+4)<=dataCNN.size()-1){
                   QVector <Ra::Engine::Dataset> dataRaw1;
                   QVector <Ra::Engine::Dataset> dataCNN1;
                   for(int i=idx;i<idx+4;i++){
                       dataRaw1.push_back(dataRaw.at(i));
                       dataCNN1.push_back(dataCNN.at(i));
                   }
                     gridLayout(grid,dataRaw1,dataCNN1);
                     idx+=4;
                   }
                   else{
                       QVector <Ra::Engine::Dataset> dataRaw1;
                       QVector <Ra::Engine::Dataset> dataCNN1;
                       for(int i=idx;i<=dataCNN.size()-1;i++){
                           dataRaw1.push_back(dataRaw.at(i));
                           dataCNN1.push_back(dataCNN.at(i));


                   }
                       gridLayout(grid,dataRaw1,dataCNN1);
                       idx = dataCNN.size()-1;
               }
           }
    }
          else if(grid ==4){
               auto dataRaw = m_LoadFolder->getDatasetRaw();
               auto dataCNN = m_LoadFolder->getDatasetCNN();
               if(idx>=dataCNN.size()-1)
                   grid4();
               else{
                   if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                       size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                       for(size_t i =0; i<tmp;i++)
                           Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                   }
                   if((idx+16)<=dataCNN.size()-1){
                   QVector <Ra::Engine::Dataset> dataRaw1;
                   QVector <Ra::Engine::Dataset> dataCNN1;
                   for(int i=idx;i<idx+16;i++){
                       dataRaw1.push_back(dataRaw.at(i));
                       dataCNN1.push_back(dataCNN.at(i));


                   }
                       gridLayout(grid,dataRaw1,dataCNN1);
                     idx+=16;
                   }
                   else{
                       QVector <Ra::Engine::Dataset> dataRaw1;
                       QVector <Ra::Engine::Dataset> dataCNN1;
                       for(int i=idx;i<=dataCNN.size()-1;i++){
                           dataRaw1.push_back(dataRaw.at(i));
                           dataCNN1.push_back(dataCNN.at(i));


                   }
                                   gridLayout(grid,dataRaw1,dataCNN1);
                       idx = dataCNN.size()-1;
               }
           }
          //      m_LoadFolder->setIndexListBox(idx,idx);
    }
         else  if(grid ==8){

               auto dataRaw = m_LoadFolder->getDatasetRaw();
               auto dataCNN = m_LoadFolder->getDatasetCNN();
               if(idx>=dataCNN.size()-1)
                   grid8();

               else{
                   if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                       size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                       for(size_t i =0; i<tmp;i++)
                           Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                   }
                   if((idx+64)<=dataCNN.size()-1){
                   QVector <Ra::Engine::Dataset> dataRaw1;
                   QVector <Ra::Engine::Dataset> dataCNN1;
                   for(int i=idx;i<idx+64;i++){
                       dataRaw1.push_back(dataRaw.at(i));
                       dataCNN1.push_back(dataCNN.at(i));


                   }
                               gridLayout(grid,dataRaw1,dataCNN1);
                     idx+=64;
                   }
                   else{
                       QVector <Ra::Engine::Dataset> dataRaw1;
                       QVector <Ra::Engine::Dataset> dataCNN1;
                       for(int i=idx;i<=dataCNN.size()-1;i++){
                           dataRaw1.push_back(dataRaw.at(i));
                           dataCNN1.push_back(dataCNN.at(i));


                   }
                        gridLayout(grid,dataRaw1,dataCNN1);
                       idx = dataCNN.size()-1;
               }
           }
             //   m_LoadFolder->setIndexListBox(idx,idx);
    }
    }
}

void  MainWindow::buttonPrev(){
    if((m_LoadFolder->getDatasetRaw().size()==0)||(m_LoadFolder->getDatasetCNN().size()==0)){
        QMessageBox msgBox;
        msgBox.setText("Nothing to show!!!");
        msgBox.exec();
    }
     else{
        if(grid ==1){
     paramProvider1->setSave(true);
    if(m_LoadFolder->getPathListCNNFull().size()!=0){
         indexRaw = m_LoadFolder->getIndexRaw();
         indexCNN = m_LoadFolder->getIndexCNN();
         if(indexRaw>0){
             indexRaw--;
             indexCNN--;
          /*   auto raw = m_LoadFolder->getPathListRawFull().at(indexRaw);
             auto cnn = m_LoadFolder->getPathListCNNFull().at(indexCNN);
             paramProvider1->setPathRaw(raw.toStdString());
             paramProvider1->setPathUnet(cnn.toStdString());
             QFileInfo file(cnn);
             label->setText(file.fileName());
              label->setAlignment(Qt::AlignCenter);*/
             auto Texraw = m_LoadFolder->getDatasetRaw().at(indexRaw).textureParametres;
             auto Texcnn = m_LoadFolder->getDatasetCNN().at(indexCNN).textureParametres;
             paramProvider1->setTextureParameterRaw(Texraw);
             paramProvider1->setTextureParameterCNN(Texcnn);
             paramProvider1->setIndexRaw(indexRaw);
             paramProvider1->setIndexCNN(indexCNN);
             m_LoadFolder->setIndexListBox(indexRaw,indexCNN);
           //  auto cnn = m_LoadFolder->getPathListCNNFull().at(indexCNN);

           /*  QFileInfo file(cnn);
             label->setText(file.fileName());
             label->setAlignment(Qt::AlignCenter);*/
             emit m_LoadFolder->needUpdate();
              m_LoadFolder->setIndexRaw(indexRaw);
              m_LoadFolder->setIndexCNN(indexCNN);
           paramProvider1->setCount(0);}
         else {
            /* auto raw = m_LoadFolder->getPathListRawFull().at(m_LoadFolder->getPathListCNNFull().size()-1);
             auto cnn = m_LoadFolder->getPathListCNNFull().at(m_LoadFolder->getPathListCNNFull().size()-1);
             paramProvider1->setPathRaw(raw.toStdString());
             paramProvider1->setPathUnet(cnn.toStdString());
             QFileInfo file(cnn);
             label->setText(file.fileName());
              label->setAlignment(Qt::AlignCenter);*/
             auto Texraw = m_LoadFolder->getDatasetRaw().at(m_LoadFolder->getPathListRawFull().size()-1).textureParametres;
             auto Texcnn = m_LoadFolder->getDatasetCNN().at(m_LoadFolder->getPathListCNNFull().size()-1).textureParametres;
           //  auto cnn = m_LoadFolder->getPathListCNNFull().at(indexCNN);
             paramProvider1->setTextureParameterRaw(Texraw);
             paramProvider1->setTextureParameterCNN(Texcnn);

           /*  QFileInfo file(cnn);
             label->setText(file.fileName());
             label->setAlignment(Qt::AlignCenter);*/
             emit m_LoadFolder->needUpdate();
             indexRaw =m_LoadFolder->getPathListRawFull().size()-1;
             indexCNN =m_LoadFolder->getPathListCNNFull().size()-1;
             paramProvider1->setIndexRaw(indexRaw);
             paramProvider1->setIndexCNN(indexCNN);
             m_LoadFolder->setIndexRaw(indexRaw);
             m_LoadFolder->setIndexCNN(indexCNN);
             m_LoadFolder->setIndexListBox(indexRaw,indexCNN);
           paramProvider1->setCount(0);}
         }
        }
       else if(grid ==2){
            auto dataRaw = m_LoadFolder->getDatasetRaw();
            auto dataCNN = m_LoadFolder->getDatasetCNN();
             if(idx==0||idx==4){
                 if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                     size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                     for(size_t i =0; i<tmp;i++)
                         Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                 }
                 QVector <Ra::Engine::Dataset> dataRaw1;
                 QVector <Ra::Engine::Dataset> dataCNN1;

                 int tmp2 = dataRaw.size()%4;
                 if(tmp2!=0){
                 for(int i= dataRaw.size()-tmp2;i<= dataRaw.size()-1;i++){
                     dataRaw1.push_back(dataRaw.at(i));
                     dataCNN1.push_back(dataCNN.at(i));
                 }
                 }
                 else{
                     for(int i= dataRaw.size()-4;i<= dataRaw.size()-1;i++){
                         dataRaw1.push_back(dataRaw.at(i));
                         dataCNN1.push_back(dataCNN.at(i));
                     }
                 }
                 gridLayout(grid,dataRaw1,dataCNN1);
                 idx = dataRaw.size()-1;
             }
             else if(idx>= dataRaw.size()-1){
                 if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                     size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                     for(size_t i =0; i<tmp;i++)
                         Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                 }
                 QVector <Ra::Engine::Dataset> dataRaw1;
                 QVector <Ra::Engine::Dataset> dataCNN1;
                 int tmp2 = dataRaw.size()/4;
                 int tmp3;
                 if(dataRaw.size()%4!=0)
                    tmp3 = tmp2*4;
                 else
                    tmp3 = (tmp2-1)*4;
                 if(dataRaw.size()>=4){
                 for(int i=tmp3-4;i<tmp3;i++){
                     dataRaw1.push_back(dataRaw.at(i));
                     dataCNN1.push_back(dataCNN.at(i));
                 }
                 gridLayout(grid,dataRaw1,dataCNN1);
                 idx = tmp3;
                 }
                 else{
                     if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                         size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                         for(size_t i =0; i<tmp;i++)
                             Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                     }
                     QVector <Ra::Engine::Dataset> dataRaw1;
                     QVector <Ra::Engine::Dataset> dataCNN1;

                     int tmp2 = dataRaw.size()%4;
                     if(tmp2!=0){
                     for(int i= dataRaw.size()-tmp2;i<= dataRaw.size()-1;i++){
                         dataRaw1.push_back(dataRaw.at(i));
                         dataCNN1.push_back(dataCNN.at(i));
                     }
                     }
                     else{
                         for(int i= dataRaw.size()-4;i<= dataRaw.size()-1;i++){
                             dataRaw1.push_back(dataRaw.at(i));
                             dataCNN1.push_back(dataCNN.at(i));
                         }
                     }
                     gridLayout(grid,dataRaw1,dataCNN1);
                     idx = dataRaw.size()-1;
                 }
             }
             else{
                 if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                     size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                     for(size_t i =0; i<tmp;i++)
                         Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                 }
                 if((idx-8)>=0){
                 QVector <Ra::Engine::Dataset> dataRaw1;
                 QVector <Ra::Engine::Dataset> dataCNN1;
                 for(int i=idx-8;i<idx-4;i++){
                     dataRaw1.push_back(dataRaw.at(i));
                     dataCNN1.push_back(dataCNN.at(i));
                 }
                   gridLayout(grid,dataRaw1,dataCNN1);
                   idx-=4;
                 }
                 else{
                     if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                         size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                         for(size_t i =0; i<tmp;i++)
                             Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                     }
                     QVector <Ra::Engine::Dataset> dataRaw1;
                     QVector <Ra::Engine::Dataset> dataCNN1;
                     for(int i= 0;i<4;i++){
                         dataRaw1.push_back(dataRaw.at(i));
                         dataCNN1.push_back(dataCNN.at(i));
                     }
                     gridLayout(grid,dataRaw1,dataCNN1);
                     idx = 4;

             }
         }

  }
        else if(grid ==4){
            auto dataRaw = m_LoadFolder->getDatasetRaw();
            auto dataCNN = m_LoadFolder->getDatasetCNN();
             if(idx==0||idx==16){
                 if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                     size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                     for(size_t i =0; i<tmp;i++)
                         Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                 }
                 QVector <Ra::Engine::Dataset> dataRaw1;
                 QVector <Ra::Engine::Dataset> dataCNN1;

                 int tmp2 = dataRaw.size()%16;
                 if(tmp2!=0){
                 for(int i= dataRaw.size()-tmp2;i<= dataRaw.size()-1;i++){
                     dataRaw1.push_back(dataRaw.at(i));
                     dataCNN1.push_back(dataCNN.at(i));
                 }
                 }
                 else{
                     for(int i= dataRaw.size()-16;i<= dataRaw.size()-1;i++){
                         dataRaw1.push_back(dataRaw.at(i));
                         dataCNN1.push_back(dataCNN.at(i));
                     }
                 }
                 gridLayout(grid,dataRaw1,dataCNN1);
                 idx = dataRaw.size()-1;
             }
             else if(idx>= dataRaw.size()-1){
                 if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                     size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                     for(size_t i =0; i<tmp;i++)
                         Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                 }
                 QVector <Ra::Engine::Dataset> dataRaw1;
                 QVector <Ra::Engine::Dataset> dataCNN1;
                 int tmp2 = dataRaw.size()/16;
                 int tmp3;
                 if(dataRaw.size()%16!=0)
                    tmp3 = tmp2*16;
                 else
                    tmp3 = (tmp2-1)*16;
                 if(dataRaw.size()>=16){
                 for(int i=tmp3-16;i<tmp3;i++){
                     dataRaw1.push_back(dataRaw.at(i));
                     dataCNN1.push_back(dataCNN.at(i));
                 }
                 gridLayout(grid,dataRaw1,dataCNN1);
                 idx = tmp3;
                 }
                 else{
                     if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                         size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                         for(size_t i =0; i<tmp;i++)
                             Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                     }
                     QVector <Ra::Engine::Dataset> dataRaw1;
                     QVector <Ra::Engine::Dataset> dataCNN1;

                     int tmp2 = dataRaw.size()%16;
                     if(tmp2!=0){
                     for(int i= dataRaw.size()-tmp2;i<= dataRaw.size()-1;i++){
                         dataRaw1.push_back(dataRaw.at(i));
                         dataCNN1.push_back(dataCNN.at(i));
                     }
                     }
                     else{
                         for(int i= dataRaw.size()-16;i<= dataRaw.size()-1;i++){
                             dataRaw1.push_back(dataRaw.at(i));
                             dataCNN1.push_back(dataCNN.at(i));
                         }
                     }
                     gridLayout(grid,dataRaw1,dataCNN1);
                     idx = dataRaw.size()-1;
                 }
             }
             else{
                 if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                     size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                     for(size_t i =0; i<tmp;i++)
                         Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                 }
                 if((idx-32)>=0){
                 QVector <Ra::Engine::Dataset> dataRaw1;
                 QVector <Ra::Engine::Dataset> dataCNN1;
                 for(int i=idx-32;i<idx-16;i++){
                     dataRaw1.push_back(dataRaw.at(i));
                     dataCNN1.push_back(dataCNN.at(i));
                 }
                   gridLayout(grid,dataRaw1,dataCNN1);
                   idx-=16;
                 }
                 else{
                     if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                         size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                         for(size_t i =0; i<tmp;i++)
                             Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                     }
                     QVector <Ra::Engine::Dataset> dataRaw1;
                     QVector <Ra::Engine::Dataset> dataCNN1;
                     for(int i= 0;i<16;i++){
                         dataRaw1.push_back(dataRaw.at(i));
                         dataCNN1.push_back(dataCNN.at(i));
                     }
                     gridLayout(grid,dataRaw1,dataCNN1);
                     idx = 16;

             }
         }
  }
       else  if(grid ==8){
            auto dataRaw = m_LoadFolder->getDatasetRaw();
            auto dataCNN = m_LoadFolder->getDatasetCNN();
             if(idx==0||idx==64){
                 if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                     size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                     for(size_t i =0; i<tmp;i++)
                         Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                 }
                 QVector <Ra::Engine::Dataset> dataRaw1;
                 QVector <Ra::Engine::Dataset> dataCNN1;

                 int tmp2 = dataRaw.size()%64;
                 if(tmp2!=0){
                 for(int i= dataRaw.size()-tmp2;i<= dataRaw.size()-1;i++){
                     dataRaw1.push_back(dataRaw.at(i));
                     dataCNN1.push_back(dataCNN.at(i));
                 }
                 }
                 else{
                     for(int i= dataRaw.size()-64;i<= dataRaw.size()-1;i++){
                         dataRaw1.push_back(dataRaw.at(i));
                         dataCNN1.push_back(dataCNN.at(i));
                     }
                 }
                 gridLayout(grid,dataRaw1,dataCNN1);
                 idx = dataRaw.size()-1;
             }
             else if(idx>= dataRaw.size()-1){
                 if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                     size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                     for(size_t i =0; i<tmp;i++)
                         Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                 }
                 QVector <Ra::Engine::Dataset> dataRaw1;
                 QVector <Ra::Engine::Dataset> dataCNN1;
                 int tmp2 = dataRaw.size()/64;
                 int tmp3;
                 if(dataRaw.size()%64!=0)
                    tmp3 = tmp2*64;
                 else
                    tmp3 = (tmp2-1)*64;
                 if(dataRaw.size()>=64){
                 for(int i=tmp3-4;i<tmp3;i++){
                     dataRaw1.push_back(dataRaw.at(i));
                     dataCNN1.push_back(dataCNN.at(i));
                 }
                 gridLayout(grid,dataRaw1,dataCNN1);
                 idx = tmp3;
                 }
                 else{
                     if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                         size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                         for(size_t i =0; i<tmp;i++)
                             Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                     }
                     QVector <Ra::Engine::Dataset> dataRaw1;
                     QVector <Ra::Engine::Dataset> dataCNN1;

                     int tmp2 = dataRaw.size()%64;
                     if(tmp2!=0){
                     for(int i= dataRaw.size()-tmp2;i<= dataRaw.size()-1;i++){
                         dataRaw1.push_back(dataRaw.at(i));
                         dataCNN1.push_back(dataCNN.at(i));
                     }
                     }
                     else{
                         for(int i= dataRaw.size()-64;i<= dataRaw.size()-1;i++){
                             dataRaw1.push_back(dataRaw.at(i));
                             dataCNN1.push_back(dataCNN.at(i));
                         }
                     }
                     gridLayout(grid,dataRaw1,dataCNN1);
                     idx = dataRaw.size()-1;
                 }
             }
             else{
                 if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                     size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                     for(size_t i =0; i<tmp;i++)
                         Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                 }
                 if((idx-128)>=0){
                 QVector <Ra::Engine::Dataset> dataRaw1;
                 QVector <Ra::Engine::Dataset> dataCNN1;
                 for(int i=idx-128;i<idx-64;i++){
                     dataRaw1.push_back(dataRaw.at(i));
                     dataCNN1.push_back(dataCNN.at(i));
                 }
                   gridLayout(grid,dataRaw1,dataCNN1);
                   idx-=64;
                 }
                 else{
                     if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
                         size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
                         for(size_t i =0; i<tmp;i++)
                             Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
                     }
                     QVector <Ra::Engine::Dataset> dataRaw1;
                     QVector <Ra::Engine::Dataset> dataCNN1;
                     for(int i= 0;i<4;i++){
                         dataRaw1.push_back(dataRaw.at(i));
                         dataCNN1.push_back(dataCNN.at(i));
                     }
                     gridLayout(grid,dataRaw1,dataCNN1);
                     idx = 64;

             }
         }
        }
}
}

void MainWindow::sliceViewer(){
    QDockWidget* dock3 = new QDockWidget("SliceViewer");
    m_SliceViewer = new SliceViewerWidget(dock3);
    dock3->setWidget(m_SliceViewer);
    this->addDockWidget(Qt::RightDockWidgetArea, dock3);
    menu3D->setEnabled(true);
}
void MainWindow::loadSlicesXY(){
    QString filter = QString("Supported Files (*.bmp *.jpg *.png );;");
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select File(s)"), QDir::homePath(), filter);
    QVector<QImage> imageSliceXY;
    for(int i =0; i < fileNames.size();i++ )
    {
           QImageReader reader;
           QSettings settings;
           reader.setFileName(fileNames.at(i));
           imageSliceXY.push_back(reader.read());
    }
    m_SliceViewer->setSliceXY(imageSliceXY);
    m_SliceViewer->setSliderXY();
    m_SliceViewer->showSliceXY(0);
}
void MainWindow::loadSlicesXZ(){
    QString filter = QString("Supported Files (*.bmp *.jpg *.png );;");
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select File(s)"), QDir::homePath(), filter);
    QVector<QImage> imageSliceXZ;
    for(int i =0; i < fileNames.size();i++ )
    {
           QImageReader reader;
           QSettings settings;
           reader.setFileName(fileNames.at(i));
           imageSliceXZ.push_back(reader.read());
    }
    m_SliceViewer->setSliceXZ(imageSliceXZ);
    m_SliceViewer->setSliderXZ();
    m_SliceViewer->showSliceXZ(0);
}
void MainWindow::loadSlicesZY(){
    QString filter = QString("Supported Files (*.bmp *.jpg *.png );;");
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select File(s)"), QDir::homePath(), filter);
    QVector<QImage> imageSliceZY;
    for(int i =0; i < fileNames.size();i++ )
    {
           QImageReader reader;
           QSettings settings;
           reader.setFileName(fileNames.at(i));
           imageSliceZY.push_back(reader.read());
    }
    m_SliceViewer->setSliceZY(imageSliceZY);
    m_SliceViewer->setSliderZY();
    m_SliceViewer->showSliceZY(0);
}
int MainWindow::gridLayout(int grid, QVector <Ra::Engine::Dataset> &DatasetRaw,  QVector <Ra::Engine::Dataset> &DatasetCNN){
  comp =0;
  std::string Text="";
  if(grid ==1){
  label->clear();
  Ra::Engine::RawShaderMaterial::registerMaterial();
  std::string vertex = {"#include \"TransformStructs.glsl\"\n"
                        "layout (location = 0) in vec3 in_position;\n"
                        "layout (location = 0) out vec3 out_pos;\n"
                        "layout (location = 1) in vec2 texCoordIn;\n"
                        "layout (location = 1) out vec2 texCoordOut;\n"
                        "uniform Transform transform;\n"
                        "void main(void)\n"
                        "{\n"
                        "    mat4 mvp    = transform.proj * transform.view;\n"
                        "    out_pos     = in_position + vec3("};
  std::string pos ="";
  pos+=std::to_string(0);
  pos+=".0,";
  pos+=std::to_string(0);
  pos+=".0,0.0);\n";
  vertex+=pos;
  std::string vertexAfter=  {"    gl_Position = mvp*vec4(out_pos, 1.0);\n"
                             "    texCoordOut = texCoordIn ;\n"
                               "}\n"};
   vertex+=vertexAfter;
  const std::string _vertexShaderSource2 =  vertex;
  const ShaderConfigType defaultConfig2 {
       {Ra::Engine::ShaderType::ShaderType_VERTEX, _vertexShaderSource2},
       {Ra::Engine::ShaderType::ShaderType_FRAGMENT, _fragmentShaderSource1},};
  auto quad2 = Ra::Core::Geometry::makeZNormalQuad({1_ra, 1_ra});
  Ra::Core::Vector2Array texcoords{{0_ra,0_ra},{1_ra,0_ra},{0_ra,1_ra},{1_ra,1_ra}};
  quad2.addAttrib("texCoordIn",texcoords);
 std::string name = "Quad Entity";
 std::string name1="Quad Material";
 std::string name2="Quad Mesh";
 Ra::Engine::Entity* e2;
 e2 = mainApp->m_engine->getEntityManager()->getEntity(name);
Ra::Core::Asset::RawShaderMaterialData mat {name1, defaultConfig2, paramProvider1};
//! [Create a geometry component using the custom material]
 auto c2 = new Ra::Engine::TriangleMeshComponent(name2, e2, std::move( quad2 ), &mat);
 auto system = mainApp->m_engine->getSystem( "GeometrySystem" );
system->addComponent( e2, c2 );
 auto m_ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
 c2->m_renderObjects[0] );
 auto mat2           = static_cast<Ra::Engine::RawShaderMaterial*>( m_ro->getMaterial().get() );
 mat2->updateShaders( defaultConfig2, paramProvider1 );
  m_viewer->getRenderer()->buildRenderTechnique(m_ro.get());
  mainApp->askForUpdate();
  }
  else{
  if(DatasetRaw.size()==0||DatasetCNN.size()==0){
      QMessageBox msgBox;
      msgBox.setText("Nothing to show!!!");
      msgBox.exec();
  }
  else{
  int tmp = grid/2;
  for(int i =tmp-1; i>=-tmp;i--){
      for(int j =-tmp;j<tmp;j++){
          if((comp)<DatasetRaw.size()){
          int posX = j*3;
          int posY = i*3;
          Ra::Engine::RawShaderMaterial::registerMaterial();
          std::string vertex = {"#include \"TransformStructs.glsl\"\n"
                                "layout (location = 0) in vec3 in_position;\n"
                                "layout (location = 0) out vec3 out_pos;\n"
                                "layout (location = 1) in vec2 texCoordIn;\n"
                                "layout (location = 1) out vec2 texCoordOut;\n"
                                "uniform Transform transform;\n"
                                "void main(void)\n"
                                "{\n"
                                "    mat4 mvp    = transform.proj * transform.view;\n"
                                "    out_pos     = in_position + vec3("};
          std::string pos ="";
          pos+=std::to_string(posX);
          pos+=".0,";
          pos+=std::to_string(posY);
          pos+=".0,0.0);\n";
          vertex+=pos;
          std::string vertexAfter=  {"    gl_Position = mvp*vec4(out_pos, 1.0);\n"
                                     "    texCoordOut = texCoordIn ;\n"
                                       "}\n"};
           vertex+=vertexAfter;
          const std::string _vertexShaderSource2 =  vertex;
          const ShaderConfigType defaultConfig2 {
               {Ra::Engine::ShaderType::ShaderType_VERTEX, _vertexShaderSource2},
               {Ra::Engine::ShaderType::ShaderType_FRAGMENT, _fragmentShaderSource1},};
          auto paramProvider2 = std::make_shared<MyParameterProvider>();
          auto quad2 = Ra::Core::Geometry::makeZNormalQuad({1_ra, 1_ra});
        //Adding texture coordinates to the geometry
         // Ra::Core::Vector2Array texcoords{{0_ra,1_ra},{0_ra,0_ra},{1_ra,1_ra},{1_ra,0_ra}};
          Ra::Core::Vector2Array texcoords{{0_ra,0_ra},{1_ra,0_ra},{0_ra,1_ra},{1_ra,1_ra}};
        //  Ra::Core::Vector2Array texcoords{{0_ra,1_ra},{.5_ra,.5_ra},{0_ra,1_ra},{1_ra,1_ra}};
       //   Ra::Core::Vector2Array texcoords{{1_ra,0_ra},{1_ra,1_ra},{0_ra,0_ra},{1_ra,0_ra},{1_ra,1_ra},{0_ra,0_ra}};
       //        Ra::Core::Vector2Array texcoords{{0_ra,1_ra},{1_ra,1_ra},{0_ra,0_ra},{1_ra,0_ra},{1_ra,0_ra},{0_ra,1_ra}};
        quad2.addAttrib("texCoordIn",texcoords);
       // auto tex= mainApp->m_engine->getTextureManager();

        //tex->updatePendingTextures();
         std::string name = "Quad Entity";
         std::string name1="Quad Material";
         std::string name2="Quad Mesh";
         name+=std::to_string(comp);
         name1+=std::to_string(comp);
         name2+=std::to_string(comp);
        Ra::Engine::Entity* e2;
        //! [Create the engine entity for the quad]
        if(mainApp->m_engine->getEntityManager()->entityExists(name)){
            e2 = mainApp->m_engine->getEntityManager()->getEntity(name);
        }
        else
            e2 = mainApp->m_engine->getEntityManager()->createEntity(name);
        //! [Create Parameter provider for the shader]

        paramProvider2->setTextureParameterRaw(DatasetRaw.at(comp).textureParametres);
        paramProvider2->setTextureParameterCNN(DatasetCNN.at(comp).textureParametres);
        Text+= DatasetCNN.at(comp).name;
        Text+= " ";

       //  paramProvider->setMouse({0,0});
        //! [Create the shader material]

        Ra::Core::Asset::RawShaderMaterialData mat {name1, defaultConfig2, paramProvider2};
        //! [Create a geometry component using the custom material]
        auto c2 = new Ra::Engine::TriangleMeshComponent(name2, e2, std::move( quad2 ), &mat);
        auto system = mainApp->m_engine->getSystem( "GeometrySystem" );
        system->addComponent( e2, c2 );
        auto m_ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
          c2->m_renderObjects[0] );
         auto mat2           = static_cast<Ra::Engine::RawShaderMaterial*>( m_ro->getMaterial().get() );
          mat2->updateShaders( defaultConfig2, paramProvider2);
          m_viewer->getRenderer()->buildRenderTechnique(m_ro.get());
          mainApp->askForUpdate();

          comp++;

      }
      }
  }
  if((grid!=8)){
  label->setText(QString::fromStdString((Text)));
  label->setAlignment(Qt::AlignCenter);
  }
  else
      label->clear();
  }
  }
  return comp;
}
void MainWindow::grid1(){
    grid =1;
    if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
        size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
        for(size_t i =0; i<tmp;i++)
            Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
    }
    auto dataRaw = m_LoadFolder->getDatasetRaw();
    auto dataCNN = m_LoadFolder->getDatasetCNN();
    gridLayout(grid,dataRaw,dataCNN);
}
void MainWindow::grid2(){
    grid =2;
    if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
        size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
        for(size_t i =0; i<tmp;i++)
            Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
    }
    auto dataRaw = m_LoadFolder->getDatasetRaw();
    auto dataCNN = m_LoadFolder->getDatasetCNN();
    idx = gridLayout(grid,dataRaw,dataCNN);
}
void MainWindow::grid4(){
    grid =4;
     if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
         size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
         for(size_t i =0; i<tmp;i++)
             Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
     }
    auto dataRaw = m_LoadFolder->getDatasetRaw();
    auto dataCNN = m_LoadFolder->getDatasetCNN();
    idx =  gridLayout(grid,dataRaw,dataCNN);
}
void MainWindow::grid8(){
    grid =8;
    if(Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount()>0){
        size_t tmp = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObjectsCount() - 0;
        for(size_t i =0; i<tmp;i++)
            Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->removeRenderObject(0+i);
    }
    auto dataRaw = m_LoadFolder->getDatasetRaw();
    auto dataCNN = m_LoadFolder->getDatasetCNN();
    idx =  gridLayout(grid,dataRaw,dataCNN);
}
