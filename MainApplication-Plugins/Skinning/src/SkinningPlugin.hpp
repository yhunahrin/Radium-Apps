#ifndef SKINNINGLUGIN_HPP_
#define SKINNINGLUGIN_HPP_

#include <SkinningPluginMacros.hpp>

#include <PluginBase/RadiumPluginInterface.hpp>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QFrame>
#include <QObject>
#include <QtPlugin>

namespace Ra {
namespace Engine {
class RadiumEngine;
struct ItemEntry;
} // namespace Engine
namespace Guibase {
class SelectionManager;
}
} // namespace Ra

namespace SkinningPlugin {

class SkinningComponent;
class SkinningSystem;

class SkinningWidget : public QFrame
{
    Q_OBJECT

    friend class SkinningPluginC;

  public:
    explicit SkinningWidget( QWidget* parent = nullptr );

  signals:
    void showWeights( bool );
    void showWeightsType( int );

  public slots:
    void setCurrent( const Ra::Engine::ItemEntry& entry, SkinningComponent* comp );
    void onShowWeightsToggled( bool on );
    void onSkinningWeightsChanged( int newType );

  signals:
    void askForUpdate();

  private slots:
    void onSkinningChanged( int newType );

    void onLSBActionTriggered();
    void onDQActionTriggered();
    void onCoRActionTriggered();
    void onSTBSLBSActionTriggered();
    void onSTBSDQSActionTriggered();

  private:
    SkinningComponent* m_current;
    QComboBox* m_skinningSelect;
    QCheckBox* m_showWeights;
    QComboBox* m_skinningWeights;
    QAction* m_actionLBS;
    QAction* m_actionDQ;
    QAction* m_actionCoR;
    QAction* m_actionSTBSLBS;
    QAction* m_actionSTBSDQS;
};

// Du to an ambiguous name while compiling with Clang, must differentiate plugin claas from plugin
// namespace
class SkinningPluginC : public QObject, Ra::Plugins::RadiumPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID "RadiumEngine.PluginInterface" )
    Q_INTERFACES( Ra::Plugins::RadiumPluginInterface )

  public:
    SkinningPluginC() = default;
    ~SkinningPluginC();

    void registerPlugin( const Ra::Plugins::Context& context ) override;

    bool doAddWidget( QString& name ) override;
    QWidget* getWidget() override;

    bool doAddMenu() override;
    QMenu* getMenu() override;

    bool doAddAction( int& nb ) override;
    QAction* getAction( int id ) override;

    bool doAddROpenGLInitializer() override;
    void openGlInitialize( const Ra::Plugins::Context& context ) override;

  signals:
    void askForUpdate();

  private slots:
    void onCurrentChanged( const QModelIndex& current, const QModelIndex& prev );
    void onShowWeights( bool on );
    void onShowWeightsType( int type );

  private:
    SkinningSystem* m_system{nullptr};
    Ra::GuiBase::SelectionManager* m_selectionManager{nullptr};
    SkinningWidget* m_widget{nullptr};
};

} // namespace SkinningPlugin

#endif // SKINNINGPLUGIN_HPP_