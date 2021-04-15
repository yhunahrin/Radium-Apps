#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <GuiBase/MainWindowInterface.hpp>
#include <GuiBase/RaGuiBase.hpp>
#include <GuiBase/SelectionManager/SelectionManager.hpp>
#include <GuiBase/TimerData/FrameTimerData.hpp>
#include <GuiBase/TreeModel/EntityTreeModel.hpp>

#include "LoadFolderWidget.h"
#include "CameraManipulator.hpp"
#include <GuiBase/MainWindowInterface.hpp>
#include "MyParameterProvider.hpp"
#include <GuiBase/Viewer/Viewer.hpp>
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
#include <MainApplication.hpp>

#include "ui_MainWindow.h"
#include <QMainWindow>

#include <QEvent>
#include <qdebug.h>

namespace Ra {
namespace Engine {
class Entity;
}
} // namespace Ra

namespace Ra {
namespace Gui {
class EntityTreeModel;
class Viewer;
} // namespace Gui
namespace GuiBase {
class Timeline;
} // namespace GuiBase
} // namespace Ra

namespace Ra {
namespace Plugins {
class RadiumPluginInterface;
}
} // namespace Ra

//namespace Ra {
//namespace GuiBase {
/// This class manages most of the GUI of the application :
/// top menu, side toolbar and side dock.
class MainWindow : public Ra::GuiBase::MainWindowInterface, private Ui::MainWindow
{
    Q_OBJECT

  public:
    /// Constructor and destructor.
    explicit MainWindow( QWidget* parent = nullptr );
    virtual ~MainWindow();

    /// Access the viewer, i.e. the rendering widget.
    Ra::Gui::Viewer* getViewer() override;

    /// Access the selection manager.
    Ra::GuiBase::SelectionManager* getSelectionManager();

    /// Access the timeline.
    Ra::GuiBase::Timeline* getTimeline() override;

    /// Update the ui from the plugins loaded.
    void updateUi( Ra::Plugins::RadiumPluginInterface* plugin ) override;

    /// Update the UI ( most importantly gizmos ) to the modifications of the engine
    void onFrameComplete() override;

    /// Add a renderer in the application: UI, viewer.
    void addRenderer( const std::string& name, std::shared_ptr<Ra::Engine::Renderer> e ) override;
    /// clear selection, fit camera and update material name in ui
    void postLoadFile( const std::string& filename ) override;
    /// Cleanup resources.
    void cleanup() override;
  //  std::shared_ptr<Ra::Engine::RenderObject> initQuad();

    void handlePicking( const Ra::Engine::Renderer::PickingResult& pickingResult );

    void saveTexture();
    void saveAsTexture();
private:
    Ra::Gui::Viewer* m_viewer{ nullptr };
   // Ra::Engine::Renderer * _renderer;
    /// Stores the internal model of engine objects for selection and visibility.
    Ra::GuiBase::ItemModel* m_itemModel{nullptr};

    /// Stores and manages the current selection.
    Ra::GuiBase::SelectionManager* m_selectionManager{nullptr};
    QWidget* m_viewerwidget;
    /// Widget to allow mm_viewerwidgetaterial edition.
    LoadFolderWidget * m_LoadFolder;
    QImage image;
protected:
   bool eventFilter(QObject *obj, QEvent *evt);
};
//}}
#endif // MAINWINDOW_H
