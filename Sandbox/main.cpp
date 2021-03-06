#include <MainApplication.hpp>

#include <GuiBase/Utils/KeyMappingManager.hpp>

#include <Gui/MainWindow.hpp>

class MainWindowFactory : public Ra::GuiBase::BaseApplication::WindowFactory
{
  public:
    using Ra::GuiBase::BaseApplication::WindowFactory::WindowFactory;
    Ra::GuiBase::MainWindowInterface* createMainWindow() const override {
        return new Ra::Gui::MainWindow();
    }
};

int main( int argc, char** argv ) {
    Ra::MainApplication app( argc, argv );
    app.initialize( MainWindowFactory() );
    app.setContinuousUpdate( false );
    return app.exec();
}
