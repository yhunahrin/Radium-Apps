#ifndef MESHPAINTPLUGINMACROS_H
#define MESHPAINTPLUGINMACROS_H
#include <Core/CoreMacros.hpp>

/// Defines the correct macro to export dll symbols.
#if defined MeshPaint_EXPORTS
#    define MESH_PAINT_PLUGIN_API DLL_EXPORT
#else
#    define MESH_PAINT_PLUGIN_API DLL_IMPORT
#endif
#endif // MESHPAINTPLUGINMACROS_H
