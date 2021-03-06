# Build Radium Painter
cmake_minimum_required(VERSION 3.8)
if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_GREATER "3.9")
    cmake_policy(SET CMP0071 NEW)
endif()

#-------------------------------------------------------------------------------
# exampleApp executables setup
project(Radium-ShaderEditor)
# Set default install location to installed-<Compiler_ID> folder in build dir
# we do not want to install to /usr by default
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/installed-${CMAKE_CXX_COMPILER_ID}" CACHE PATH
            "Install path prefix, prepended onto install directories." FORCE )
    message( "Set install prefix to ${CMAKE_INSTALL_PREFIX}" )
endif()

find_package( Radium REQUIRED Core Engine GuiBase IO PluginBase)

#------------------------------------------------------------------------------
# Application specific

find_package(Qt5 COMPONENTS Core Widgets OpenGL REQUIRED)
set( Qt5_LIBRARIES Qt5::Core Qt5::Widgets Qt5::OpenGL )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(app_sources
        main.cpp
        ShaderEditorWidget.cpp
        MeshPaintUI.cpp
        Segmentation.cpp
        LoadFolderWidget.cpp
        Content.cpp
        MainWindow.cpp
        MainApplication.cpp
        SliceViewerWidget.cpp
    )

set(app_headers
    CameraManipulator.hpp
    ShaderEditorWidget.hpp
    MyParameterProvider.hpp
    MeshPaintUI.h
    Segmentation.h
    MeshPaintPluginMacros.h
    LoadFolderWidget.h
    SliceViewerWidget.h
    Content.h
    MainWindow.h
    MainApplication.hpp
   )

set(app_uis
    ShaderEditorWidget.ui
    MeshPaintUI.ui
    Segmentation.ui
    LoadFolderWidget.ui
    MainWindow.ui
    SliceViewerWidget.ui
   )

qt5_wrap_ui(app_uis_moc ${app_uis})

set(app_resources
   )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} # Moc
    )

add_executable(
    ${PROJECT_NAME} MACOSX_BUNDLE
    ${app_sources}
    ${app_headers}
    ${app_inlines}
    ${app_uis_moc}
    ${app_resources}
    )

target_link_libraries (${PROJECT_NAME} PUBLIC
    Radium::Core
    Radium::Engine
    Radium::GuiBase
    Radium::IO
    ${Qt5_LIBRARIES}
    )
configure_radium_app(
    NAME ${PROJECT_NAME}
    USE_PLUGINS
)

# radium_cotire( ${app_target} )
