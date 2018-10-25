#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(ASSETS_LIB)
#  define ASSETS_EXPORT Q_DECL_EXPORT
# else
#  define ASSETS_EXPORT Q_DECL_IMPORT
# endif
#else
# define ASSETS_EXPORT
#endif
