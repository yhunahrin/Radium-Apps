#ifndef SEGEMENTATION_H
#define SEGEMENTATION_H
#pragma once

#include <QWidget>
#include <QString>
#include <Engine/Renderer/Texture/TextureManager.hpp>
#include <QLabel>
#include <QPainter>
#include <QLine>
#include <QMouseEvent>
#include <QPainterPath>
#include <QFileDialog>
#include <GuiBase/MainWindowInterface.hpp>
#include <GuiBase/RaGuiBase.hpp>
#include <GuiBase/SelectionManager/SelectionManager.hpp>
#include <GuiBase/TimerData/FrameTimerData.hpp>
#include <GuiBase/TreeModel/EntityTreeModel.hpp>
#include "ui_Segmentation.h"
namespace Ui {
class Segmentation;
}

namespace Ra{
namespace Engine{
    class RenderObject;
    class Renderer;
    class ShaderParameterProvider;
}
}
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
enum CNN { Unet, CNN2, CNN3, CNN4 };
///
/// \todo Add automatic UI generation from parameter provider
class Segmentation : public QWidget
{
    Q_OBJECT

public:
    explicit Segmentation(QString pathProgram, QWidget* parent = nullptr);
    ~Segmentation();
    /// Access the viewer, i.e. the rendering widget.
    const QStringList& getPathTest() const;
    const QString& getPathOut() const;
    void setPathTest( const QStringList& pathTest);
    void setPathOut(const QString& pathOut);
    const QString& getPathModelTrained() const;
    void setPathModelTrained( const QString& pathModelTrained);
    const QString& getPathProgram() const;
    void setPathProgram( const QString& pathProgram);
public slots:
    //void postLoadFile( const std::string& filename ) override;
    /// Cleanup resources.
    //void cleanup() override;
    void runLoad();
    void runSave();
    void checkScriptTrain();
    void checkScriptTest();
    void checkPreTrained();
    void pathListImageRaw();
    void pathListMask();
    void pathModelCheckPoint();
    void pathModelTrained();
    void pathImageTest();
    void pathImageOut();
    void checkUnet();
    void checkCNN2();
    void checkCNN3();
    void checkCNN4();
    void runExecute();
private:
    Ui::Segmentation *ui;
    QString _pathListImageRaw;
    QString _pathListMask;
    QDir dirListImageRaw;
    QDir dirListMask;
    QString _pathModelCheck;
    QString _pathModelTrained;
    QStringList _pathTest;
    QString _pathOut;
    QString _pathProg;
    QDir dirPathOut;
    QString typeModel;
    int typeCNN =0;
    bool scriptTrain =false;
    bool scriptTest = false;
    bool pretrained =false;
    QString textScript;

};
#endif // SEGEMENTATION_H
